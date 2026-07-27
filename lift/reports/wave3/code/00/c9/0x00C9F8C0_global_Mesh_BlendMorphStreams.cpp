#include <cstddef>
#include <cstdint>
#include <malloc.h>

struct DetailLODSlotOverlay {
    void* target; // 0x00
};
static_assert(offsetof(DetailLODSlotOverlay, target) == 0x00);

struct MeshStreamSetOverlay {
    std::byte pad00[0x04];
    std::int32_t streamCount;                   // 0x04
    std::byte pad08[0x04];
    DetailLODSlotOverlay* detailLodSlots[4];   // 0x0C
    void* morphLayoutBlob;                      // 0x1C
};
static_assert(offsetof(MeshStreamSetOverlay, streamCount) == 0x04);
static_assert(offsetof(MeshStreamSetOverlay, detailLodSlots) == 0x0C);
static_assert(offsetof(MeshStreamSetOverlay, morphLayoutBlob) == 0x1C);

struct MeshBlendHostOverlay {
    std::byte pad00[0x04];
    MeshStreamSetOverlay* streamSet; // 0x04
};
static_assert(offsetof(MeshBlendHostOverlay, streamSet) == 0x04);

struct MeshBlendMorphStreamsContext {
    std::uint32_t* streamBufferTable; // 0x00
    std::byte pad04[0x18];
    std::int32_t meshStreamSetIndex;  // 0x1C
    std::byte pad20[0x04];
    std::uint32_t activeMorphMask;    // 0x24
    std::byte pad28[0x18];
    MeshBlendHostOverlay* host;       // 0x40
};
static_assert(offsetof(MeshBlendMorphStreamsContext, meshStreamSetIndex) == 0x1C);
static_assert(offsetof(MeshBlendMorphStreamsContext, activeMorphMask) == 0x24);
static_assert(offsetof(MeshBlendMorphStreamsContext, host) == 0x40);

struct MorphBlendJobOverlay {
    std::int32_t groupCount;                    // 0x000
    std::int32_t streamGroupIndices[0x100];    // 0x004
    std::int32_t streamTypeRemap[0x10];        // 0x404
    std::int32_t groupTypeIndices[0x10];       // 0x444
    std::int32_t pairCount;                    // 0x484
    std::int32_t pairLeftStreamIndices[0x100]; // 0x488
    std::int32_t pairRightStreamIndices[0x100];// 0x888
};
static_assert(offsetof(MorphBlendJobOverlay, streamGroupIndices) == 0x004);
static_assert(offsetof(MorphBlendJobOverlay, streamTypeRemap) == 0x404);
static_assert(offsetof(MorphBlendJobOverlay, groupTypeIndices) == 0x444);
static_assert(offsetof(MorphBlendJobOverlay, pairCount) == 0x484);
static_assert(offsetof(MorphBlendJobOverlay, pairLeftStreamIndices) == 0x488);
static_assert(offsetof(MorphBlendJobOverlay, pairRightStreamIndices) == 0x888);

struct MorphDispatchOverlay {
    std::byte pad00[0x14];
    std::uint32_t (__thiscall* createScratch)(MeshBlendMorphStreamsContext* self); // 0x14
    void (__thiscall* finalizeScratch)(MeshBlendMorphStreamsContext* self, std::uint32_t scratchStream, std::uint32_t dstStream); // 0x18
    void (__thiscall* blendGroup)(MeshBlendMorphStreamsContext* self, std::uint32_t* selectedStreams, std::uint32_t* selectedPresent, std::int32_t count); // 0x1C
};
static_assert(offsetof(MorphDispatchOverlay, createScratch) == 0x14);
static_assert(offsetof(MorphDispatchOverlay, finalizeScratch) == 0x18);
static_assert(offsetof(MorphDispatchOverlay, blendGroup) == 0x1C);

extern MorphDispatchOverlay* PTR_DAT_013a3dc8[];
extern MorphDispatchOverlay* PTR_DAT_013a3dd0[];
extern const float _DAT_0122dedc;
extern void __thiscall Mesh_ComputeDetailLOD(void* detailLodTarget, std::uint32_t streamBuffer);

std::uint32_t __fastcall Mesh_BlendMorphStreams(
    MeshBlendMorphStreamsContext* self,
    MorphBlendJobOverlay* job)
{
    MeshStreamSetOverlay* const streamSet = self->host->streamSet;
    auto* const morphLayout = static_cast<std::byte*>(streamSet->morphLayoutBlob);
    const std::uint32_t morphWordMask =
        *reinterpret_cast<const std::uint32_t*>(morphLayout + self->meshStreamSetIndex * sizeof(std::uint32_t));

    self->activeMorphMask = morphWordMask;

    const std::int32_t streamCount = streamSet->streamCount;

    auto* const scratchStreams =
        static_cast<std::uint32_t*>(_alloca(sizeof(std::uint32_t) * static_cast<std::size_t>(streamCount)));
    auto* const streamPresent =
        static_cast<std::uint32_t*>(_alloca(sizeof(std::uint32_t) * static_cast<std::size_t>(streamCount)));
    auto* const selectedPresent =
        static_cast<std::uint32_t*>(_alloca(sizeof(std::uint32_t) * static_cast<std::size_t>(streamCount)));
    auto* const selectedStreams =
        static_cast<std::uint32_t*>(_alloca(sizeof(std::uint32_t) * static_cast<std::size_t>(streamCount)));

    std::int32_t streamIndex = 0;
    if (0 < streamCount) {
        do {
            const std::int32_t remapIndex = job->streamTypeRemap[job->streamGroupIndices[streamIndex]];
            auto* const dispatch =
                PTR_DAT_013a3dc8[*reinterpret_cast<const std::int32_t*>(morphLayout + 0x320 + remapIndex * sizeof(std::int32_t))];

            const std::uint32_t scratch = dispatch->createScratch(self);
            scratchStreams[streamIndex] = scratch;
            streamPresent[streamIndex] = static_cast<std::uint32_t>(scratch != 0);

            auto* dst = reinterpret_cast<std::uint32_t*>(self->streamBufferTable[streamIndex]);
            for (std::uint32_t count = (morphWordMask & 0x3fffffffU) >> 1; count != 0; --count) {
                *dst = 0;
                ++dst;
            }
            for (std::int32_t tail = static_cast<std::int32_t>((morphWordMask & 1U) << 1); tail != 0; --tail) {
                *reinterpret_cast<std::uint8_t*>(dst) = 0;
                dst = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(dst) + 1);
            }

            ++streamIndex;
        } while (streamIndex < streamCount);
    }

    std::int32_t pairIndex = 0;
    if (0 < job->pairCount) {
        do {
            const std::int32_t leftIndex = job->pairLeftStreamIndices[pairIndex];
            const std::int32_t rightIndex = job->pairRightStreamIndices[pairIndex];
            if ((streamPresent[leftIndex] != 0) || (streamPresent[rightIndex] != 0)) {
                streamPresent[leftIndex] = 1;
                streamPresent[rightIndex] = 1;
            }
            ++pairIndex;
        } while (pairIndex < job->pairCount);
    }

    std::int32_t groupIndex = 0;
    if (0 < job->groupCount) {
        do {
            std::int32_t selectedCount = 0;
            std::int32_t groupStreamIndex = 0;

            if (0 < streamCount) {
                do {
                    if (job->streamGroupIndices[groupStreamIndex] == groupIndex) {
                        selectedPresent[selectedCount] = static_cast<std::uint32_t>(streamPresent[groupStreamIndex] != 0);
                        selectedStreams[selectedCount] = self->streamBufferTable[groupStreamIndex];
                        ++selectedCount;
                    }
                    ++groupStreamIndex;
                } while (groupStreamIndex < streamCount);
            }

            auto* const dispatch =
                PTR_DAT_013a3dd0[*reinterpret_cast<const std::int32_t*>(
                    morphLayout + 0x520 + job->groupTypeIndices[groupIndex] * sizeof(std::int32_t))];
            dispatch->blendGroup(self, selectedStreams, selectedPresent, selectedCount);

            ++groupIndex;
        } while (groupIndex < job->groupCount);
    }

    std::int32_t remainingPairs = job->pairCount;
    if ((remainingPairs - 1) >= 0) {
        const std::int32_t floatCount = static_cast<std::int32_t>(morphWordMask / 2U);
        std::int32_t reversePairIndex = remainingPairs - 1;

        do {
            const std::uint32_t leftBuffer = self->streamBufferTable[job->pairLeftStreamIndices[reversePairIndex]];
            const std::uint32_t rightBuffer = self->streamBufferTable[job->pairRightStreamIndices[reversePairIndex]];

            std::int32_t processedCount = 0;

            if (3 < floatCount) {
                float* leftPtr = reinterpret_cast<float*>(leftBuffer + 4);
                const std::intptr_t delta = static_cast<std::intptr_t>(rightBuffer) - static_cast<std::intptr_t>(leftBuffer);
                float* rightPtr = reinterpret_cast<float*>(rightBuffer + 0x0C);
                std::int32_t quadCount = static_cast<std::int32_t>(((static_cast<std::uint32_t>(floatCount) - 4U) >> 2) + 1U);
                processedCount = quadCount * 4;

                do {
                    float fVar1 = leftPtr[-1];
                    float fVar2 = rightPtr[-3];
                    if (fVar1 <= _DAT_0122dedc) {
                        if (fVar2 <= _DAT_0122dedc) {
                            rightPtr[-3] = fVar1;
                            fVar1 = fVar1 - fVar2;
                            leftPtr[-1] = fVar1;
                        } else {
                            rightPtr[-3] = fVar2 + fVar1;
                        }
                    } else if (fVar2 <= _DAT_0122dedc) {
                        rightPtr[-3] = fVar1;
                        fVar1 = fVar2 + fVar1;
                        leftPtr[-1] = fVar1;
                    } else {
                        rightPtr[-3] = fVar1 - fVar2;
                    }

                    fVar1 = *leftPtr;
                    fVar2 = *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta);
                    if (fVar1 <= _DAT_0122dedc) {
                        if (fVar2 <= _DAT_0122dedc) {
                            *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1;
                            fVar1 = fVar1 - fVar2;
                            *leftPtr = fVar1;
                        } else {
                            *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar2 + fVar1;
                        }
                    } else if (fVar2 <= _DAT_0122dedc) {
                        *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1;
                        fVar1 = fVar2 + fVar1;
                        *leftPtr = fVar1;
                    } else {
                        *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1 - fVar2;
                    }

                    fVar1 = leftPtr[1];
                    fVar2 = rightPtr[-1];
                    if (fVar1 <= _DAT_0122dedc) {
                        if (fVar2 <= _DAT_0122dedc) {
                            rightPtr[-1] = fVar1;
                            fVar1 = fVar1 - fVar2;
                            leftPtr[1] = fVar1;
                        } else {
                            rightPtr[-1] = fVar2 + fVar1;
                        }
                    } else if (fVar2 <= _DAT_0122dedc) {
                        rightPtr[-1] = fVar1;
                        fVar1 = fVar2 + fVar1;
                        leftPtr[1] = fVar1;
                    } else {
                        rightPtr[-1] = fVar1 - fVar2;
                    }

                    fVar1 = leftPtr[2];
                    fVar2 = *rightPtr;
                    if (fVar1 <= _DAT_0122dedc) {
                        if (fVar2 <= _DAT_0122dedc) {
                            *rightPtr = fVar1;
                            fVar1 = fVar1 - fVar2;
                            leftPtr[2] = fVar1;
                        } else {
                            *rightPtr = fVar2 + fVar1;
                        }
                    } else if (fVar2 <= _DAT_0122dedc) {
                        *rightPtr = fVar1;
                        fVar1 = fVar2 + fVar1;
                        leftPtr[2] = fVar1;
                    } else {
                        *rightPtr = fVar1 - fVar2;
                    }

                    leftPtr += 4;
                    rightPtr += 4;
                    --quadCount;
                } while (quadCount != 0);
            }

            if (processedCount < floatCount) {
                float* leftPtr = reinterpret_cast<float*>(leftBuffer + processedCount * sizeof(float));
                const std::intptr_t delta = static_cast<std::intptr_t>(rightBuffer) - static_cast<std::intptr_t>(leftBuffer);
                std::int32_t remainder = floatCount - processedCount;

                do {
                    float fVar1 = *leftPtr;
                    float fVar2 = *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta);
                    if (fVar1 <= _DAT_0122dedc) {
                        if (fVar2 <= _DAT_0122dedc) {
                            *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1;
                            fVar1 = fVar1 - fVar2;
                            *leftPtr = fVar1;
                        } else {
                            *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar2 + fVar1;
                        }
                    } else if (fVar2 <= _DAT_0122dedc) {
                        *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1;
                        fVar1 = fVar2 + fVar1;
                        *leftPtr = fVar1;
                    } else {
                        *reinterpret_cast<float*>(reinterpret_cast<std::byte*>(leftPtr) + delta) = fVar1 - fVar2;
                    }

                    ++leftPtr;
                    --remainder;
                } while (remainder != 0);
            }

            --reversePairIndex;
            --remainingPairs;
        } while (remainingPairs != 0);
    }

    streamIndex = 0;
    if (0 < streamCount) {
        do {
            const std::int32_t remapIndex = job->streamTypeRemap[job->streamGroupIndices[streamIndex]];
            auto* const dispatch =
                PTR_DAT_013a3dc8[*reinterpret_cast<const std::int32_t*>(morphLayout + 0x320 + remapIndex * sizeof(std::int32_t))];
            dispatch->finalizeScratch(self, scratchStreams[streamIndex], self->streamBufferTable[streamIndex]);

            ++streamIndex;
        } while (streamIndex < streamCount);
    }

    streamIndex = 0;
    if (0 < streamCount) {
        const DetailLODSlotOverlay* const slot = streamSet->detailLodSlots[self->meshStreamSetIndex];
        do {
            Mesh_ComputeDetailLOD(slot->target, self->streamBufferTable[streamIndex]);
            ++streamIndex;
        } while (streamIndex < streamCount);
    }

    return 0;
}