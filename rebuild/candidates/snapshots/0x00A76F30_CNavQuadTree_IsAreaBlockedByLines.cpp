namespace
{
    using C2DLineList = std::list<C2DLineF>;
    using C2DLineListPtrVector = std::vector<const C2DLineList*>;

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
    static_assert(offsetof(C2DLineFListNodeBase, Next) == 0x0);
    static_assert(offsetof(C2DLineFListNodeBase, Prev) == 0x4);

    struct C2DLineFListNode
    {
        C2DLineFListNodeBase Links;
        C2DLineFOverlay Value;
    };
    static_assert(offsetof(C2DLineFListNode, Value) == 0x8);

    struct C2DLineFListOverlay
    {
        C2DLineFListNodeBase* Head;
        std::uint32_t Size;
    };
    static_assert(offsetof(C2DLineFListOverlay, Head) == 0x0);

    struct C2DLineListPtrVectorOverlay
    {
        const C2DLineList* const* First;
        const C2DLineList* const* Last;
        const C2DLineList* const* End;
    };
    static_assert(offsetof(C2DLineListPtrVectorOverlay, First) == 0x0);
    static_assert(offsetof(C2DLineListPtrVectorOverlay, Last) == 0x4);
    static_assert(offsetof(C2DLineListPtrVectorOverlay, End) == 0x8);
}

bool CNavQuadTree::IsAreaBlockedByLines(
    const C2DBoxF& area,
    const std::vector<const std::list<C2DLineF>*>& lineLists) const
{
    const auto& areaRaw = reinterpret_cast<const C2DBoxFOverlay&>(area);
    const auto& lineListsRaw = reinterpret_cast<const C2DLineListPtrVectorOverlay&>(lineLists);

    auto listIt = lineListsRaw.First;
    if (listIt != lineListsRaw.Last)
    {
        do
        {
            const auto* const listRaw =
                reinterpret_cast<const C2DLineFListOverlay*>(*listIt);
            C2DLineFListNodeBase* const head = listRaw->Head;
            C2DLineFListNodeBase* node = head->Next;

            if (node != head)
            {
                do
                {
                    const auto* const lineNode =
                        reinterpret_cast<const C2DLineFListNode*>(node);
                    const C2DLineF* const this_00 =
                        reinterpret_cast<const C2DLineF*>(&lineNode->Value);
                    const auto& lineRaw = lineNode->Value;

                    double local_18 = static_cast<double>(lineRaw.End.x);
                    double local_10 = static_cast<double>(lineRaw.Start.x);

                    const double* pdVar5 = &local_18;
                    if (lineRaw.End.x <= lineRaw.Start.x)
                    {
                        pdVar5 = &local_10;
                    }

                    double dVar3 = static_cast<double>(-DAT_0129ba3c);
                    if (((*pdVar5 - static_cast<double>(areaRaw.MinX)) < dVar3) ==
                        ((*pdVar5 - static_cast<double>(areaRaw.MinX)) == dVar3))
                    {
                        local_10 = static_cast<double>(lineRaw.End.x);
                        local_18 = static_cast<double>(lineRaw.Start.x);

                        pdVar5 = &local_10;
                        if (lineRaw.Start.x <= lineRaw.End.x)
                        {
                            pdVar5 = &local_18;
                        }

                        if ((*pdVar5 - static_cast<double>(areaRaw.MaxX)) <
                            static_cast<double>(DAT_0129ba3c))
                        {
                            local_18 = static_cast<double>(lineRaw.End.y);
                            local_10 = static_cast<double>(lineRaw.Start.y);

                            pdVar5 = &local_18;
                            if (lineRaw.End.y <= lineRaw.Start.y)
                            {
                                pdVar5 = &local_10;
                            }

                            dVar3 = static_cast<double>(-DAT_0129ba3c);
                            if (((*pdVar5 - static_cast<double>(areaRaw.MinY)) < dVar3) ==
                                ((*pdVar5 - static_cast<double>(areaRaw.MinY)) == dVar3))
                            {
                                local_10 = static_cast<double>(lineRaw.End.y);
                                local_18 = static_cast<double>(lineRaw.Start.y);

                                pdVar5 = &local_10;
                                if (lineRaw.Start.y <= lineRaw.End.y)
                                {
                                    pdVar5 = &local_18;
                                }

                                if (((*pdVar5 - static_cast<double>(areaRaw.MaxY)) <
                                     static_cast<double>(DAT_0129ba3c)) &&
                                    ((C2DBoxF::ContainsPoint(
                                          const_cast<C2DBoxF*>(
                                              reinterpret_cast<const C2DBoxF*>(&area)),
                                          const_cast<C2DVector*>(&lineRaw.Start)) ||
                                      C2DBoxF::ContainsPoint(
                                          const_cast<C2DBoxF*>(
                                              reinterpret_cast<const C2DBoxF*>(&area)),
                                          const_cast<C2DVector*>(&lineRaw.End))) ||
                                     C2DLineF::IntersectsWith(
                                         const_cast<C2DLineF*>(this_00),
                                         const_cast<C2DBoxF*>(
                                             reinterpret_cast<const C2DBoxF*>(&area)))))
                                {
                                    return true;
                                }
                            }
                        }
                    }

                    node = node->Next;
                } while (node != head);
            }

            ++listIt;
        } while (listIt != lineListsRaw.Last);
    }

    return false;
}