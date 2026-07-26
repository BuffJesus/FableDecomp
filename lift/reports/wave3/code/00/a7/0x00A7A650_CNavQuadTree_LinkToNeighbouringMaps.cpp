#include <cstddef>
#include <cstdint>

namespace
{
#pragma pack(push, 1)
    struct CNavQuadTreeLinkToNeighbouringMapsOverlay
    {
        std::byte Pad00[0x06];
        bool Initialising;                  // +0x06
        float MapPosX;                      // +0x07
        float MapPosY;                      // +0x0B
        float MapWidth;                     // +0x10
        float MapHeight;                    // +0x14
        std::int32_t MapIndex;              // +0x18
        std::uint32_t NavigationLayersFirst; // +0x1C
        std::uint32_t NavigationLayersLast;  // +0x20
        std::byte Pad24[0x54];
        std::uint32_t Field78;              // +0x78
        std::uint32_t Field7C;              // +0x7C
        std::byte Pad80[0x04];
        std::uint8_t Byte84;                // +0x84
        std::uint8_t Byte85;                // +0x85
    };
#pragma pack(pop)

    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, Initialising) == 0x06);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, MapPosX) == 0x07);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, MapPosY) == 0x0B);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, MapWidth) == 0x10);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, MapHeight) == 0x14);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, MapIndex) == 0x18);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, NavigationLayersFirst) == 0x1C);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, NavigationLayersLast) == 0x20);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, Field78) == 0x78);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, Field7C) == 0x7C);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, Byte84) == 0x84);
    static_assert(offsetof(CNavQuadTreeLinkToNeighbouringMapsOverlay, Byte85) == 0x85);
    static_assert(sizeof(CNavQuadTreeLinkToNeighbouringMapsOverlay) == 0x86);

    extern float _DAT_0123078c;

    void __thiscall ConnectVerticalMapEdge(
        CNavQuadTree* self,
        float upper,
        float lower,
        const CNavigatorManager& navManager);

    void __fastcall Vector_MakeHeap_SoundPair(
        std::uint32_t field78,
        std::uint32_t field7C,
        unsigned int byte84);
}

void CNavQuadTree::LinkToNeighbouringMaps(const CNavigatorManager& navManager)
{
    auto* const self = reinterpret_cast<CNavQuadTreeLinkToNeighbouringMapsOverlay*>(this);

    if (self->NavigationLayersFirst != self->NavigationLayersLast)
    {
        const float mapWidth = self->MapWidth;
        self->Initialising = true;
        const float mapHeight = self->MapHeight;
        const float mapPosX = self->MapPosX;
        const float mapPosXCopy = self->MapPosX;
        const float mapPosY = self->MapPosY;

        ConnectVerticalMapEdge(
            this,
            self->MapPosY + _DAT_0123078c,
            self->MapPosY - _DAT_0123078c,
            navManager);
        ConnectVerticalMapEdge(
            this,
            (mapHeight + mapPosY) - _DAT_0123078c,
            mapHeight + mapPosY + _DAT_0123078c,
            navManager);
        ConnectVerticalMapEdge(
            this,
            mapPosX + _DAT_0123078c,
            mapPosX - _DAT_0123078c,
            navManager);
        ConnectVerticalMapEdge(
            this,
            (mapWidth + mapPosXCopy) - _DAT_0123078c,
            mapWidth + mapPosXCopy + _DAT_0123078c,
            navManager);

        if (self->Byte85 != 0)
        {
            Vector_MakeHeap_SoundPair(self->Field78, self->Field7C, self->Byte84);
            self->Byte85 = 0;
        }
    }

    self->Initialising = false;
}