struct Vec8 { float a; float b; };

struct SubObj {
    bool Query(Vec8* out, Vec8* in);
};

struct CTCLook {
    char pad[0x2c];
    SubObj sub;
    bool IsTurningBody(Vec8 arg);
};

bool CTCLook::IsTurningBody(Vec8 arg)
{
    Vec8 tmp;
    return this->sub.Query(&tmp, &arg);
}