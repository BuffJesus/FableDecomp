struct UpdateShadowScene_MapArea
{
    std::byte pad_00[0x08];
    UpdateShadowScene_MapArea* next;
    std::byte pad_0C[0x04];
    std::uint32_t value_10_bits;
    float value_14;
};

static_assert(offsetof(UpdateShadowScene_MapArea, next) == 0x08);
static_assert(offsetof(UpdateShadowScene_MapArea, value_10_bits) == 0x10);
static_assert(offsetof(UpdateShadowScene_MapArea, value_14) == 0x14);

struct UpdateShadowScene_MapData
{
    UpdateShadowScene_MapArea* head;
    std::uint32_t value_04_bits;
};

static_assert(offsetof(UpdateShadowScene_MapData, head) == 0x00);
static_assert(offsetof(UpdateShadowScene_MapData, value_04_bits) == 0x04);

extern "C" UpdateShadowScene_MapArea* __cdecl sub_004292C0(UpdateShadowScene_MapArea* area);

void __fastcall UpdateShadowScene_WriteMapData(
    CDataOutputStream* stream,
    UpdateShadowScene_MapData* mapData)
{
    stream->WriteCBYTE(std::bit_cast<float>(mapData->value_04_bits));

    UpdateShadowScene_MapArea* area = mapData->head->next;
    if (area != mapData->head)
    {
        do
        {
            stream->WriteCBYTE(std::bit_cast<float>(area->value_10_bits));
            stream->WriteCBYTE(area->value_14);
            area = sub_004292C0(area);
        } while (area != mapData->head);
    }
}