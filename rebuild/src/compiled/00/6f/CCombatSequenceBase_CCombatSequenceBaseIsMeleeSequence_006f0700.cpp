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