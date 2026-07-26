#include <cstdio>

struct CSub {
	unsigned char pad[0x10];
	unsigned char kind;
};

struct CTCPerceiveThing {
	unsigned char pad0[0xc];
	unsigned char sub[0x22];
	unsigned char flag;
};

static CSub g_sub;
static int g_return_null = 0;

extern "C" CSub* __fastcall Sub_Get(void* self)
{
	(void)self;
	if (g_return_null) return 0;
	return &g_sub;
}

int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
	void* sub = (char*)self + 0xc;
	if (Sub_Get(sub) != 0) {
		if (Sub_Get(sub)->kind == 2) {
			if (self->flag != 0) {
				return 1;
			}
		}
	}
	return 0;
}

int main()
{
	CTCPerceiveThing t;
	// case: all conditions true
	g_return_null = 0;
	g_sub.kind = 2;
	t.flag = 1;
	if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&t) != 1) { std::printf("FAIL case1\n"); return 1; }

	// case: null accessor
	g_return_null = 1;
	if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&t) != 0) { std::printf("FAIL case2\n"); return 1; }

	// case: kind != 2
	g_return_null = 0;
	g_sub.kind = 3;
	t.flag = 1;
	if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&t) != 0) { std::printf("FAIL case3\n"); return 1; }

	// case: flag == 0
	g_sub.kind = 2;
	t.flag = 0;
	if (CTCPerceiveThing_IsPlayerExtendedLineOfSight(&t) != 0) { std::printf("FAIL case4\n"); return 1; }

	std::printf("CTCPerceiveThing_00744870_TEST PASS\n");
	return 0;
}