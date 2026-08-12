#include <cstdio>

struct CCharString;

struct Inner {
	CCharString* Ucopy(void* last, void* first, CCharString* dest);
};

struct Outer {
	char pad[0x188];
	Inner* m188;
	CCharString* Ucopy_forward(void* first, void* last, CCharString* dest);
};

// record what the inner call received
static void* g_last;
static void* g_first;
static CCharString* g_dest;

CCharString* Inner::Ucopy(void* last, void* first, CCharString* dest)
{
	g_last = last;
	g_first = first;
	g_dest = dest;
	return dest;
}

CCharString* Outer::Ucopy_forward(void* first, void* last, CCharString* dest)
{
	return this->m188->Ucopy(last, first, dest);
}

int main()
{
	Inner inner;
	Outer outer;
	outer.m188 = &inner;

	void* first = (void*)0x1111;
	void* last  = (void*)0x2222;
	CCharString* dest = (CCharString*)0x3333;

	CCharString* r = outer.Ucopy_forward(first, last, dest);

	bool ok = (r == dest)
		&& (g_last == last)
		&& (g_first == first)
		&& (g_dest == dest);

	if (ok) {
		printf("UCOPY_FWD_OK\n");
		return 0;
	}
	printf("UCOPY_FWD_FAIL\n");
	return 1;
}