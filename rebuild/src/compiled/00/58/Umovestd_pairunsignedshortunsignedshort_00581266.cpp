#pragma optimize("s",on)
// _Umove forwarder @ 0x00581266  (member => this in ecx, 3 stack args, ret 0xc)
struct Pr { unsigned short a; unsigned short b; };

extern "C" void __stdcall helper(Pr* first, Pr* last, char* al, int flag);

struct Vec {
    char pad[16];
    Pr* pair_unsigned_short_unsigned_short_x_(Pr* first, Pr* last, Pr* dest);
};

Pr* Vec::pair_unsigned_short_unsigned_short_x_(Pr* first, Pr* last, Pr* dest)
{
    char temp;
    helper(first, last, &temp, 0);
    return (Pr*)this;
}