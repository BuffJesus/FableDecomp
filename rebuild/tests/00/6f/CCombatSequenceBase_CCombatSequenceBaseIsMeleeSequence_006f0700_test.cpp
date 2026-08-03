struct CCombatSequenceBase {
    struct VTable {
        void* slot0;
        void* slot4;
        void* slot8;
        int (__fastcall *GetSequenceType)(CCombatSequenceBase* self);
    };
    VTable* vfptr;
};

int __fastcall CCombatSequenceBase_IsMeleeSequence(CCombatSequenceBase* self, void* edx)
{
    return self->vfptr->GetSequenceType(self) == 1;
}

static int __fastcall FakeGetType1(CCombatSequenceBase* self) { return 1; }
static int __fastcall FakeGetType0(CCombatSequenceBase* self) { return 0; }
static int __fastcall FakeGetType2(CCombatSequenceBase* self) { return 2; }

#include <cstdio>

int main()
{
    CCombatSequenceBase::VTable vt1 = {0,0,0,FakeGetType1};
    CCombatSequenceBase obj1; obj1.vfptr = &vt1;
    int r1 = CCombatSequenceBase_IsMeleeSequence(&obj1, 0);

    CCombatSequenceBase::VTable vt0 = {0,0,0,FakeGetType0};
    CCombatSequenceBase obj0; obj0.vfptr = &vt0;
    int r0 = CCombatSequenceBase_IsMeleeSequence(&obj0, 0);

    CCombatSequenceBase::VTable vt2 = {0,0,0,FakeGetType2};
    CCombatSequenceBase obj2; obj2.vfptr = &vt2;
    int r2 = CCombatSequenceBase_IsMeleeSequence(&obj2, 0);

    if (r1 == 1 && r0 == 0 && r2 == 0) {
        printf("MELEE_SEQ_TEST_OK\n");
    } else {
        printf("MELEE_SEQ_TEST_FAIL %d %d %d\n", r1, r0, r2);
    }
    return 0;
}