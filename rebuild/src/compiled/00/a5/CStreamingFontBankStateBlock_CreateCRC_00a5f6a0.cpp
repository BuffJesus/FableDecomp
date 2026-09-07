struct CStreamingFontBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
int CStreamingFontBankStateBlock::CreateCRC() { return Helper(&crcData, 0x10); }