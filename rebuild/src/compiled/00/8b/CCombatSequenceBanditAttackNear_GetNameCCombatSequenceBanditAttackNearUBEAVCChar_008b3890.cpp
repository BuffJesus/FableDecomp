// CCombatSequence_BanditAttackNear::GetName  @ 0x008b3890
// Returns a CCharString by value constructed from a fixed string literal.
// The 'this' pointer is unused; the function just builds a constant name
// string into the hidden struct-return buffer.

class CCharString
{
public:
	// CCharString::CCharString(const char* str, int len)  @ 0x0099ebf0
	// Real out-of-line ctor; declared but not defined here so the compiler
	// emits a __fastcall call to it (ecx = this buffer, str, len on stack).
	CCharString(const char* str, int len);
private:
	char* m_data;
};

class CCombatSequence_BanditAttackNear
{
public:
	CCharString GetName();
};

CCharString CCombatSequence_BanditAttackNear::GetName()
{
	return CCharString((const char*)0x012787dc, -1);
}