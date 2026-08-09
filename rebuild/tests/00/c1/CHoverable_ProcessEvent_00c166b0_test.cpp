#include <stdio.h>
struct CHoverable;
static CHoverable* g_curHover;
static char g_defHover;
static void model(CHoverable* self)
{
	g_curHover = self;
	if (self == 0)
		g_curHover = (CHoverable*)&g_defHover;
}
int main(){
	CHoverable* p = (CHoverable*)0x1234;
	model(p);
	if (g_curHover != p) { printf("FAIL\n"); return 1; }
	model(0);
	if (g_curHover != (CHoverable*)&g_defHover) { printf("FAIL\n"); return 1; }
	printf("OK_00c166b0\n");
	return 0;
}