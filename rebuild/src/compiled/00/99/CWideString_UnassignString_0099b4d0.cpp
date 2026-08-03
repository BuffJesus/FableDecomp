struct CWideStringData
{
    void* buffer;
    int unused1;
    int unused2;
    int refCount;
};

struct CWideString
{
    CWideStringData* data;
};

void __cdecl FreeStringBuffer(void* buffer);
void __cdecl FreeStringData(CWideStringData* data);

void __fastcall CWideString_UnassignString(CWideString* self)
{
    CWideStringData* data = self->data;
    if (data)
    {
        if (--data->refCount <= 0)
        {
            CWideStringData* p = self->data;
            if (p)
            {
                void* buf = p->buffer;
                if (buf)
                    FreeStringBuffer(buf);
                FreeStringData(p);
            }
        }
        self->data = 0;
    }
}