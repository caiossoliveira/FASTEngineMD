#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "t145toFIXFile.h"
 
FILE* openFile(char* fileName);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, 
	__uint8_t* FASTMessage, int FASTMessage_length);

void MDHeartbeat_144(__uint8_t* FASTMessage, int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, int FASTMessage_length);

void MD145Handler(__uint32_t MDUpdateAction, char* MDEntryType, __uint32_t RptSeq, char* QuoteCondition, 
	__uint64_t SecurityID, __uint32_t MDEntryTime, float MDEntryPx, __uint64_t MDEntrySize);

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

float getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	float previousValue, int operator, 
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
float decimalOperator(float previousValue, __int64_t valueExp, __int64_t previousValueExp, __int64_t initialValueExp,
	__int64_t valueMan, __int64_t previousValueMan, __int64_t initialValueMan, 
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
//int isNegative64(int64_t val, __uint32_t size);
int isNullable(int isOptional, int operator);
int twosComplement(int number, int size);
 
__uint64_t globalSecurityID = 3809639;
float book[10] = {9999.0, 9999.0, 9999.0, 9999.0, 9999.0, -9999.0, -9999.0, -9999.0, -9999.0, -9999.0};
int levels = 1;

int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
	//test();

	/*printf("\n --Book----------------------------------- \n");
	printf("   SecurityID: %ld \n", globalSecurityID);
	if(levels == 1){
		printf("    -TOB--------------------------------\n");
		printf("     Best offer: %.2f \n", book[0]);
		printf("     Best bid: %.2f \n\n\n", book[5]);
	}*/

    return 0;
}

void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, int FASTMessage_length){
	#define NONEBITMAP 0
	#define MDUPDATEACTION 1
	#define MDENTRYTYPE 2
	#define SECURITYID 3
	#define RPTSEQ 4
	#define QUOTECONDITION 5
	#define MDENTRYPX 6
	#define MDENTRYINTERESTRATE 7
	#define	NUMBEROFORDERS 8
	#define MDENTRYTIME 9
	#define MDENTRYDATE 10
	#define MDINSERTDATE 11
	#define MDINSERTTIME 12
	#define MDSTREAMID 13
	#define CURRENCY 14
	#define NETCHGPREVDAY 15
	#define SELLERDAYS 16
	#define TICKDIRECTION 17
	#define ORDERID 18
	#define TRADEID 19
	#define MDENTRYBUYER 20
	#define MDENTRYSELLER 21
	#define MDENTRYPOSITIONNO 22
	#define PRICEBANDTYPE 23
	#define PRICELIMITTYPE 24
	#define LOWLIMITPRICE 25
	#define HIGHLIMITPRICE 26
	#define TRADINGREFERENCEPRICE 27

	#define UNDERLYINGPXTYPE 1

	#define NONEOPERATOR 0
	#define DEFAULT 1
	#define COPY 2
	#define INCREMENT 3
	#define DELTA 4

	#define OPTIONAL 1
	#define MANDATORY 0

	#define UNDEFINED -90
	#define ABSENT 0xF0000000 //biggest negative number

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
	__int64_t MDEntrySize[10] = {0}; //it'll need a special function for signal int bcs of the signal bit calculation
	char MDEntryType[10][1000] = {"UNDEFINED"};
	char QuoteCondition[10][1000] = {"UNDEFINED"};
	char PriceType[10][1000] = {"UNDEFINED"}; //change to EMPTY
	char MDStreamID[10][1000] = {"UNDEFINED"};
	char Currency[10][1000] = {"UNDEFINED"};
	char TickDirection[10][1000] = {"UNDEFINED"};
	char TradeCondition[10][1000] = {"UNDEFINED"};
	char OrderID[10][1000] = {"UNDEFINED"};
	char TradeID[10][1000] = {"UNDEFINED"};
	char MDEntryBuyer[10][1000] = {"UNDEFINED"};
	char MDEntrySeller[10][1000] = {"UNDEFINED"};
	char PriceBandType[10][1000] = {"UNDEFINED"};
	float MDEntryPx[10] = {0.0}, MDEntryInterestRate[10] = {0.0}, NetChgPrevDay[10] = {0.0}, LowLimitPrice[10] = {0.0}; 
	float HighLimitPrice[10] = {0.0}, TradingReferencePrice[10] = {0.0};
	//SequenceUnderlyings
	__uint32_t NoUnderlyings = 0, UnderlyingPXType = 0;
	__uint64_t UnderlyingSecurityID = 0; 
	float UnderlyingPx = 0.0;
	//template
	__uint64_t IndexSeq[10] = {0};

	char* initialValueC = "EMPTY";
	int initialValueI = 0;

	MsgSeqNum = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP, NONEBITMAP, NONEBITMAP, 
		MsgSeqNum, initialValueI, MANDATORY, NONEOPERATOR);

	SendintTime = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP, NONEBITMAP,	NONEBITMAP, 
		SendintTime, NONEOPERATOR, initialValueI, MANDATORY);

	TradeDate = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP, NONEBITMAP, NONEBITMAP, 
		TradeDate, initialValueI, OPTIONAL, NONEOPERATOR); 

	NoMDEntries = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
		NONEBITMAP, NONEBITMAP, NONEBITMAP, 
		NoMDEntries, initialValueI, MANDATORY, NONEOPERATOR);

	if(NoMDEntries > 0){ //sequence
		for(int i = 0; i < NoMDEntries; i++){
			__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length,	
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 0);
			
			MDEntriesSequence_PMap_length = fieldLength(aux);
			MDEntriesSequence_PMap = bytetoInt32Decoder(aux);

			MDUpdateAction[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDUPDATEACTION,
				MDUpdateAction[i != 0 ? i-1 : i], 1, MANDATORY, COPY);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYTYPE,
				MDEntryType[i], MDEntryType[i != 0 ? i-1 : i], "0", COPY);

			SecurityID[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, SECURITYID,
				SecurityID[i != 0 ? i-1 : i], initialValueI, MANDATORY, COPY);		 //decrement if i != 0

			RptSeq[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, RPTSEQ, 
				RptSeq[i != 0 ? i-1 : i], initialValueI, MANDATORY, INCREMENT);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, QUOTECONDITION,  
				QuoteCondition[i], QuoteCondition[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			MDEntryPx[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYPX, 
				MDEntryPx[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, -2);
			
			MDEntryInterestRate[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length,
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYINTERESTRATE,
				MDEntryInterestRate[i != 0 ? i-1 : i], NONEOPERATOR, 
				DEFAULT, DELTA, -2);
			
			NumberOfOrders[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, NUMBEROFORDERS,
				NumberOfOrders[i != 0 ? i-1 : i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				PriceType[i], PriceType[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			MDEntryTime[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYTIME,  
				MDEntryTime[i != 0 ? i-1 : i], initialValueI, MANDATORY, COPY);

			MDEntrySize[i] = getField64I(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,
				MDEntrySize[i != 0 ? i-1 : i], initialValueI, OPTIONAL, DELTA);

			MDEntryDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYDATE,  
				MDEntryDate[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);

			MDInsertDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDINSERTDATE, 
				MDInsertDate[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);
		
			MDInsertTime[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDINSERTTIME, 
				MDInsertTime[i != 0 ? i-1 : i], initialValueI, OPTIONAL, COPY);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
				MDEntriesSequence_PMap_length, MDSTREAMID,  
				MDStreamID[i], MDStreamID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
				MDEntriesSequence_PMap_length, CURRENCY, 
				Currency[i], Currency[i != 0 ? i-1 : i], initialValueC, COPY);
			
			NetChgPrevDay[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, NETCHGPREVDAY, 
				NetChgPrevDay[i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			SellerDays[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, SELLERDAYS, 
				SellerDays[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			TradeVolume[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				TradeVolume[i != 0 ? i-1 : i], initialValueI, OPTIONAL, DELTA);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TICKDIRECTION,  
				TickDirection[i], TickDirection[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,	 
				TradeCondition[i], TradeCondition[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			TradingSessionID[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				TradingSessionID[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			OpenCloseSettlFlag[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				OpenCloseSettlFlag[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, ORDERID,  
				OrderID[i], OrderID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TRADEID,  
				TradeID[i], TradeID[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length,
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYBUYER, 
				MDEntryBuyer[i], MDEntryBuyer[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYSELLER,  
				MDEntrySeller[i], MDEntrySeller[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			MDEntryPositionNo[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDENTRYPOSITIONNO,  
				MDEntryPositionNo[i], initialValueI, OPTIONAL, NONEOPERATOR);
				
			SettPriceType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				SettPriceType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			LastTradeDate[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				LastTradeDate[i], initialValueI, OPTIONAL, NONEOPERATOR);

			PriceAdjustmentMethod[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				PriceAdjustmentMethod[i], initialValueI, OPTIONAL, NONEOPERATOR);

			getFieldS(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, PRICEBANDTYPE,
				PriceBandType[i], PriceBandType[i != 0 ? i-1 : i], initialValueC, NONEOPERATOR);

			PriceLimitType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, PRICELIMITTYPE, 
				PriceLimitType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			LowLimitPrice[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, LOWLIMITPRICE,  
				LowLimitPrice[i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			HighLimitPrice[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, HIGHLIMITPRICE, 
				HighLimitPrice[i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);

			TradingReferencePrice[i] = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
				MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, TRADINGREFERENCEPRICE, 
				TradingReferencePrice[i], NONEOPERATOR, 
				DEFAULT, DELTA, 0);
			
			PriceBandMidpointPriceType[i] = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				PriceBandMidpointPriceType[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			AvgDailyTradedQty[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,
				AvgDailyTradedQty[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			ExpireDate[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				ExpireDate[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			EarlyTermination[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				EarlyTermination[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			MaxTradeVol[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				MaxTradeVol[i], initialValueI, OPTIONAL, NONEOPERATOR);
			
			NoUnderlyings = getField32UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP,  
				NoUnderlyings, initialValueI, MANDATORY, NONEOPERATOR);

			if(NoUnderlyings > 0){}

			IndexSeq[i] = getField64UI(&ptr_FASTMessage, FASTMessage_length, 
				NONEBITMAP, NONEBITMAP, NONEBITMAP, 
				IndexSeq[i], initialValueI, OPTIONAL, NONEOPERATOR);
		}
	}
	//MD145Handler(MDUpdateAction, MDEntryType, RptSeq, QuoteCondition, SecurityID, MDEntryTime, MDEntryPx, MDEntrySize);

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
    //t144toFIX(MsgSeqNum, SendingTime);
}

void MD145Handler(__uint32_t MDUpdateAction, char* MDEntryType, __uint32_t RptSeq, char* QuoteCondition, 
	__uint64_t SecurityID, __uint32_t MDEntryTime, float MDEntryPx, __uint64_t MDEntrySize){

	if(SecurityID == globalSecurityID){
		if(MDUpdateAction == 0){ //new
			if(levels == 1){
				if(*MDEntryType == '0'){ //bid
					book[0] = MDEntryPx;
				}
				else if(*MDEntryType == '1'){ //offer
					book[5] = MDEntryPx;
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

	for(int i = 0; i < 1250; i++){ // number of messages //1250
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
		if(MsgSeqNum > 731952 && MsgSeqNum < 732034){ //731915){ //only to compare with the FIX log
			printf("\n-----------------------------------------------------------------------------------------------------\n");
			//printf(" \n Message %d: \n", i+1);
			//printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);
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
		value = ABSENT; //the value is absent
	}

	return value;
}

__uint32_t uint32Operator(__uint32_t value, __uint32_t previousValue, __uint32_t initialValue, 
	int operator, int PMapIs1){
	
	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT; // 0;
		}
    }
    else if(operator == INCREMENT && !PMapIs1){
		if(previousValue != UNDEFINED && previousValue !=  ABSENT){ //assigned
			value = previousValue + 1; //the value of the field is the previous value +1
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT; 
		}
    }
    else if(operator == DELTA){
    	__int64_t delta = 0, base = 0;
    	delta = value; //the delta is present in the stream
    	if(previousValue != UNDEFINED && previousValue != ABSENT){ //} 0){ //assigned
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT; 
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
		value = ABSENT; 
	}

	return value;
}

__uint64_t uint64Operator(__uint64_t value, __uint64_t previousValue, __uint64_t initialValue, 
	int operator, int PMapIs1){

	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT; 
		}
    }
    else if(operator == INCREMENT && !PMapIs1){
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned
			value++; //the value of the field is the previous value +1
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT; // 0;
		}
    }
    else if(operator == DELTA){
		__int64_t delta = 0, base = 0;
    	delta = value; //a delta value is present in the stream
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
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
		value = ABSENT; 
	}

	return value;
}

__int64_t int64Operator(__int64_t value, __int64_t previousValue, __int64_t initialValue, int operator, 
	int PMapIs1){

	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT;
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
			value = ABSENT;
		}
    }
    else if(operator == DELTA){
		__int64_t delta = 0, base = 0;
    	delta = value; //a delta value is present in the stream
		if(previousValue != UNDEFINED && previousValue != ABSENT){ //assigned (think about a null code)
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == ABSENT){ //EMPTY
			value = ABSENT;
		}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    }

    return value;
}

float getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, int PMap_length, int PMap_order,
	float previousValue, int operator, 
	int expOperator, int mantOperator, __int32_t initialExp){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    __uint8_t streamValue[1500];

    __uint8_t* ptrExp = NULL;
    __uint8_t* ptrMant = NULL;
    int exp = 0;
    long int mant = 0;
    float decimal = 0.0;
    int thereIsPMap = 0, PMapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PMapIs1 = 1;			
		}
	}

	if(thereIsPMap && PMapIs1){ //If set, the value appears in the stream in a nullable representation
		//printf("\nThere is pmap and is 1: %d ", PMap_order);
		ptrExp = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); //there is a exp in the msg
		if(*ptrExp != 0x80){ //if it is non null
			exp = bytetoInt32Decoder(ptrExp); //decode the exp

			int size = fieldLength(ptrExp);
			size = (size * 8) - size; //- size because of the end bits

			if(isNegative(exp, size)){
				//exp-= 128; //2's complement
				exp = twosComplement(exp, size);
			}
			else{
				exp-= 1; //nullable -1
			}
			//printf("\nExp: %d %d ", PMap_order, exp);
			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); //get the mantissa
			mant = bytetoInt64Decoder(ptrMant); 
		}
	}
	else{ //if the bit is 0, default exp = initialExp
		exp = -80;
		if(initialExp != 0){ 
			//exp = initialExp; //change to the operator function // so there is no exp, then is no exp in the msg, so the default is -2
			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_length, PMap_order, 1); //get the mantissa
			mant = bytetoInt64Decoder(ptrMant); //decode the mantissa
		}
	}

	//decimal = decimalOperator(exp, 0.0, initialExp, 0, mant, 0.0, 0.0, 0, PMapIs1);

	decimal = decimalOperator(previousValue, exp, 0.0, initialExp, mant, 0, 0, 0, DEFAULT, DELTA, PMapIs1);

	return decimal;
}

float decimalOperator(float previousValue, __int64_t valueExp, __int64_t previousValueExp, __int64_t initialValueExp,
	__int64_t valueMan, __int64_t previousValueMan, __int64_t initialValueMan, 
	int operatorEnt, int operatorExp, int operatorMan, 
	int PMapIs1){

	__int64_t delta = 0, base = 0;

	if(operatorEnt != NONEOPERATOR || (operatorExp == NONEOPERATOR && operatorMan == NONEOPERATOR)){
		int scaledNumber = 1;
	}

	if(operatorExp == DEFAULT && valueExp == -80){ //null
		valueExp = initialValueExp;
	}

	if(operatorMan == DELTA && !PMapIs1){ /*need to improve all these functions*/
		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
			base = pow(10, valueExp * -1); //recaucalculate de mantissa
			valueMan = (previousValue * base);
			float decimal = pow(10, valueExp);
		}
	}

	if(valueExp == 0){
		

	}
		
	float decimal = pow(10, valueExp);
	
	return valueMan * decimal;
}

/*void getFieldS(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length, __uint32_t noFields, __uint32_t i,
	char* value, unsigned int operator, char* initialValue){*/

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
	/*else{
		*streamValue = 0x80; //stream value is absent
	}*/

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
			/*printf("\n\n here ");
			printf("\n %s ", auxValue);
			printf("\n %s ", previousValue);*/
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

/*int isNegative64(int64_t val, __uint32_t size){
	int isNegative = 0;
    
	if(size == 1){
		if(val & 0b01000000){ //8bits
            isNegative = 1;
        }
	}
	else if(size == 2){
		if(val & 0b0100000000000000){ // 16 bits
			isNegative = 1;
        }
	}
	else if(size == 4){
		if(val & 0b01000000000000000000000000000000){ // 32 bits 
			isNegative = 1;
        }
	}
	else if(size == 8){
		if(val & 0b0100000000000000000000000000000000000000000000000000000000000000){ // 64 bits
			isNegative = 1;
        }
	}
    return isNegative;
}*/

/*int isNegative(int val){
	int isNegative = 0;
    
    if(val < 0x10000){
        if (val < 0x100){
            if(val & 0b01000000){ //8bits
                isNegative = 1;
            }
        }
        else{
			if(val & 0b0100000000000000){ // 16 bits
				isNegative = 1;
            }
        } 
    }else{
        if (val < 0x100000000L){
			if(val & 0b01000000000000000000000000000000){ // 32 bits 
				isNegative = 1;
            }
        }
        else{ // 64 bit
			if(val & 0b0100000000000000000000000000000000000000000000000000000000000000){ // 64 bits
				isNegative = 1;
            }
        }
    }
    
    return isNegative;
}*/

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

	/*if(field_length == 1)
		field_length = 2;*/

	//if the bitmap is 0, the value is 0x00, if the field hasnt bit(optional), the value is 0x80
	if(*field == 0x00 || *field == 0x80){ 
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