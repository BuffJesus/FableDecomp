// Forward to a virtual (vtable slot 7) on the sub-object at this+4, passing `this`
// through as the first argument. __fastcall this=ecx, arg=stack (ret 4).
struct T;
struct Sub {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual bool Check(T* owner, void* arg);   // slot 7 -> call [edx+0x1c]
};
struct T {
    void* pad0;
    Sub* sub;
    bool IsNewDestinationGoingToResetNavigation(void* arg);
};
bool T::IsNewDestinationGoingToResetNavigation(void* arg) {
    return this->sub->Check(this, arg);
}
