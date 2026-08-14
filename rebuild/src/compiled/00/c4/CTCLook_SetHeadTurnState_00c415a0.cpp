struct CTCLook {
    char pad[0x1dc];
};

void __fastcall SetHeadTurnState(CTCLook* self, int edx_unused, long state)
{
    long* pState = (long*)((char*)self + 0x19c);
    if (*pState != state) {
        *pState = state;
        *(unsigned long*)((char*)self + 0x1d8) |= 0x400000;
    }
}