#include <stdio.h>
#include <string.h>
#include <unistd.h>

FILE *fileFLog; 
__uint8_t aux;
__uint8_t fullMessage[1500];

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

void identifyTemplate(__uint8_t* FASTMessage, int FASTMessage_length){
	__uint8_t field[1500];
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
				if(!(PMap & 0b01000000)){
					printf(" TemplateID do not specified in the message. \n");
				}
			}
			else if(noCurrentField == 2 && (PMap & 0b01000000)){
				TemplateID = bytetoPMapDecoder(field, field_length);
			}
			if(TemplateID > 0){
                if(/*TemplateID == 144 ||*/ TemplateID == 145){
                    for(int i = 0; i < FASTMessage_length; i++){
                        fullMessage[i + 10] = FASTMessage[i];
                    }

                    fileFLog = fopen("filteredLog145.bin", "ab");
                    fwrite(fullMessage, 1, FASTMessage_length + 10, fileFLog);
                    fclose(fileFLog);

                    break;
                }
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

	for(int i = 0; i < 2000; i++){ // number of messages //1250
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

		if(MsgSeqNum > 731915 && MsgSeqNum < 732490){ //only to compare with the FIX log
			printf("\n-----------------------------------------------------------------------------------------------------");

			/*printf("\n ");
			for(int i = 0; i < 10; i++){ //print header
				printf("%02x", header[i]);
			}*/

            /*for(int i = 0; i < 10; i++){
                aux = header[i];
                fputc(aux, fileFLog); //filtered log
            }

            for(int i = 0; i < FASTMessage_length; i++){
                aux = FASTMessage[i];
                fputc(aux, fileFLog); //filtered log
            }*/

            for(int i = 0; i < 1500; i++){
                fullMessage[i] = 0;
            }

            for(int i = 0; i < 10; i++){
                fullMessage[i] = header[i];
            }
            
			printf(" \n Message %d: \n", i+1);
			printf(" MsgSeqNum: %d \n NoChunks: %d \n CurrentChunk: %d \n MsgLength: %d \n\n", MsgSeqNum, NoChunks, CurrentChunk, MsgLength);
			identifyTemplate(FASTMessage, FASTMessage_length);
		}
		FASTMessage_length = 0;

		//printf(" ---------------------------------------------------------------------------\n\n");
	}
}

int main () {

	readMessage(openFile("51_Inc_FAST.bin"));
	//readMessage(openFile("filteredLog.bin"));

    return 0;
}