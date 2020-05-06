void t145toFIX(
	__uint32_t MsgSeqNum, __uint32_t TradeDate, __uint64_t* SendintTime, __uint32_t NoMDEntries, 
	__uint32_t MDEntriesSequence_PMap, __uint32_t MDEntriesSequence_PMap_length, __uint32_t MDUpdateAction, 
	__uint32_t RptSeq, __uint32_t NumberOfOrders, __uint32_t MDEntryTime, __uint32_t MDEntryDate, 
	__uint32_t MDInsertDate, __uint32_t MDInsertTime, __uint32_t SellerDays, __uint32_t TradingSessionID, 
	__uint32_t OpenCloseSettlFlag, __uint32_t MDEntryPositionNo, __uint32_t SettPriceType,
	__uint32_t LastTradeDate, __uint32_t PriceAdjustmentMethod, __uint32_t PriceLimitType, 
	__uint32_t PriceBandMidpointPriceType, float MDEntryPx, __uint32_t NoUnderlyings, __uint32_t UnderlyingPXType
){
	printf("\n\n");

	printf(" 34=%d |", MsgSeqNum);

	printf(" 52=");
	while(*SendintTime){
		printf( " %02x", (unsigned int) *SendintTime++);
	}
	printf(" |");

	printf(" 75=%d |", TradeDate);

	printf(" 268=%d |", NoMDEntries);

	if(NoMDEntries > 0){ //sequence
		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		MDEntriesSequence_PMap = byteDecoder32(aux, fieldLength(aux));
		MDEntriesSequence_PMap_length = fieldLength(aux);
		printf(" MDEntriesSequence_PMap: %d \n", MDEntriesSequence_PMap);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDUPDATEACTION, MDEntriesSequence_PMap_length);
		printf(" MDUpdateAction: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTYPE, MDEntriesSequence_PMap_length);
		printf(" MDEntryType: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SECURITYID, MDEntriesSequence_PMap_length);
		printf(" SecurityID: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, RPTSEQ, MDEntriesSequence_PMap_length);
		RptSeq = byteDecoder32(aux, fieldLength(aux));
		printf(" RptSeq: %d \n", RptSeq);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, QUOTECONDITION, MDEntriesSequence_PMap_length);
		printf(" QuoteCondition: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPX, MDEntriesSequence_PMap_length);
		MDEntryPx = (byteDecoder32(aux, fieldLength(aux)) * 0.01);
		printf(" MDEntryPx: %.3f \n", MDEntryPx);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYINTERESTRATE, MDEntriesSequence_PMap_length);
		printf(" MDEntryInterestRate: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NUMBEROFORDERS, MDEntriesSequence_PMap_length);
		NumberOfOrders = byteDecoder32(aux, fieldLength(aux));
		printf(" NumberOfOrders: %d \n", NumberOfOrders);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" PriceType: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYTIME, MDEntriesSequence_PMap_length);
		printf(" EntryTime: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" MDEntrySize: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYDATE, MDEntriesSequence_PMap_length);
		MDEntryDate = byteDecoder32(aux, fieldLength(aux));
		printf(" MDEntryDate: %d \n", MDEntryDate);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTDATE, MDEntriesSequence_PMap_length);
		MDInsertDate = byteDecoder32(aux, fieldLength(aux));
		printf(" MDInsertDate: %d \n", MDInsertDate);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDINSERTTIME, MDEntriesSequence_PMap_length);
		MDInsertTime = byteDecoder32(aux, fieldLength(aux));
		printf(" MDInsertTime: %d \n", MDInsertTime);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDSTREAMID, MDEntriesSequence_PMap_length);
		printf(" MDStreamID: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, CURRENCY, MDEntriesSequence_PMap_length);
		printf(" Currency: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, NETCHGPREVDAY, MDEntriesSequence_PMap_length);
		printf(" NetChgPrevDay: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, SELLERDAYS, MDEntriesSequence_PMap_length);
		printf(" SellerDays: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" TradeVolume: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TICKDIRECTION, MDEntriesSequence_PMap_length);
		printf(" TickDirection: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" TradeCondition: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		TradingSessionID = byteDecoder32(aux, fieldLength(aux));
		printf(" TradingSessionID: %d \n", TradingSessionID);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		OpenCloseSettlFlag = byteDecoder32(aux, fieldLength(aux));
		printf(" OpenCloseSettlFlag: %d \n", OpenCloseSettlFlag);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, ORDERID, MDEntriesSequence_PMap_length);
		printf(" OrderID: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADEID, MDEntriesSequence_PMap_length);
		printf(" TradeID: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYBUYER, MDEntriesSequence_PMap_length);
		printf(" MDEntryBuyer: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYSELLER, MDEntriesSequence_PMap_length);
		printf(" MDEntrySeller: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, MDENTRYPOSITIONNO, MDEntriesSequence_PMap_length);
		MDEntryPositionNo = byteDecoder32(aux, fieldLength(aux));
		printf(" MDEntryPositionNo: %d \n", MDEntryPositionNo);		

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		SettPriceType = byteDecoder32(aux, fieldLength(aux));
		printf(" SettPriceType: %d \n", SettPriceType);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		LastTradeDate = byteDecoder32(aux, fieldLength(aux));
		printf(" LastTradeDate: %d \n", LastTradeDate);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		PriceAdjustmentMethod = byteDecoder32(aux, fieldLength(aux));
		printf(" PriceAdjustmentMethod: %d \n", PriceAdjustmentMethod);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICEBANDTYPE, MDEntriesSequence_PMap_length);
		printf(" PriceBandType: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, PRICELIMITTYPE, MDEntriesSequence_PMap_length);
		PriceAdjustmentMethod = byteDecoder32(aux, fieldLength(aux));
		printf(" PriceLimitType: %d \n", PriceLimitType);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, LOWLIMITPRICE, MDEntriesSequence_PMap_length);
		printf(" LowLimitPrice: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, HIGHLIMITPRICE, MDEntriesSequence_PMap_length);
		printf(" HighLimitPrice: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, MDEntriesSequence_PMap, TRADINGREFERENCEPRICE, MDEntriesSequence_PMap_length);
		printf(" TradingReferencePrice: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		PriceAdjustmentMethod = byteDecoder32(aux, fieldLength(aux));
		printf(" PriceBandMidpointPriceType: %d \n", PriceBandMidpointPriceType);

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" AvgDailyTradedQty: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" ExpireDate: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" EarlyTermination: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" MaxTradeVol: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		NoUnderlyings = byteDecoder32(aux, fieldLength(aux));
		printf(" NoUnderlyings: %d \n", NoUnderlyings);

		if(NoUnderlyings > 0){}

		aux = getField(field, &ptr_FASTMessage, FASTMessage_length, NONEBITMAP, NONEBITMAP, NONEBITMAP);
		printf(" IndexSeq: ");
		while(*aux){
			printf( " %02x", (unsigned int) *aux++);
		}
		printf("\n");
}