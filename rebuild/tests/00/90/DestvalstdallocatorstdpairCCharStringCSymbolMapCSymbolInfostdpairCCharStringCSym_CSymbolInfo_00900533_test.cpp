#include <cstdio>

struct CCharString {
	char* m_ptr;
	int   m_len;
	~CCharString();
};
struct CSymbolInfoTail {
	int a, b, c, d, e;
	~CSymbolInfoTail();
};
struct SymPair {
	CSymbolInfoTail tail;
	CCharString     s14;
	CCharString     s1c;
};
void __fastcall _Dest_val_CSymbolInfo___(SymPair* self, void* al);

static int g_str_dtor = 0;
static int g_tail_dtor = 0;
CCharString::~CCharString() { g_str_dtor++; }
CSymbolInfoTail::~CSymbolInfoTail() { g_tail_dtor++; }

int main() {
	SymPair p;
	p.s14.m_ptr = 0; p.s14.m_len = 0;
	p.s1c.m_ptr = 0; p.s1c.m_len = 0;
	p.tail.a = p.tail.b = p.tail.c = p.tail.d = p.tail.e = 0;

	_Dest_val_CSymbolInfo___(&p, 0);

	if (g_str_dtor == 2 && g_tail_dtor == 1) {
		printf("DEST_VAL_OK\n");
		return 0;
	}
	printf("DEST_VAL_FAIL s=%d t=%d\n", g_str_dtor, g_tail_dtor);
	return 1;
}