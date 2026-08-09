struct CHoverable;
extern CHoverable* g_hover;

void __fastcall CHoverable_ProcessEvent(CHoverable* self)
{
    g_hover = self;
    if (self == 0)
        g_hover = (CHoverable*)0x143bb34;
}