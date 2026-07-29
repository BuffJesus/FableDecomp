// CFrontendGameComponent::SetQuit @ 0x00494370.
//
// Retail marks only the quit-request byte at +8.

class CFrontendGameComponent_00494370
{
public:
    unsigned char padding_0000[8];
    unsigned char quit_requested;

    void SetQuit();
};

__declspec(naked)
void CFrontendGameComponent_00494370::SetQuit()
{
    __asm
    {
        mov byte ptr [ecx + 8], 1
        ret
    }
}
