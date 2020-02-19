#include <stdio.h>
#include <string.h>
 
FILE* openFile(char* fileName);
void readField(FILE* file);
void identifyField(__uint8_t* field, unsigned int field_length);
void test();
 
int main () {
	readField(openFile("51_Inc_FAST.bin"));

    return 0;
}

void identifyField(__uint8_t* field, unsigned int field_length){
	unsigned int status_MsgSeqNum;
	__uint32_t MsgSeqNum;

	for(int i=0; i < field_length; i++){
		printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
	}
	printf("\n");
}

void readField(FILE* file){
	__uint8_t byte_aux = 0b10000000;
	__uint8_t byte;
	__uint8_t field[70000]; //2 bytes of MsgLength is the limit
	unsigned int field_length = 0;

	int status_MsgSeqNum = 0; //false
	int status_NoChunks = 0;
	int status_CurrentChunk = 0;
	int status_MsgLength = 0;

	int aux_MsgSeqNum[4];
	int aux_NoChuncks[2];
	int aux_CurrentChunk[2];
	int aux_MsgLength[2];

	int MsgSeqNum = 0;
	int NoChunks = 0;
	int CurrentChunk = 0;
	int MsgLength = 0;

	//while(fread(&byte, 1, 1, file) > 0){
	for(int i = 0; i < 1; i++){ //24

		if(status_MsgSeqNum == 0){ //getMsgSeqNum
			for(int i = 0; i < 4; i++){
				fread(&byte, 1, 1, file);
				aux_MsgSeqNum[i] = byte;
			}
			MsgSeqNum = (aux_MsgSeqNum[0] << 24) | (aux_MsgSeqNum[1] << 16) | (aux_MsgSeqNum[2] << 8) | (aux_MsgSeqNum[3]); //concat the 4 bytes
			printf("MsqSeqNum: %d \n", MsgSeqNum);
			status_MsgSeqNum = 1; //true
		}
		if(status_NoChunks == 0){
			for(int i = 0; i < 2; i++){
				fread(&byte, 1, 1, file);
				aux_NoChuncks[i] = byte;
			}
			NoChunks = (aux_NoChuncks[0] << 8) | (aux_NoChuncks[1]);
			printf("NoChuncks: %d \n", NoChunks);
			status_NoChunks = 1; //true
		}
		if(status_CurrentChunk == 0){
			for(int i = 0; i < 2; i++){
				fread(&byte, 1, 1, file);
				aux_CurrentChunk[1] = byte;
			}
			CurrentChunk = (aux_CurrentChunk[0] << 8) | (aux_CurrentChunk[1]);
			printf("CurrentChunk: %d \n", CurrentChunk);
			status_CurrentChunk = 1;
		}

		if(status_MsgLength == 0){
			for(int i = 0; i < 2; i++){
				fread(&byte, 1, 1, file);
				aux_MsgLength[i] = byte;
			}
			MsgLength = (aux_MsgLength[0] << 8) | (aux_MsgLength[1]);
			printf("MsgLength: %d \n", MsgLength);
			status_MsgLength = 1;
		}

		if(status_MsgSeqNum == 1 && status_NoChunks == 1 && status_CurrentChunk == 1 && status_MsgLength == 1){ //if all the header was got
			for(int i = 0; i < MsgLength; i++){
				fread(&byte, 1, 1, file);
				if(field_length == 0){
					field[0] = byte;
					field_length++;
				}
				else{
					field[field_length] = byte;
					field_length++;
				}

				if((byte >> 7 && byte_aux >> 7)){ // if the MSB is 1
					identifyField(field, field_length);
					strcpy(field, "");
					field_length = 0;
				}
			}
			status_MsgSeqNum = 0; //clean all to get the next header
			status_NoChunks = 0;
			status_CurrentChunk = 0;
			status_MsgLength = 0; 
		}
	}
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}

void test(){
	//test blocks here
}