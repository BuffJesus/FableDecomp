// CTCActionUseReadable::OnCreate  @ 0x007e8140
struct Sub;
struct Consumer;

struct Consumer {
    void Consume(int* arg);   // direct thiscall target 0x7e8200
};
struct Sub {
    char      pad[0x70];
    Consumer* f70;      // +0x70
};
struct Owner {
    void* f0;
    Sub*  f4;           // +4
    char  pad[0x14 - 8];
    int   f14;          // +0x14 (arg passed by address)
    void  OnCreate();
    void  Base_OnCreate();   // direct call 0x7eac10, this in ecx
};

void Owner::OnCreate()
{
    this->Base_OnCreate();
    this->f4->f70->Consume(&this->f14);
}