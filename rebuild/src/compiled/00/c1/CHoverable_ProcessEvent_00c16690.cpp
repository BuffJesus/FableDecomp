struct CHoverable;
static CHoverable* g_hover = 0;
void __fastcall CHoverable_ProcessEvent(CHoverable* self, int /*edx*/)
{
    g_hover = self;
    if (!self)
        g_hover = (CHoverable*)0x143bb34;
}