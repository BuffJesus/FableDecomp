#include <stdio.h>

struct CCombatActionBase {
    virtual bool IsActionReadyForInterruption() const;
    char pad[0xd8 - 4];
    bool m_flagA;
    char pad2[2];
    bool m_flagB;
};

bool CCombatActionBase::IsActionReadyForInterruption() const
{
    return m_flagA && m_flagB;
}

int main()
{
    CCombatActionBase o;
    bool ok = true;

    o.m_flagA = false; o.m_flagB = false;
    if (o.IsActionReadyForInterruption() != false) ok = false;

    o.m_flagA = true; o.m_flagB = false;
    if (o.IsActionReadyForInterruption() != false) ok = false;

    o.m_flagA = false; o.m_flagB = true;
    if (o.IsActionReadyForInterruption() != false) ok = false;

    o.m_flagA = true; o.m_flagB = true;
    if (o.IsActionReadyForInterruption() != true) ok = false;

    if (ok) printf("INTERRUPT_PARITY_OK\n");
    else printf("INTERRUPT_PARITY_FAIL\n");
    return ok ? 0 : 1;
}