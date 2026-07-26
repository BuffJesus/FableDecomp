#pragma optimize("s",on)
struct CBase { void Copy(const CBase* other); unsigned char _b[0x30]; };
struct CGuildMasterDef : CBase { void Copy(const CGuildMasterDef* other); };
void CGuildMasterDef::Copy(const CGuildMasterDef* other) {
    this->CBase::Copy((const CBase*)other);
    *(unsigned long*)((char*)this + 0x28) = *(const unsigned long*)((const char*)other + 0x28);
    *(unsigned long*)((char*)this + 0x2c) = *(const unsigned long*)((const char*)other + 0x2c);
}