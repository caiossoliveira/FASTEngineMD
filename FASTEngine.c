#include <stdio.h>
#include <string.h>
 
FILE* openFile(char* fileName);
__uint32_t byteDecoder32(__uint8_t* field, unsigned int field_length);
void readMessage(FILE* file);
void identifyField(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDecoder(__uint16_t TemplateID, __uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDHeartbeat_144(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void MDIncRefresh_145(__uint8_t* FASTMessage, unsigned int FASTMessage_length);
void templateDoNotIdentified(__uint16_t TemplateID);
void test();
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));
    return 0;
}

void MDIncRefresh_145(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
	unsigned int field_length = 0;
	unsigned int noCurrentField = 0;

	__uint32_t MsgSeqNum = 0;
	__uint64_t SendintTime = 0;
	__uint32_t TradeDate = 0;


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

void templateDecoder(__uint16_t TemplateID, __uint8_t* FASTMessage, unsigned int FASTMessage_length){
	switch(TemplateID)
	{
		case 144 : MDHeartbeat_144(FASTMessage, FASTMessage_length);
		break;

		case 145: MDIncRefresh_145(FASTMessage, FASTMessage_length);
		break;

		default : templateDoNotIdentified(TemplateID);
	}
}

void identifyField(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
    __uint32_t PMAP = 0;
    __uint16_t TemplateID = 0;
    unsigned int field_length = 0;
	unsigned int noCurrentField = 0;

    for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){
    		noCurrentField++;
    		if(noCurrentField == 1){
				printf(" PMap: %d \n", field[0]);
			}
			else if(noCurrentField == 2){
				TemplateID = byteDecoder32(field, field_length);
			}
			if(TemplateID > 0){
				//talvez colocar um break aqui e mandar a msg pro template
				//templateDecoder(TemplateID, field, field_length, noCurrentField);
				templateDecoder(TemplateID, FASTMessage, FASTMessage_length);
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
	for(int i = 0; i < 1171; i++){ // number of messages
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

		identifyField(FASTMessage, FASTMessage_length);

		FASTMessage_length = 0;

		printf(" ------------------------------------------------------------------------\n\n");
	}
}

void templateDoNotIdentified(__uint16_t TemplateID){
	printf(" TemplateID do not identified: %d \n", TemplateID);
}

__uint32_t byteDecoder32(__uint8_t* field, unsigned int field_length){
    int j = field_length - 2; //1
    __uint32_t result;
    
    result = field[field_length-1];
    
    for(int i = 0; i < field_length - 1; i++){
        result = result << 1;
        result = (field[j] << ((i+1) * 8)) | result;
        j--;
    } 
    result = result >> (field_length-1); 
    
    return result;
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}

void test(){

}