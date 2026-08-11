// CBuyHouseDef::Copy  @ 00a5aca0
// __fastcall void Copy(CBuyHouseDef* this, CDefClassBase* src)
// calls base Copy (target 0xa5ae00) then copies own byte field at +0xd

struct CDefClassBase;

struct CBuyHouseBase {
    char pad[0xd];
    char field_d;   // +0xd
    void BaseCopy(CDefClassBase* src);   // base Copy @ 0xa5ae00, real __fastcall
};

struct CBuyHouseDef : CBuyHouseBase {
    void Copy(CDefClassBase* src);
};

void CBuyHouseDef::Copy(CDefClassBase* src)
{
    this->CBuyHouseBase::BaseCopy(src);
    this->field_d = ((CBuyHouseBase*)src)->field_d;
}