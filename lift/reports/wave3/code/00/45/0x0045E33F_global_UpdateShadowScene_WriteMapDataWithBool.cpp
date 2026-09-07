struct ShadowMapDataNode
{
    std::uint8_t _pad0[0x10];
    std::uint32_t byteValue;
    bool boolValue;
};
static_assert(offsetof(ShadowMapDataNode, byteValue) == 0x10);
static_assert(offsetof(ShadowMapDataNode, boolValue) == 0x14);

struct ShadowMapDataListHead
{
    std::uint8_t _pad0[0x08];
    ShadowMapDataNode* first;
};
static_assert(offsetof(ShadowMapDataListHead, first) == 0x08);

void __fastcall UpdateShadowScene_WriteMapDataWithBool(CDataOutputStream* stream, unsigned long* mapData)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(mapData[1]));

    auto* const head = reinterpret_cast<ShadowMapDataListHead*>(mapData[0]);
    ShadowMapDataNode* node = head->first;
    if (node != reinterpret_cast<ShadowMapDataNode*>(head))
    {
        do
        {
            CDataOutputStream::WriteCBYTE(
                stream,
                static_cast<char>(node->byteValue));

            CDataOutputStream::WriteEBOOL(stream, node->boolValue);

            node = reinterpret_cast<ShadowMapDataNode* (__cdecl*)(ShadowMapDataNode*)>(0x00429AA0)(node);
        } while (node != reinterpret_cast<ShadowMapDataNode*>(head));
    }
}