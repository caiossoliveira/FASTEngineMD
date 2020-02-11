#include <stdio.h>
 
FILE* openFile(char* fileName);
 
int main () {

	unsigned char byte;
	__uint8_t byte_aux = 0b10000000;

	FILE* file = openFile("51_Inc_FAST.bin");

	while(fread(&byte, 1, 1, file) > 0){
		printf("%x ", byte);
		if((byte >> 7 && byte_aux >> 7)){
			printf("Last byte of the field \n");
		}
		else{
			printf("Do not last byte of the field \n");
		}
	}

   return 0;
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}