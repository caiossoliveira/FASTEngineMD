#include <stdio.h>
#include <string.h>
 
FILE* openFile(char* fileName);
__uint32_t byteDecoder32(__uint8_t* field, unsigned int field_length);
int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDoNotIdentified(__uint16_t TemplateID);
void test(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
	//test();
    return 0;
}

void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
	unsigned int field_length = 0;
	unsigned int MDEntriesSequence_PMap_length = 0;
	unsigned int noTemplateField = 1;
	unsigned int fieldAlocated = 0; //false

	__uint32_t MsgSeqNum = 0;
	__uint64_t SendintTime = 0;
	__uint32_t TradeDate = 0;
	//SequenceMDEntries
	__uint32_t NoMDEntries = 0;
	__uint32_t MDEntriesSequence_PMap = 0;
	__uint32_t MDUpdateAction = 1;
	//String MDEntryType
	__uint64_t SecurityID = 0;
	__uint32_t RptSeq = 0;
	//String QuoteCondition
	//float MDEntryPx
	//float MDEntryInterestRate
	__uint32_t NumberOfOrders = 0;
	//String PriceType
	__uint32_t MDEntryTime = 0;
	__uint64_t MDEntrySize = 0;
	__uint32_t MDEntryDate = 0;
	__uint32_t MDInsertDate = 0;
	__uint32_t MDInsertTime = 0;
	//String MDStreamID;
	//String Currency;
	//float NetChgPrevDay;
	__uint32_t SellerDays = 0;
	__uint64_t TradeVolume = 0;
	//String TickDirection = 0;
	//String TradeCondition = 0;
	__uint32_t TradingSessionID = 0;
	__uint32_t OpenCloseSettlFlag = 0;
	//String OrderID = 0;
	//String TradeID = 0;
	//String MDEntryBuyer = 0;
	//String MDEntrySeller = 0;
	__uint32_t MDEntryPositionNo = 0;
	__uint32_t SettPriceType = 0;
	__uint32_t LastTradeDate = 0;
	__uint32_t PriceAdjustmentMethod = 0;
	//String PriceBandType = 0;
	__uint32_t PriceLimitType = 0;
	//float LowLimitPrice = 0;
	//float HighLimitPrice = 0;
	//float TradingReferencePrice = 0;
	__uint32_t PriceBandMidpointPriceType = 0;
	__uint64_t AvgDailyTradedQty = 0;
	__uint64_t ExpireDate = 0;
	__uint64_t EarlyTermination = 0;
	__uint64_t MaxTradeVol = 0;
	//SequenceUnderlyings
	__uint32_t NoUnderlyings = 0;
	__uint32_t UnderlyingsSequence_PMap = 0;
	__uint64_t UnderlyingSecurityID = 0;
	//float UnderlyingPX = 0;
	__uint32_t UnderlyingPXType = 0;
	//EndSequenceUnderLyings
	__uint64_t IndexSeq = 0;
	//EndSequenceMDEntries

	printf(" TemplateID: 145 || Template name=MDIncRefresh_145 \n");


	for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){

    		alocate: 

    		if(noTemplateField == 3){
				MsgSeqNum = byteDecoder32(field, field_length);
				printf(" MsgSeqNum: %d \n", MsgSeqNum);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 4){
				printf(" SendingTime: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 				
				}
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 5){ 
				TradeDate = byteDecoder32(field, field_length);
				printf(" TradeDate: %d \n", TradeDate);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 6){ 
				NoMDEntries = byteDecoder32(field, field_length);
				printf(" NoMDEntries: %d \n", NoMDEntries);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 7){ 
				MDEntriesSequence_PMap = byteDecoder32(field, field_length);
				printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
				MDEntriesSequence_PMap_length = field_length;
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 8 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 1)){
				MDUpdateAction = byteDecoder32(field, field_length); //copy function
				printf(" MDUpdateAction: %d \n", MDUpdateAction);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 8 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 1))){
				printf(" MDUpdateAction: %d \n", MDUpdateAction); //previous value
				fieldAlocated = 0;
			}

			else if(noTemplateField == 9 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 2)){
				printf(" MDEntryType: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 					
				}
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 9 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 2))){
				printf(" Do not implemented yet: <copy MDEntryType>. \n");
				fieldAlocated = 0;
			}

			else if(noTemplateField == 10 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 3)){
				printf(" SecurityID: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 10 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 3))){
				printf(" Do not implemented yet: <copy SecurityID>. \n");
				fieldAlocated = 0;
			}

			else if(noTemplateField == 11 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 4)){
				RptSeq = byteDecoder32(field, field_length); //copy function
				printf(" RptSeq: %d \n", RptSeq);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 11 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 4))){
				if(RptSeq > 0){ //if assigned
					RptSeq = RptSeq++; //increment
				}
				printf(" RptSeq: %d \n", RptSeq); //if undefined is the previous value
				fieldAlocated = 0;
			}

			else if(noTemplateField == 12 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 5)){
				printf(" QuoteCondition: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 					
				}
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 12 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 5))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 13 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
				printf(" MDEntryPx: do not implemented yet. \n");
				fieldAlocated = 1; //true
			}
			/*else if(noTemplateField == 13 || noTemplateField == 14){
				if(noTemplateField == 13 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
					printf(" MDEntryPx: Expoente: -2 ||");
				}
				else if(noTemplateField == 14 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
					printf(" Mantissa: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 					
					}
					printf("\n");
				}
			}*/
			else if(noTemplateField == 14 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 7)){
				printf(" MDEntryInterestRate: do not implemented yet. \n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 15 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 8)){
				NumberOfOrders = byteDecoder32(field, field_length);
				printf(" NumberOfOrders: %d \n", NumberOfOrders);
				fieldAlocated = 1; //true			
			}
			else if(noTemplateField == 15 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 8))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 16){
				printf(" PriceType: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 17 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 9)){
				MDEntryTime = byteDecoder32(field, field_length);
				printf(" MDEntryTime: %d \n", MDEntryTime); //copy
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 17 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 9))){
				printf(" MDEntryTime: %d \n", MDEntryTime); //previous value
				fieldAlocated = 0;
			}

			else if(noTemplateField == 18){
				printf(" MDEntrySize: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <delta>");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 19 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 10)){
				MDEntryDate = byteDecoder32(field, field_length); //copy
				printf(" MDEntryDate: %d \n", MDEntryDate);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 19 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 10))){
				printf(" MDEntryDate: %d \n", MDEntryDate); //previous value
				fieldAlocated = 0;
			}

			else if(noTemplateField == 20 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 11)){
				MDInsertDate = byteDecoder32(field, field_length); //copy
				printf(" MDInsertDate: %d \n", MDInsertDate);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 20 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 11))){
				if(MDInsertDate > 0){
					printf(" MDInsertDate: %d \n", MDInsertDate); //previous value
				}
				fieldAlocated = 0;
			}

			else if(noTemplateField == 21 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 12)){
				MDInsertDate = byteDecoder32(field, field_length);
				printf(" MDInsertTime: %d \n", MDInsertDate); //copy
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 21 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 12))){
				if(MDInsertDate > 0){
					printf(" MDInsertTime: %d \n", MDInsertDate); //previous value
				}
				fieldAlocated = 0;
			}

			else if(noTemplateField == 22 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 13)){
				printf(" MDStreamID: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 22 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 13))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 23 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 14)){
				printf(" Currency: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <copy>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 23 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 14))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 24 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 15)){
				printf(" NetChgPrevDay: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet. ");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 24 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 15))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 25 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 16)){
				SellerDays = byteDecoder32(field, field_length);
				printf(" SellerDays: %d \n", SellerDays);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 25 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 16))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 26){
				printf(" TradeVolume: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <delta>");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 27 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 17)){
				printf(" TickDirection: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 27 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 17))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 28){
				printf(" TradeCondition: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.>");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 29){
				TradingSessionID = byteDecoder32(field, field_length);
				printf(" TradingSessionID: %d \n", TradingSessionID);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 30){
				OpenCloseSettlFlag = byteDecoder32(field, field_length);
				printf(" OpenCloseSettlFlag: %d \n", OpenCloseSettlFlag);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 31 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 18)){
				printf(" OrderID: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 31 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 18))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 32 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 19)){
				printf(" TradeID: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 32 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 19))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 33 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 20)){
				printf(" MDEntryBuyer: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 33 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 20))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 34 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 21)){
				printf(" MDEntrySeller: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 34 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 21))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 35 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 22)){
				MDEntryPositionNo = byteDecoder32(field, field_length);
				printf(" MDEntryPositionNo: %d \n", MDEntryPositionNo);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 35 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 22))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 36){
				SettPriceType = byteDecoder32(field, field_length);
				printf(" SettPriceType: %d \n", SettPriceType);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 37){
				LastTradeDate = byteDecoder32(field, field_length);
				printf(" LastTradeDate: %d \n", LastTradeDate);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 38){
				PriceAdjustmentMethod = byteDecoder32(field, field_length);
				printf(" PriceAdjustmentMethod: %d \n", PriceAdjustmentMethod);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 39 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 23)){
				printf(" PriceBandType: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet: <default>");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 39 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 23))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 40 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 24)){
				PriceLimitType = byteDecoder32(field, field_length);
				printf(" PriceLimitType: %d \n", PriceLimitType);
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 40 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 24))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 41 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 25)){
				printf(" LowLimitPrice: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 41 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 25))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 42 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 26)){
				printf(" HighLimitPrice: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 42 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 26))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 43 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 27)){
				printf(" TradingReferencePrice: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}
			else if(noTemplateField == 43 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 27))){
				fieldAlocated = 0;
			}

			else if(noTemplateField == 44){
				PriceBandMidpointPriceType = byteDecoder32(field, field_length);
				printf(" PriceBandMidpointPriceType: %d \n", PriceBandMidpointPriceType);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 45){
				printf(" AvgDailyTradedQty: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 46){
				printf(" ExpireDate: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 47){
				printf(" EarlyTermination: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 48){
				printf(" MaxTradeVol: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 49){
				NoUnderlyings = byteDecoder32(field, field_length);
				printf(" NoUnderlyings: %d \n", NoUnderlyings);
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 50){
				printf(" IndexSeq: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf(" Do not implemented yet.");
				printf("\n");
				fieldAlocated = 1; //true
			}

			else if(noTemplateField == 1 || noTemplateField == 2){
				fieldAlocated = 1; //false
			}

			else{
				//fieldAlocated = 0; //false
				printf(" Unexpected Error in identificate field: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf("\n");
			}

			noTemplateField++;
			if(fieldAlocated == 0){
				goto alocate;
			}
			field_length = 0;
    	}
    }
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
				MsgSeqNum = byteDecoder32(field, field_length);
				printf(" MsgSeqNum: %d \n", MsgSeqNum);
			}
			else if(noTemplateField == 4){
				printf(" SendingTime: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
				}
				printf("\n");
			}
			else if(!(noTemplateField == 0 || noTemplateField == 1 || noTemplateField == 2)){
				printf(" Field number %d do not identified: ", noTemplateField);
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
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

		case 145 : test(PMap, FASTMessage, FASTMessage_length); //MDIncRefresh_145(PMap, FASTMessage, FASTMessage_length);
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

    	if((field[field_length-1] >> 7) & 0b00000001){
    		noCurrentField++;
    		if(noCurrentField == 1){
    			PMap = byteDecoder32(field, field_length);
				printf(" PMap: %02x \n", PMap);
				if(!(PMap & 0b01000000)){
					printf(" TemplateID do not specified in the message. \n");
				}
			}
			else if(noCurrentField == 2 && (PMap & 0b01000000)){
				TemplateID = byteDecoder32(field, field_length);
			}
			if(TemplateID > 0){
				//talvez colocar um break aqui e mandar a msg pro template
				//templateDecoder(TemplateID, field, field_length, noCurrentField);
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
	for(int i = 0; i < 1171; i++){ // number of messages //1250
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

__uint32_t byteDecoder32(__uint8_t* field, unsigned int field_length){
    int j = field_length - 2;
    __uint32_t result;
    
    result = field[field_length-1];

    if(field_length == 1){
    	field_length = 2;
    }
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 33 - ((i + 1) * 8);
        result = result >> 32 - ((i + 1) * 8);
        result = (field[j] << ((i + 1) * 8)) | result;
        j--;
    } 

    result = result >> (field_length-1); 
    
    return result;
}

int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField){
	__uint32_t aux_bitMap = 0b00000000000000000000000000000001;

	if(PMap & (aux_bitMap << (32 - PMap_length - noCurrentField))){
		return 1;
	}
	else{
		return 0;
	}
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}

int fieldLength(__uint8_t* field){
	__uint8_t* aux = field;
	int counter = 0;
	while(*aux){
		counter++;
		*aux++;
	}
	return counter;
}

__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, int templateOrder, __uint32_t PMap, unsigned int PMap_length, unsigned int PMap_order){
	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
	__uint8_t field[7000];
    int field_length = 0, zeroCounter = 0, fieldCounter = 0, newFieldCounter = 0;

	if(PMap_order > 0){
		//*FASTMessage = *FASTMessage+1;
		for(int i = 1; i <= PMap_order - 1; i++){
			if(!(pMapCheck(PMap, PMap_length, i))){
				zeroCounter++;
			}
		}
	}

	for(int i = 0; i < 7000; i++){
		newField[i] = 0x00;
	}

	for(int i = 0; i < FASTMessage_length; i++){
		field[field_length] = *(*FASTMessage + i);
    	field_length++;
    	if((field[field_length-1] >> 7) & 0b00000001){
    		fieldCounter++;
    		if(fieldCounter == (templateOrder - zeroCounter)){ //TemplateOrderIndex
    			for(int j = 0; j < i - ((i - field_length)); j++){
    				newField[j] = *(*FASTMessage+((i - field_length + 1) + j));
    				newFieldCounter++;
    			}
	    		return newField;
    		}
    		field_length = 0;
    	}
    }
}

void test(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
	
	#define MSGSEQNUM 3
	#define SENDINGTIME 4
	#define TRADEDATE 5
	#define NOMDENTRIES 6 //SequenceMDEntries
	#define MDENTRIESSEQUENCE_PMAP 7
	#define MDUPDATEACTION 1
	#define MDENTRYTYPE 2
	#define SECURITYID 3
	#define RPTSEQ 4
	#define QUOTECONDITION 5
	#define MDENTRYPX 6

	__uint8_t aux_FASTMessage[7000];
	for(int i = 0; i < 7000; i++){
		aux_FASTMessage[i] = FASTMessage[i];
	}
	__uint8_t* ptr_FASTMessage = FASTMessage;

	__uint8_t field[7000] = {0x80};
	
	unsigned int MDEntriesSequence_PMap_length = 0;

	__uint32_t MsgSeqNum = 0, TradeDate = 0;
	__uint64_t SendintTime = 0;
	
	//SequenceMDEntries
	__uint32_t NoMDEntries = 0;
	__uint32_t MDEntriesSequence_PMap = 0;
	__uint32_t MDUpdateAction = 1;

	__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MSGSEQNUM, 0, MDEntriesSequence_PMap_length, 0);
	MsgSeqNum = byteDecoder32(aux, fieldLength(aux));
	printf("\n MsgSeqNum: %d \n", MsgSeqNum);

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, SENDINGTIME, 0, MDEntriesSequence_PMap_length, 0);
	printf(" SendintTime: ");
	while(*aux){
		printf( " %02x", (unsigned int) *aux++);
	}
	printf("\n");

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NOMDENTRIES, 0, MDEntriesSequence_PMap_length, 0);
	NoMDEntries = byteDecoder32(aux, fieldLength(aux));
	printf(" NoMDEntries: %d \n", NoMDEntries);

	if(NoMDEntries > 0){ //sequence
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDENTRIESSEQUENCE_PMAP, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 0);
		printf(" MDEntriesSequence_PMap: %d \n", byteDecoder32(aux, fieldLength(aux)));

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDUPDATEACTION, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 1);
		printf(" MDUpdateAction: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDENTRYTYPE, MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 2);
		printf(" MDEntryType: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");
	}
}