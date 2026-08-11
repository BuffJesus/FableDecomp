#include <cstdio>

static int g_ctor_calls = 0;

struct CWideString {
	// Local stand-in for the external CWideString copy-ctor (0x0099ec30).
	// Out-of-line so it emits a real symbol s.obj can link against.
	void CopyCtor(CWideString* src);
	unsigned short* m_buf;
};

void CWideString::CopyCtor(CWideString* src) { this->m_buf = src->m_buf; ++g_ctor_calls; }

struct PairKF {
	CWideString key;
	float val;
};

void __fastcall Cons_val(PairKF* dest, PairKF* src);

int main()
{
	unsigned short buf[] = { 'h', 'e', 'r', 'o', 0 };
	PairKF src;
	src.key.m_buf = buf;
	src.val = 3.5f;

	PairKF dest;
	dest.key.m_buf = 0;
	dest.val = 0.0f;

	Cons_val(&dest, &src);

	bool ok = (dest.key.m_buf == buf) && (dest.val == 3.5f) && (g_ctor_calls == 1);

	// null dest must be a no-op (no ctor call, no crash)
	g_ctor_calls = 0;
	Cons_val(0, &src);
	ok = ok && (g_ctor_calls == 0);

	printf(ok ? "CONS_VAL_OK\n" : "CONS_VAL_FAIL\n");
	return ok ? 0 : 1;
}