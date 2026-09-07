struct CDialogueLayerDef_Vector_Overlay
{
    NSpeechGainManager::CDialogueLayerDef* first;        // 0x00
    NSpeechGainManager::CDialogueLayerDef* last;         // 0x04
    NSpeechGainManager::CDialogueLayerDef* capacityLast; // 0x08
};

static_assert(offsetof(CDialogueLayerDef_Vector_Overlay, first) == 0x00);
static_assert(offsetof(CDialogueLayerDef_Vector_Overlay, last) == 0x04);
static_assert(offsetof(CDialogueLayerDef_Vector_Overlay, capacityLast) == 0x08);
static_assert(sizeof(NSpeechGainManager::CDialogueLayerDef) == 0x40);

CDialogueLayerDef_Vector_Overlay* __thiscall
CDialogueLayerDef_Vector_AssignRange(
    CDialogueLayerDef_Vector_Overlay* const self,
    const CDialogueLayerDef_Vector_Overlay* const other)
{
    if (other != self)
    {
        const int otherLastInt =
            reinterpret_cast<int>(other->last);
        auto* otherFirst =
            other->first;
        auto* selfFirst =
            self->first;

        const std::uint32_t otherCount =
            static_cast<std::uint32_t>((otherLastInt - reinterpret_cast<int>(otherFirst)) >> 6);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<int>(self->capacityLast) - reinterpret_cast<int>(selfFirst)) >> 6) < otherCount)
        {
            const int newFirstInt =
                CDialogueLayerDef_AllocateAndConstruct(otherCount, otherFirst, otherLastInt);

            CFileInstaller::CActiveFile::OnReadFinished(
                reinterpret_cast<CFileInstaller::CActiveFile*>(self));

            self->first = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(newFirstInt);
            self->capacityLast =
                reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>((otherCount * 0x40) + newFirstInt);
        }
        else
        {
            const std::uint32_t selfCount =
                static_cast<std::uint32_t>((reinterpret_cast<int>(self->last) - reinterpret_cast<int>(selfFirst)) >> 6);

            auto* dst = selfFirst;
            if (selfCount < otherCount)
            {
                int copyCount = static_cast<int>((selfCount << 6) >> 6);
                if (copyCount > 0)
                {
                    do
                    {
                        *dst = *otherFirst;
                        otherFirst = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(otherFirst) + 0x40);
                        --copyCount;
                        dst = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(dst) + 0x40);
                    } while (copyCount != 0);
                }

                auto* oldLast = self->last;
                auto* otherLast = other->last;
                auto* srcTail = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                    (((reinterpret_cast<int>(oldLast) - reinterpret_cast<int>(self->first)) >> 6) * 0x40) +
                    reinterpret_cast<int>(other->first));

                if (srcTail != otherLast)
                {
                    do
                    {
                        if (oldLast != nullptr)
                        {
                            ::new (oldLast) NSpeechGainManager::CDialogueLayerDef(*srcTail);
                        }

                        srcTail = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(srcTail) + 0x40);
                        oldLast = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(oldLast) + 0x40);
                    } while (srcTail != otherLast);
                }
            }
            else
            {
                int copyCount = (otherLastInt - reinterpret_cast<int>(otherFirst)) >> 6;
                if (copyCount > 0)
                {
                    do
                    {
                        *dst = *otherFirst;
                        dst = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(dst) + 0x40);
                        otherFirst = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
                            reinterpret_cast<int>(otherFirst) + 0x40);
                        --copyCount;
                    } while (copyCount != 0);
                }

                std::_Destroy(
                    dst,
                    self->last,
                    reinterpret_cast<CIntelligentPointer<CThing_const_>*>(
                        reinterpret_cast<int>(&dst) + 3));
            }
        }

        self->last = reinterpret_cast<NSpeechGainManager::CDialogueLayerDef*>(
            (otherCount * 0x40) + reinterpret_cast<int>(self->first));
    }

    return self;
}