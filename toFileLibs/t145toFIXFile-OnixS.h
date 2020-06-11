#include <string.h>

void printi(char* text, int var, char* buff){
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

	FILE *file;
	char buff[7000];
	file = fopen("../validate-FASTEngineMD/logFIX51-FASTEngineMD.txt", "w");
	strcpy(buff, "");

	#define PRINTI(file, var) if(var != -80){printf(file, var); printi(file, var, buff);} //print for int 
	#define PRINTD(file, var) if(var > 0.00) printf(file, var)	//print for decimals
	#define PRINTS(file, var) if(strcmp(var, "EMPTY") != 0) printf(file, var)	//(different of NULL) print for strings

	//printf("\n   ---------------------------------------------F-I-X---------------------------------------------\n");
	//printf("\n");

	//printi("34=%d|", MsgSeqNum, buff);

	const char* ApplVerID = "7"; //"9";
	const char* MsgType = "X";
	const int SecurityIDSource = 8;
	const char SecurityExchange[4] = "BVMF";

	//template
	printf("%d: ", MsgSeqNum);
	printf("8=FIXT1.1|9=0|");
	PRINTS("35=%s|", MsgType);
	PRINTS("1128=%s|", ApplVerID);
	printf("34=%d|", MsgSeqNum);
	printf("52=%ld|", SendintTime);
	printf("268=%d|", NoMDEntries); //turn back for printf //maybe to use another var like -1 to indicates null
	if(NoMDEntries > 0){ //sequence
		//printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
		printf("279=%d|", MDUpdateAction);
		PRINTS("269=%s|", MDEntryType);
		printf("22=%d|", SecurityIDSource);
		printf("48=%ld|", SecurityID);
		printf("83=%d|", RptSeq);
		PRINTS("276=%s|", QuoteCondition);
		PRINTD("270=%.2f|", MDEntryPx);
		PRINTD("37014=%.2f|", MDEntryInterestRate);
		PRINTI("346=%d|", NumberOfOrders);
		PRINTS("423=%s|", PriceType);
		printf("273=%d|", MDEntryTime);
		printf("271=%ld|", MDEntrySize);
		PRINTS("207=%s|", SecurityExchange);
		printf("272=%d|", MDEntryDate);
		printf("37016=%d|", MDInsertDate);
		printf("37017=%d|", MDInsertTime);
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
		printf("290=%d|", MDEntryPositionNo);
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
		PRINTI("711=%d|", NoUnderlyings);
		if(NoUnderlyings > 0){}

		PRINTI("37100=%ld|", IndexSeq);
	}
	printf("75=%d|", TradeDate);
	printf("10=000|");

	printf("\n\n buffer: %s \n\n", buff);

	printf("\n");
	//printf("\n-----------------------------------------------------------------------------------------------------\n\n");
	//printf("\n-----------------------------------------------------------------------------------------------------\n");
}