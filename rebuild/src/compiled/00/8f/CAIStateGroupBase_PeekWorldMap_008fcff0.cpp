struct CWorldMap;

struct CInner {
    virtual CWorldMap* Peek0();
    virtual CWorldMap* Peek1();
    virtual CWorldMap* Peek2();
    virtual CWorldMap* Peek3();
    virtual CWorldMap* Peek4();
    virtual CWorldMap* Peek5();
    virtual CWorldMap* Peek6();
    virtual CWorldMap* Peek7();
    virtual CWorldMap* Peek8();
    virtual CWorldMap* Peek9();
    virtual CWorldMap* Peek10();
    virtual CWorldMap* Peek11();
    virtual CWorldMap* Peek12();
    virtual CWorldMap* PeekWorldMap();
};

struct CMid { char pad[0x24]; CInner* inner; };
struct CHolder { char pad[0x1c]; CMid* mid; };
struct CAIStateGroupBase { char pad[0x4]; CHolder* holder; };

CWorldMap* __fastcall CAIStateGroupBase_PeekWorldMap(CAIStateGroupBase* self)
{
    return self->holder->mid->inner->PeekWorldMap();
}