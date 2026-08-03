struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };

extern char g_EmptyCharString_0129A8E0[1];

const char* __fastcall CCharString_operator_char_const(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129A8E0;
    return r->data;
}