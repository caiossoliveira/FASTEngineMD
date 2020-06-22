#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//#include "t145toFIX.h"
#include "t145toFIXFile.h"
 
FILE* openFile(char* fileName);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, 
	__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDoNotIdentified(__uint16_t TemplateID);
void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);

__uint32_t getField32I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order, 
	__uint32_t previousValue, unsigned int operator, __uint32_t initialValue, unsigned int isNullable);

__uint64_t getField64I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order, 
	__uint64_t previousValue, unsigned int operator, __uint64_t initialValue, unsigned int isNullable);

void getFieldS(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order, 
	char* previousValue, unsigned int operator, char* initialValue);

float getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order, 
	float previousValue, unsigned int operator, __int32_t initialExp);

__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, 
	unsigned int PMap_order, unsigned int PMap_length);
__uint32_t int32Operator(__uint32_t value, __uint32_t previousValue, __uint32_t initialValue, int operator, 
	int PMapis1);
__uint64_t int64Operator(__uint64_t value, __uint64_t previousValue, __uint64_t initialValue, int operator, 
	int PMapis1);
void stringOperator(char* value, char* streamValue, char* previousValue, char* initialValue, int operator, 
	int PMapIs1);
float decimalOperator(__int64_t valueExp, __int64_t previousValueExp, __int64_t initialValueExp, int operatorExp,
	__int64_t valueMan, __int64_t previousValueMan, __int64_t initialValueMan, int operatorMan, 
	int PMapis1);

char* bytetoStringDecoder(__uint8_t* field);
float bytetoDecimalDecoder(__uint8_t* field);
__uint32_t bytetoInt32Decoder(__uint8_t* field);
__uint64_t bytetoInt64Decoder(__uint8_t* field);
__uint32_t bytetoPMapDecoder(__uint8_t* field, __int32_t field_length);
__uint32_t fieldLength(__uint8_t* field);
int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField);
int isDecimal(unsigned int PMap_order);

void test();
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
	//test();
    return 0;
}

void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
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

	#define NULLABLE 1
	#define NON_NULLABLE 0

	#define UNDEFINED -90

	__uint8_t* ptr_FASTMessage = FASTMessage+3; //MsgSeqNum is the first here but the third in the message
	__uint8_t field[7000] = {0x80}; //FIX/FAST encoded MD is no larger than 1420 bytes including the header

	//Template
	__uint32_t MsgSeqNum = UNDEFINED, TradeDate = UNDEFINED;
	__uint64_t SendintTime = 0;
	//SequenceMDEntries
	__uint32_t NoMDEntries = UNDEFINED, MDEntriesSequence_PMap = UNDEFINED; 
	__uint32_t MDEntriesSequence_PMap_length = UNDEFINED, NumberOfOrders = UNDEFINED;
	__uint32_t SellerDays = UNDEFINED, TradingSessionID = UNDEFINED, OpenCloseSettlFlag = UNDEFINED;
	__uint32_t MDEntryPositionNo = UNDEFINED, SettPriceType = UNDEFINED, LastTradeDate = UNDEFINED; 
	__uint32_t PriceAdjustmentMethod = UNDEFINED, PriceLimitType = UNDEFINED;
	__uint32_t PriceBandMidpointPriceType = UNDEFINED;
	__uint64_t AvgDailyTradedQty = 0, ExpireDate = 0, EarlyTermination = 0, MaxTradeVol = 0;
	__uint32_t MDUpdateAction = UNDEFINED, RptSeq = UNDEFINED, MDEntryTime = UNDEFINED; 
	__uint32_t MDEntryDate = UNDEFINED, MDInsertDate = UNDEFINED, MDInsertTime = UNDEFINED;
	__uint64_t SecurityID = 0, TradeVolume = 0;
	__int64_t MDEntrySize = 0; //it'll need a special function for signal int bcs of the signal bit calculation
	char MDEntryType[1000] = "UNDEFINED";
	char QuoteCondition[1000] = "UNDEFINED";
	char PriceType[1000] = "UNDEFINED"; //change to EMPTY
	char MDStreamID[1000] = "UNDEFINED";
	char Currency[1000] = "UNDEFINED";
	char TickDirection[1000] = "UNDEFINED";
	char TradeCondition[1000] = "UNDEFINED";
	char OrderID[1000] = "UNDEFINED";
	char TradeID[1000] = "UNDEFINED";
	char MDEntryBuyer[1000] = "UNDEFINED";
	char MDEntrySeller[1000] = "UNDEFINED";
	char PriceBandType[1000] = "UNDEFINED";
	float MDEntryPx = 0.0, MDEntryInterestRate = 0.0, NetChgPrevDay = 0.0, LowLimitPrice = 0.0; 
	float HighLimitPrice = 0.0, TradingReferencePrice = 0.0;
	//SequenceUnderlyings
	__uint32_t NoUnderlyings = 0, UnderlyingPXType = 0;
	__uint64_t UnderlyingSecurityID = 0; 
	float UnderlyingPx = 0.0;
	//template
	__uint64_t IndexSeq = 0;

	char* initialValueC = "EMPTY";
	int initialValueI = 0;

	MsgSeqNum = getField32I(&ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, 
		NONEBITMAP, MsgSeqNum, NONEOPERATOR, initialValueI, NON_NULLABLE);

	SendintTime = getField64I(&ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, 
		NONEBITMAP, SendintTime, NONEOPERATOR, initialValueI, NON_NULLABLE);

	TradeDate = getField32I(&ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, 
		NONEBITMAP, TradeDate, NONEOPERATOR, initialValueI, NULLABLE);

	NoMDEntries = getField32I(&ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, 
		NONEBITMAP, NoMDEntries, NONEOPERATOR, initialValueI, NON_NULLABLE);
	
	/*printf("\n MsgSeqNum: %d \n", MsgSeqNum);
	printf(" SendintTime: %ld \n", SendintTime);
	printf(" TradeDate: %d \n", TradeDate);
	printf(" NoMDEntries: %d \n", NoMDEntries);*/

	if(NoMDEntries > 0){ //sequence
		__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, NONEBITMAP);
		MDEntriesSequence_PMap = bytetoInt32Decoder(aux);
		MDEntriesSequence_PMap_length = fieldLength(aux);

		/*printf("\n ");
		for(int i = 0; i < MDEntriesSequence_PMap_length * 8; i++){
			printf("%d", pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, i+1));
		}
		printf("\n");*/

		MDUpdateAction = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDUPDATEACTION, MDUpdateAction, COPY, 1, NON_NULLABLE);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYTYPE, MDEntryType, COPY, "0");

		SecurityID = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			SECURITYID, SecurityID, COPY, NON_NULLABLE, initialValueI);

		RptSeq = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			RPTSEQ, RptSeq, INCREMENT, NON_NULLABLE, initialValueI);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			QUOTECONDITION, QuoteCondition, NONEOPERATOR, initialValueC);

		MDEntryPx = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYPX, MDEntryPx, NONEOPERATOR, -2);
		
		MDEntryInterestRate = getFieldD(&ptr_FASTMessage, FASTMessage_length,
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length,
			MDENTRYINTERESTRATE, MDEntryInterestRate, NONEOPERATOR, -2);
		
		NumberOfOrders = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			NUMBEROFORDERS, NumberOfOrders, NONEOPERATOR, initialValueI, NULLABLE);
		
		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, PriceType, NONEOPERATOR, initialValueC);

		MDEntryTime = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYTIME, MDEntryTime, COPY, initialValueI, NON_NULLABLE);

		MDEntrySize = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, MDEntrySize, DELTA, initialValueI, NULLABLE);

		MDEntryDate = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYDATE, MDEntryDate, COPY, initialValueI, NULLABLE);

		MDInsertDate = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDINSERTDATE, MDInsertDate, COPY, initialValueI, NULLABLE);
	
		MDInsertTime = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDINSERTTIME, MDInsertTime, COPY, initialValueI, NULLABLE);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
			MDEntriesSequence_PMap_length, 
			MDSTREAMID, MDStreamID, NONEOPERATOR, initialValueC);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, 
			MDEntriesSequence_PMap_length, 
			CURRENCY, Currency, COPY, initialValueC);
		
		NetChgPrevDay = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			NETCHGPREVDAY, NetChgPrevDay, NONEOPERATOR, 0);
		
		SellerDays = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			SELLERDAYS, SellerDays, NONEOPERATOR, initialValueI, NULLABLE);
		
		TradeVolume = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, TradeVolume, DELTA, initialValueI, NULLABLE);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			TICKDIRECTION, TickDirection, NONEOPERATOR, initialValueC);
		
		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP,	TradeCondition, NONEOPERATOR, initialValueC);

		TradingSessionID = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, TradingSessionID, NONEOPERATOR, initialValueI, NULLABLE);
		
		OpenCloseSettlFlag = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, OpenCloseSettlFlag, NONEOPERATOR, initialValueI, NULLABLE);
		
		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			ORDERID, OrderID, NONEOPERATOR, initialValueC);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			TRADEID, TradeID, NONEOPERATOR, initialValueC);

		getFieldS(&ptr_FASTMessage, FASTMessage_length,
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYBUYER, MDEntryBuyer, NONEOPERATOR, initialValueC);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYSELLER, MDEntrySeller, NONEOPERATOR, initialValueC);

		MDEntryPositionNo = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			MDENTRYPOSITIONNO, MDEntryPositionNo, NONEOPERATOR, initialValueI, NULLABLE);
			
		SettPriceType = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, SettPriceType, NONEOPERATOR, initialValueI, NULLABLE);
		
		LastTradeDate = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, LastTradeDate, NONEOPERATOR, initialValueI, NULLABLE);

		PriceAdjustmentMethod = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, PriceAdjustmentMethod, NONEOPERATOR, initialValueI, NULLABLE);

		getFieldS(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			PRICEBANDTYPE, PriceBandType, NONEOPERATOR, initialValueC);

		PriceLimitType = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			PRICELIMITTYPE, PriceLimitType, NONEOPERATOR, initialValueI, NULLABLE);
		
		LowLimitPrice = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			LOWLIMITPRICE, LowLimitPrice, NONEOPERATOR, 0);
		
		HighLimitPrice = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			HIGHLIMITPRICE, HighLimitPrice, NONEOPERATOR, 0);

		TradingReferencePrice = getFieldD(&ptr_FASTMessage, FASTMessage_length, 
			MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 
			TRADINGREFERENCEPRICE, TradingReferencePrice, NONEOPERATOR, 0);
		
		PriceBandMidpointPriceType = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, PriceBandMidpointPriceType, NONEOPERATOR, initialValueI, NULLABLE);
		
		AvgDailyTradedQty = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, AvgDailyTradedQty, NONEOPERATOR, initialValueI, NULLABLE);
		
		ExpireDate = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, ExpireDate, NONEOPERATOR, initialValueI, NULLABLE);
		
		EarlyTermination = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, EarlyTermination, NONEOPERATOR, initialValueI, NULLABLE);
		
		MaxTradeVol = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, MaxTradeVol, NONEOPERATOR, initialValueI, NULLABLE);
		
		NoUnderlyings = getField32I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, NoUnderlyings, NONEOPERATOR, initialValueI, NON_NULLABLE);
		
		/*printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
		printf(" MDUpdateAction: %d \n", MDUpdateAction);
		printf(" MDEntryType: %s \n", MDEntryType);
		printf(" SecurityID: %ld \n", SecurityID);
		printf(" RptSeq: %d \n", RptSeq);
		printf(" QuoteCondition: %s \n", QuoteCondition);
		printf(" MDEntryPx: %.2f \n", MDEntryPx);
		printf(" MDEntryInterestRate: %.2f \n", MDEntryInterestRate);
		printf(" NumberOfOrders: %d \n", NumberOfOrders);
		printf(" PriceType: %s \n", PriceType);
		printf(" MDEntryTime: %d \n", MDEntryTime);
		printf(" MDEntrySize: %ld \n", MDEntrySize);
		printf(" MDEntryDate: %d \n", MDEntryDate);
		printf(" MDInsertDate: %d \n", MDInsertDate);
		printf(" MDInsertTime: %d \n", MDInsertTime);
		printf(" MDStreamID: %s \n", MDStreamID);
		printf(" Currency: %s \n", Currency);
		printf(" NetChgPrevDay: %.2f \n", NetChgPrevDay);
		printf(" SellerDays: %d \n", SellerDays);
		printf(" TradeVolume: %ld \n", TradeVolume);
		printf(" TickDirection: %s \n", TickDirection);
		printf(" TradeCondition: %s \n", TradeCondition);
		printf(" TradingSessionID: %d \n", TradingSessionID);
		printf(" OpenCloseSettlFlag: %d \n", OpenCloseSettlFlag);
		printf(" OrderID: %s \n", OrderID);
		printf(" TradeID: %s \n", TradeID);
		printf(" MDEntryBuyer: %s \n", MDEntryBuyer);
		printf(" MDEntrySeller: %s \n", MDEntrySeller);
		printf(" MDEntryPositionNo: %d \n", MDEntryPositionNo);
		printf(" SettPriceType: %d \n", SettPriceType);
		printf(" LastTradeDate: %d \n", LastTradeDate);	
		printf(" PriceAdjustmentMethod: %d \n", PriceAdjustmentMethod);
		printf(" PriceBandType: %s \n", PriceBandType);
		printf(" PriceLimitType: %d \n", PriceLimitType);
		printf(" LowLimitPrice: %.2f \n", LowLimitPrice);
		printf(" HighLimitPrice: %.2f \n", HighLimitPrice);
		printf(" TradingReferencePrice: %.2f \n", TradingReferencePrice);
		printf(" PriceBandMidpointPriceType: %d \n", PriceBandMidpointPriceType);
		printf(" AvgDailyTradedQty: %ld \n", AvgDailyTradedQty);
		printf(" ExpireDate: %ld \n", ExpireDate);
		printf(" EarlyTermination: %ld \n", EarlyTermination);
		printf(" MaxTradeVol: %ld \n", MaxTradeVol);
		printf(" NoUnderlyings: %d \n", NoUnderlyings);*/


		if(NoUnderlyings > 0){}

		IndexSeq = getField64I(&ptr_FASTMessage, FASTMessage_length, 
			NONEBITMAP, NONEBITMAP, 
			NONEBITMAP, IndexSeq, NONEOPERATOR, initialValueI, NULLABLE);
		/*printf(" IndexSeq: %ld \n", IndexSeq);*/
	}

	t145toFIX(
		//Template
		MsgSeqNum, TradeDate, SendintTime,
		//SequenceMDEntries
		NoMDEntries, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDUpdateAction, RptSeq, 
		NumberOfOrders, MDEntryTime, MDEntryDate, MDInsertDate, MDInsertTime, SellerDays, TradingSessionID, 
		OpenCloseSettlFlag, MDEntryPositionNo, SettPriceType, LastTradeDate, PriceAdjustmentMethod, 
		PriceLimitType, PriceBandMidpointPriceType, SecurityID, MDEntrySize, TradeVolume, AvgDailyTradedQty, 
		ExpireDate, EarlyTermination, MaxTradeVol, MDEntryType, QuoteCondition, PriceType, MDStreamID, 
		Currency, TickDirection, TradeCondition, OrderID, TradeID, MDEntryBuyer, MDEntrySeller, PriceBandType, 
		MDEntryPx, MDEntryInterestRate, NetChgPrevDay, LowLimitPrice, HighLimitPrice, TradingReferencePrice,
		//SequenceUnderlyings
		NoUnderlyings, UnderlyingPXType, UnderlyingSecurityID, IndexSeq, UnderlyingPx
	);
}

void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000] = {0x00};
	unsigned int field_length = 0;
	unsigned int noTemplateField = 0;
	__uint32_t MsgSeqNum = 0;
	__uint64_t SendingTime = 0;

	//printf(" TemplateID: 144 || Template name=MDHeartbeat_144 \n");
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
    //printf(" MsgSeqNum: %d \n", MsgSeqNum);
    //printf(" SendingTime: %ld \n", SendingTime);
    //t144toFIX(MsgSeqNum, SendingTime);
}

void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, 
	__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	
	switch(TemplateID)
	{
		case 144 : MDHeartbeat_144(FASTMessage, FASTMessage_length);
		break;

		case 145 : MDIncRefresh_145(PMap, FASTMessage, FASTMessage_length);
		break;

		default : templateDoNotIdentified(TemplateID);
	}
}

void identifyTemplate(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
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
				//printf(" PMap: %02x \n", PMap);
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
	__uint8_t FASTMessage[70000]; //2 bytes of MsgLength is the limit
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
		/*if(MsgSeqNum > 731915){ //only to compare with the FIX log
			printf("\n-----------------------------------------------------------------------------------------------------");
			printf(" \n Message %d: \n", i+1);
			printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);
			identifyTemplate(FASTMessage, FASTMessage_length);
		}*/
		identifyTemplate(FASTMessage, FASTMessage_length);
		FASTMessage_length = 0;

		//printf(" ---------------------------------------------------------------------------\n\n");
	}
}

void templateDoNotIdentified(__uint16_t TemplateID){
	printf(" TemplateID do not identified: %d \n", TemplateID);
}

__uint32_t getField32I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length, 
	__uint32_t previousValue, unsigned int operator, __uint32_t initialValue, unsigned int isNullable){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamValue[7000];
    __uint32_t value;
  	int thereIsPMap = 0, PmapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the stream (nullable or not)
		__uint8_t* pt_value = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length);
		value = bytetoInt32Decoder(pt_value);
	}
	else{
		value = 0x00; //null
	}

	value = int32Operator(value, previousValue, initialValue, operator, PmapIs1);

    if(isNullable == NULLABLE && value != 0x00){
		value--; //If an integer is nullable, every non-negative integer is incremented by 1 before it is encoded
	}
	else if(isNullable == NULLABLE && value == 0x00){ //added now
		value = -80; //null, absent
	}

	return value;
}

__uint32_t int32Operator(__uint32_t value, __uint32_t previousValue, __uint32_t initialValue, 
	int operator, int PMapIs1){
	
	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == 0){ //EMPTY
			value = 0;
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
			value = 0;
		}
    }
    else if(operator == DELTA){
    	int delta = 0, base = 0;
    	if(PMapIs1){ //if pmap is 1 //the value is present in the stream
    		delta = value; //delta //the delta is present in the stream
    	}
    	else{ //value is not present in the stream
    		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
    			base = previousValue;
    		}
    		else if(previousValue == UNDEFINED){ //undefined 
				base = initialValue; //the value of the field is the initial value
			}
			else if(previousValue == 0){ //EMPTY
				value = 0;
			}
    	}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    } 	

    return value;
}

__uint64_t getField64I(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length, 
	__uint64_t previousValue, unsigned int operator, __uint64_t initialValue, unsigned int isNullable){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamValue[7000];
	__uint64_t value;
  	int thereIsPMap = 0, PmapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the stream (nullable or not)
		__uint8_t* pt_value = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length);
		value = bytetoInt64Decoder(pt_value);
	}
	else{
		value = 0x00; //null
	}

	value = int64Operator(value, previousValue, initialValue, operator, PmapIs1);

    if(isNullable == NULLABLE && value != 0x00){
		value--; //If an integer is nullable, every non-negative int is incremented by 1 before it is encoded
	}
	else if(isNullable == NULLABLE && value == 0x00){ //added now
		value = -80; //null, absent
	}

	return value;
}

__uint64_t int64Operator(__uint64_t value, __uint64_t previousValue, __uint64_t initialValue, 
	int operator, int PMapIs1){

	__int64_t delta = 0, base = 0;

	//if the value isnt present in the stream, bcs if yes the value in the stream is the new value
	if(operator == COPY && !PMapIs1){ 
		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
			value = previousValue; //the value of the field is the previous value
		}
		else if(previousValue == UNDEFINED){ //undefined 
			value = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == 0){ //EMPTY
			value = 0;
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
			value = 0;
		}
    }
    else if(operator == DELTA){
    	delta = value; //a delta value is present in the stream
		if(previousValue != UNDEFINED && previousValue != 0){ //assigned
			base = previousValue;
		}
		else if(previousValue == UNDEFINED){ //undefined 
			base = initialValue; //the value of the field is the initial value
		}
		else if(previousValue == 0){ //EMPTY
			value = 0;
		}
    	value = base + delta;
    }
    else if(operator == DEFAULT && !PMapIs1){ 
		value = initialValue;
    }

    return value;
}

float getFieldD(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order,
	float previousValue, unsigned int operator, __int32_t initialExp){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    __uint8_t streamValue[7000];

    __uint8_t* ptrExp = NULL;
    __uint8_t* ptrMant = NULL;
    float exp = 0.0;
    double mant = 0.0;
    float decimal = 0.0;
    int thereIsPMap = 0, PMapIs1 = 0;

	if(PMap_order > 0){
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PMapIs1 = 1;			
		}
	}

	if(thereIsPMap && PMapIs1){ //If set, the value appears in the stream in a nullable representation
		ptrExp = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); //there is a exp in the msg
		if(*ptrExp != 0x80){ //if it is no zero
			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); //get the mantissa
			exp = bytetoInt32Decoder(ptrExp); //decode the exp
			mant = bytetoInt64Decoder(ptrMant); //decode the mant
		}
	}
	else{ //if the bit is 0, default exp = initialExp
		if(initialExp != 0){ 
			exp = initialExp; //so there is no exp, then is no exp in the msg, so the default is -2
			ptrMant = getField(streamValue, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); //get the mantissa
			mant = bytetoInt64Decoder(ptrMant); //decode the mantissa
		}
	}

	decimal = decimalOperator(exp, 0.0, initialExp, 0, mant, 0.0, 0.0, 0, PMapIs1);
}

float decimalOperator(__int64_t valueExp, __int64_t previousValueExp, __int64_t initialValueExp, int operatorExp,
	__int64_t valueMan, __int64_t previousValueMan, __int64_t initialValueMan, int operatorMan, 
	int PMapIs1){

	float decimal = pow(10, valueExp);
	return valueMan * decimal;
}

void getFieldS(__uint8_t** FASTMessage, int FASTMessage_length, 
	__uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length, 
	char* value, unsigned int operator, char* initialValue){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t streamField[7000];

  	int thereIsPMap = 0, PmapIs1 = 0;
  	char previousValue[1500], auxValue[1500], streamValue[1500];

  	strcpy(previousValue, value);

	if(PMap_order > 0){ //if the field has a bit representation in bitmap
		thereIsPMap = 1;
		if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
			PmapIs1 = 1;			
		}
	}

	if((thereIsPMap && PmapIs1) || !thereIsPMap){ //if the value is in the field (nullable or not)
		__uint8_t* pt_streamValue = getField(streamField, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length);
		strcpy(streamValue, pt_streamValue); //get the stream value
	}
	else{
		*streamValue = 0x80; //stream value is absent
	}

	strcpy(streamValue, bytetoStringDecoder(streamValue)); //decode the stram value

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
	__uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length){

	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    int field_length = 0;

	for(int i = 0; i < 7000; i++){ //clean the buffer
		newField[i] = 0x00;
	}

	if(PMap_order > 0){
		if(!(pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 0 (!1)
			if(!isDecimal(PMap_order)){ //if bitsmap's bit is 0 and is not decimal, return NULL
				newField[0] = 0x00; //need to think about this character
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

int isDecimal(unsigned int PMap_order){
	switch(PMap_order){
		case MDENTRYPX : return 1;
		case MDENTRYINTERESTRATE : return 1;
		case LOWLIMITPRICE : return 1;
		case HIGHLIMITPRICE : return 1;
		case TRADINGREFERENCEPRICE: return 1;
		default : return 0;
	}
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

int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField){
	__uint32_t aux_bitMap = 0b00000000000000000000000000000001;

	if(PMap & (aux_bitMap << (32 - PMap_length - noCurrentField))){ //if bitsmap's bit is 1
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