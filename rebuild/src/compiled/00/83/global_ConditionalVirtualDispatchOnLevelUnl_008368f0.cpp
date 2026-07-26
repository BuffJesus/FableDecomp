extern int g_enabledFlag;

struct Target {
    virtual void v0(int);
    virtual void v1(int);
    virtual void v2(int);
    virtual void v3(int);
    virtual void v4(int);
    virtual void v5(int);
    virtual void v6(int);
    virtual void v7(int);
    virtual void v8(int);
    virtual void v9(int);
    virtual void v10(int);
    virtual void v11(int);
    virtual void v12(int);
    virtual void OnLevelUnload(int);   /* slot 13 -> vtbl +0x34 */
};

struct Owner {
    char pad[0x91];
    unsigned char flags;
    char pad2[0x98 - 0x92];
    Target* target;
    void Owner_ConditionalVirtualDispatch_OnLevelUnload(int arg);
};

void Owner::Owner_ConditionalVirtualDispatch_OnLevelUnload(int arg)
{
    if (g_enabledFlag == 0)
        return;
    if (this->flags & 0x1)
        return;
    this->target->OnLevelUnload(arg);
}