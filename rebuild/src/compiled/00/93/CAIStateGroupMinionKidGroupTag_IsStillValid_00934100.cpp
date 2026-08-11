// CAIStateGroup_MinionKidGroupTag::IsStillValid  @ 0x00934100
// disasm:
//   mov ecx,[ecx+0x18]; test ecx,ecx; je false; call 0x8138e0;
//   test eax,eax; je false; mov eax,1; ret; false: xor eax,eax; ret

struct Inner {
    int CheckValid();    // __fastcall @ 0x8138e0, ecx=this
};

struct CAIStateGroup_MinionKidGroupTag {
    char pad[0x18];
    Inner* inner;        // +0x18
    int IsStillValid();
};

int CAIStateGroup_MinionKidGroupTag::IsStillValid()
{
    Inner* p = this->inner;
    if (p != 0 && p->CheckValid())
        return 1;
    return 0;
}