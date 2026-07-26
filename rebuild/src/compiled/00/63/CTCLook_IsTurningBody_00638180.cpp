struct Sub { char pad[0x20]; };

extern bool __fastcall Sub_Compute(Sub* self, int* edxreg, int* outA, int* inB);

bool __fastcall CTCLook_IsTurningBody(void* self, int edxpad, int arg)
{
    int local0[2];
    return Sub_Compute((Sub*)((char*)self + 0x20), &local0[0], &local0[0], &arg);
}