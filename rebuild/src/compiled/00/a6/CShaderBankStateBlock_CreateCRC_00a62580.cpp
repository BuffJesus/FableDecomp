struct CShaderBankStateBlock { int reserved; int crcData; int Helper(void*, int); int CreateCRC(); };
int CShaderBankStateBlock::CreateCRC() { return Helper(&crcData, 8); }