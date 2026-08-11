// CCombatSequence_TrollRockThrowRock::GetName  @ 0x008b4440
// Virtual const method returning a CCharString by value, built from a fixed
// string literal. 'this' is unused; the function constructs a constant name
// string into the hidden struct-return buffer and returns it.
// Retail: push esi; mov esi,[esp+8]; push -1; push 0x1278b60; mov ecx,esi;
//         call CCharString::ctor(const char*,int); mov eax,esi; pop esi; ret 4

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

class CCombatSequence_TrollRockThrowRock
{
public:
	virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockThrowRock::GetName() const
{
	return CCharString((const char*)0x01278b60, -1);
}