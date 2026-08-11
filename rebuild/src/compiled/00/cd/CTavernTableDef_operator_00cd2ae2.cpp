// CTavernTableDef::operator= — retail 0x00cd2ae2
// push edi(rhs); mov esi,ecx(this); call base::operator=(this,rhs);
// copy byte [rhs+0xc] -> [this+0xc]; return this.

struct CBase {
    int a;   // +0
    int b;   // +4
    int c;   // +8  (base occupies 0..0xb = 12 bytes)
    // Declaration only: external at 0x4abe90 -> emitted as a real call, not inlined.
    CBase& operator=(const CBase& rhs);
};

struct CTavernTableDef : public CBase {
    char m_flag; // +0xc
    CTavernTableDef& operator=(const CTavernTableDef& rhs);
};

CTavernTableDef& CTavernTableDef::operator=(const CTavernTableDef& rhs) {
    CBase::operator=(rhs);
    m_flag = rhs.m_flag;
    return *this;
}