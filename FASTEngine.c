#include <stdio.h>
#include <string.h>
//#include "t145toFIX.h"
 
FILE* openFile(char* fileName);
float bytetoDecimalDecoder(__uint8_t* field);
__uint32_t bytetoInt32Decoder(__uint8_t* field);
__uint64_t bytetoInt64Decoder(__uint8_t* field);
__uint32_t bytetoPMapDecoder(__uint8_t* field, __int32_t field_length);
char* bytetoStringDecoder(__uint8_t* field);
__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length);
__uint32_t fieldLength(__uint8_t* field);
int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField);
int isDecimal(unsigned int PMap_order);
void templateDoNotIdentified(__uint16_t TemplateID);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
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

	__uint8_t aux_FASTMessage[7000];
	for(int i = 0; i < 7000; i++){
		aux_FASTMessage[i] = FASTMessage[i];
	}
	__uint8_t* ptr_FASTMessage = FASTMessage+3; //MsgSeqNum is the first here but the third in the message
	__uint8_t field[7000] = {0x80};

	//Template
	__uint32_t MsgSeqNum = 0, TradeDate = 0;
	__uint64_t SendintTime = 0;
	//SequenceMDEntries
	__uint32_t NoMDEntries = 0, MDEntriesSequence_PMap = 0, MDEntriesSequence_PMap_length = 0, MDUpdateAction = 1;
	__uint32_t RptSeq = 0, NumberOfOrders = 0, MDEntryTime = 0, MDEntryDate = 0, MDInsertDate = 0, MDInsertTime = 0;
	__uint32_t SellerDays = 0, TradingSessionID = 0, OpenCloseSettlFlag = 0, MDEntryPositionNo = 0, SettPriceType = 0;
	__uint32_t LastTradeDate = 0, PriceAdjustmentMethod = 0, PriceLimitType = 0, PriceBandMidpointPriceType = 0;
	__uint64_t SecurityID = 0, MDEntrySize = 0, TradeVolume = 0, AvgDailyTradedQty = 0, ExpireDate = 0, EarlyTermination = 0;
	__uint64_t MaxTradeVol = 0;
	char MDEntryType[1000];
	char QuoteCondition[1000];
	char PriceType[1000];
	char MDStreamID[1000];
	char Currency[1000];
	char TickDirection[1000];
	char TradeCondition[1000];
	char OrderID[1000];
	char TradeID[1000];
	char MDEntryBuyer[1000];
	char MDEntrySeller[1000];
	char PriceBandType[1000];
	float MDEntryPx = 0.0, MDEntryInterestRate = 0.0, NetChgPrevDay = 0.0, LowLimitPrice = 0.0, HighLimitPrice = 0.0;
	float TradingReferencePrice = 0.0;
	//SequenceUnderlyings
	__uint32_t NoUnderlyings = 0, UnderlyingPXType = 0;
	__uint64_t UnderlyingSecurityID = 0, IndexSeq = 0;
	float UnderlyingPx = 0.0;

	__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	MsgSeqNum = bytetoInt32Decoder(aux);

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	SendintTime = bytetoInt64Decoder(aux);

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	TradeDate = bytetoInt32Decoder(aux);

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	NoMDEntries = bytetoInt32Decoder(aux);
	
	printf("\n MsgSeqNum: %d \n", MsgSeqNum);
	printf(" SendintTime: %ld \n", SendintTime);
	printf(" TradeDate: %d \n", TradeDate);
	printf(" NoMDEntries: %d \n", NoMDEntries);

	if(NoMDEntries > 0){ //sequence
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		MDEntriesSequence_PMap = bytetoInt32Decoder(aux);
		MDEntriesSequence_PMap_length = fieldLength(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDUPDATEACTION, MDEntriesSequence_PMap_length);
		MDUpdateAction = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTYPE, MDEntriesSequence_PMap_length);
		strcpy(MDEntryType, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SECURITYID, MDEntriesSequence_PMap_length);
		SecurityID = bytetoInt64Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, RPTSEQ, MDEntriesSequence_PMap_length);
		RptSeq = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, QUOTECONDITION, MDEntriesSequence_PMap_length);
		strcpy(QuoteCondition, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPX, MDEntriesSequence_PMap_length);
		MDEntryPx = bytetoDecimalDecoder(aux); //bytetoInt32Decoder(aux) * 0.01;

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYINTERESTRATE, MDEntriesSequence_PMap_length);
		MDEntryInterestRate = bytetoDecimalDecoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NUMBEROFORDERS, MDEntriesSequence_PMap_length);
		NumberOfOrders = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		strcpy(PriceType, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTIME, MDEntriesSequence_PMap_length);
		MDEntryTime = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		MDEntrySize = bytetoInt64Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYDATE, MDEntriesSequence_PMap_length);
		MDEntryDate = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTDATE, MDEntriesSequence_PMap_length);
		MDInsertDate = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTTIME, MDEntriesSequence_PMap_length);
		MDInsertTime = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDSTREAMID, MDEntriesSequence_PMap_length);
		strcpy(MDStreamID, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, CURRENCY, MDEntriesSequence_PMap_length);
		strcpy(Currency, bytetoStringDecoder(aux));
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NETCHGPREVDAY, MDEntriesSequence_PMap_length);
		NetChgPrevDay = bytetoDecimalDecoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SELLERDAYS, MDEntriesSequence_PMap_length);
		SellerDays = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		TradeVolume = bytetoInt64Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TICKDIRECTION, MDEntriesSequence_PMap_length);
		strcpy(TickDirection, bytetoStringDecoder(aux));
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		strcpy(TradeCondition, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		TradingSessionID = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		OpenCloseSettlFlag = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, ORDERID, MDEntriesSequence_PMap_length);
		strcpy(OrderID, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADEID, MDEntriesSequence_PMap_length);
		strcpy(TradeID, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYBUYER, MDEntriesSequence_PMap_length);
		strcpy(MDEntryBuyer, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYSELLER, MDEntriesSequence_PMap_length);
		strcpy(MDEntrySeller, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPOSITIONNO, MDEntriesSequence_PMap_length);
		MDEntryPositionNo = bytetoInt32Decoder(aux);
			
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		SettPriceType = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		LastTradeDate = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		PriceAdjustmentMethod = bytetoInt32Decoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICEBANDTYPE, MDEntriesSequence_PMap_length);
		strcpy(PriceBandType, bytetoStringDecoder(aux));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICELIMITTYPE, MDEntriesSequence_PMap_length);
		PriceAdjustmentMethod = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, LOWLIMITPRICE, MDEntriesSequence_PMap_length);
		LowLimitPrice = bytetoDecimalDecoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, HIGHLIMITPRICE, MDEntriesSequence_PMap_length);
		HighLimitPrice = bytetoDecimalDecoder(aux);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADINGREFERENCEPRICE, MDEntriesSequence_PMap_length);
		TradingReferencePrice = bytetoDecimalDecoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		PriceAdjustmentMethod = bytetoInt32Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		AvgDailyTradedQty = bytetoInt64Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		ExpireDate = bytetoInt64Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		EarlyTermination = bytetoInt64Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		MaxTradeVol = bytetoInt64Decoder(aux);
		
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		NoUnderlyings = bytetoInt32Decoder(aux);
		
		printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
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
		printf(" NoUnderlyings: %d \n", NoUnderlyings);


		if(NoUnderlyings > 0){}

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		IndexSeq = bytetoInt64Decoder(aux);
		printf(" IndexSeq: %ld \n", IndexSeq);
	}

	/*t145toFIX(
		//Template
		MsgSeqNum, TradeDate, SendintTime,
		//SequenceMDEntries
		NoMDEntries, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, MDUpdateAction,
		RptSeq, NumberOfOrders, MDEntryTime, MDEntryDate, MDInsertDate, MDInsertTime,
		SellerDays, TradingSessionID, OpenCloseSettlFlag, MDEntryPositionNo, SettPriceType,
		LastTradeDate, PriceAdjustmentMethod, PriceLimitType, PriceBandMidpointPriceType,
		MDEntryPx,
		//SequenceUnderlyings
		NoUnderlyings, UnderlyingPXType
	);*/
}

void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
	unsigned int field_length = 0;
	unsigned int noTemplateField = 0;
	__uint32_t MsgSeqNum = 0;
	__uint64_t SendingTime = 0;

	printf(" TemplateID: 144 || Template name=MDHeartbeat_144 \n");
	for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){
    		noTemplateField++;
    		if(noTemplateField == 3){ //&& (pmap >><< fieldOrder)
				MsgSeqNum = bytetoInt32Decoder(field);
				printf(" MsgSeqNum: %d \n", MsgSeqNum);
			}
			else if(noTemplateField == 4){
				printf(" SendingTime: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes 
				}
				printf("\n");
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
}

void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
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
				printf(" PMap: %02x \n", PMap);
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

	//while(fread(&byte, 1, 1, file) > 0){
	for(int i = 0; i < 1250; i++){ // number of messages //1250
		printf(" Message %d:    ----------------------------------------------------------\n", i+1);
		for(int i = 0; i < 10; i++){ //read header
			fread(&byte, 1, 1, file);
			header[i] = byte;
		}
		MsgSeqNum = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | (header[3]); //concatenate the bytes
		NoChunks = (header[4] << 8) | (header[5]);
		CurrentChunk = (header[6] << 8) | (header[7]);
		MsgLength = (header[8] << 8) | (header[9]);

		printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);

		for(int i = 0; i < MsgLength; i++){

			fread(&byte, 1, 1, file);

			FASTMessage[FASTMessage_length] = byte;
			FASTMessage_length++;
		}

		identifyTemplate(FASTMessage, FASTMessage_length);

		FASTMessage_length = 0;

		printf(" ---------------------------------------------------------------------------\n\n");
	}
}

void templateDoNotIdentified(__uint16_t TemplateID){
	printf(" TemplateID do not identified: %d \n", TemplateID);
}

__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length){
	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    int field_length = 0, zeroCounter = 0;

	for(int i = 0; i < 7000; i++){ //clean the buffer
		newField[i] = 0x00;
	}

	if(PMap_order > 0){
		if(!(pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is !1 (0)
			if(!isDecimal(PMap_order)){ //if bitsmap's bit is 0 and not decimal, return NULL
				newField[0] = 0x00; 
				return newField;
			}
		}
	}

	for(int i = 0; i < FASTMessage_length; i++){
		newField[field_length] = *(*FASTMessage);
    	field_length++;
    	*FASTMessage = *FASTMessage+1;
    	if((newField[field_length-1] >> 7) & 0b00000001){
	    	return newField;
    		//field_length = 0;
    	}
    }
}

int isDecimal(unsigned int PMap_order){
	switch(PMap_order){
		case MDENTRYPX : return 1;
		default : return 0;
	}
}

float bytetoDecimalDecoder(__uint8_t* field){
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

	for(int i = 0; i < field_length + 1; i++){
		if((field[i] >> 7) & 0b00000001){ //if MSB is 1
			field[i] = field[i] << 1; //save only the 7 LSB in a 8 bits buffer
        	field[i] = field[i] >> 1; //
		}
		result[i] = field[i];
	}

	if(field[0] == 0x00)
		strcpy(result, "NULL");

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
	while(*aux){
		counter++;
		*aux++;
	}
	return counter;
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}