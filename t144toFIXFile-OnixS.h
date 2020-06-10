#include <string.h>

void t144toFIX(__uint32_t MsgSeqNum, __uint64_t SendintTime){

	#define PRINTI(file, var) if(var > 0) printf(file, var)	//print for int 
	#define PRINTD(file, var) if(var > 0.00) printf(file, var)	//print for decimals
	#define PRINTS(file, var) if(strcmp(var, "EMPTY") != 0) printf(file, var)	//(different of NULL) print for strings

	const char* ApplVerID = "7"; //"9";
	const char* MsgType = "0";
	const int SecurityIDSource = 8;
	const char SecurityExchange[4] = "BVMF";

	//template
	printf("%d: ", MsgSeqNum);
	printf("8=FIXT1.1|9=0|");
	PRINTS("35=%s|", MsgType);
	PRINTS("1128=%s|", ApplVerID);
	printf("34=%d|", MsgSeqNum);
	printf("52=%ld|", SendintTime);	
	printf("10=000|");

	printf("\n");
}