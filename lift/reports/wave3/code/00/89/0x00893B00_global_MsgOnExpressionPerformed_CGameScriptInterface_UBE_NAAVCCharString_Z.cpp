namespace
{
struct CGameScriptInterface_Overlay
{
    std::byte pad00[0x04];
    void* context; // 0x04
    std::byte pad08[0x08];
    CDefinitionManager* definitionManager; // 0x10
};
static_assert(offsetof(CGameScriptInterface_Overlay, context) == 0x04);
static_assert(offsetof(CGameScriptInterface_Overlay, definitionManager) == 0x10);

struct CGameScriptInterfaceContext_Overlay
{
    std::byte pad00[0x60];
    CMessageEventManager* messageEventManager; // 0x60
};
static_assert(offsetof(CGameScriptInterfaceContext_Overlay, messageEventManager) == 0x60);

struct CMessageEvent_Overlay
{
    std::byte pad00[0x3C];
    long definitionGlobalIndex; // 0x3C
};
static_assert(offsetof(CMessageEvent_Overlay, definitionGlobalIndex) == 0x3C);

struct CDefName_Overlay
{
    long stringIndex; // 0x00
};
static_assert(offsetof(CDefName_Overlay, stringIndex) == 0x00);

struct CFilter_Bind2_Overlay
{
    void* first;  // local_8
    void* second; // local_4
};
static_assert(offsetof(CFilter_Bind2_Overlay, first) == 0x00);
static_assert(offsetof(CFilter_Bind2_Overlay, second) == 0x04);

extern CDefStringTable DAT_013ca828;
} // namespace

bool CGameScriptInterface::MsgOnExpressionPerformed(CCharString& outExpressionName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    const auto* const context = reinterpret_cast<const CGameScriptInterfaceContext_Overlay*>(self->context);

    alignas(CCharString) std::byte local14_storage[sizeof(CCharString)];
    alignas(CDefName_Overlay) std::byte local10_storage[sizeof(CDefName_Overlay)];
    CFilter_Bind2_Overlay localFilter{};
    long localC;

    const long maxWorldFrame = this->GetMaxWorldFrameForMessages();
    localC = this->GetMaxWorldFrameForMessages();

    *reinterpret_cast<long*>(local14_storage) = 0x2A;
    *reinterpret_cast<long*>(local10_storage) = maxWorldFrame;
    localFilter.first = local14_storage;
    localFilter.second = local10_storage;
    (void)localC;

    CMessageEvent* const messageEvent =
        context->messageEventManager
            ->FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>>(
                reinterpret_cast<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard>*>(&localFilter));

    if (messageEvent != nullptr)
    {
        const auto* const eventOverlay = reinterpret_cast<const CMessageEvent_Overlay*>(messageEvent);

        CDefName_Overlay* const defName =
            self->definitionManager->GetDefNameFromGlobalIndex(
                reinterpret_cast<CDefName_Overlay*>(local10_storage),
                eventOverlay->definitionGlobalIndex);

        DAT_013ca828.GetString(
            reinterpret_cast<CCharString*>(local14_storage),
            defName->stringIndex);

        outExpressionName = *reinterpret_cast<CCharString*>(local14_storage);
        std::destroy_at(reinterpret_cast<CCharString*>(local14_storage));
        return true;
    }

    return false;
}