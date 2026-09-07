struct CEngineSceneGrid_Overlay_0x00
{
    void** field_0x00;
};
static_assert(offsetof(CEngineSceneGrid_Overlay_0x00, field_0x00) == 0x00);

CEngineSceneGrid* __thiscall CEngineSceneGrid_InitFromCell(CEngineSceneGrid* self, unsigned long* cell)
{
    unsigned char listNodeStorage;
    std_list_node_Init(&listNodeStorage);

    const unsigned long rawCell = *cell;
    const unsigned long cellValue = *reinterpret_cast<unsigned long*>(rawCell);
    auto* const gridOverlay = reinterpret_cast<CEngineSceneGrid_Overlay_0x00*>(self);
    auto* const boundingBox = reinterpret_cast<C3DBoundingBox*>(*gridOverlay->field_0x00);

    self->GetCellBoundingBox(boundingBox, cellValue, rawCell);
    return self;
}