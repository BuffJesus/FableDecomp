#include <cstdio>

struct CTransferArc;

static int g_base_called = 0;
static int g_arc_called = 0;
static int* g_field_seen = 0;
static void* g_self_seen = 0;

struct CThingVillageDef {
    void BaseTransfer(CTransferArc* arc);
    char pad[0x70];
    int member70;
};
struct CTransferArc {
    void TransferField(int* field);
};

void CThingVillageDef::BaseTransfer(CTransferArc* arc)
{
    (void)arc;
    g_base_called = 1;
    g_self_seen = this;
}
void CTransferArc::TransferField(int* field)
{
    g_arc_called = 1;
    g_field_seen = field;
}

void __fastcall CThingVillageDef_Transfer(CThingVillageDef* self, void* edx, CTransferArc* arc)
{
    (void)edx;
    self->BaseTransfer(arc);
    arc->TransferField(&self->member70);
}

int main()
{
    CThingVillageDef obj;
    obj.member70 = 0x1234;
    CTransferArc arc;
    CThingVillageDef_Transfer(&obj, 0, &arc);
    if (g_base_called && g_arc_called && g_self_seen == (void*)&obj && g_field_seen == &obj.member70) {
        std::printf("CThingVillageDef_00454f5f_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}