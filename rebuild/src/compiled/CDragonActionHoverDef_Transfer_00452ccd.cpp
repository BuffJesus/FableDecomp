struct CSubField { int x; };

struct CSerializer {
    void Transfer(CSubField* p);
};

struct CDragonActionHoverDef {
    char pad[0x28];
    CSubField field28;
};

void __fastcall CDragonActionHoverDef_Transfer(CDragonActionHoverDef* self, void* /*edx*/, CSerializer* s)
{
    s->Transfer(&self->field28);
}