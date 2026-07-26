
struct CTCBData {
    char pad[0xC4];
    int lifeStage;
};
struct CThingCreatureBase {
    char pad[0xE0];
    CTCBData* data;
};
bool __fastcall CThingCreatureBase_IsChild(const CThingCreatureBase* self) {
    return self->data->lifeStage == 1;
}