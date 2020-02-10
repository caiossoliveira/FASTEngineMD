#include <stdio.h>
 
FILE* openFile(char* fileName);
 
/* main function to call above defined function */
int main () {
	unsigned char byte;
	//__int8_t byte_aux = 0b10000000;
	unsigned int byte_aux = 128;

	FILE* file = openFile("51_Inc_FAST.bin");

	//while(fread(&byte, 1, 1, file) > 0){
	for(int i=0; i < 10; i++){
		fread(&byte, 1, 1, file);

		printf("%x ", byte);

		if((byte - byte_aux) <= 128){ //if( byte AND 10000000) >= 10000000 : means the first bit is 1.
			printf("It is the last byte. \n"); //- Byte: %d Byte_aux: %d \n", byte, byte_aux);
		}
		else{
			printf("It is not the last byte. \n");
		}
	}

   return 0;
}

FILE* openFile(char* fileName) {
   static FILE* file;
   file = fopen(fileName, "rb"); 
   return file;
}