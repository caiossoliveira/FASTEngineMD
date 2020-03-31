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
void test();
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
	//test();
    return 0;
}

void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
	unsigned int field_length = 0;
	unsigned int MDEntriesSequence_PMap_length = 0;
	unsigned int noCurrentField = 0;

	unsigned int fieldAlocated = 0; //false
	unsigned int n_error = 0;

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
    	fieldAlocated = 0;

    	if((field[field_length-1] >> 7) & 0b00000001){

    		while(fieldAlocated == 0 && n_error < 55){
    			noCurrentField++;
    			n_error++;

	    		if(noCurrentField == 3){
					MsgSeqNum = byteDecoder32(field, field_length);
					printf(" MsgSeqNum: %d \n", MsgSeqNum);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 4){
					printf(" SendingTime: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 				
					}
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 5){ 
					TradeDate = byteDecoder32(field, field_length);
					printf(" TradeDate: %d \n", TradeDate);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 6){ 
					NoMDEntries = byteDecoder32(field, field_length);
					printf(" NoMDEntries: %d \n", NoMDEntries);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 7){ 
					MDEntriesSequence_PMap = byteDecoder32(field, field_length);
					printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
					MDEntriesSequence_PMap_length = field_length;
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 8 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 1)){
					MDUpdateAction = byteDecoder32(field, field_length); //copy function
					printf(" MDUpdateAction: %d \n", MDUpdateAction);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 8 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 1))){
					printf(" MDUpdateAction: %d \n", MDUpdateAction); //previous value
					fieldAlocated = 0;
				}

				else if(noCurrentField == 9 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 2)){
					printf(" MDEntryType: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 					
					}
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 9 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 2))){
					printf(" Do not implemented yet: <copy MDEntryType>. \n");
					fieldAlocated = 0;
				}

				else if(noCurrentField == 10 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 3)){
					printf(" SecurityID: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 10 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 3))){
					printf(" Do not implemented yet: <copy SecurityID>. \n");
					fieldAlocated = 0;
				}

				else if(noCurrentField == 11 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 4)){
					RptSeq = byteDecoder32(field, field_length); //copy function
					printf(" RptSeq: %d \n", RptSeq);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 11 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 4))){
					if(RptSeq > 0){ //if assigned
						RptSeq = RptSeq++; //increment
					}
					printf(" RptSeq: %d \n", RptSeq); //if undefined is the previous value
					fieldAlocated = 0;
				}

				else if(noCurrentField == 12 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 5)){
					printf(" QuoteCondition: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 					
					}
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 12 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 5))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 13 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
					printf(" MDEntryPx: do not implemented yet. \n");
					fieldAlocated = 1; //true
				}
				/*else if(noCurrentField == 13 || noCurrentField == 14){
					if(noCurrentField == 13 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
						printf(" MDEntryPx: Expoente: -2 ||");
					}
					else if(noCurrentField == 14 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 6)){
						printf(" Mantissa: ");
						for(int i=0; i < field_length; i++){
							printf("%02x ", (unsigned int) field[i]); 					
						}
						printf("\n");
					}
				}*/
				else if(noCurrentField == 14 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 7)){
					printf(" MDEntryInterestRate: do not implemented yet. \n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 15 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 8)){
					NumberOfOrders = byteDecoder32(field, field_length);
					printf(" NumberOfOrders: %d \n", NumberOfOrders);
					fieldAlocated = 1; //true			
				}
				else if(noCurrentField == 15 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 8))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 16){
					printf(" PriceType: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 17 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 9)){
					MDEntryTime = byteDecoder32(field, field_length);
					printf(" MDEntryTime: %d \n", MDEntryTime); //copy
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 17 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 9))){
					printf(" MDEntryTime: %d \n", MDEntryTime); //previous value
					fieldAlocated = 0;
				}

				else if(noCurrentField == 18){
					printf(" MDEntrySize: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <delta>");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 19 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 10)){
					MDEntryDate = byteDecoder32(field, field_length); //copy
					printf(" MDEntryDate: %d \n", MDEntryDate);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 19 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 10))){
					printf(" MDEntryDate: %d \n", MDEntryDate); //previous value
					fieldAlocated = 0;
				}

				else if(noCurrentField == 20 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 11)){
					MDInsertDate = byteDecoder32(field, field_length); //copy
					printf(" MDInsertDate: %d \n", MDInsertDate);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 20 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 11))){
					if(MDInsertDate > 0){
						printf(" MDInsertDate: %d \n", MDInsertDate); //previous value
					}
					fieldAlocated = 0;
				}

				else if(noCurrentField == 21 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 12)){
					MDInsertDate = byteDecoder32(field, field_length);
					printf(" MDInsertTime: %d \n", MDInsertDate); //copy
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 21 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 12))){
					if(MDInsertDate > 0){
						printf(" MDInsertTime: %d \n", MDInsertDate); //previous value
					}
					fieldAlocated = 0;
				}

				else if(noCurrentField == 22 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 13)){
					printf(" MDStreamID: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 22 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 13))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 23 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 14)){
					printf(" Currency: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <copy>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 23 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 14))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 24 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 15)){
					printf(" NetChgPrevDay: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet. ");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 24 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 15))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 25 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 16)){
					SellerDays = byteDecoder32(field, field_length);
					printf(" SellerDays: %d \n", SellerDays);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 25 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 16))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 26){
					printf(" TradeVolume: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <delta>");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 27 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 17)){
					printf(" TickDirection: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 27 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 17))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 28){
					printf(" TradeCondition: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.>");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 29){
					TradingSessionID = byteDecoder32(field, field_length);
					printf(" TradingSessionID: %d \n", TradingSessionID);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 30){
					OpenCloseSettlFlag = byteDecoder32(field, field_length);
					printf(" OpenCloseSettlFlag: %d \n", OpenCloseSettlFlag);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 31 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 18)){
					printf(" OrderID: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 31 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 18))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 32 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 19)){
					printf(" TradeID: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 32 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 19))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 33 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 20)){
					printf(" MDEntryBuyer: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 33 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 20))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 34 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 21)){
					printf(" MDEntrySeller: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 34 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 21))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 35 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 22)){
					MDEntryPositionNo = byteDecoder32(field, field_length);
					printf(" MDEntryPositionNo: %d \n", MDEntryPositionNo);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 35 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 22))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 36){
					SettPriceType = byteDecoder32(field, field_length);
					printf(" SettPriceType: %d \n", SettPriceType);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 37){
					LastTradeDate = byteDecoder32(field, field_length);
					printf(" LastTradeDate: %d \n", LastTradeDate);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 38){
					PriceAdjustmentMethod = byteDecoder32(field, field_length);
					printf(" PriceAdjustmentMethod: %d \n", PriceAdjustmentMethod);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 39 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 23)){
					printf(" PriceBandType: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet: <default>");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 39 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 23))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 40 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 24)){
					PriceLimitType = byteDecoder32(field, field_length);
					printf(" PriceLimitType: %d \n", PriceLimitType);
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 40 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 24))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 41 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 25)){
					printf(" LowLimitPrice: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 41 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 25))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 42 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 26)){
					printf(" HighLimitPrice: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 42 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 26))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 43 && pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 27)){
					printf(" TradingReferencePrice: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 43 && !(pMapCheck(MDEntriesSequence_PMap, MDEntriesSequence_PMap_length, 27))){
					fieldAlocated = 0;
				}

				else if(noCurrentField == 44){
					PriceBandMidpointPriceType = byteDecoder32(field, field_length);
					printf(" PriceBandMidpointPriceType: %d \n", PriceBandMidpointPriceType);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 45){
					printf(" AvgDailyTradedQty: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 46){
					printf(" ExpireDate: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 47){
					printf(" EarlyTermination: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 48){
					printf(" MaxTradeVol: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 49){
					NoUnderlyings = byteDecoder32(field, field_length);
					printf(" NoUnderlyings: %d \n", NoUnderlyings);
					fieldAlocated = 1; //true
				}

				else if(noCurrentField == 50){
					printf(" IndexSeq: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); 
					}
					printf(" Do not implemented yet.");
					printf("\n");
					fieldAlocated = 1; //true
				}
				else if(noCurrentField == 0 || noCurrentField == 1 || noCurrentField == 2){
					fieldAlocated = 1; //false
				}
				else{
					fieldAlocated = 0; //false
				}
			}
    		if(n_error > 50){
    			printf(" Field alocation error: ");
    			for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]);
				}
				printf("\n");
    		}

    		n_error = 0;
			field_length = 0;
    	}
    }
}

void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
	unsigned int field_length = 0;
	unsigned int noCurrentField = 0;
	__uint32_t MsgSeqNum = 0;
	__uint64_t SendingTime = 0;

	printf(" TemplateID: 144 || Template name=MDHeartbeat_144 \n");
	for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){
    		noCurrentField++;
    		if(noCurrentField == 3){ //&& (pmap >><< fieldOrder)
				MsgSeqNum = byteDecoder32(field, field_length);
				printf(" MsgSeqNum: %d \n", MsgSeqNum);
			}
			else if(noCurrentField == 4){
				printf(" SendingTime: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
				}
				printf("\n");
			}
			else if(!(noCurrentField == 0 || noCurrentField == 1 || noCurrentField == 2)){
				printf(" Field number %d do not identified: ", noCurrentField);
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

void test(){
	#define field_length 4
	__uint8_t field[field_length] = {0x7b, 0x30, 0x09, 0xc0};
	__uint32_t PMap = byteDecoder32(field, field_length);

	if(pMapCheck(PMap, 4, 5)){
		printf(" Field enabled \n");
	}
	else{
		printf(" Field unabled \n");
	}

	//printf(" Qtd de bits do PMap: %d \n", 32 - field_length);
	//printf("%d \n", byteDecoder32(field, field_length));
}