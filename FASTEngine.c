/* 
	FASTEngine.c is a "hardware like" implementation in C programming language for FAST to FIX decoding and order book handling.
	Author: Caio Oliveira (caiooliveira@usp.br)

*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/*#include <time.h>

clock_t c1 = 0, c2 = 0;
double tempo;
char valuation[1500];
FILE *fileV;
char auxValuation[1500];*/

#define NONEOPERATOR 0
#define DEFAULT 1
#define COPY 2
#define INCREMENT 3
#define DELTA 4

#define OPTIONAL 1
#define MANDATORY 0

#define UNDEFINED -90
#define ABSENT_32 0x80000000 //biggest 32 bits negative number
#define ABSENT_64 0x8000000000000000 //biggest 64 bits negative number

#define PRINTI(file, var) {printf(file, var);}
#define PRINTD(file, var) {printf(file, var);}
#define PRINTS(file, var) {printf(file, var);}
 
FILE* openFile(char* fileName);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, 
	__uint8_t* FASTMessage, int FASTMessage_length);

void MDHeartbeat_144(__uint8_t* FASTMessage, int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, int FASTMessage_length);

void MD145Handler(__uint32_t NoMDEntries, __uint32_t* MDUpdateAction, char (*MDEntryType)[1500], __uint32_t* RptSeq, char (*QuoteCondition)[1500], __uint64_t* SecurityID, 
__uint32_t* MDEntryTime, __uint32_t* MDInsertDate, __uint32_t* MDInsertTime, float (*MDEntryPx)[2], __uint32_t* NumberofOrders, 
__uint32_t* MDEntryDate, __uint64_t* MDEntrySize, char (*MDStreamID)[1500], __uint64_t* ExpireDate, __uint64_t* EarlyTermination, 
char (*MDEntryBuyer)[1500], char (*MDEntrySeller)[1500], __uint32_t* MDEntryPositionNo, char (*OrderID)[1500]);

void t145toFIX(
	__uint32_t MsgSeqNum, __uint32_t TradeDate, __uint64_t SendintTime, __uint32_t NoMDEntries, 
	__uint32_t MDEntriesSequence_PMap, __uint32_t MDEntriesSequence_PMap_length, __uint32_t* MDUpdateAction, 
	__uint32_t* RptSeq, __uint32_t* NumberOfOrders, __uint32_t* MDEntryTime, __uint32_t* MDEntryDate, __uint32_t* MDInsertDate, 
	__uint32_t* MDInsertTime, __uint32_t* SellerDays, __uint32_t* TradingSessionID, __uint32_t* OpenCloseSettlFlag, 
	__uint32_t* MDEntryPositionNo, __uint32_t* SettPriceType, __uint32_t* LastTradeDate, __uint32_t* PriceAdjustmentMethod, 
	__uint32_t* PriceLimitType, __uint32_t* PriceBandMidpointPriceType, __uint64_t* SecurityID, __uint64_t* MDEntrySize, 
	__uint64_t* TradeVolume, __uint64_t* AvgDailyTradedQty, __uint64_t* ExpireDate, __uint64_t* EarlyTermination, 
	__uint64_t* MaxTradeVol, char (*MDEntryType)[1500], char (*QuoteCondition)[1500], char (*PriceType)[1500], 
	char (*MDStreamID)[1500], char (*Currency)[1500], char (*TickDirection)[1500], char (*TradeCondition)[1500], 
	char (*OrderID)[1500], char (*TradeID)[1500], char (*MDEntryBuyer)[1500], char (*MDEntrySeller)[1500], 
	char (*PriceBandType)[1500], float (*MDEntryPx)[2], float (*MDEntryInterestRate)[2], float (*NetChgPrevDay)[2], float (*LowLimitPrice)[2], 
	float (*HighLimitPrice)[2], float (*TradingReferencePrice)[2], __uint32_t NoUnderlyings, __uint32_t* UnderlyingPXType, 
	__uint64_t* UnderlyingSecurityID, __uint64_t* IndexSeq, float (*UnderlyingPx)[2]
);

void t144toFIX(__uint32_t MsgSeqNum, __uint64_t SendingTime);

void printBook();

__uint32_t getField32UI(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__uint32_t previousValue, __uint32_t initialValue, int isOptional, int operator);
__uint64_t getField64UI(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__uint64_t previousValue, __uint64_t initialValue, int isOptional, int operator);
__int64_t getField64I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__int64_t previousValue, __int64_t initialValue, int isOptional, int operator);
void getFieldS(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	char* value, char* previousValue, char* initialValue, int operator);
void getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	float* value, float* previousValue, int operator, 
	int expOperator, int mantOperator, __int32_t initialExp);
__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order, int isDecimal);

__uint64_t uint64Operator(__uint64_t value, __uint64_t previousValue, __uint64_t initialValue, int operator, 
	int PMapis1);
__int64_t int64Operator(__int64_t value, __int64_t previousValue, __int64_t initialValue, int operator, 
	int PMapis1);
__uint32_t uint32Operator(__uint32_t value, __uint32_t previousValue, __uint32_t initialValue, int operator, 
	int PMapis1);
void stringOperator(char* value, char* streamValue, char* previousValue, char* initialValue, int operator, 
	int PMapIs1);
float decimalOperator(float previousValue, __int32_t valueExp, __int32_t previousValueExp, __int32_t initialValueExp,
	__int64_t valueMan, __int64_t previousValueMan,
	int operatorEnt, int operatorExp, int operatorMan, 
	int PMapIs1);

char* bytetoStringDecoder(__uint8_t* field);
float bytetoDecimalDecoder(__uint8_t* field);
__uint32_t bytetoInt32Decoder(__uint8_t* field);
__uint64_t bytetoInt64Decoder(__uint8_t* field);
__uint32_t bytetoPMapDecoder(__uint8_t* field, __int32_t field_length);
__uint32_t fieldLength(__uint8_t* field);
int pMapCheck(__uint32_t PMap, int PMap_length, __uint32_t noCurrentField);
int isNegative(__int64_t val, __uint32_t size);
int isNullable(int isOptional, int operator);
int twosComplement(int number, int size);

void print64i(char* text, __int64_t var, char* buff);
void print64ui(char* text, __uint64_t var, char* buff);
void print32ui(char* text, __uint32_t var, char* buff);
void prints(char* text, char* var, char* buff);
void printvs(char* text, char (*var)[1500], char* buff);
void printd(char* text, float var, char* buff);

void test();
 
__uint64_t globalSecurityID = 3809639;
float book[10] = {9999.0, 9999.0, 9999.0, 9999.0, 9999.0, -9999.0, -9999.0, -9999.0, -9999.0, -9999.0};
float bidOrderDepthBook[10][2];
float offerOrderDepthBook[10][2];
int levels = 1;

int main () {	

	/*strcpy(valuation, "");
	c1 = clock();*/

	readMessage(openFile("51_Inc_FAST.bin"));
	//readMessage(openFile("filteredLog.bin"));
	printBook();

	//test();

    return 0;
}

void printBook(){
	/*printf("\n --Book----------------------------------- \n");
	printf("   SecurityID: %ld \n", globalSecurityID);
	if(levels == 1){
		printf("    -TOB--------------------------------\n");
		printf("     Best offer: %.2f \n", book[0]); //best option to buy
		printf("     Best bid: %.2f \n\n\n", book[5]); //best option to sell
	}*/

	printf("\n --Book-------------------------------------------------------------- \n");
	printf("   SecurityID: %ld \n\n", globalSecurityID);
	printf("    \t \t Bid \t \t \t \t Offer \n\n");
	printf("     PosNo \t Size \t Px \t \t Px \t Size \t PosNo \n");

	for(int i = 0; i < 10; i++){
		printf("      %.d \t %.d \t %.2f \t \t %.2f \t %.d \t %.d \n", i+1, (int) bidOrderDepthBook[i][0], bidOrderDepthBook[i][1], offerOrderDepthBook[i][1],
			(int) offerOrderDepthBook[i][0], i+1);
	}
	printf("\n");
}

void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, int FASTMessage_length){
	#define NONEBITMAP_145 0
	#define MDUPDATEACTION 1
	#define MDENTRYTYPE 2
	#define SECURITYID_145 3
	#define RPTSEQ_145 4           
	#define QUOTECONDITION_145 5
	#define MDENTRYPX_145 6
	#define MDENTRYINTERESTRATE_145 7
	#define	NUMBEROFORDERS_145 8
	#define MDENTRYTIME_145 9
	#define MDENTRYDATE_145 10
	#define MDINSERTDATE_145 11
	#define MDINSERTTIME_145 12
	#define MDSTREAMID_145 13
	#define CURRENCY_145 14
	#define NETCHGPREVDAY_145 15
	#define SELLERDAYS_145 16
	#define TICKDIRECTION_145 17
	#define ORDERID_145 18
	#define TRADEID_145 19
	#define MDENTRYBUYER_145 20
	#define MDENTRYSELLER_145 21
	#define MDENTRYPOSITIONNO_145 22
	#define PRICEBANDTYPE_145 23
	#define PRICELIMITTYPE_145 24
	#define LOWLIMITPRICE_145 25
	#define HIGHLIMITPRICE_145 26
	#define TRADINGREFERENCEPRICE_145 27

	#define UNDERLYINGPX_145 1

	__uint8_t* ptr_FASTMessage = FASTMessage+3; //MsgSeqNum is the first here but the third in the message
	__uint8_t field[1500] = {0x80}; //FIX/FAST encoded MD is no larger than 1420 bytes including the header

	//Template
	__uint32_t MsgSeqNum = UNDEFINED, TradeDate = UNDEFINED;
	__uint64_t SendintTime = 0;
	//SequenceMDEntries
	__uint32_t NoMDEntries = UNDEFINED, MDEntriesSequence_PMap = UNDEFINED; 
	__uint32_t MDEntriesSequence_PMap_length = UNDEFINED, NumberOfOrders[10] = {UNDEFINED};
	__uint32_t SellerDays[10] = {UNDEFINED}, TradingSessionID[10] = {UNDEFINED}, OpenCloseSettlFlag[10] = {UNDEFINED};
	__uint32_t MDEntryPositionNo[10] = {UNDEFINED}, SettPriceType[10] = {UNDEFINED}, LastTradeDate[10] = {UNDEFINED}; 
	__uint32_t PriceAdjustmentMethod[10] = {UNDEFINED}, PriceLimitType[10] = {UNDEFINED};
	__uint32_t PriceBandMidpointPriceType[10] = {UNDEFINED};
	__uint64_t AvgDailyTradedQty[10] = {0}, ExpireDate[10] = {0}, EarlyTermination[10] = {0}, MaxTradeVol[10] = {0};
	__uint32_t MDUpdateAction[10] = {UNDEFINED}, RptSeq[10] = {UNDEFINED}, MDEntryTime[10] = {UNDEFINED}; 
	__uint32_t MDEntryDate[10] = {UNDEFINED}, MDInsertDate[10] = {UNDEFINED}, MDInsertTime[10] = {UNDEFINED};
	__uint64_t SecurityID[10] = {0}, TradeVolume[10] = {0};
	__int64_t MDEntrySize[10] = {0}; 
	char MDEntryType[10][1500] = {"UNDEFINED"};
	char QuoteCondition[10][1500] = {"UNDEFINED"};
	char PriceType[10][1500] = {"UNDEFINED"}; //change to EMPTY
	char MDStreamID[10][1500] = {"UNDEFINED"};
	char Currency[10][1500] = {"UNDEFINED"};
	char TickDirection[10][1500] = {"UNDEFINED"};
	char TradeCondition[10][1500] = {"UNDEFINED"};
	char OrderID[10][1500] = {"UNDEFINED"};
	char TradeID[10][1500] = {"UNDEFINED"};
	char MDEntryBuyer[10][1500] = {"UNDEFINED"};
	char MDEntrySeller[10][1500] = {"UNDEFINED"};
	char PriceBandType[10][1500] = {"UNDEFINED"};
	float MDEntryPx[10][2] = {0.0}, MDEntryInterestRate[10][2] = {0.0}, NetChgPrevDay[10][2] = {0.0}, LowLimitPrice[10][2] = {0.0}; 
	float HighLimitPrice[10][2] = {0.0}, TradingReferencePrice[10][2] = {0.0};
	//SequenceUnderlyings
	__uint32_t UnderlyingsSequence_PMap = UNDEFINED, UnderlyingsSequence_PMap_length = UNDEFINED;
	__uint32_t NoUnderlyings = 0, UnderlyingPXType[10] = {0};
	__uint64_t UnderlyingSecurityID[10] = {0}; 
	float UnderlyingPx[10][2] = {0.0};
	//template
	__uint64_t IndexSeq[10] = {0};

	char* initialValueC = "EMPTY";
	int initialValueI = 0;

	MsgSeqNum = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
		MsgSeqNum, initialValueI, MANDATORY, NONEOPERATOR);

	SendintTime = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP_145, NONEBITMAP_145,	NONEBITMAP_145, 
		SendintTime, NONEOPERATOR, initialValueI, MANDATORY);

	TradeDate = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
		TradeDate, initialValueI, OPTIONAL, NONEOPERATOR); 

	NoMDEntries = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
		NoMDEntries, initialValueI, MANDATORY, NONEOPERATOR);

	if(NoMDEntries > 0){ //sequence
		for(int i = 0; i < NoMDEntries; i++){
			__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length,	
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 0);
			
			MDEntriesSequence_PMap_length = fieldLength(aux);
			MDEntriesSequence_PMap = bytetoInt32Decoder(aux);

			MDUpdateAction[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDUPDATEACTION,
				MDUpdateAction[i != 0 ? i-1 : i], 1, MANDATORY, COPY);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYTYPE,
				MDEntryType[i], MDEntryType[i != 0 ? i-1 : i], "0", COPY);

			SecurityID[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, SECURITYID_145,
				SecurityID[i != 0 ? i-1 : i], initialValueI, MANDATORY, COPY);		 //decrement if i != 0

			RptSeq[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, RPTSEQ_145, 
				RptSeq[i != 0 ? i-1 : i], initialValueI, MANDATORY, INCREMENT);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, QUOTECONDITION_145,  
				QuoteCondition[i], QuoteCondition[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYPX_145, 
				MDEntryPx[i], MDEntryPx[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, -2);
			
			getFieldD(&ptr_FASTMessage, FASTMessage_length,
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYINTERESTRATE_145,
				MDEntryInterestRate[i], MDEntryInterestRate[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, -2);
			
			NumberOfOrders[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, NUMBEROFORDERS_145,
				NumberOfOrders[i != 0 ? i-1 : i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				PriceType[i], PriceType[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			MDEntryTime[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYTIME_145,  
				MDEntryTime[i != 0 ? i-1 : i], initialValueI, MANDATORY, COPY);

			MDEntrySize[i] = getField64I(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,
				MDEntrySize[i != 0 ? i-1 : i], initialValueI, OPTIONAL, DELTA);

			MDEntryDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYDATE_145,  
				MDEntryDate[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);

			MDInsertDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDINSERTDATE_145, 
				MDInsertDate[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);
		
			MDInsertTime[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDINSERTTIME_145, 
				MDInsertTime[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
				MDEntriesSequence_PMap_length, MDSTREAMID_145,  
				MDStreamID[i], MDStreamID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
				MDEntriesSequence_PMap_length, CURRENCY_145, 
				Currency[i], Currency[i != 0 ? i-1 : i], initialValueC, COPY);
			
			getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, NETCHGPREVDAY_145, 
				NetChgPrevDay[i], NetChgPrevDay[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			SellerDays[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, SELLERDAYS_145, 
				SellerDays[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			TradeVolume[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				TradeVolume[i != 0 ? i-1 : i], initialValueI, OPTIONAL, DELTA);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TICKDIRECTION_145,  
				TickDirection[i], TickDirection[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,	 
				TradeCondition[i], TradeCondition[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			TradingSessionID[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				TradingSessionID[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			OpenCloseSettlFlag[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				OpenCloseSettlFlag[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, ORDERID_145,  
				OrderID[i], OrderID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TRADEID_145,  
				TradeID[i], TradeID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length,
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYBUYER_145, 
				MDEntryBuyer[i], MDEntryBuyer[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYSELLER_145,  
				MDEntrySeller[i], MDEntrySeller[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			MDEntryPositionNo[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYPOSITIONNO_145,  
				MDEntryPositionNo[i], initialValueI, OPTIONAL, NONEOPERATOR);
				
			SettPriceType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				SettPriceType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			LastTradeDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				LastTradeDate[i], initialValueI, OPTIONAL, NONEOPERATOR);

			PriceAdjustmentMethod[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				PriceAdjustmentMethod[i], initialValueI, OPTIONAL, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, PRICEBANDTYPE_145,
				PriceBandType[i], PriceBandType[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			PriceLimitType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, PRICELIMITTYPE_145, 
				PriceLimitType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, LOWLIMITPRICE_145,  
				LowLimitPrice[i], LowLimitPrice[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, HIGHLIMITPRICE_145, 
				HighLimitPrice[i], HighLimitPrice[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);

			getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TRADINGREFERENCEPRICE_145, 
				TradingReferencePrice[i], TradingReferencePrice[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			PriceBandMidpointPriceType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				PriceBandMidpointPriceType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			AvgDailyTradedQty[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,
				AvgDailyTradedQty[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			ExpireDate[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				ExpireDate[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			EarlyTermination[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				EarlyTermination[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			MaxTradeVol[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				MaxTradeVol[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			NoUnderlyings = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,  
				NoUnderlyings, initialValueI, MANDATORY, NONEOPERATOR);

			if(NoUnderlyings > 0){

				aux = getField(field, &ptr_FASTMessage, FASTMessage_length,	
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 0);
			
				UnderlyingsSequence_PMap_length = fieldLength(aux);
				UnderlyingsSequence_PMap = bytetoInt32Decoder(aux);

				UnderlyingSecurityID[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length,
					NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,
					UnderlyingSecurityID[i != 0 ? i-1 : i], initialValueI, MANDATORY, DELTA);

				getFieldD(&ptr_FASTMessage, FASTMessage_length, 
					UnderlyingsSequence_PMap, UnderlyingsSequence_PMap_length, UNDERLYINGPX_145, 
					UnderlyingPx[i], UnderlyingPx[i != 0 ? i-1 : i], NONEOPERATOR, 
					DEFAULT, DELTA, -2);
				
				UnderlyingPXType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length,
					NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145,
					UnderlyingPXType[i != 0 ? i-1 : i], initialValueI, OPTIONAL, NONEOPERATOR);
			}

			IndexSeq[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP_145, NONEBITMAP_145, NONEBITMAP_145, 
				IndexSeq[i], initialValueI, OPTIONAL, NONEOPERATOR);
		}
	}

	MD145Handler(NoMDEntries, MDUpdateAction, MDEntryType, RptSeq, QuoteCondition, SecurityID, MDEntryTime, MDInsertDate, MDInsertTime,
		MDEntryPx, NumberOfOrders, MDEntryDate, MDEntrySize, MDStreamID, ExpireDate, EarlyTermination, MDEntryBuyer, 
		MDEntrySeller, MDEntryPositionNo, OrderID);

	t145toFIX(
		//Template
		MsgSeqNum, TradeDate, SendintTime,
		//SequenceMDEntries
		NoMDEntries, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDUpdateAction, RptSeq, 
		NumberOfOrders, MDEntryTime, MDEntryDate, MDInsertDate, MDInsertTime, SellerDays, TradingSessionID, 
		OpenCloseSettlFlag, MDEntryPositionNo, SettPriceType, LastTradeDate, PriceAdjustmentMethod, 
		PriceLimitType, PriceBandMidpointPriceType, SecurityID, MDEntrySize, TradeVolume, AvgDailyTradedQty, 
		ExpireDate, EarlyTermination, MaxTradeVol, MDEntryType, QuoteCondition, PriceType, MDStreamID, 
		Currency, TickDirection, TradeCondition, OrderID, TradeID, MDEntryBuyer, MDEntrySeller, 
		PriceBandType, 
		MDEntryPx, MDEntryInterestRate, NetChgPrevDay, LowLimitPrice, HighLimitPrice, TradingReferencePrice,
		//SequenceUnderlyings
		NoUnderlyings, UnderlyingPXType, UnderlyingSecurityID, IndexSeq, UnderlyingPx
	);

	/*c2 = clock();
	tempo = (double)((c2 - c1)) / CLOCKS_PER_SEC;
	printf("\n decodeOneMessage145(): %d bytes", FASTMessage_length);
	printf("\n decodeOneMessage145(): %lf seg ", tempo);
	printf("\n decodeOneMessage145(): %.1lf bytes/s \n\n", (float) FASTMessage_length / tempo);
	sprintf(auxValuation, " decodeOneMessage145bs(): %.1lf bytes/s \n", (float) FASTMessage_length / tempo);
	strcat(valuation, auxValuation);
	fileV = fopen("valuation/decodeOneMessage145bs.txt", "a");
	fputs(valuation, fileV);
	fclose(fileV);*/
}

void MDHeartbeat_144(__uint8_t* FASTMessage, int FASTMessage_length){
	__uint8_t field[1500] = {0x00};
	unsigned int field_length = 0;
	unsigned int noTemplateField = 0;
	__uint32_t MsgSeqNum = 0;
	__uint64_t SendingTime = 0;

	for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){
    		noTemplateField++;
    		if(noTemplateField == 3){
				MsgSeqNum = bytetoInt32Decoder(field);
			}
			else if(noTemplateField == 4){
				SendingTime = bytetoInt64Decoder(field);
			}
			else if(!(noTemplateField == 0 || noTemplateField == 1 || noTemplateField == 2)){
				printf(" Field number %d do not identified: ", noTemplateField);
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf("\n");
			}
			field_length = 0;
    	}
    }
    t144toFIX(MsgSeqNum, SendingTime);
}

void MD145Handler(__uint32_t NoMDEntries, __uint32_t* MDUpdateAction, char (*MDEntryType)[1500], __uint32_t* RptSeq, char (*QuoteCondition)[1500], __uint64_t* SecurityID, 
__uint32_t* MDEntryTime, __uint32_t* MDInsertDate, __uint32_t* MDInsertTime, float (*MDEntryPx)[2], __uint32_t* NumberofOrders, 
__uint32_t* MDEntryDate, __uint64_t* MDEntrySize, char (*MDStreamID)[1500], __uint64_t* ExpireDate, __uint64_t* EarlyTermination, 
char (*MDEntryBuyer)[1500], char (*MDEntrySeller)[1500], __uint32_t* MDEntryPositionNo, char (*OrderID)[1500]){

	for(int i = 0; i < NoMDEntries; i++){
		if(SecurityID[i] == globalSecurityID && MDEntryPositionNo[i] < 11){
			if(MDUpdateAction[i] == 0){ //new
				if(MDEntryType[i][0] == '0'){ //bid
					for(int j = 9; j >= MDEntryPositionNo[i]; j--){ //shift
						bidOrderDepthBook[j][1] = bidOrderDepthBook[j-1][1];
						bidOrderDepthBook[j][0] = bidOrderDepthBook[j-1][0];
					}
					bidOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //add
					bidOrderDepthBook[MDEntryPositionNo[i] -1][0] = MDEntrySize[i];

					if(bidOrderDepthBook[MDEntryPositionNo[i] -1][1] == 0){
						bidOrderDepthBook[MDEntryPositionNo[i] -1][0] = 0;
					}
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					for(int j = 9; j >= MDEntryPositionNo[i]; j--){ //shift
						offerOrderDepthBook[j][1] = offerOrderDepthBook[j-1][1];
						offerOrderDepthBook[j][0] = offerOrderDepthBook[j-1][0];
					}
					offerOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //add
					offerOrderDepthBook[MDEntryPositionNo[i] -1][0] = MDEntrySize[i];
				}
			}
			else if(*MDUpdateAction == 1){ //change
				if(MDEntryType[i][0] == '0'){ //bid
					bidOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //change
					bidOrderDepthBook[MDEntryPositionNo[i] -1][0] = MDEntrySize[i]; //change
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					offerOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //change
					offerOrderDepthBook[MDEntryPositionNo[i] -1][0] = MDEntrySize[i]; //change
				}
			}
			else if(*MDUpdateAction == 2){ //delete
				if(MDEntryType[i][0] == '0'){ //bid
					bidOrderDepthBook[MDEntryPositionNo[i] - 1][1] = 0.0;
					bidOrderDepthBook[MDEntryPositionNo[i] - 1][0] = 0; 
					for(int j = MDEntryPositionNo[i] - 1; j < 10; j++){ 
						bidOrderDepthBook[MDEntryPositionNo[j]][1] = bidOrderDepthBook[j+1][1];
						bidOrderDepthBook[MDEntryPositionNo[j]][0] = bidOrderDepthBook[j+1][0];
					}
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					offerOrderDepthBook[MDEntryPositionNo[i] - 1][1] = 0.0;
					offerOrderDepthBook[MDEntryPositionNo[i] - 1][0] = 0;
					for(int j = MDEntryPositionNo[i]; j < 10; j++){
						offerOrderDepthBook[MDEntryPositionNo[j]][1] = offerOrderDepthBook[j+1][1];
						offerOrderDepthBook[MDEntryPositionNo[j]][0] = offerOrderDepthBook[j+1][0];
					}
				}
			}
			else if(*MDUpdateAction == 3){ //deletethu
				if(MDEntryType[i][0] == '0'){ //bid
					for(int j = 0; j < 10; j++){ //delete
						bidOrderDepthBook[j][1] = 0.0;
						bidOrderDepthBook[j][0] = 0;
					}
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					for(int j = 0; j < 10; j++){ //delete
						offerOrderDepthBook[j][1] = 0.0;
						offerOrderDepthBook[j][0] = 0;
					}
				}
			}
			else if(*MDUpdateAction == 4){ //deletefrom
				if(MDEntryType[i][0] == '0'){ //bid
					for(int j = 0; j < MDEntryPositionNo[i]; j++){ //delete
						bidOrderDepthBook[j][1] = 0.0;
						bidOrderDepthBook[j][0] = 0;
					}
					for(int j = 0; j < 10; j++){ //shift
						bidOrderDepthBook[j][1] = bidOrderDepthBook[j + MDEntryPositionNo[i]][1];
						bidOrderDepthBook[j][0] = bidOrderDepthBook[j + MDEntryPositionNo[i]][0];
					}
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					for(int j = 0; j < MDEntryPositionNo[i]; j++){
						offerOrderDepthBook[j][1] = 0.0;
						offerOrderDepthBook[j][0] = 0;
					}
					for(int j = 0; j < 10; j++){ //shift
						offerOrderDepthBook[j][1] = offerOrderDepthBook[j + MDEntryPositionNo[i]][1];
						offerOrderDepthBook[j][0] = offerOrderDepthBook[j + MDEntryPositionNo[i]][0];
					}
				}
			}
			else if(*MDUpdateAction == 5){ //overlay
				if(MDEntryType[i][0] == '0'){ //bid
					if(MDEntryPx[i][0] > 0){
						bidOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //overlay
					}
					else{ //delete
						bidOrderDepthBook[MDEntryPositionNo[i] - 1][1] = 0.0; 
						bidOrderDepthBook[MDEntryPositionNo[i] - 1][0] = 0; 
						for(int j = MDEntryPositionNo[i] - 1; j < 10; j++){ //shift
							bidOrderDepthBook[MDEntryPositionNo[j]][1] = bidOrderDepthBook[j+1][1];
							bidOrderDepthBook[MDEntryPositionNo[j]][0] = bidOrderDepthBook[j+1][0];
						}
					}
				}
				else if(MDEntryType[i][0] == '1'){ //offer
					if(MDEntryPx[i][0] > 0){
						offerOrderDepthBook[MDEntryPositionNo[i] -1][1] = MDEntryPx[i][0]; //overlay
					}
					else{
						offerOrderDepthBook[MDEntryPositionNo[i] - 1][1] = 0.0;
						offerOrderDepthBook[MDEntryPositionNo[i] - 1][0] = 0;
						for(int j = MDEntryPositionNo[i]; j < 10; j++){
							offerOrderDepthBook[MDEntryPositionNo[j]][1] = offerOrderDepthBook[j+1][1];
							offerOrderDepthBook[MDEntryPositionNo[j]][0] = offerOrderDepthBook[j+1][0];
						}
					}
				}
			}
		}
	}
}

void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, 
	__uint8_t* FASTMessage, int FASTMessage_length){
	
	switch(TemplateID)
	{
		case 144 : MDHeartbeat_144(FASTMessage, FASTMessage_length);
		break;

		case 145 : MDIncRefresh_145(PMap, FASTMessage, FASTMessage_length);
		break;

		default : printf(" TemplateID do not identified: %d \n", TemplateID);
	}
}

void identifyTemplate(__uint8_t* FASTMessage, int FASTMessage_length){
	__uint8_t field[1500];
    __uint32_t PMap = 0;
    __uint16_t TemplateID = 0;
    unsigned int field_length = 0;
	unsigned int noCurrentField = 0;

    for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){ //if MSB is 1
    		noCurrentField++;
    		if(noCurrentField == 1){
    			PMap = bytetoPMapDecoder(field, field_length);
				if(!(PMap & 0b01000000)){
					printf(" TemplateID do not specified in the message. \n");
				}
			}
			else if(noCurrentField == 2 && (PMap & 0b01000000)){
				TemplateID = bytetoPMapDecoder(field, field_length);
			}
			if(TemplateID > 0){
				templateDecoder(TemplateID, PMap, FASTMessage, FASTMessage_length);
				break;
			}
			strcpy(field, "");
			field_length = 0;
    	}
    }
}

void readMessage(FILE* file){
	__uint8_t header[10];
	__uint8_t byte;
	__uint8_t FASTMessage[1500]; //2 bytes of MsgLength is the limit
	unsigned int FASTMessage_length = 0;
	unsigned int current_field = 0;
	int MsgSeqNum = 0;
	int NoChunks = 0;
	int CurrentChunk = 0;
	int MsgLength = 0;

	for(int i = 0; i < 2000; i++){ // number of messages //1250
	//while(1){
		for(int i = 0; i < 10; i++){ //read header
			if(fread(&byte, 1, 1, file) > 0){
				header[i] = byte;
			}
			else{
				exit(1);
			}
		}
		MsgSeqNum = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | (header[3]); //concat the bytes
		NoChunks = (header[4] << 8) | (header[5]);
		CurrentChunk = (header[6] << 8) | (header[7]);
		MsgLength = (header[8] << 8) | (header[9]);

		for(int i = 0; i < MsgLength; i++){

			fread(&byte, 1, 1, file);

			FASTMessage[FASTMessage_length] = byte;
			FASTMessage_length++;
		}

		//to compare with the onix log
		if(MsgSeqNum > 731915 && MsgSeqNum < 732491){ //731915 732491){ //just to compare with the FIX log
			printf("\n---------------------------------------------------------------------------------------\n");
			//printf(" \n Message %d: \n", i+1);
			//printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, 
				//NoChunks, CurrentChunk, MsgLength);

			identifyTemplate(FASTMessage, FASTMessage_length);
		}
		//identifyTemplate(FASTMessage, FASTMessage_length);
		FASTMessage_length = 0;
	}
}

__uint32_t getField32UI(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__uint32_t previousValue, __uint32_t initialValue, int isOptional, int operator){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamValue[1500];
    __uint32_t value = 0;
  	int thereIsPMap = 0, PmapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the stream (nullable or not)
		__uint8_t* pt_value = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 0);
		value = bytetoInt32Decoder(pt_value);
	}

	if(isNullable(isOptional, operator) && ((int) previousValue) >= 0){
		previousValue+=1; //If an integer is nullable, every non-negative integer is incremented by 1 before it is encoded
	}

	value = uint32Operator(value, previousValue, initialValue, operator, PmapIs1);

    if(isNullable(isOptional, operator) && (int) value > 0 ){ 
		value--; //If an integer is nullable, every non-negative integer is incremented by 1 before it is encoded
	}
	else if(isNullable(isOptional, operator) && value == 0){ //added now
		value = ABSENT_32; //the value is absent
	}

	return value;
}

__uint32_t uint32Operator(__uint32_t value, __uint32_t previousValue, __uint32_t initialValue, 
	int operator, int PMapIs1){
	
	if(operator == COPY && !PMapIs1){ //if the value isnt present in stream, bcs if y the value in the stream = the new value
		if(previousValue != UNDEFINED && previousValue != ABSENT_32){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_32){ //EMPTY
			value = ABSENT_32; // 0;
		}
    }
    else if(operator == INCREMENT && !PMapIs1){
		if(previousValue != UNDEFINED && previousValue !=  ABSENT_32){ //assigned
			value = previousValue + 1; //the value of the field is the previous value +1
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_32){ //EMPTY
			value = ABSENT_32; 
		}
    }
    else if(operator == DELTA){
    	__int64_t delta = 0, base = 0;
    	delta = value; //the delta is present in the stream
    	if(previousValue != UNDEFINED && previousValue != ABSENT_32){ //} 0){ //assigned
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_32){ //EMPTY
			value = ABSENT_32; 
		}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    } 	

    return value;
}

__uint64_t getField64UI(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__uint64_t previousValue, __uint64_t initialValue, int isOptional, int operator){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamValue[1500];
	__uint64_t value = 0;
  	int thereIsPMap = 0, PmapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the stream (nullable or not)
		__uint8_t* pt_value = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 0);
		value = bytetoInt64Decoder(pt_value);
	}

	if(isNullable(isOptional, operator) && (((int) previousValue) >= 0)){
		previousValue+=1; //If an integer is nullable, every non-negative integer is incremented by 1 before it is encoded
	}

	value = uint64Operator(value, previousValue, initialValue, operator, PmapIs1);

    if(isNullable(isOptional, operator) && (int) value > 0){ 
		value--; //If an integer is nullable, every non-negative int is incremented by 1 before it is encoded
	}
	else if(isNullable(isOptional, operator) && value == 0){ 
		value = ABSENT_32; 
	}

	return value;
}

__uint64_t uint64Operator(__uint64_t value, __uint64_t previousValue, __uint64_t initialValue, 
	int operator, int PMapIs1){

	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_64){ //EMPTY
			value = ABSENT_64; 
		}
    }
    else if(operator == INCREMENT && !PMapIs1){
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned
			value++; //the value of the field is the previous value +1
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_64){ //EMPTY
			value = ABSENT_64; // 0;
		}
    }
    else if(operator == DELTA){
		__int64_t delta = 0, base = 0;
    	delta = value; //a delta value is present in the stream
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_64){ //EMPTY
			value = 0;
		}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    }

    return value;
}

__int64_t getField64I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	__int64_t previousValue, __int64_t initialValue, int isOptional, int operator){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamValue[1500];
	__int64_t value = 0;
  	int thereIsPMap = 0, PmapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the stream (nullable or not)
		__uint8_t* pt_value = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 0);

		value = bytetoInt64Decoder(pt_value);

		if(*pt_value == 0x80){
			return ABSENT_64;
		}
		
		int size = fieldLength(pt_value);
		size = (size * 8) - size; //- size because of the end bits

		value = twosComplement(value, size);
		if(isNegative(value, size)){
			value+=1; //nullable
		}
	}

	value = int64Operator(value, previousValue, initialValue, operator, PmapIs1);

    if(isNullable(isOptional, operator) && value != 0x00){
		value--; //If an integer is nullable, every non-negative int is incremented by 1 before it is encoded
	}
	else if(isNullable(isOptional, operator) && value == 0x00){ //added now
		value = ABSENT_64; 
	}

	return value;
}

__int64_t int64Operator(__int64_t value, __int64_t previousValue, __int64_t initialValue, int operator, 
	int PMapIs1){

	if(operator == COPY && !PMapIs1){  //value is not present in the stream, otherwise the stream value is the new value
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_64){ //EMPTY
			value = ABSENT_64;
		}
    }
    else if(operator == INCREMENT && !PMapIs1){
		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
			value++; //the value of the field is the previous value +1
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == 0){ //EMPTY
			value = ABSENT_64;
		}
    }
    else if(operator == DELTA){
		__int64_t delta = 0, base = 0;
    	delta = value; //a delta value is present in the stream
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned (think about a null code)
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT_64){ //EMPTY
			value = ABSENT_64;
		}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    }

    return value;
}

void getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	float* value, float* previousValue, int operator, 
	int expOperator, int mantOperator, __int32_t initialExp){


	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    __uint8_t streamValue[1500];

    __uint8_t* ptrExp = NULL;
    __uint8_t* ptrMant = NULL;
    int exp = 0;
    long int mant = 0;
    float decimal = 0.0;
    int thereIsPMap = 0, PMapIs1 = 0;
	int isAbsent = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PMapIs1 = 1;			
		}
	}
	int test1 = 0;
	int test2 = 0;
	if(thereIsPMap && PMapIs1){ //If set, the value appears in stream in a nullable representation. There's a exp in the msg

		ptrExp = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); 

		if(*ptrExp != 0x80){ //if it is non null
			exp = bytetoInt32Decoder(ptrExp); //decode the exp

			int size = fieldLength(ptrExp);
			size = (size * 8) - size; //- size because of the end bits

			if(isNegative(exp, size)){
				exp = twosComplement(exp, size);
			}
			else{
				exp-= 1; //nullable -1
			}
			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); //get the mant
			mant = bytetoInt64Decoder(ptrMant); 

			size = fieldLength(ptrMant);
			size = (size * 8) - size;

			if(isNegative(mant, size)){
				//printf("\n\n it is negative (%ld)", mant);
				mant = twosComplement(mant, size);
				//printf("\n Negative: %ld", mant);

			}
			/*else{
				printf("\n\n it is not negative  (%ld) \n\n", mant);
			}*/

			/*if(PMap_order == MDENTRYPX_145){
				printf("\nTest exp: %d ", exp);
				printf("\nTest mant: %ld \n", mant);
			}*/
		}
		else{
			isAbsent = 1;
		}
	}
	else{ //if the bit is 0, default exp = initialExp
		exp = -80;

		if(initialExp != 0){ 
			exp = initialExp;

			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); //get the mant
			mant = bytetoInt64Decoder(ptrMant); //decode the mantissa

			int size = fieldLength(ptrMant);
			size = (size * 8) - size; //- size because of the end bits

			mant = twosComplement(mant, size);
		}
	}

	float aux = pow(10, (__int32_t) previousValue[1]);
	__int64_t previousMant = previousValue[0] / aux;

	/*if(PMap_order == MDENTRYPX_145){
		printf("\nTest previousMant: %ld \n", previousMant);
	}*/

	if(isAbsent == 1){
		value[0] = 0.0;
		value[1] = 0; 
	}
	else{
		decimal = decimalOperator(*previousValue, exp, (__int32_t) previousValue[1], initialExp, mant, previousMant, 
		operator, expOperator, mantOperator, PMapIs1);

		value[0] = decimal;
		value[1] = exp; 

		/*if(PMap_order == MDENTRYPX_145){
			printf("\nTest decimal: %.2f \n", decimal);
		}*/
	}
}

float decimalOperator(float previousValue, __int32_t valueExp, __int32_t previousValueExp, __int32_t initialValueExp,
	__int64_t valueMan, __int64_t previousValueMan,
	int operatorEnt, int operatorExp, int operatorMan, 
	int PMapIs1){

	if(operatorEnt != NONEOPERATOR || (operatorExp == NONEOPERATOR && operatorMan == NONEOPERATOR)){
		int scaledNumber = 1;
	}

	if(operatorExp == DEFAULT && valueExp == -80){ //null
		valueExp = initialValueExp;
	}

	if(operatorMan == DELTA && valueExp != -80){ 
		__int64_t delta = 0, base = 0;
		delta = valueMan; //value in the stream
		if(previousValue != UNDEFINED && previousValue != ABSENT_64){ //assigned
			base = previousValueMan;
			valueMan = base + delta;
			float decimal = pow(10, valueExp);
		}
	}
		
	float decimal = pow(10, valueExp);

	return valueMan * decimal;
}

void getFieldS(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	char* value, char* previousValue, char* initialValue, int operator){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamField[1500];

  	int thereIsPMap = 0, PmapIs1 = 0;
  	char auxValue[1500], streamValue[1500] = {0};

	if(PMap_order > 0){ //if the field has a bit representation in bitmap
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the field (nullable or not)
		__uint8_t* pt_streamValue = getField(streamField, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 0);
		strcpy(streamValue, pt_streamValue); //get the stream value
	}

	strcpy(streamValue, bytetoStringDecoder(streamValue)); //decode the stream value

	stringOperator(auxValue, streamValue, previousValue, initialValue, operator, PmapIs1); //apply operator

	strcpy(value, auxValue); //copy to the new value of the field
}

void stringOperator(char* value, char* streamValue, char* previousValue, char* initialValue, 
	int operator, int PMapIs1){

	char auxValue[1500];
	strcpy(auxValue, value);

	if(operator == NONEOPERATOR || PMapIs1){
		strcpy(auxValue, streamValue); //if there's no operator or PMap is 1, the new value is the stream value
	}
    else if(operator == COPY && !PMapIs1){ //there is operator and is COPY
		if(strcmp(previousValue, "UNDEFINED") != 0 && strcmp(previousValue, "EMPTY") != 0){ //assigned
			strcpy(auxValue, previousValue); //the value of the field is the previous value
		}
		else if(strcmp(previousValue, "UNDEFINED") == 0){ //undefined 
			strcpy(auxValue, initialValue); //the value of the field is the initial value
		}
		else if(strcmp(previousValue, "EMPTY") == 0){ //EMPTY
			strcpy(auxValue, "EMPTY"); 
		}
    }
    strcpy(value, auxValue); //copy the new value to the value
}

__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order, int isDecimal){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    int field_length = 0;

	for(int i = 0; i < 1500; i++){ //clean the buffer
		newField[i] = 0x00;
	}

	if(PMap_order > 0){
		if(!(pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 0 (!1)
			if(!isDecimal){ //if bitsmap's bit is 0 and is not decimal, return NULL
				newField[0] = 0x80; //biggest negative number and null symbol (need to think about this character)
				return newField;
			}
		}
	}

	for(int i = 0; i < FASTMessage_length; i++){
		newField[field_length] = *(*FASTMessage); //newField gets the bytes of FASTMessage
    	field_length++;
    	*FASTMessage = *FASTMessage+1; //increments the address of the ptr
    	if((newField[field_length-1] >> 7) & 0b00000001){ //if it is the end of the fild
	    	return newField;
    	}
    }
}

int twosComplement(int number, int size){
    const int isNegative = (number & (1 << size - 1)) != 0;
    
    if(isNegative){
      return number | ~((1 << size) - 1);
    }
    else{
      return number;
    }
}

int isNullable(int isOptional, int operator){
	if(isOptional == OPTIONAL && operator == NONEOPERATOR){
		return 1;
	}
	else if(operator == DEFAULT || operator == COPY || operator == INCREMENT){
		if(isOptional == MANDATORY){
			return 0;
		}
		else if(isOptional == OPTIONAL){
			return 1;
		}
	}
	else if(operator == DELTA){
		if(isOptional == MANDATORY){
			return 0;
		}
		else if(isOptional == OPTIONAL){
			return 1;
		}
	}	
}

int isNegative(int64_t number, __uint32_t size){
	return (number & (1 << size - 1)) != 0;
}

float bytetoDecimalDecoder(__uint8_t* field){
	field = field+1;
    __int32_t field_length = fieldLength(field);
    int j = field_length - 2;
    __uint32_t result;
    float value = 0.0;
    
    result = field[field_length-1];

    if(field_length == 1){ //to do not get negative adds
    	field_length = 2;
    }
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 33 - ((i + 1) * 8); //save only the 7 LSB in a 32 bits buffer
        result = result >> 32 - ((i + 1) * 8); // ´´
        result = (field[j] << ((i + 1) * 8)) | result; //concat with the next byte
        j--;
    } 

    result = result >> (field_length-1); 
    value = result * 0.01;
    return value;
}

__uint32_t bytetoInt32Decoder(__uint8_t* field){
    __int32_t field_length = fieldLength(field);
    int j = field_length - 2;
    __uint32_t result;
    
    result = field[field_length-1];

    if(field_length == 1){ //to do not get negative adds
    	field_length = 2;
    }
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 33 - ((i + 1) * 8); //save only the 7 LSB in a 32 bits buffer
        result = result >> 32 - ((i + 1) * 8); // ´´
        result = (field[j] << ((i + 1) * 8)) | result; //concat with the next byte
        j--;
    } 

    result = result >> (field_length-1); 
    
    return result;
}

__uint32_t bytetoPMapDecoder(__uint8_t* field, __int32_t field_length){
    int j = field_length - 2;
    __uint32_t result;
    
    result = field[field_length-1];

    if(field_length == 1){ //to do not get negative adds
    	field_length = 2;
    }
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 33 - ((i + 1) * 8); //save only the 7 LSB in a 32 bits buffer
        result = result >> 32 - ((i + 1) * 8); // ´´
        result = (field[j] << ((i + 1) * 8)) | result; //concat with the next byte
        j--;
    } 

    result = result >> (field_length-1); 
    
    return result;
}

__uint64_t bytetoInt64Decoder(__uint8_t* field){
    __int32_t field_length = fieldLength(field);
    int j = field_length - 2;
    __uint64_t result, aux;
    
    result = field[field_length-1];

    if(field_length == 1){ //to do not get negative adds
    	field_length = 2;
    }
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 65 - ((i + 1) * 8); //save only the 7 LSB in a 64 bits buffer
        result = result >> 64 - ((i + 1) * 8); // ''
        aux = field[j];
        result = (aux << ((i + 1) * 8)) | result; //concat with the next byte
        j--;
    } 

    result = result >> (field_length-1); 
    
    return result;
}

char* bytetoStringDecoder(__uint8_t* field){
	__int32_t field_length = fieldLength(field);
	char result[field_length], aux;
	
	if(*field == 0x00 || *field == 0x80){ //if the bmap is 0, the value = 0x00, if the field hasnt bit(optional), value: 0x80
		strcpy(result, "EMPTY");
	}
	else{
		for(int i = 0; i < field_length + 1; i++){
			if((field[i] >> 7) & 0b00000001){ //if MSB is 1
				field[i] = field[i] << 1; //save only the 7 LSB in a 8 bits buffer
	        	field[i] = field[i] >> 1; //
			}
			result[i] = field[i];
		}
	}

	strcpy(field, result);
	return field;
}

int pMapCheck(__uint32_t PMap, int PMap_length, __uint32_t noCurrentField){
	__uint32_t aux_bitMap = 0b00000000000000000000000000000001;

	if(PMap & (aux_bitMap << ((PMap_length * 8) - PMap_length - noCurrentField))){ //if bitsmap's bit is 1
		return 1;
	}
	else{
		return 0;
	}
}

__uint32_t fieldLength(__uint8_t* field){
	__uint8_t* aux = field;
	__int32_t counter = 0;

	int stop = 1;
	while(stop){
		counter++;
		if(*aux >> 7 & 0b00000001){ //if MSB is 1 -> last byte of the field -> so stop in the next loop 
			stop = 0;
		}
		*aux++;
	}
	return counter;
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); //read binary
   return file;
}

void t144toFIX(__uint32_t MsgSeqNum, __uint64_t SendingTime){
	char buff[1500];
	strcpy(buff, "");
	FILE *file;

	file = fopen("validation/logFIX51-FASTEngineMD.txt", "a");

	char* ApplVerID = "9";
	char* MsgType = "0";
	int SecurityIDSource = 8;
	char* SecurityExchange = "BVMF";

	//template
	prints("1128=%s|", ApplVerID, buff);
	prints("35=%s|", MsgType, buff);
	print32ui("34=%d|", MsgSeqNum, buff);
	print64ui("52=%ld|", SendingTime, buff);

	printf("\n");

	strcat(buff, "\n");

	fputs(buff, file);
	fclose(file);
}

void t145toFIX(
	//Template
	__uint32_t MsgSeqNum, __uint32_t TradeDate, __uint64_t SendingTime,
	//SequenceMDEntries
	__uint32_t NoMDEntries, __uint32_t MDEntriesSequence_PMap, __uint32_t MDEntriesSequence_PMap_length, 
	__uint32_t* MDUpdateAction, __uint32_t* RptSeq, __uint32_t* NumberOfOrders, __uint32_t* MDEntryTime, 
	__uint32_t* MDEntryDate, __uint32_t* MDInsertDate, __uint32_t* MDInsertTime, __uint32_t* SellerDays, 
	__uint32_t* TradingSessionID, __uint32_t* OpenCloseSettlFlag, __uint32_t* MDEntryPositionNo, __uint32_t* SettPriceType, 
	__uint32_t* LastTradeDate, __uint32_t* PriceAdjustmentMethod, __uint32_t* PriceLimitType, 
	__uint32_t* PriceBandMidpointPriceType, __uint64_t* SecurityID, __uint64_t* MDEntrySize, __uint64_t* TradeVolume, 
	__uint64_t* AvgDailyTradedQty, __uint64_t* ExpireDate, __uint64_t* EarlyTermination, __uint64_t* MaxTradeVol, 
	char (*MDEntryType)[1500], char (*QuoteCondition)[1500], char (*PriceType)[1500], char (*MDStreamID)[1500], 
	char (*Currency)[1500], char (*TickDirection)[1500], char (*TradeCondition)[1500], char (*OrderID)[1500], 
	char (*TradeID)[1500], char (*MDEntryBuyer)[1500], char (*MDEntrySeller)[1500], char (*PriceBandType)[1500], 
	float (*MDEntryPx)[2], float (*MDEntryInterestRate)[2], float (*NetChgPrevDay)[2], float (*LowLimitPrice)[2], float (*HighLimitPrice)[2], 
	float (*TradingReferencePrice)[2],
	//SequenceUnderlyings
	__uint32_t NoUnderlyings, __uint32_t* UnderlyingPXType, __uint64_t* UnderlyingSecurityID, __uint64_t* IndexSeq,
	float (*UnderlyingPx)[2]
){
	char buff[1500];
	strcpy(buff, "");
	FILE *file;

	file = fopen("validation/logFIX51-FASTEngineMD.txt", "a");

	char* ApplVerID = "9";
	char* MsgType = "X";
	int SecurityIDSource = 8;
	char* SecurityExchange = "BVMF";

	//template
	prints("1128=%s|", ApplVerID, buff);
	prints("35=%s|", MsgType, buff);
	print32ui("34=%d|", MsgSeqNum, buff);
	print64ui("52=%ld|", SendingTime, buff);
	print32ui("75=%d|", TradeDate, buff);

	//SequenceMDEntries
	if(NoMDEntries > 0){ //sequence
		print32ui("268=%d|", NoMDEntries, buff);
		for(int i = 0; i < NoMDEntries; i++){
			print32ui("279=%d|", MDUpdateAction[i], buff);
			printvs("269=%s|", MDEntryType + i, buff);
			print32ui("22=%d|", SecurityIDSource, buff); //cte
			prints("207=%s|", SecurityExchange, buff);
			print64ui("48=%ld|", SecurityID[i], buff);
			print32ui("83=%d|", RptSeq[i], buff);
			printvs("276=%s|", QuoteCondition + i, buff);
			printd("270=%.4g|", MDEntryPx[i][0], buff);
			printd("37014=%.2f|", MDEntryInterestRate[i][0], buff);
			print32ui("346=%d|", NumberOfOrders[i], buff);
			printvs("423=%s|", PriceType + i, buff);
			print32ui("273=%d|", MDEntryTime[i], buff);
			print64i("271=%ld|", MDEntrySize[i], buff);
			print32ui("272=%d|", MDEntryDate[i], buff);
			print32ui("37016=%d|", MDInsertDate[i], buff);
			print32ui("37017=%d|", MDInsertTime[i], buff);
			printvs("1500=%s|", MDStreamID + i, buff);
			printvs("15=%s|", Currency + i, buff);
			printd("451=%.2f|", NetChgPrevDay[i][0], buff);
			print32ui("287=%d|", SellerDays[i], buff);
			print64ui("1020=%ld|", TradeVolume[i], buff);
			printvs("274=%s|", TickDirection + i, buff);
			printvs("277=%s|", TradeCondition + i, buff);
			print32ui("336=%d|", TradingSessionID[i], buff);
			print32ui("286=%d|", OpenCloseSettlFlag[i], buff);
			printvs("37=%s|", OrderID + i, buff);
			printvs("1003=%s|", TradeID + i, buff);
			printvs("288=%s|", MDEntryBuyer + i, buff);
			printvs("289=%s|", MDEntrySeller + i, buff);
			print32ui("290=%d|", MDEntryPositionNo[i], buff);
			print32ui("731=%d|", SettPriceType[i], buff);
			print32ui("9325=%d|", LastTradeDate[i], buff);	
			print32ui("37013=%d|", PriceAdjustmentMethod[i], buff);
			printvs("6939=%s|", PriceBandType + i, buff);
			print32ui("1306=%d|", PriceLimitType[i], buff);
			printd("1148=%.2f|", LowLimitPrice[i][0], buff);
			printd("1149=%.2f|", HighLimitPrice[i][0], buff);
			printd("1150=%.2f|", TradingReferencePrice[i][0], buff);
			print32ui("37008=%d|", PriceBandMidpointPriceType[i], buff);
			print64ui("37003=%ld|", AvgDailyTradedQty[i], buff);
			print64ui("432=%ld|", ExpireDate[i], buff);
			print64ui("37019=%ld|", EarlyTermination[i], buff);
			print64ui("1140=%ld|", MaxTradeVol[i], buff);

			//SequenceUnderlyings
			if(NoUnderlyings > 0){
				print32ui("711=%d|", NoUnderlyings, buff);
			}

			print64ui("37100=%ld", IndexSeq[i], buff);
		}
	}

	printf("\n");

	strcat(buff, "\n");

	fputs(buff, file);
	fclose(file);
}

void print64ui(char* text, __uint64_t var, char* buff){
	if((int) var > 0){
		char auxBuff[1500];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void print64i(char* text, __int64_t var, char* buff){ 
	if(var != ABSENT_64){ 
		char auxBuff[1500];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void print32ui(char* text, __uint32_t var, char* buff){
	if((int) var >= 0){
		char auxBuff[1500];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void printvs(char* text, char (*var)[1500], char* buff){
	if(strcmp(var[0], "EMPTY") != 0 && var[0][0] != '\0'){
		char auxBuff[1500];
		sprintf(auxBuff, text, var[0]);
		strcat(buff, auxBuff);
		PRINTS(text, var);
	}
}

void prints(char* text, char* var, char* buff){ 
	if(strcmp(var, "EMPTY") != 0 && *var != '\0'){
		char auxBuff[1500];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTS(text, var);
	}
}

void printd(char* text, float var, char* buff){
	if(var > 0.00){
		char auxBuff[1500];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTD(text, var);
	}
}

void test2(float (*preco)[2]){ 
	
	printf("\n exp: %d ", (__int32_t) preco[0][1]);
	printf("\n preco: %.2f ", preco[0][0]);

	float decimal = pow(10, (__int32_t) preco[0][1]);
	__int64_t mant = preco[0][0] / decimal;

	printf("\n mant: %ld ", mant);

	printf("\n");

}

void test(){
	__uint8_t field[2] = {0x0a, 0x85};
	__int64_t mant = bytetoInt64Decoder(field);
	__int32_t exp = -2;

	float decimal = pow(10, exp);
	decimal = mant * decimal;

	float preco[10][2] = {decimal, exp};

	test2(preco);
}