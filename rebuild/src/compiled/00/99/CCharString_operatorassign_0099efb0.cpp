typedef unsigned char uint8;
typedef int int32;

#pragma pack(push, 1)
struct CCharStringData
{
    uint8 pad[0xd];
    int32 refcount;
};
#pragma pack(pop)

struct CCharString
{
    CCharStringData* data;

    void UnassignString();
};

CCharString* __fastcall CCharString_operator_assign(CCharString* self, void* /*edx*/, CCharString* rhs)
{
    if (rhs != self)
    {
        if (self->data != 0)
            self->UnassignString();

        CCharStringData* p = rhs->data;
        if (p != 0)
        {
            self->data = p;
            p->refcount++;
        }
    }
    return self;
}