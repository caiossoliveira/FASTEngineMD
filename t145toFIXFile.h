#include <string.h>

void print64i(char* text, __int64_t var, char* buff);
void print64ui(char* text, __uint64_t var, char* buff);
void print32ui(char* text, __uint32_t var, char* buff);
void prints(char* text, char* var, char* buff);
void printvs(char* text, char (*var)[1500], char* buff);
void printd(char* text, float var, char* buff);

void t145toFIX(
	//Template
	__uint32_t MsgSeqNum, __uint32_t TradeDate, __uint64_t SendintTime,
	//SequenceMDEntries
	__uint32_t NoMDEntries, __uint32_t MDEntriesSequence_PMap, __uint32_t MDEntriesSequence_PMap_length, __uint32_t* MDUpdateAction,
	__uint32_t* RptSeq, __uint32_t* NumberOfOrders, __uint32_t* MDEntryTime, __uint32_t* MDEntryDate, __uint32_t* MDInsertDate,
	__uint32_t* MDInsertTime, __uint32_t* SellerDays, __uint32_t* TradingSessionID, __uint32_t* OpenCloseSettlFlag, 
	__uint32_t* MDEntryPositionNo, __uint32_t* SettPriceType, __uint32_t* LastTradeDate, __uint32_t* PriceAdjustmentMethod,
	__uint32_t* PriceLimitType, __uint32_t* PriceBandMidpointPriceType,
	__uint64_t* SecurityID, __uint64_t* MDEntrySize, __uint64_t* TradeVolume, __uint64_t* AvgDailyTradedQty, 
	__uint64_t* ExpireDate, __uint64_t* EarlyTermination, __uint64_t* MaxTradeVol,
	char (*MDEntryType)[1500], char (*QuoteCondition)[1500], char (*PriceType)[1500], char (*MDStreamID)[1500], char (*Currency)[1500], 
	char (*TickDirection)[1500], char (*TradeCondition)[1500], char (*OrderID)[1500], char (*TradeID)[1500],
	char (*MDEntryBuyer)[1500], char (*MDEntrySeller)[1500], char (*PriceBandType)[1500], 
	float* MDEntryPx, float* MDEntryInterestRate, float* NetChgPrevDay, float* LowLimitPrice, float* HighLimitPrice, 
	float* TradingReferencePrice,
	//SequenceUnderlyings
	__uint32_t NoUnderlyings, __uint32_t UnderlyingPXType, __uint64_t UnderlyingSecurityID, __uint64_t* IndexSeq,
	float UnderlyingPx
){
	#define PRINTI(file, var) { printf(file, var); }//if(var != -80) printi(file, var, buff);}	//print for int 
	#define PRINTD(file, var) { printf(file, var); }//if(var > 0.00) printd(file, var, buff);}	//print for decimals
	#define PRINTS(file, var) { printf(file, var); }//if(strcmp(var, "EMPTY") != 0); prints(file, var, buff);}	//(different of NULL) print for strings

	char buff[7000];
	strcpy(buff, "");
	FILE *file;

	//file = fopen("../validate-FASTEngineMD/logFIX51-FASTEngineMD.txt", "a"); //writing in appending mode
	file = fopen("validation/logFIX51-FASTEngineMD.txt", "a");

	char* ApplVerID = "9";
	char* MsgType = "X";
	int SecurityIDSource = 8;
	char* SecurityExchange = "BVMF";

	//template
	prints("1128=%s|", ApplVerID, buff);
	prints("35=%s|", MsgType, buff);
	print32ui("34=%d|", MsgSeqNum, buff);
	print64ui("52=%ld|", SendintTime, buff);
	print32ui("75=%d|", TradeDate, buff);

	//SequenceMDEntries
	if(NoMDEntries > 0){ //sequence
		print32ui("268=%d|", NoMDEntries, buff);
		for(int i = 0; i < NoMDEntries; i++){
			print32ui("279=%d|", MDUpdateAction[i], buff);
			printvs("269=%s|", MDEntryType + i, buff);
			print32ui("22=%d|", SecurityIDSource, buff); //cte
			prints("207=%s|", SecurityExchange, buff);
			print64ui("48=%ld|", SecurityID[i], buff);
			print32ui("83=%d|", RptSeq[i], buff);
			printvs("276=%s|", QuoteCondition + i, buff);
			printd("270=%.4g|", MDEntryPx[i], buff);
			printd("37014=%.2f|", MDEntryInterestRate[i], buff);
			print32ui("346=%d|", NumberOfOrders[i], buff);
			printvs("423=%s|", PriceType + i, buff);
			print32ui("273=%d|", MDEntryTime[i], buff);
			print64i("271=%ld|", MDEntrySize[i], buff);
			print32ui("272=%d|", MDEntryDate[i], buff);
			print32ui("37016=%d|", MDInsertDate[i], buff);
			print32ui("37017=%d|", MDInsertTime[i], buff);
			printvs("1500=%s|", MDStreamID + i, buff);
			printvs("15=%s|", Currency + i, buff);
			printd("451=%.2f|", NetChgPrevDay[i], buff);
			print32ui("287=%d|", SellerDays[i], buff);
			print64ui("1020=%ld|", TradeVolume[i], buff);
			printvs("274=%s|", TickDirection + i, buff);
			printvs("277=%s|", TradeCondition + i, buff);
			print32ui("336=%d|", TradingSessionID[i], buff);
			print32ui("286=%d|", OpenCloseSettlFlag[i], buff);
			printvs("37=%s|", OrderID + i, buff);
			printvs("1003=%s|", TradeID + i, buff);
			printvs("288=%s|", MDEntryBuyer + i, buff);
			printvs("289=%s|", MDEntrySeller + i, buff);
			print32ui("290=%d|", MDEntryPositionNo[i], buff);
			print32ui("731=%d|", SettPriceType[i], buff);
			print32ui("9325=%d|", LastTradeDate[i], buff);	
			print32ui("37013=%d|", PriceAdjustmentMethod[i], buff);
			printvs("6939=%s|", PriceBandType + i, buff);
			print32ui("1306=%d|", PriceLimitType[i], buff);
			printd("1148=%.2f|", LowLimitPrice[i], buff);
			printd("1149=%.2f|", HighLimitPrice[i], buff);
			printd("1150=%.2f|", TradingReferencePrice[i], buff);
			print32ui("37008=%d|", PriceBandMidpointPriceType[i], buff);
			print64ui("37003=%ld|", AvgDailyTradedQty[i], buff);
			print64ui("432=%ld|", ExpireDate[i], buff);
			print64ui("37019=%ld|", EarlyTermination[i], buff);
			print64ui("1140=%ld|", MaxTradeVol[i], buff);

			//SequenceUnderlyings
			if(NoUnderlyings > 0){
				print32ui("711=%d|", NoUnderlyings, buff);
			}

			print64ui("37100=%ld", IndexSeq[i], buff);
		}
	}

	printf("\n");

	strcat(buff, "\n");

	fputs(buff, file);
	fclose(file);
}

void print64ui(char* text, __uint64_t var, char* buff){
	if((int) var > 0){
		char auxBuff[150];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void print64i(char* text, __int64_t var, char* buff){
	if((int) var != 0xF0000000){ //biggest 32bits negative number
		char auxBuff[150];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void print32ui(char* text, __uint32_t var, char* buff){
	if((int) var >= 0){
		char auxBuff[150];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTI(text, var);
	}
}

void printvs(char* text, char (*var)[1500], char* buff){
	if(strcmp(var[0], "EMPTY") != 0 && var[0][0] != '\0'){
		char auxBuff[150];
		sprintf(auxBuff, text, var[0]);
		strcat(buff, auxBuff);
		PRINTS(text, var);
	}
}

void prints(char* text, char* var, char* buff){ 
	if(strcmp(var, "EMPTY") != 0 && *var != '\0'){
		char auxBuff[150];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTS(text, var);
	}
}

void printd(char* text, float var, char* buff){
	if(var > 0.00){
		char auxBuff[150];
		sprintf(auxBuff, text, var);
		strcat(buff, auxBuff);
		PRINTD(text, var);
	}
}