#include <cstddef>
#include <cstdlib>
#include <utility>

class CThing;
class CThingAICreature;
class CThingCreatureBase;
class CThingPhysical;
class CThingSearchTools;
class CTCPerceiveThing;
class CTCPhysicsBase;
class CVertexBufferWin32;
class CCharString;
class CNavigationAreaInit;
class CWorld;
enum EHeroMorphType;
namespace CParticleMorphs
{
class CEntry;
}

struct C2DBoxI;
struct C3DVector
{
    float x;
    float y;
    float z;
};

struct CWorldMap_GetMapNavigationAreaInit_BoxIOverlay
{
    long minX;
    long minY;
    long maxX;
    long maxY;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_BoxIOverlay) == 0x10);

struct CWorldMap_GetMapNavigationAreaInit_BoxFOverlay
{
    float minX;
    float minY;
    float maxX;
    float maxY;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_BoxFOverlay) == 0x10);

struct CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay
{
    float x;
    float y;
    long layer;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay) == 0x0C);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay, layer) == 0x08);

struct CWorldMap_GetMapNavigationAreaInit_MapInfoOverlay
{
    CWorldMap_GetMapNavigationAreaInit_BoxIOverlay bounds;
    std::byte pad_10[0x38];
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_MapInfoOverlay) == 0x48);

struct CWorldMap_GetMapNavigationAreaInit_WorldMapOverlay
{
    std::byte pad_00[0x08];
    CWorld* world;
    std::byte pad_0C[0x14];
    void* mapTable;
};
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_WorldMapOverlay, world) == 0x08);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_WorldMapOverlay, mapTable) == 0x20);

struct CWorldMap_GetMapNavigationAreaInit_WorldOverlay
{
    std::byte pad_00[0x20];
    CThingSearchTools* thingSearchTools;
};
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_WorldOverlay, thingSearchTools) == 0x20);

struct CWorldMap_GetMapNavigationAreaInit_ListNode
{
    CWorldMap_GetMapNavigationAreaInit_ListNode* next;
    CWorldMap_GetMapNavigationAreaInit_ListNode* prev;
    void* value;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_ListNode) == 0x0C);

struct CWorldMap_GetMapNavigationAreaInit_CreatureOverlay
{
    std::byte pad_00[0x60];
    CTCPhysicsBase* physics;
};
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_CreatureOverlay, physics) == 0x60);

struct CWorldMap_GetMapNavigationAreaInit_PerceiveThingOverlay
{
    std::byte pad_00[0x04];
    CVertexBufferWin32* thingAt4;
};
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_PerceiveThingOverlay, thingAt4) == 0x04);

struct CWorldMap_GetMapNavigationAreaInit_VectorOverlay
{
    void** begin;
    void** end;
    void** capacityEnd;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_VectorOverlay) == 0x0C);

struct CWorldMap_GetMapNavigationAreaInit_FilterLeafThings
{
    const CCharString* questName;
    const C2DBoxI** area;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_FilterLeafThings) == 0x08);

struct CWorldMap_GetMapNavigationAreaInit_FilterAllThings
{
    const CCharString* questName;
    const long* maximumCount;
    const float* range;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_FilterAllThings) == 0x0C);

struct CWorldMap_GetMapNavigationAreaInit_RangeBlock
{
    float minX;
    float minY;
    float maxX;
    float maxY;
};
static_assert(sizeof(CWorldMap_GetMapNavigationAreaInit_RangeBlock) == 0x10);

struct CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay
{
    std::byte pad_00[0x30];
    CWorldMap_GetMapNavigationAreaInit_BoxIOverlay area;
    std::byte pad_40[0x08];
    std::byte* blockerWrite;
    std::byte* blockerEnd;
    std::byte* pointWrite;
    std::byte* pointEnd;
};
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay, area) == 0x30);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay, blockerWrite) == 0x48);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay, blockerEnd) == 0x4C);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay, pointWrite) == 0x54);
static_assert(offsetof(CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay, pointEnd) == 0x58);

using CWorldMap_GetMapNavigationAreaInit_Pair =
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

namespace std
{
void __thiscall _Cons_val(
    void* this_,
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_edx,
    CWorldMap_GetMapNavigationAreaInit_Pair* unaff_edi);
}

void __thiscall CWorldMap::InitializeMapNavigationHeightmap(
    long mapNumber,
    const C2DBoxI* area,
    const CCharString& questName,
    CNavigationAreaInit& navigationAreaInit) const;
long __thiscall CWorldMap::GetNavigationLayerAt(
    const C3DVector* position,
    CThing* thing) const;

void __thiscall CTCPhysicsBase::GetBoundingInfo(C3DVector* centre, float* radius) const;
float* __thiscall CVertexBufferWin32::DoSizeof() const;

void __thiscall VectorInsertRangeGeneric(
    void* dst,
    const void* src,
    void* scratch,
    int count,
    int elementCount);
void __thiscall std_vector_construct_with_fill(
    void* dst,
    const void* src,
    void* scratch,
    int count,
    int elementCount);

void __thiscall CThingSearchTools::GetAllLeafThings<
    CThingAICreature,
    std::list<const CThingPhysical*, std::allocator<const CThingPhysical*>>,
    CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>>(
        void* filter,
        void* outList) const;

void __thiscall CThingSearchTools::GetAllTCsInRoughArea<
    CTCPerceiveThing,
    CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>,
    std::list<CTCPerceiveThing*, std::allocator<CTCPerceiveThing*>>>(
        const C2DBoxI* area,
        void* filter,
        void* outVector) const;

void __thiscall CThingSearchTools::GetPBestThingInRoughArea<
    CFilter_Bind3<
        CThingFilter_IsAvailableForCreatureInteraction,
        CThingFilter_IsAICreatureAwareOf,
        std::unary_negate<CThingFilter_IsIncapacitated>>,
    CThingCompare_Nearest,
    CThingAICreature>(
        const C2DBoxI* area,
        void* filter,
        void* outVector,
        CThingAICreature** context) const;

void __thiscall CThingSearchTools::GetAllThings<
    CThingCreatureBase,
    std::vector<CThingCreatureBase*, std::allocator<CThingCreatureBase*>>,
    CFilter_Bind3<CThingFilter_IsHuman, CThingFilter_IsNot, CThingFilter_In2DRange>>(
        void* filter,
        void* outVector) const;

void CWorldMap::GetMapNavigationAreaInit(
    long mapNumber,
    const C2DBoxI* area,
    const CCharString& questName,
    CNavigationAreaInit& navigationAreaInit) const
{
    const auto* const self =
        reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_WorldMapOverlay*>(this);
    const auto* const world =
        reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_WorldOverlay*>(self->world);
    auto* const out =
        reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationAreaInitOverlay*>(
            &navigationAreaInit);

    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_00;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_01;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_02;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_03;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_04;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_05;
    CWorldMap_GetMapNavigationAreaInit_Pair* extraout_EDX_06;
    CWorldMap_GetMapNavigationAreaInit_Pair* unaff_EDI;

    char local_79;
    CWorldMap_GetMapNavigationAreaInit_BoxIOverlay local_20{};

    if (area == nullptr)
    {
        const auto* const mapInfo =
            reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_MapInfoOverlay*>(
                static_cast<const std::byte*>(self->mapTable) + mapNumber * 0x48);
        local_20 = mapInfo->bounds;
        area = reinterpret_cast<const C2DBoxI*>(&local_20);
    }

    InitializeMapNavigationHeightmap(mapNumber, area, questName, navigationAreaInit);

    auto* const local_70 =
        static_cast<CWorldMap_GetMapNavigationAreaInit_ListNode*>(std::malloc(0x0C));
    local_70->next = local_70;
    local_70->prev = local_70;

    const CThingSearchTools* const local_78 = world->thingSearchTools;
    CWorldMap_GetMapNavigationAreaInit_ListNode* local_58 = local_70;
    const C2DBoxI* local_6c = area;

    {
        CCharString local_74(questName);
        const CCharString* local_60 = &local_74;
        const C2DBoxI** local_5c = const_cast<const C2DBoxI**>(&local_6c);
        CWorldMap_GetMapNavigationAreaInit_FilterLeafThings filter{local_60, local_5c};
        local_78->GetAllLeafThings<
            CThingAICreature,
            std::list<const CThingPhysical*, std::allocator<const CThingPhysical*>>,
            CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>>(
                &filter,
                local_70);
        std::_Cons_val(&local_74, extraout_EDX, unaff_EDI);
    }

    local_6c = area;
    {
        CCharString local_74(questName);
        const CCharString* local_60 = &local_74;
        const C2DBoxI** local_5c = const_cast<const C2DBoxI**>(&local_6c);
        CWorldMap_GetMapNavigationAreaInit_FilterLeafThings filter{local_60, local_5c};
        local_78->GetAllLeafThings<
            CThingAICreature,
            std::list<const CThingPhysical*, std::allocator<const CThingPhysical*>>,
            CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>>(
                &filter,
                local_70);
        std::_Cons_val(&local_74, extraout_EDX_00, unaff_EDI);
    }

    for (auto* node = local_70->next; node != local_70; node = node->next)
    {
        auto* const creature =
            reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_CreatureOverlay*>(node->value);

        using RawIndirectPredicate = char(__thiscall*)(void*);
        auto* const vftable = *reinterpret_cast<void***>(creature);
        const char predicateResult =
            reinterpret_cast<RawIndirectPredicate>(vftable[0x39])(creature);

        if (predicateResult != '\0')
        {
            CTCPhysicsBase* const physics = creature->physics;
            if (physics != nullptr)
            {
                C3DVector local_2c{};
                float local_74 = 0.0f;
                physics->GetBoundingInfo(&local_2c, &local_74);

                const auto* const areaBox =
                    reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_BoxIOverlay*>(area);

                CWorldMap_GetMapNavigationAreaInit_BoxFOverlay local_3c{};
                local_3c.minX = local_2c.x - local_74;
                local_3c.minY = local_2c.y - local_74;
                if (local_3c.minX < static_cast<float>(areaBox->minX))
                {
                    local_3c.minX = static_cast<float>(areaBox->minX);
                }
                if (local_3c.minY < static_cast<float>(areaBox->minY))
                {
                    local_3c.minY = static_cast<float>(areaBox->minY);
                }

                local_3c.maxX = local_2c.x + local_74;
                if (static_cast<float>(areaBox->maxX) < local_3c.maxX)
                {
                    local_3c.maxX = static_cast<float>(areaBox->maxX);
                }

                const float local_maxYCandidate = local_2c.y + local_74;
                float local_clippedMaxY = local_maxYCandidate;
                if (static_cast<float>(areaBox->maxY) < local_maxYCandidate)
                {
                    local_clippedMaxY = static_cast<float>(areaBox->maxY);
                }
                local_3c.maxY = local_clippedMaxY;

                std::byte* const blockerWrite = out->blockerWrite;
                if (blockerWrite == out->blockerEnd)
                {
                    VectorInsertRangeGeneric(blockerWrite, &local_3c, &local_79, 1, 1);
                }
                else if (blockerWrite == nullptr)
                {
                    out->blockerWrite += 0x10;
                }
                else
                {
                    auto* const dst =
                        reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_BoxFOverlay*>(
                            blockerWrite);
                    dst->maxY = local_3c.maxY;
                    dst->minX = local_3c.minX;
                    dst->minY = local_3c.minY;
                    dst->maxX = local_3c.maxX;
                    out->blockerWrite += 0x10;
                }
            }
        }
    }

    out->area = *reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_BoxIOverlay*>(area);

    auto* const local_74_list =
        static_cast<CWorldMap_GetMapNavigationAreaInit_ListNode*>(std::malloc(0x0C));
    local_74_list->next = local_74_list;
    local_74_list->prev = local_74_list;

    {
        const C2DBoxI* local_60 = area;
        CCharString local_70_q(questName);
        const CCharString* local_54 = &local_70_q;
        const C2DBoxI* const* local_50 = &local_60;
        CWorldMap_GetMapNavigationAreaInit_FilterLeafThings filter{local_54, local_50};
        local_78->GetAllLeafThings<
            CThingAICreature,
            std::list<const CThingPhysical*, std::allocator<const CThingPhysical*>>,
            CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>>(
                &filter,
                local_74_list);
        std::_Cons_val(&local_70_q, extraout_EDX_01, unaff_EDI);
    }

    for (auto* node = local_74_list->next; node != local_74_list; node = node->next)
    {
        auto* const thing = reinterpret_cast<CVertexBufferWin32*>(node->value);
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_2c{};
        local_2c.x = xy[0];
        local_2c.y = xy[1];
        local_2c.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_2c, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_2c.x;
                dst->y = local_2c.y;
                dst->layer = local_2c.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    {
        auto* node = local_74_list->next;
        while (node != local_74_list)
        {
            auto* const next = node->next;
            std::free(node);
            node = next;
        }
        local_74_list->next = local_74_list;
        local_74_list->prev = local_74_list;
    }

    {
        const C2DBoxI* local_60 = area;
        CCharString local_70_q(questName);
        const CCharString* local_54 = &local_70_q;
        const C2DBoxI* const* local_50 = &local_60;
        CWorldMap_GetMapNavigationAreaInit_FilterLeafThings filter{local_54, local_50};
        local_78->GetAllLeafThings<
            CThingAICreature,
            std::list<const CThingPhysical*, std::allocator<const CThingPhysical*>>,
            CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>>(
                &filter,
                local_74_list);
        std::_Cons_val(&local_70_q, extraout_EDX_02, unaff_EDI);
    }

    for (auto* node = local_74_list->next; node != local_74_list; node = node->next)
    {
        auto* const thing = reinterpret_cast<CVertexBufferWin32*>(node->value);
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_2c{};
        local_2c.x = xy[0];
        local_2c.y = xy[1];
        local_2c.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_2c, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_2c.x;
                dst->y = local_2c.y;
                dst->layer = local_2c.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    CWorldMap_GetMapNavigationAreaInit_VectorOverlay local_2c_vec{};
    {
        CCharString local_6c_q(questName);
        local_78->GetAllTCsInRoughArea<
            CTCPerceiveThing,
            CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>,
            std::list<CTCPerceiveThing*, std::allocator<CTCPerceiveThing*>>>(
                area,
                &local_6c_q,
                &local_2c_vec);
        std::_Cons_val(&local_6c_q, extraout_EDX_03, unaff_EDI);
    }

    for (void** it = local_2c_vec.begin; it != local_2c_vec.end; ++it)
    {
        auto* const perceiveThing =
            reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_PerceiveThingOverlay*>(*it);
        CVertexBufferWin32* const thing = perceiveThing->thingAt4;
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_3c{};
        local_3c.x = xy[0];
        local_3c.y = xy[1];
        local_3c.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_3c, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_3c.x;
                dst->y = local_3c.y;
                dst->layer = local_3c.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    CWorldMap_GetMapNavigationAreaInit_VectorOverlay local_3c_vec{};
    {
        CCharString local_6c_q(questName);
        local_78->GetPBestThingInRoughArea<
            CFilter_Bind3<
                CThingFilter_IsAvailableForCreatureInteraction,
                CThingFilter_IsAICreatureAwareOf,
                std::unary_negate<CThingFilter_IsIncapacitated>>,
            CThingCompare_Nearest,
            CThingAICreature>(
                area,
                &local_6c_q,
                &local_3c_vec,
                reinterpret_cast<CThingAICreature**>(unaff_EDI));
        std::_Cons_val(&local_6c_q, extraout_EDX_04, unaff_EDI);
    }

    for (void** it = local_3c_vec.begin; it != local_3c_vec.end; ++it)
    {
        auto* const thing = reinterpret_cast<CVertexBufferWin32*>(*it);
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_48{};
        local_48.x = xy[0];
        local_48.y = xy[1];
        local_48.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_48, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_48.x;
                dst->y = local_48.y;
                dst->layer = local_48.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    const auto* const areaBox =
        reinterpret_cast<const CWorldMap_GetMapNavigationAreaInit_BoxIOverlay*>(area);
    CWorldMap_GetMapNavigationAreaInit_RangeBlock local_range{};
    local_range.maxX = static_cast<float>(areaBox->maxX);
    local_range.maxY = static_cast<float>(areaBox->maxY);
    const long local_minY = areaBox->minY;
    local_range.minX = static_cast<float>(areaBox->minX);
    local_range.minY = static_cast<float>(local_minY);

    CWorldMap_GetMapNavigationAreaInit_VectorOverlay local_48_vec{};
    long local_60_count = 0x63;
    {
        CCharString local_6c_q(questName);
        const CCharString* local_54 = &local_6c_q;
        const long* local_50 = &local_60_count;
        const float* local_4c = &local_range.minX;
        CWorldMap_GetMapNavigationAreaInit_FilterAllThings filter{local_54, local_50, local_4c};
        local_78->GetAllThings<
            CThingCreatureBase,
            std::vector<CThingCreatureBase*, std::allocator<CThingCreatureBase*>>,
            CFilter_Bind3<CThingFilter_IsHuman, CThingFilter_IsNot, CThingFilter_In2DRange>>(
                &filter,
                &local_48_vec);
        std::_Cons_val(&local_6c_q, extraout_EDX_05, unaff_EDI);
    }

    for (void** it = local_48_vec.begin; it != local_48_vec.end; ++it)
    {
        auto* const thing = reinterpret_cast<CVertexBufferWin32*>(*it);
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_54_point{};
        local_54_point.x = xy[0];
        local_54_point.y = xy[1];
        local_54_point.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_54_point, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_54_point.x;
                dst->y = local_54_point.y;
                dst->layer = local_54_point.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    CWorldMap_GetMapNavigationAreaInit_VectorOverlay local_54_vec{};
    {
        CCharString local_6c_q(questName);
        local_78->GetPBestThingInRoughArea<
            CFilter_Bind3<
                CThingFilter_IsAvailableForCreatureInteraction,
                CThingFilter_IsAICreatureAwareOf,
                std::unary_negate<CThingFilter_IsIncapacitated>>,
            CThingCompare_Nearest,
            CThingAICreature>(
                area,
                &local_6c_q,
                &local_54_vec,
                reinterpret_cast<CThingAICreature**>(unaff_EDI));
        std::_Cons_val(&local_6c_q, extraout_EDX_06, unaff_EDI);
    }

    for (void** it = local_54_vec.begin; it != local_54_vec.end; ++it)
    {
        auto* const thing = reinterpret_cast<CVertexBufferWin32*>(*it);
        float* const xy = thing->DoSizeof();

        CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay local_6c_point{};
        local_6c_point.x = xy[0];
        local_6c_point.y = xy[1];
        local_6c_point.layer =
            GetNavigationLayerAt(reinterpret_cast<const C3DVector*>(thing->DoSizeof()), nullptr);

        std::byte* const pointWrite = out->pointWrite;
        if (pointWrite == out->pointEnd)
        {
            std_vector_construct_with_fill(pointWrite, &local_6c_point, &local_79, 1, 1);
        }
        else
        {
            if (pointWrite != nullptr)
            {
                auto* const dst =
                    reinterpret_cast<CWorldMap_GetMapNavigationAreaInit_NavigationPointOverlay*>(
                        pointWrite);
                dst->x = local_6c_point.x;
                dst->y = local_6c_point.y;
                dst->layer = local_6c_point.layer;
            }
            out->pointWrite += 0x0C;
        }
    }

    if (local_54_vec.begin != nullptr)
    {
        std::free(local_54_vec.begin);
    }
    if (local_48_vec.begin != nullptr)
    {
        std::free(local_48_vec.begin);
    }
    if (local_3c_vec.begin != nullptr)
    {
        std::free(local_3c_vec.begin);
    }
    if (local_2c_vec.begin != nullptr)
    {
        std::free(local_2c_vec.begin);
    }

    {
        auto* node = local_74_list->next;
        while (node != local_74_list)
        {
            auto* const next = node->next;
            std::free(node);
            node = next;
        }
        local_74_list->next = local_74_list;
        local_74_list->prev = local_74_list;
        std::free(local_74_list);
    }

    {
        auto* node = local_58->next;
        while (node != local_58)
        {
            auto* const next = node->next;
            std::free(node);
            node = next;
        }
        local_58->next = local_58;
        local_58->prev = local_58;
        std::free(local_58);
    }
}