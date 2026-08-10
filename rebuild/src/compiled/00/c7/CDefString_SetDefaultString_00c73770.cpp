extern "C" unsigned char g_defStringInit;
struct CDefString;
extern CDefString* __fastcall SetDefaultString_helper(CDefString* self);

CDefString* __fastcall SetDefaultString(CDefString* self)
{
    if (!g_defStringInit)
        SetDefaultString_helper(self);
    return self;
}