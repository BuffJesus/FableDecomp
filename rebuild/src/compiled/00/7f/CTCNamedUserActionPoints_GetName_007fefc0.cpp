struct CCharString {
    char* p;
    void Assign(const CCharString& src);
};

struct CTCNamedUserActionPoints {
    char pad[0x34];
    CCharString* arr;
    void GetName(long index, CCharString* out);
};

void CTCNamedUserActionPoints::GetName(long index, CCharString* out)
{
    out->Assign(this->arr[index]);
}