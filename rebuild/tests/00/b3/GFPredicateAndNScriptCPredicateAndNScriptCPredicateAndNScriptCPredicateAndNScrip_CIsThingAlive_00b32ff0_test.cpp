#include <stdio.h>
#include <string.h>

struct CIsThingAlive;

struct CResult
{
	int a;
	int b;
	int c;
};

struct CNested
{
	CResult Build(CIsThingAlive* thing);
};

struct COuter
{
	char pad[0x298];
	CNested* m298;

	CResult CPredicate_And_Build(CIsThingAlive* thing);
};

// Provide the nested Build for the standalone link. It stamps a signature into
// the result buffer derived from the thing pointer so we can prove forwarding.
CResult CNested::Build(CIsThingAlive* thing)
{
	CResult r;
	r.a = 0x1111 + (int)(long)thing;
	r.b = 0x2222;
	r.c = 0x3333;
	return r;
}

int main()
{
	COuter outer;
	memset(&outer, 0, sizeof(outer));
	CNested nested;
	outer.m298 = &nested;

	CIsThingAlive* thing = (CIsThingAlive*)0x40;
	CResult out = outer.CPredicate_And_Build(thing);

	int okA = (out.a == 0x1111 + 0x40);
	int okB = (out.b == 0x2222);
	int okC = (out.c == 0x3333);

	if (okA && okB && okC)
	{
		printf("PARITY_OK_B32FF0\n");
		return 0;
	}
	printf("FAIL a=%d b=%d c=%d\n", out.a, out.b, out.c);
	return 1;
}