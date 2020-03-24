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
	unsigned int Sequence_PMap_length = 0;
	unsigned int noCurrentField = 0;

	__uint32_t MsgSeqNum = 0;
	__uint64_t SendintTime = 0;
	__uint32_t TradeDate = 0;
	__uint32_t NoMDEntries = 0;
	__uint32_t Sequence_PMap = 0;
	__uint32_t MDUpdateAction = 1;
	__uint64_t SecurityID = 0;
	__uint32_t RptSeq = 0;


	printf(" TemplateID: 145 || Template name=MDIncRefresh_145 \n");
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
			else if(noCurrentField == 5){ 
				TradeDate = byteDecoder32(field, field_length);
				printf(" TradeDate: %d \n", TradeDate);
			}
			else if(noCurrentField == 6){ 
				NoMDEntries = byteDecoder32(field, field_length);
				printf(" NoMDEntries: %d \n", NoMDEntries);
			}
			else if(noCurrentField == 7){ 
				Sequence_PMap = byteDecoder32(field, field_length);
				printf(" Sequence_PMap: %d \n", Sequence_PMap);
				Sequence_PMap_length = field_length;
			}
			else if(noCurrentField == 8){
				if(pMapCheck(Sequence_PMap, Sequence_PMap_length, 1)){
					MDUpdateAction = byteDecoder32(field, field_length); //copy function
					printf(" MDUpdateAction: %d \n", MDUpdateAction);
				}
				else{
					printf(" MDUpdateAction: %d \n", MDUpdateAction); //previous value
				}
			}
			else if(noCurrentField == 9){
				if(pMapCheck(Sequence_PMap, Sequence_PMap_length, 1)){
					printf(" MDEntryType: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
					}
					printf("\n");
				}
				else{
					printf(" Do not implemented yet: <copy MDEntryType>. \n");
				}
			}
			else if(noCurrentField == 10){
				if(pMapCheck(Sequence_PMap, Sequence_PMap_length, 1)){
					printf(" SecurityID: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
					}
					printf("\n");
				}
				else{
					printf(" Do not implemented yet: <copy SecurityID>. \n");
				}
			}
			else if(noCurrentField == 11){
				if(pMapCheck(Sequence_PMap, Sequence_PMap_length, 1)){
					RptSeq = byteDecoder32(field, field_length); //copy function
					printf(" RptSeq: %d \n", RptSeq);
				}
				else{
					if(RptSeq > 0){ //if assigned
						RptSeq = RptSeq++; //increment
					}
					printf(" RptSeq: %d \n", RptSeq); //if undefined is the previous value
				}
			}
			else if(noCurrentField == 12){
				if(pMapCheck(Sequence_PMap, Sequence_PMap_length, 1)){
					printf(" QuoteCondition: ");
					for(int i=0; i < field_length; i++){
						printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
					}
					printf("\n");
				}
				else{
					printf(" QuoteCondition: Absent. \n");
				}
			}
			else if(!(noCurrentField == 0 || noCurrentField == 1 || noCurrentField == 2)){
				printf(" Field number %d do not identified: ", noCurrentField);
				for(int i=0; i < field_length; i++){
					printf("%02x ", (unsigned int) field[i]);
				}
				printf("\n");
			}
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

		printf(" MsqSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);

		for(int i = 0; i < MsgLength; i++){

			fread(&byte, 1, 1, file);

			FASTMessage[FASTMessage_length] = byte;
			FASTMessage_length++;
		}

		identifyTemplate(FASTMessage, FASTMessage_length);

		FASTMessage_length = 0;

		printf(" ------------------------------------------------------------------------\n\n");
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