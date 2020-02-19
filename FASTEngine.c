#include <stdio.h>
#include <string.h>
 
FILE* openFile(char* fileName);
void readMessage(FILE* file);
void identifyField(__uint8_t* field, unsigned int field_length);
void test();
 
int main () {
	readMessage(openFile("51_Inc_FAST.bin"));

    return 0;
}

void identifyField(__uint8_t* field, unsigned int field_length){

	for(int i=0; i < field_length; i++){
		printf(" %02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe
	}
	printf("\n");
}

void readMessage(FILE* file){
	__uint8_t header[10];
	__uint8_t byte_aux = 0b10000000;
	__uint8_t byte;
	__uint8_t field[70000]; //2 bytes of MsgLength is the limit
	unsigned int field_length = 0;
	int MsgSeqNum = 0;
	int NoChunks = 0;
	int CurrentChunk = 0;
	int MsgLength = 0;

	//while(fread(&byte, 1, 1, file) > 0){
	for(int i = 0; i < 1; i++){ // number of messages
		for(int i = 0; i < 10; i++){ //read header
			fread(&byte, 1, 1, file);
			header[i] = byte;
		}
		MsgSeqNum = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | (header[3]); //concatenate the bytes
		NoChunks = (header[4] << 8) | (header[5]);
		CurrentChunk = (header[6] << 8) | (header[7]);
		MsgLength = (header[8] << 8) | (header[9]);

		printf(" MsqSeqNum: %d \n", MsgSeqNum);
		printf(" NoChunks: %d \n", NoChunks);
		printf(" CurrentChunk: %d \n", CurrentChunk);
		printf(" MsgLength: %d \n", MsgLength);
		printf(" Fields: \n");

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