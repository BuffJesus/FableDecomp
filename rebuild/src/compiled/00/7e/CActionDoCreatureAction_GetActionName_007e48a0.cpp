// CActionDoCreatureAction::GetActionName  @ 0x007e48a0
// Returns a CCharString by value constructed from a fixed string literal.
// The 'this' pointer (CActionDoCreatureAction*) is unused; the function just
// builds a constant name string into the hidden struct-return buffer.

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

class CActionDoCreatureAction
{
public:
	CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
	return CCharString((const char*)0x0123e30c, -1);
}