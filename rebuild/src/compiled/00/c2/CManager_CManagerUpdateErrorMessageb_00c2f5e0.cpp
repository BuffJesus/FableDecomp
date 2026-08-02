struct IErrTargetB {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void OnUpdateError(void* arg);
};

struct CManagerB_Self {
    void* field0;
    IErrTargetB* pTarget;
};

void __fastcall CManager_UpdateErrorMessage_b(CManagerB_Self* self) {
    self->pTarget->OnUpdateError(self);
}