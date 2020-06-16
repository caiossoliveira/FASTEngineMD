#include <stdio.h>

void main(){
	static FILE* file;
	file = fopen("fixtestlog.txt", "r"); 

	__uint8_t byte;

	while(fread(&byte, 1, 1, file) > 0){ 
		printf("%c", byte);
	}

	fclose(file);
}


