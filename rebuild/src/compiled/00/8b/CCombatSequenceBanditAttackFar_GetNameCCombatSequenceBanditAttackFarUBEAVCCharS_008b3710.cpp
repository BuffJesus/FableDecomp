// CCombatSequence_BanditAttackFar::GetName  @ 0x008b3710
// Virtual const method returning a CCharString by value, built from a fixed
// string literal. 'this' is unused; the function constructs a constant name
// string into the hidden struct-return buffer and returns it.

class CCharString
{
public:
	// CCharString::CCharString(const char* str, int len)  @ 0x0099ebf0
	// Real out-of-line ctor; declared but not defined here so the compiler
	// emits a call to it (ecx = this buffer, str + len on stack).
	CCharString(const char* str, int len);
private:
	char* m_data;
};

class CCombatSequence_BanditAttackFar
{
public:
	virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAttackFar::GetName() const
{
	return CCharString((const char*)0x0127876c, -1);
}