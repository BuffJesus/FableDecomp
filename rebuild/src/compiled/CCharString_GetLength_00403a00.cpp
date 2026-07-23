struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };
long __fastcall CCharString_GetLength(const CCharString* self) {
    StringRep* r = self->rep;
    if (r == 0) return 0;
    return r->len;
}