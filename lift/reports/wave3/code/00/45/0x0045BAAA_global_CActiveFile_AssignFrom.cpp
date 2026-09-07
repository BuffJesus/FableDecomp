struct CActiveFileVectorOverlay {
    CRGBColour* begin;       // 0x00
    CRGBColour* end;         // 0x04
    CRGBColour* capacityEnd; // 0x08
};

static_assert(offsetof(CActiveFileVectorOverlay, begin) == 0x00);
static_assert(offsetof(CActiveFileVectorOverlay, end) == 0x04);
static_assert(offsetof(CActiveFileVectorOverlay, capacityEnd) == 0x08);

CActiveFile* __thiscall CActiveFile_AssignFrom(CActiveFile* self, CActiveFile* other)
{
    if (other != self) {
        auto& dst = *reinterpret_cast<CActiveFileVectorOverlay*>(self);
        auto& src = *reinterpret_cast<const CActiveFileVectorOverlay*>(other);

        const std::uint32_t srcCount =
            static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(src.end) -
                                        reinterpret_cast<std::uintptr_t>(src.begin)) >> 2);

        if (static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(dst.capacityEnd) -
                                        reinterpret_cast<std::uintptr_t>(dst.begin)) >> 2) < srcCount) {
            const auto newBegin = reinterpret_cast<CRGBColour*>(
                Vector_AllocateAndCopyRGB(
                    srcCount,
                    reinterpret_cast<void*>(src.begin),
                    reinterpret_cast<int>(src.end)));

            CFileInstaller::CActiveFile::OnReadFinished(self);

            dst.begin = newBegin;
            dst.capacityEnd = reinterpret_cast<CRGBColour*>(
                reinterpret_cast<std::uintptr_t>(newBegin) + srcCount * 4);
        } else {
            const std::uint32_t dstCount =
                static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(dst.end) -
                                            reinterpret_cast<std::uintptr_t>(dst.begin)) >> 2);

            if (dstCount < srcCount) {
                std::uint32_t copyCount = dstCount;
                CRGBColour* dstIt = dst.begin;
                const CRGBColour* srcIt = src.begin;

                if (static_cast<int>(copyCount) > 0) {
                    do {
                        *reinterpret_cast<std::uint32_t*>(dstIt) =
                            *reinterpret_cast<const std::uint32_t*>(srcIt);
                        dstIt = reinterpret_cast<CRGBColour*>(
                            reinterpret_cast<std::uintptr_t>(dstIt) + 4);
                        srcIt = reinterpret_cast<const CRGBColour*>(
                            reinterpret_cast<std::uintptr_t>(srcIt) + 4);
                        --copyCount;
                    } while (copyCount != 0);
                }

                CRGBColour* constructDst = dst.end;
                const CRGBColour* constructEnd = src.end;
                const CRGBColour* constructSrc = reinterpret_cast<const CRGBColour*>(
                    reinterpret_cast<std::uintptr_t>(src.begin) +
                    (((reinterpret_cast<std::uintptr_t>(constructDst) -
                       reinterpret_cast<std::uintptr_t>(dst.begin)) >> 2) * 4));

                for (; constructSrc != constructEnd;
                     constructSrc = reinterpret_cast<const CRGBColour*>(
                         reinterpret_cast<std::uintptr_t>(constructSrc) + 4)) {
                    std::_Construct<CRGBColour, const CRGBColour&>(constructDst, constructSrc);
                    constructDst = reinterpret_cast<CRGBColour*>(
                        reinterpret_cast<std::uintptr_t>(constructDst) + 4);
                }
            } else {
                int copyCount = static_cast<int>(
                    (reinterpret_cast<std::uintptr_t>(src.end) -
                     reinterpret_cast<std::uintptr_t>(src.begin)) >> 2);

                if (copyCount > 0) {
                    CRGBColour* dstIt = dst.begin;
                    const CRGBColour* srcIt = src.begin;

                    do {
                        *reinterpret_cast<std::uint32_t*>(dstIt) =
                            *reinterpret_cast<const std::uint32_t*>(srcIt);
                        srcIt = reinterpret_cast<const CRGBColour*>(
                            reinterpret_cast<std::uintptr_t>(srcIt) + 4);
                        dstIt = reinterpret_cast<CRGBColour*>(
                            reinterpret_cast<std::uintptr_t>(dstIt) + 4);
                        --copyCount;
                    } while (copyCount != 0);
                }
            }
        }

        dst.end = reinterpret_cast<CRGBColour*>(
            reinterpret_cast<std::uintptr_t>(dst.begin) + srcCount * 4);
    }

    return self;
}