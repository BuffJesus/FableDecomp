// CThingCreatureBase::EditWorldUpdate  retail 0x00838ba0
struct CThingCreatureBase {
    bool Helper1();          // 0x6a6880 (thiscall, no args)
    void Helper2();          // 0x838aa0 (thiscall, void)
    bool EditWorldUpdate();
};

bool CThingCreatureBase::EditWorldUpdate()
{
    if (Helper1())
        return true;
    Helper2();
    return false;
}