#include <cstdio>
#include <cstring>

// ---- reproduce the source under test (self-contained) ----
class CCharString
{
public:
	CCharString(const char* str, int len);
	const char* data() const { return m_data; }
private:
	char* m_data;
};

// Local stand-in for CCharString::CCharString(const char*, int) @ 0x0099ebf0.
// Records the literal pointer and length it was invoked with.
static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* str, int len)
{
	g_lastStr = str;
	g_lastLen = len;
	m_data = (char*)str;
}

class CActionDoCreatureAction
{
public:
	CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
	return CCharString((const char*)0x0123e30c, -1);
}

int main()
{
	CActionDoCreatureAction obj;
	CCharString s = obj.GetActionName();

	bool ok = true;
	if (g_lastStr != (const char*)0x0123e30c) ok = false;
	if (g_lastLen != -1) ok = false;
	if (s.data() != (const char*)0x0123e30c) ok = false;

	if (ok)
		printf("GETACTIONNAME_OK\n");
	else
		printf("GETACTIONNAME_FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);

	return ok ? 0 : 1;
}