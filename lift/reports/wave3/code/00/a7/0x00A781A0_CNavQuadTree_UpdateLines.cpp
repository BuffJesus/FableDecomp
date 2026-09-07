#include "candidates/nav_quad_tree_initialise_lines_compiled.h"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <list>
#include <vector>

namespace
{
    struct C2DBoxIOverlay
    {
        int MinX;
        int MinY;
        int MaxX;
        int MaxY;
    };
    static_assert(sizeof(C2DBoxIOverlay) == 0x10);
    static_assert(offsetof(C2DBoxIOverlay, MinX) == 0x0);
    static_assert(offsetof(C2DBoxIOverlay, MinY) == 0x4);
    static_assert(offsetof(C2DBoxIOverlay, MaxX) == 0x8);
    static_assert(offsetof(C2DBoxIOverlay, MaxY) == 0xC);

    struct C2DBoxFOverlay
    {
        float MinX;
        float MinY;
        float MaxX;
        float MaxY;
    };
    static_assert(sizeof(C2DBoxFOverlay) == 0x10);
    static_assert(offsetof(C2DBoxFOverlay, MinX) == 0x0);
    static_assert(offsetof(C2DBoxFOverlay, MinY) == 0x4);
    static_assert(offsetof(C2DBoxFOverlay, MaxX) == 0x8);
    static_assert(offsetof(C2DBoxFOverlay, MaxY) == 0xC);

    struct C2DLineFOverlay
    {
        C2DVector Start;
        C2DVector End;
    };
    static_assert(sizeof(C2DLineFOverlay) == 0x10);
    static_assert(offsetof(C2DLineFOverlay, Start) == 0x0);
    static_assert(offsetof(C2DLineFOverlay, End) == 0x8);

    struct C2DLineFListNodeBase
    {
        C2DLineFListNodeBase* Next;
        C2DLineFListNodeBase* Prev;
    };
    static_assert(sizeof(C2DLineFListNodeBase) == 0x8);
    static_assert(offsetof(C2DLineFListNodeBase, Next) == 0x0);
    static_assert(offsetof(C2DLineFListNodeBase, Prev) == 0x4);

    struct UpdateLinesListNode
    {
        C2DLineFListNodeBase Links;
        C2DLineFOverlay Value;
    };
    static_assert(sizeof(UpdateLinesListNode) == 0x18);
    static_assert(offsetof(UpdateLinesListNode, Links) == 0x0);
    static_assert(offsetof(UpdateLinesListNode, Value) == 0x8);

    struct C2DLineFListOverlay
    {
        C2DLineFListNodeBase* Head;
    };
    static_assert(sizeof(C2DLineFListOverlay) == 0x4);
    static_assert(offsetof(C2DLineFListOverlay, Head) == 0x0);

    struct C2DLineFListVectorOverlay
    {
        C2DLineFListOverlay* First;
        C2DLineFListOverlay* Last;
        C2DLineFListOverlay* End;
    };
    static_assert(sizeof(C2DLineFListVectorOverlay) == 0xC);
    static_assert(offsetof(C2DLineFListVectorOverlay, First) == 0x0);
    static_assert(offsetof(C2DLineFListVectorOverlay, Last) == 0x4);
    static_assert(offsetof(C2DLineFListVectorOverlay, End) == 0x8);

    struct C2DLineFVectorOverlay
    {
        const C2DLineF* First;
        const C2DLineF* Last;
        const C2DLineF* End;
    };
    static_assert(sizeof(C2DLineFVectorOverlay) == 0xC);
    static_assert(offsetof(C2DLineFVectorOverlay, First) == 0x0);
    static_assert(offsetof(C2DLineFVectorOverlay, Last) == 0x4);
    static_assert(offsetof(C2DLineFVectorOverlay, End) == 0x8);

    // Verified from the call site and later uses only:
    // +0x0 is freed at function exit and used as the span base,
    // +0x4 participates in (Last - First) >> 4 bounds checking,
    // +0x8 exists because the helper is called as a local std::vector-like object.
    struct VectorConstructInitializedC2DLineFResultOverlay
    {
        void Construct(int count);
        void* First;
        void* Last;
        void* End;
    };
    static_assert(sizeof(VectorConstructInitializedC2DLineFResultOverlay) == 0xC);
    static_assert(offsetof(VectorConstructInitializedC2DLineFResultOverlay, First) == 0x0);
    static_assert(offsetof(VectorConstructInitializedC2DLineFResultOverlay, Last) == 0x4);
    static_assert(offsetof(VectorConstructInitializedC2DLineFResultOverlay, End) == 0x8);

    static int Ftol2(const float value)
    {
        return static_cast<int>(value);
    }

    void VectorConstructInitializedC2DLineFResultOverlay::Construct(int count)
    {
        First = count > 0 ? std::malloc(count * 0x10) : 0;
        Last = First ? static_cast<unsigned char*>(First) + count * 0x10 : 0;
        End = Last;
    }
}

void CNavQuadTree::UpdateLines(
    CNavLineCellVector& lineLists,
    const std::vector<C2DLineF>& lines,
    const C2DBoxI& area) const
{
    const C2DLineFListVectorOverlay& lineListsRaw =
        reinterpret_cast<C2DLineFListVectorOverlay&>(lineLists);
    const C2DLineFVectorOverlay& linesRaw =
        reinterpret_cast<const C2DLineFVectorOverlay&>(lines);
    const C2DBoxIOverlay& areaRaw =
        reinterpret_cast<const C2DBoxIOverlay&>(area);

    const int iVar9 = Ftol2(MapWidth * DAT_0123095c);

    VectorConstructInitializedC2DLineFResultOverlay local_8c = {0};
    local_8c.Construct(
        (areaRaw.MaxX - areaRaw.MinX) * (areaRaw.MaxY - areaRaw.MinY));

    unsigned int local_18 = static_cast<unsigned int>(areaRaw.MinX);
    if (local_18 < static_cast<unsigned int>(areaRaw.MaxX))
    {
        do
        {
            unsigned int local_1c = static_cast<unsigned int>(areaRaw.MinY);
            if (local_1c < static_cast<unsigned int>(areaRaw.MaxY))
            {
                float fVar4 = static_cast<float>(static_cast<int>(local_18));
                if (static_cast<int>(local_18) < 0)
                {
                    fVar4 = fVar4 + DAT_0122dcb4;
                }
                fVar4 = fVar4 * DAT_0125916c;

                int local_c =
                    (static_cast<int>(local_1c) * iVar9 + static_cast<int>(local_18)) * 4;

                do
                {
                    const float fVar5 = fVar4 + MapPos.X;

                    float fVar6 = static_cast<float>(static_cast<int>(local_1c));
                    if (static_cast<int>(local_1c) < 0)
                    {
                        fVar6 = fVar6 + DAT_0122dcb4;
                    }
                    fVar6 = fVar6 * DAT_0125916c + MapPos.Y;

                    float* const pfVar10 = reinterpret_cast<float*>(
                        reinterpret_cast<unsigned long>(local_8c.First) +
                        static_cast<unsigned long>(
                            (((areaRaw.MaxX - areaRaw.MinX) *
                                  (static_cast<int>(local_1c) - areaRaw.MinY) -
                              areaRaw.MinX) +
                             static_cast<int>(local_18)) *
                            0x10));

                    pfVar10[0] = fVar5;
                    pfVar10[1] = fVar6;
                    pfVar10[2] = fVar5 + DAT_0125916c;
                    pfVar10[3] = fVar6 + DAT_0125916c;

                    C2DLineFListOverlay* const piVar17 = reinterpret_cast<C2DLineFListOverlay*>(
                        reinterpret_cast<unsigned long>(lineListsRaw.First) +
                        static_cast<unsigned long>(local_c));
                    C2DLineFListNodeBase* const head = piVar17->Head;
                    C2DLineFListNodeBase* node = head->Next;

                    if (node != head)
                    {
                        do
                        {
                            C2DLineFListNodeBase* const next = node->Next;
                            std::free(node);
                            node = next;
                        } while (node != head);
                    }

                    head->Next = head;
                    local_c = local_c + iVar9 * 4;
                    head->Prev = head;
                    local_1c = local_1c + 1;
                } while (local_1c < static_cast<unsigned int>(areaRaw.MaxY));
            }

            local_18 = local_18 + 1;
        } while (local_18 < static_cast<unsigned int>(areaRaw.MaxX));
    }

    const C2DLineF* pCVar15 = linesRaw.First;
    const C2DLineF* local_1c_line = pCVar15;
    if (pCVar15 != linesRaw.Last)
    {
        do
        {
            const C2DLineFOverlay& lineRaw =
                reinterpret_cast<const C2DLineFOverlay&>(*pCVar15);

            double local_14 = static_cast<double>(lineRaw.End.x);
            double local_28 = static_cast<double>(lineRaw.Start.x);
            const double* pdVar11 = &local_14;
            if (lineRaw.End.x <= lineRaw.Start.x)
            {
                pdVar11 = &local_28;
            }

            float fVar4 = (static_cast<float>(*pdVar11) - MapPos.X) * DAT_0123095c;
            int local_40 = static_cast<int>(fVar4 + 0.5f);
            if (fVar4 == static_cast<float>(local_40) - 1.0f)
            {
                local_40 = local_40 - 1;
            }

            int local_c = local_40;
            if (areaRaw.MaxX < local_40)
            {
                local_c = areaRaw.MaxX;
            }

            local_14 = static_cast<double>(lineRaw.End.y);
            local_28 = static_cast<double>(lineRaw.Start.y);
            pdVar11 = &local_14;
            if (lineRaw.End.y <= lineRaw.Start.y)
            {
                pdVar11 = &local_28;
            }

            fVar4 = (static_cast<float>(*pdVar11) - MapPos.Y) * DAT_0123095c;
            int local_48 = static_cast<int>(fVar4 + 0.5f);
            if (fVar4 == static_cast<float>(local_48) - 1.0f)
            {
                local_48 = local_48 - 1;
            }

            int local_38 = local_48;
            if (areaRaw.MaxY < local_48)
            {
                local_38 = areaRaw.MaxY;
            }

            local_28 = static_cast<double>(lineRaw.End.x);
            local_14 = static_cast<double>(lineRaw.Start.x);
            pdVar11 = &local_28;
            if (lineRaw.Start.x <= lineRaw.End.x)
            {
                pdVar11 = &local_14;
            }

            fVar4 = (static_cast<float>(*pdVar11) - MapPos.X) * DAT_0123095c;
            int local_44 = static_cast<int>(fVar4 - 0.5f);
            if (fVar4 == static_cast<float>(local_44) + 1.0f)
            {
                local_44 = local_44 + 1;
            }

            int local_20 = local_44;
            int local_8 = local_44;
            if (local_44 < areaRaw.MinX)
            {
                local_20 = areaRaw.MinX;
                local_8 = areaRaw.MinX;
            }

            local_28 = static_cast<double>(lineRaw.End.y);
            local_14 = static_cast<double>(lineRaw.Start.y);
            pdVar11 = &local_28;
            if (lineRaw.Start.y <= lineRaw.End.y)
            {
                pdVar11 = &local_14;
            }

            fVar4 = (static_cast<float>(*pdVar11) - MapPos.Y) * DAT_0123095c;
            unsigned int local_30 =
                static_cast<unsigned int>(static_cast<int>(fVar4 - 0.5f));
            if (fVar4 == static_cast<float>(static_cast<int>(local_30)) + 1.0f)
            {
                local_30 = local_30 + 1;
            }

            if (static_cast<int>(local_30) < areaRaw.MinY)
            {
                local_30 = static_cast<unsigned int>(areaRaw.MinY);
            }

            local_20 = local_8;
            local_1c_line = pCVar15;
            if (local_8 < local_c)
            {
                do
                {
                    local_18 = local_30;
                    if (static_cast<int>(local_30) < local_38)
                    {
                        const long long lVar7 = static_cast<long long>(local_8);
                        local_20 = (static_cast<int>(local_30) * iVar9 + local_8) * 4;

                        do
                        {
                            const unsigned int uVar2 =
                                static_cast<unsigned int>(areaRaw.MinY);
                            const int iVar1 = areaRaw.MinX;
                            const int iVar16 = areaRaw.MaxX - iVar1;

                            const long long lVar18 =
                                (static_cast<long long>(
                                     static_cast<int>(local_18) - static_cast<int>(uVar2)) *
                                     static_cast<long long>(iVar16) -
                                 static_cast<long long>(iVar1)) +
                                lVar7;

                            pCVar15 = local_1c_line;
                            if ((lVar18 < 0) || (0x7fffffffLL < lVar18))
                            {
                                if (local_8c.First != 0)
                                {
                                    std::free(local_8c.First);
                                }
                                return;
                            }

                            const unsigned int uVar12 =
                                ((local_18 - static_cast<unsigned int>(areaRaw.MinY)) *
                                     static_cast<unsigned int>(iVar16) -
                                 static_cast<unsigned int>(areaRaw.MinX)) +
                                static_cast<unsigned int>(local_8);

                            if ((uVar12 <= static_cast<unsigned int>(
                                             (reinterpret_cast<unsigned long>(local_8c.Last) -
                                              reinterpret_cast<unsigned long>(local_8c.First)) >>
                                             4)) &&
                                (-1 < static_cast<int>(uVar12)))
                            {
                                C2DBoxF* const pCVar14 = reinterpret_cast<C2DBoxF*>(
                                    reinterpret_cast<unsigned long>(local_8c.First) +
                                    static_cast<unsigned long>(uVar12) * 0x10);
                                const C2DBoxFOverlay& boxRaw =
                                    reinterpret_cast<const C2DBoxFOverlay&>(*pCVar14);

                                bool bVar8;
                                if (((boxRaw.MinX <= lineRaw.Start.x) &&
                                     (lineRaw.Start.x < boxRaw.MaxX) &&
                                     (boxRaw.MinY <= lineRaw.Start.y) &&
                                     (lineRaw.Start.y < boxRaw.MaxY)) ||
                                    (((boxRaw.MinX <= lineRaw.End.x) &&
                                      (lineRaw.End.x < boxRaw.MaxX) &&
                                      (boxRaw.MinY <= lineRaw.End.y) &&
                                      (lineRaw.End.y < boxRaw.MaxY)) ||
                                    ((bVar8 = local_1c_line->IntersectsWith(pCVar14)),
                                      bVar8)))
                                {
                                    C2DLineFListOverlay* const list = reinterpret_cast<C2DLineFListOverlay*>(
                                        reinterpret_cast<unsigned long>(lineListsRaw.First) +
                                        static_cast<unsigned long>(local_20));
                                    UpdateLinesListNode* const newNode =
                                        static_cast<UpdateLinesListNode*>(std::malloc(0x18));

                                    if (newNode + 1 != 0)
                                    {
                                        newNode->Value.Start = lineRaw.Start;
                                        newNode->Value.End = lineRaw.End;
                                    }

                                    C2DLineFListNodeBase* const prev = list->Head->Prev;
                                    newNode->Links.Next = list->Head;
                                    newNode->Links.Prev = prev;
                                    prev->Next = &newNode->Links;
                                    list->Head->Prev = &newNode->Links;
                                }
                            }

                            local_18 = local_18 + 1;
                            local_20 = local_20 + iVar9 * 4;
                            pCVar15 = local_1c_line;
                        } while (static_cast<int>(local_18) < local_38);
                    }

                    local_8 = local_8 + 1;
                } while (local_8 < local_c);
            }

            pCVar15 = reinterpret_cast<const C2DLineF*>(
                reinterpret_cast<const unsigned char*>(pCVar15) + 0x10);
            local_1c_line = pCVar15;
        } while (pCVar15 != linesRaw.Last);
    }

    if (local_8c.First != 0)
    {
        std::free(local_8c.First);
    }
}
