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
	printf("\n\n");
	printf("------------------------------------------------F-I-X------------------------------------------------\n");

	const int SecurityIDSource = 8;
	const char SecurityExchange[4] = "BVMF";

	//template
	printf(" 34=%d |", MsgSeqNum);
	printf(" 52=%ld |", SendintTime);
	printf(" 75=%d |", TradeDate);

	//SequenceMDEntries
	printf(" 268=%d |", NoMDEntries);
	if(NoMDEntries > 0){ //sequence
		//printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);
		printf(" 279=%d |", MDUpdateAction);
		printf(" 269=%s |", MDEntryType);
		printf(" 22=%d |", SecurityIDSource);
		printf(" 207=%s |", SecurityExchange);
		printf(" 48=%ld |", SecurityID);
		printf(" 83=%d |", RptSeq);
		printf(" 276=%s |", QuoteCondition);
		printf(" 270=%.2f |", MDEntryPx);
		printf(" 37014=%.2f |", MDEntryInterestRate);
		printf(" 346=%d |", NumberOfOrders);
		printf(" 423=%s |", PriceType);
		printf(" 273=%d |", MDEntryTime);
		printf(" 271=%ld |", MDEntrySize);
		printf(" 272=%d |", MDEntryDate);
		printf(" 37016=%d |", MDInsertDate);
		printf(" 37017=%d |", MDInsertTime);
		printf(" 1500=%s |", MDStreamID);
		printf(" 15=%s |", Currency);
		printf(" 451=%.2f |", NetChgPrevDay);
		printf(" 287=%d |", SellerDays);
		printf(" 1020=%ld |", TradeVolume);
		printf(" 274=%s |", TickDirection);
		printf(" 277=%s |", TradeCondition);
		printf(" 336=%d |", TradingSessionID);
		printf(" 286=%d |", OpenCloseSettlFlag);
		printf(" 37=%s |", OrderID);
		printf(" 1003=%s |", TradeID);
		printf(" 288=%s |", MDEntryBuyer);
		printf(" 289=%s |", MDEntrySeller);
		printf(" 290=%d |", MDEntryPositionNo);
		printf(" 731=%d |", SettPriceType);
		printf(" 9325=%d |", LastTradeDate);	
		printf(" 37013=%d |", PriceAdjustmentMethod);
		printf(" 6939=%s |", PriceBandType);
		printf(" 1306=%d |", PriceLimitType);
		printf(" 1148=%.2f |", LowLimitPrice);
		printf(" 1149=%.2f |", HighLimitPrice);
		printf(" 1150=%.2f |", TradingReferencePrice);
		printf(" 37008=%d |", PriceBandMidpointPriceType);
		printf(" 37003=%ld |", AvgDailyTradedQty);
		printf(" 432=%ld |", ExpireDate);
		printf(" 37019=%ld |", EarlyTermination);
		printf(" 1140=%ld |", MaxTradeVol);

		//SequenceUnderlyings
		printf(" 711=%d |", NoUnderlyings);
		if(NoUnderlyings > 0){}

		printf(" 37100=%ld", IndexSeq);
	}

	printf("\n");
}