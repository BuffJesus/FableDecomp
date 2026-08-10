#pragma optimize("s",on)

struct CInputProcessJumpingAndRolling {
    char pad[0x4c];
    char field_4c;
};

struct CProcessedInput;

extern "C" char __stdcall JR_Helper(int a, int b);

void __fastcall Update(CInputProcessJumpingAndRolling* self, int edx_ignored, CProcessedInput& in)
{
    char r = JR_Helper(0x3c, 2);
    if (!r)
        self->field_4c = r;
}