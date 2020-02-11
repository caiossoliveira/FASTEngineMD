#include <stdio.h>
#include<string.h>
 
FILE* openFile(char* fileName);
void fieldRead(FILE* file);
void fieldIdentify(unsigned char* field, unsigned int field_length);
void test();
 
int main () {

	FILE* file = openFile("51_Inc_FAST.bin");
	fieldRead(file);

	//test();

   return 0;
}

void fieldIdentify(unsigned char* field, unsigned int field_length){

	for(int i=0; i < field_length; i++){
		printf("%02x ", (unsigned int) field[i]); //%u to a series of bytes while(*field){printf("%02x ", (unsigned int) *field++); // cast the character to an unsigned type to be safe}
	}

	printf("\n");
}

void fieldRead(FILE* file){
	__uint8_t byte_aux = 0b10000000;
	unsigned char byte;
	unsigned char field[5000];
	unsigned int field_length = 0;

	//while(fread(&byte, 1, 1, file) > 0){
	for(int i = 0; i < 20; i++){
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
			fieldIdentify(field, field_length);
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

void test(){
	__uint8_t byte = 0b00000000;
	__uint8_t field[5000];

	field[0] = byte;
	field[1] = byte;

	printf("Field length: %ld --> ", strlen(field));
	printf("%02x ", (unsigned int) field[0]); // cast the character to an unsigned type to be safe
	printf("%02x ", (unsigned int) field[1]); // cast the character to an unsigned type to be safe
	

	//fieldIdentify(field);

	//printf("%02x", byte2);
}