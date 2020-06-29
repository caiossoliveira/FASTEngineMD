#include <string.h>

void printi(char* text, __int64_t var, char* buff){
	char auxBuff[150];
	sprintf(auxBuff, text, var);
	strcat(buff, auxBuff);
}

void prints(char* text, char* var, char* buff){
	char auxBuff[150];
	sprintf(auxBuff, text, var);
	strcat(buff, auxBuff);
}

void printd(char* text, float var, char* buff){
	char auxBuff[150];
	sprintf(auxBuff, text, var);
	strcat(buff, auxBuff);
}

void t145toFIX(
	//Template
	__uint32_t MsgSeqNum, __uint32_t TradeDate, __uint64_t SendintTime,
	//SequenceMDEntries
	__uint32_t NoMDEntries, __uint32_t MDEntriesSequence_PMap, __uint32_t MDEntriesSequence_PMap_length, __uint32_t MDUpdateAction,
	__uint32_t RptSeq, __uint32_t NumberOfOrders, __uint32_t MDEntryTime, __uint32_t MDEntryDate, __uint32_t MDInsertDate,
	__uint32_t MDInsertTime, __uint32_t SellerDays, __uint32_t TradingSessionID, __uint32_t OpenCloseSettlFlag, 
	__uint32_t MDEntryPositionNo, __uint32_t SettPriceType, __uint32_t LastTradeDate, __uint32_t PriceAdjustmentMethod,
	__uint32_t PriceLimitType, __uint32_t PriceBandMidpointPriceType,
	__uint64_t SecurityID, __uint64_t MDEntrySize, __uint64_t TradeVolume, __uint64_t AvgDailyTradedQty, 
	__uint64_t ExpireDate, __uint64_t EarlyTermination, __uint64_t MaxTradeVol,
	char* MDEntryType, char* QuoteCondition, char* PriceType, char* MDStreamID, char* Currency, char* TickDirection, 
	char* TradeCondition, char* OrderID, char* TradeID, char* MDEntryBuyer, char* MDEntrySeller, char* PriceBandType, 
	float MDEntryPx, float MDEntryInterestRate, float NetChgPrevDay, float LowLimitPrice, float HighLimitPrice, 
	float TradingReferencePrice,
	//SequenceUnderlyings
	__uint32_t NoUnderlyings, __uint32_t UnderlyingPXType, __uint64_t UnderlyingSecurityID, __uint64_t IndexSeq,
	float UnderlyingPx
){

	#define PRINTI(file, var) if(var != -80){ printf(file, var); printi(file, var, buff);}	//print for int 
	#define PRINTD(file, var) if(var > 0.00){ printf(file, var); printd(file, var, buff);}	//print for decimals
	#define PRINTS(file, var) if(strcmp(var, "EMPTY") != 0){ printf(file, var); prints(file, var, buff);}	//(different of NULL) print for strings

	char buff[7000];
	strcpy(buff, "");
	FILE *file;

	file = fopen("../validate-FASTEngineMD/logFIX51-FASTEngineMD.txt", "a"); //writing in appending mode

	char* ApplVerID = "9";
	char* MsgType = "X";
	int SecurityIDSource = 8;
	char* SecurityExchange = "BVMF";

	//template
	PRINTS("1128=%s|", ApplVerID);
	PRINTS("35=%s|", MsgType);
	PRINTI("34=%d|", MsgSeqNum);
	PRINTI("52=%ld|", SendintTime);
	PRINTI("75=%d|", TradeDate);

	//SequenceMDEntries
	if(NoMDEntries > 0){ //sequence
		PRINTI("268=%d|", NoMDEntries);
		//printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
		PRINTI("279=%d|", MDUpdateAction);
		PRINTS("269=%s|", MDEntryType);
		PRINTI("22=%d|", SecurityIDSource);
		PRINTS("207=%s|", SecurityExchange);
		PRINTI("48=%ld|", SecurityID);
		PRINTI("83=%d|", RptSeq);
		PRINTS("276=%s|", QuoteCondition);
		PRINTD("270=%.4g|", MDEntryPx);
		PRINTD("37014=%.2f|", MDEntryInterestRate);
		PRINTI("346=%d|", NumberOfOrders);
		PRINTS("423=%s|", PriceType);
		PRINTI("273=%d|", MDEntryTime);
		PRINTI("271=%ld|", MDEntrySize);
		PRINTI("272=%d|", MDEntryDate);
		PRINTI("37016=%d|", MDInsertDate);
		PRINTI("37017=%d|", MDInsertTime);
		PRINTS("1500=%s|", MDStreamID);
		PRINTS("15=%s|", Currency);
		PRINTD("451=%.2f|", NetChgPrevDay);
		PRINTI("287=%d|", SellerDays);
		PRINTI("1020=%ld|", TradeVolume);
		PRINTS("274=%s|", TickDirection);
		PRINTS("277=%s|", TradeCondition);
		PRINTI("336=%d|", TradingSessionID);
		PRINTI("286=%d|", OpenCloseSettlFlag);
		PRINTS("37=%s|", OrderID);
		PRINTS("1003=%s|", TradeID);
		PRINTS("288=%s|", MDEntryBuyer);
		PRINTS("289=%s|", MDEntrySeller);
		PRINTI("290=%d|", MDEntryPositionNo);
		PRINTI("731=%d|", SettPriceType);
		PRINTI("9325=%d|", LastTradeDate);	
		PRINTI("37013=%d|", PriceAdjustmentMethod);
		PRINTS("6939=%s|", PriceBandType);
		PRINTI("1306=%d|", PriceLimitType);
		PRINTD("1148=%.2f|", LowLimitPrice);
		PRINTD("1149=%.2f|", HighLimitPrice);
		PRINTD("1150=%.2f|", TradingReferencePrice);
		PRINTI("37008=%d|", PriceBandMidpointPriceType);
		PRINTI("37003=%ld|", AvgDailyTradedQty);
		PRINTI("432=%ld|", ExpireDate);
		PRINTI("37019=%ld|", EarlyTermination);
		PRINTI("1140=%ld|", MaxTradeVol);

		//SequenceUnderlyings
		if(NoUnderlyings > 0){
			PRINTI("711=%d|", NoUnderlyings);
		}

		PRINTI("37100=%ld", IndexSeq);
		printf("\n");
		//printf("\n buffer: %s \n", buff);
	}

	//buff[strlen(buff)] = '\n';
	strcat(buff, "\n");

	fputs(buff, file);
   	fclose(file);
}