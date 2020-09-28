#include <stdio.h>
#include <string.h>

void printFields(__uint8_t* FASTMessage, unsigned int FASTMessage_length){
	__uint8_t field[7000];
    __uint32_t PMap = 0;
    __uint16_t TemplateID = 0;
    unsigned int field_length = 0;
	unsigned int noCurrentField = 0;

    for(int i = 0; i < FASTMessage_length; i++){
    	field[field_length] = FASTMessage[i];
    	field_length++;

    	if((field[field_length-1] >> 7) & 0b00000001){ //if MSB is 1
			printf(" field: ");
			for(int i=0; i < field_length; i++){
				printf("%02x ", (unsigned int) field[i]); 
			}
			printf("\n");

			strcpy(field, "");
			field_length = 0;
    	}
    }
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
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

	for(int i = 0; i < 1266; i++){ // number of messages //1250
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

		if(MsgSeqNum > 731915){ //only to compare with the FIX log
			printf("\n-----------------------------------------------------------------------------------------------------");

			printf(" \n Message %d: \n", i+1);
			printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n\n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);
			printFields(FASTMessage, FASTMessage_length);
		}
		FASTMessage_length = 0;

		//printf(" ---------------------------------------------------------------------------\n\n");
	}
}

int main () {
	//readMessage(openFile("51_Inc_FAST.bin"));
	readMessage(openFile("filteredLog.bin"));
    return 0;
}