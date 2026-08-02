#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    unsigned int unknown04;
    unsigned int unknown08;
    unsigned char flags0C;
    int owners;
};
#pragma pack(pop)

struct CCharString
{
    CCharStringData* storage_;

    void __fastcall UnassignString();
};

void __fastcall CCharString::UnassignString()
{
    CCharStringData* p = storage_;
    if (p != 0)
    {
        if (--p->owners <= 0)
        {
            CCharStringData* data = storage_;
            if (data != 0)
            {
                if (data->text != 0)
                {
                    delete[] data->text;
                    data->text = 0;
                }
                unsigned int flag = data->unknown08 & 0x80000000;
                data->unknown04 = 0;
                data->unknown08 = flag;
                delete data;
            }
        }
        storage_ = 0;
    }
}