// Forward declarations of the minimal surface this function touches.
class CAIBrain;
class CThingAICreature;
class CCharString;

enum EScriptAIPriority
{
    ESCRIPTAIPRIORITY_DUMMY = 0
};

// Base AI state group class. Only the parts this translation unit needs are
// modeled: a single virtual member function slot for Initialise (matches the
// retail vtable layout enough to make CAIStateGroupBase::Initialise a real,
// separately-compiled symbol we can forward to) plus the data layout up to
// offset 0x1c that the derived override writes.
class CAIStateGroupBase
{
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long param3,
                             EScriptAIPriority priority, const CCharString& name);

    // Padding so that field written by the derived override lands at +0x1c.
    unsigned char m_pad[0x1c - sizeof(void*)];
    long m_field_1c;
};

// Derived state group. Overrides Initialise to clear m_field_1c before
// forwarding to the base implementation (a genuine tail-call in the retail
// binary: mov dword ptr [ecx+0x1c], 0 ; jmp CAIStateGroupBase::Initialise).
class CAIStateGroup_MinionReactToTrespassing : public CAIStateGroupBase
{
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long param3,
                             EScriptAIPriority priority, const CCharString& name);
};

void CAIStateGroup_MinionReactToTrespassing::Initialise(CAIBrain& brain, CThingAICreature& creature,
                                                          long param3, EScriptAIPriority priority,
                                                          const CCharString& name)
{
    m_field_1c = 0;
    CAIStateGroupBase::Initialise(brain, creature, param3, priority, name);
}