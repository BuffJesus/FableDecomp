struct CHoverable;
extern CHoverable* g_curHover;
extern char g_defHover;
void __fastcall CHoverable_ProcessEvent(CHoverable* self)
{
	g_curHover = self;
	if (self == 0)
		g_curHover = (CHoverable*)&g_defHover;
}