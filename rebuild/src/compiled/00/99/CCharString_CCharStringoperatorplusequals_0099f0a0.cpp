#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    unsigned char pad[9];
    unsigned int refCount;

    void AppendData(const char* src, int len);
};
#pragma pack(pop)

extern char g_emptyCharStringBuf;

struct CCharString
{
    CCharStringData* data;

    void MakeOwnCopyOfData();
    CCharString* operator+=(CCharString* rhs);
};

extern "C" CCharString* __fastcall CCharString_operator_plusequals(CCharString* self, void*, CCharString* rhs)
{
    if (self->data != 0)
    {
        self->MakeOwnCopyOfData();

        CCharStringData* rhsData = rhs->data;
        char* rhsText = &g_emptyCharStringBuf;
        if (rhsData != 0)
        {
            rhsText = rhsData->text;
        }

        CCharStringData* selfData = self->data;

        int len = 0;
        if (rhsText[0] != 0)
        {
            do
            {
                ++len;
            } while (rhsText[len] != 0);
        }

        selfData->AppendData(rhsText, len);

        return self;
    }

    if (rhs != self)
    {
        CCharStringData* rhsData = rhs->data;
        if (rhsData != 0)
        {
            self->data = rhsData;
            ++rhsData->refCount;
        }
    }

    return self;
}