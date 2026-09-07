struct CFontBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
int CFontBankStateBlock::CreateCRC() { return Helper(&crcData, 0x0c); }