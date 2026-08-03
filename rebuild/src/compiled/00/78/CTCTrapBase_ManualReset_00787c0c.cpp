#pragma optimize("s",on)
// CTCTrapBase::ManualReset (retail 0x00787c0c)
// cmp byte ptr [ecx+0x58],0 ; je +5 ; jmp Cleanup ; ret
class CAIStateGroup_MinionBehaviourCrowdBase;

extern "C" void __fastcall Cleanup(CAIStateGroup_MinionBehaviourCrowdBase *self, void *edx);

class CTCTrapBase
{
public:
    char pad[0x58];
    unsigned char m_active; // +0x58
};

void __fastcall CTCTrapBase_ManualReset(CTCTrapBase *self, void *edx)
{
    if (self->m_active != 0)
    {
        Cleanup((CAIStateGroup_MinionBehaviourCrowdBase*)self, edx);
    }
}