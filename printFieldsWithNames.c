#include <stdio.h>
#include <string.h>
 
FILE* openFile(char* fileName);
__uint32_t bytetoInt32Decoder(__uint8_t* field);
__uint32_t bytetoPMapDecoder(__uint8_t* field, __int32_t field_length);
__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length);
void getFieldD(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length);
__uint32_t fieldLength(__uint8_t* field);
int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField);
int isDecimal(unsigned int PMap_order);
void templateDoNotIdentified(__uint16_t TemplateID);
void readMessage(FILE* file);
void identifyTemplate(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDIncRefresh_145(__uint32_t PMap, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
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

	#define PRINT \
		n = *aux == 0x00 ? 1 : fieldLength(aux); \
		for(int i = 0; i < n; i++){ \
			printf("%02x ", *aux++); \
		}

	__uint8_t* ptr_FASTMessage = FASTMessage+3; //MsgSeqNum is the first here but the third in the message
	__uint8_t field[7000] = {0x80};

	//SequenceMDEntries
	__uint32_t NoMDEntries = 0, MDEntriesSequence_PMap = 0, MDEntriesSequence_PMap_length = 0;
	__uint32_t NoUnderlyings = 0;

	int n = 0;

	__uint8_t* aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	printf("\n MsgSeqNum: ");
	PRINT;

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	printf("\n SendintTime: ");
	PRINT;

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	printf("\n TradeDate: ");
	PRINT;

	aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
	NoMDEntries = bytetoInt32Decoder(aux);
	printf("\n NoMDEntries: ");
	PRINT;

	if(NoMDEntries > 0){ //sequence
		for(int i = 0; i < NoMDEntries; i++)
		{
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			MDEntriesSequence_PMap_length = fieldLength(aux);
			MDEntriesSequence_PMap = bytetoInt32Decoder(aux);

			printf("\n\n PMAP: ");
			for(int i=0; i < MDEntriesSequence_PMap_length; i++){
				printf("%02x ", (unsigned int) *aux++); 
			}

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDUPDATEACTION, MDEntriesSequence_PMap_length);
			printf("\n MDUpdateAction: ");
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTYPE, MDEntriesSequence_PMap_length);
			printf("\n MDEntryType: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SECURITYID, MDEntriesSequence_PMap_length);
			printf("\n SecurityID: ");
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, RPTSEQ, MDEntriesSequence_PMap_length);
			printf("\n RptSeq: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, QUOTECONDITION, MDEntriesSequence_PMap_length);
			printf("\n QuoteCondition: ");
			PRINT;

			printf("\n MDEntryPx: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPX, MDEntriesSequence_PMap_length);
			
			printf("\n MDEntryInterestRate: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYINTERESTRATE, MDEntriesSequence_PMap_length);
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NUMBEROFORDERS, MDEntriesSequence_PMap_length);
			printf("\n NumberOfOrders: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n PriceType: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTIME, MDEntriesSequence_PMap_length);
			printf("\n MDEntryTime: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n MDEntrySize: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYDATE, MDEntriesSequence_PMap_length);
			printf("\n MDEntryDate: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTDATE, MDEntriesSequence_PMap_length);
			printf("\n MDInsertDate: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTTIME, MDEntriesSequence_PMap_length);
			printf("\n MDInsertTime: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDSTREAMID, MDEntriesSequence_PMap_length);
			printf("\n MDStreamID: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, CURRENCY, MDEntriesSequence_PMap_length);
			printf("\n Currency: "); 
			PRINT;
			
			printf("\n NetChgPrevDay: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NETCHGPREVDAY, MDEntriesSequence_PMap_length);
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SELLERDAYS, MDEntriesSequence_PMap_length);
			printf("\n SellerDays: ");
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n TradeVolume: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TICKDIRECTION, MDEntriesSequence_PMap_length);
			printf("\n TickDirection: ");
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n TradeCondition: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n TradingSessionID: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n OpenCloseSettlFlag: "); 
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, ORDERID, MDEntriesSequence_PMap_length);
			printf("\n OrderID: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADEID, MDEntriesSequence_PMap_length);
			printf("\n TradeID: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYBUYER, MDEntriesSequence_PMap_length);
			printf("\n MDEntryBuyer: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYSELLER, MDEntriesSequence_PMap_length);
			printf("\n MDEntrySeller: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPOSITIONNO, MDEntriesSequence_PMap_length);
			printf("\n MDEntryPositionNo: "); 
			PRINT;
				
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n SettPriceType: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n LastTradeDate: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n PriceAdjustmentMethod: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICEBANDTYPE, MDEntriesSequence_PMap_length);
			printf("\n PriceBandType: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICELIMITTYPE, MDEntriesSequence_PMap_length);
			printf("\n PriceLimitType: ");
			PRINT;
			
			printf("\n LowLimitPrice: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, LOWLIMITPRICE, MDEntriesSequence_PMap_length);
			
			printf("\n HighLimitPrice: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, HIGHLIMITPRICE, MDEntriesSequence_PMap_length);

			printf("\n TradingReferencePrice: ");
			getFieldD(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADINGREFERENCEPRICE, MDEntriesSequence_PMap_length);
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n PriceBandMidpointPriceType: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n AvgDailyTradedQty: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n ExpireDate: "); 
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n EarlyTermination: ");
			PRINT;

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n MaxTradeVol: ");
			PRINT;
			
			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n NoUnderlyings: ");
			PRINT;

			if(NoUnderlyings > 0){}

			aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
			printf("\n IndexSeq: ");
			PRINT;
		}
	}

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
				printf(" MsgSeqNum: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
				}
				printf("\n");
			}
			else if(noTemplateField == 4){
				printf(" SendingTime: ");
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]); 
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
				//printf(" PMap: %02x \n", PMap);
				if(!(PMap & 0b01000000)){
					//printf(" TemplateID do not specified in the message. \n");
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
	for(int i = 0; i < 1700; i++){ // number of messages //1250
		for(int i = 0; i < 10; i++){ //read header
			fread(&byte, 1, 1, file);
			header[i] = byte;
		}
		MsgSeqNum = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | (header[3]); //concatenate the bytes
		NoChunks = (header[4] << 8) | (header[5]);
		CurrentChunk = (header[6] << 8) | (header[7]);
		MsgLength = (header[8] << 8) | (header[9]);

		for(int i = 0; i < MsgLength; i++){

			fread(&byte, 1, 1, file);

			FASTMessage[FASTMessage_length] = byte;
			FASTMessage_length++;
		}

		if(MsgSeqNum > 731800){ //only to compare with the FIX log
			printf("\n-----------------------------------------------------------------------------------------------------");
			printf(" \n Message %d: \n", i+1);
			printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);
			identifyTemplate(FASTMessage, FASTMessage_length);
		}
		FASTMessage_length = 0;

		printf("\n-----------------------------------------------------------------------------------------------------\n");
	}
}

void templateDoNotIdentified(__uint16_t TemplateID){
	printf(" TemplateID do not identified: %d \n", TemplateID);
}

__uint8_t* getField(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length){
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

void getFieldD(__uint8_t* newField, __uint8_t** FASTMessage, int FASTMessage_length, __uint32_t PMap, unsigned int PMap_order, unsigned int PMap_length){
	const __uint32_t aux_bitMap = 0b00000000000000000000000000000001;
    int field_length = 0, n = 0;
    __uint8_t* ptrExp;
    __uint8_t* ptrMant;
    __uint8_t* aux;

	for(int i = 0; i < 7000; i++){ //clean the buffer
		newField[i] = 0x00;
	}

	if(PMap_order > 0){
		if(isDecimal(PMap_order)){ //sure if the field is decimal
			if((pMapCheck(PMap, PMap_length, PMap_order))){ //if the bitmap's bit is 1
				ptrExp = getField(newField, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); //there is a exp in the msg
				n = fieldLength(ptrExp); 
				aux = ptrExp;
				for(int i = 0; i < n; i++){ 
					printf("%02x ", *aux++); 
				}
				if(*ptrExp != 0x80){ //if it is no zero
					printf("\n           ");
					ptrMant = getField(newField, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); 
					n = fieldLength(ptrMant); 
					for(int i = 0; i < n; i++){ 
						printf("%02x ", *ptrMant++); 
					}
				}
			}
			else{ //if the bit is 0
				if(PMap_order == MDENTRYPX || PMap_order == MDENTRYINTERESTRATE){
					printf("-2 \n           ");
					ptrMant = getField(newField, FASTMessage, FASTMessage_length, PMap, PMap_order, PMap_length); 
					n = fieldLength(ptrMant); 
					for(int i = 0; i < n; i++){ 
						printf(" %02x", *ptrMant++); 
					}
				}
			}
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

int pMapCheck(__uint32_t PMap, unsigned int PMap_length, __uint32_t noCurrentField){
	__uint32_t aux_bitMap = 0b00000000000000000000000000000001;

	//if(PMap & (aux_bitMap << (32 - PMap_length - noCurrentField))){ //if bitsmap's bit is 1
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
	//printf("\n");
	while(stop){
		//printf(" %02x", *aux);
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
   file = fopen(fileName, "rb"); 
   return file;
}