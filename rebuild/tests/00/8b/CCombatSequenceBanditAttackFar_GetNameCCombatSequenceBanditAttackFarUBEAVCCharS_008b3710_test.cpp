#include <stdio.h>
#include <string.h>

class CCharString
{
public:
	CCharString(const char* str, int len);
	char* m_data;
};

class CCombatSequence_BanditAttackFar
{
public:
	virtual CCharString GetName() const;
};

// Local stand-in for the engine ctor at 0x99ebf0.
static char g_captured[64];
static int  g_capturedLen = 0;
static const char* g_capturedPtr = 0;

CCharString::CCharString(const char* str, int len)
{
	g_capturedLen = len;
	g_capturedPtr = str;
	m_data = (char*)str;
}

int main()
{
	CCombatSequence_BanditAttackFar obj;

	CCharString r = obj.GetName();

	if (g_capturedLen == -1 &&
		g_capturedPtr == (const char*)0x0127876c &&
		r.m_data == (char*)0x0127876c)
	{
		printf("GETNAME_BANDITFAR_OK\n");
		return 0;
	}
	printf("GETNAME_BANDITFAR_FAIL len=%d ptr=%p\n", g_capturedLen, g_capturedPtr);
	return 1;
}