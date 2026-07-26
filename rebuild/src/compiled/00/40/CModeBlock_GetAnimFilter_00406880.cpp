struct CAnimFilterBase;
struct CModeBlock { char pad[0x80]; CAnimFilterBase* filter; };
CAnimFilterBase* __fastcall CModeBlock_GetAnimFilter(CModeBlock* self) {
    return (CAnimFilterBase*)((char*)self + 0x80);
}