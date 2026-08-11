// StrPtr::operator!=  retail 0x00c36150
// this in ecx points to a char* (the held string pointer); param is a char*.
// Body: r = (*g_cmp)(*this, rhs); return (r == 0x102) ? 1 : 0;

struct StrPtr {
    char* p;
};

typedef int (__stdcall *CmpFn)(char*, char*);
CmpFn g_cmp;

int __fastcall StrPtr_operator_ne(StrPtr* self, int /*edx*/, char* rhs)
{
    return (*g_cmp)(self->p, rhs) == 0x102;
}