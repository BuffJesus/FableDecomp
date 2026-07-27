#include <cstddef>
#include <cstdint>

extern float _DAT_012BF314;
extern void DSP_ComputeRunningMinEnvelope(int sampleBuffer, int windowLength, int sampleCount);

namespace
{
    struct FloatAt20
    {
        std::byte pad_00[0x20];
        float value_20;
    };
    static_assert(offsetof(FloatAt20, value_20) == 0x20);

    struct FFTWaveformEnvelopeContext
    {
        int segmentCount;                  // 0x00
        std::uint32_t limitObject;         // 0x04
        std::byte pad_08[0x14 - 0x08];
        std::uint32_t segmentBoundaries;   // 0x14
        std::byte pad_18[0x1C - 0x18];
        int sampleOffset;                  // 0x1C
        std::byte pad_20[0x24 - 0x20];
        int windowLength;                  // 0x24
        int sampleCount;                   // 0x28
    };
    static_assert(offsetof(FFTWaveformEnvelopeContext, segmentCount) == 0x00);
    static_assert(offsetof(FFTWaveformEnvelopeContext, limitObject) == 0x04);
    static_assert(offsetof(FFTWaveformEnvelopeContext, segmentBoundaries) == 0x14);
    static_assert(offsetof(FFTWaveformEnvelopeContext, sampleOffset) == 0x1C);
    static_assert(offsetof(FFTWaveformEnvelopeContext, windowLength) == 0x24);
    static_assert(offsetof(FFTWaveformEnvelopeContext, sampleCount) == 0x28);
}

void FFT_ComputeWaveformEnvelope(int param_1, int param_2)
{
    const FFTWaveformEnvelopeContext* unaff_EDI;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm {
        mov unaff_EDI, edi
    }
#elif defined(__GNUC__) && defined(__i386__)
    asm("" : "=D"(unaff_EDI));
#else
#   error "This reverse requires an x86 EDI live-in context."
#endif

    int iVar1;
    float fVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;

    iVar5 = unaff_EDI->windowLength;
    iVar6 = 0;
    DSP_ComputeRunningMinEnvelope(param_1, iVar5, unaff_EDI->sampleCount);
    iVar5 =
        (*reinterpret_cast<const int*>(static_cast<std::uintptr_t>(unaff_EDI->segmentBoundaries)) -
         (iVar5 >> 1)) -
        unaff_EDI->sampleOffset;
    if (1 < unaff_EDI->segmentCount) {
        do {
            fVar2 = *reinterpret_cast<float*>(param_1 + iVar5 * 4);
            iVar3 = unaff_EDI->sampleOffset;
            iVar4 = iVar5;
            if (reinterpret_cast<const FloatAt20*>(static_cast<std::uintptr_t>(unaff_EDI->limitObject))->value_20 <
                fVar2) {
                fVar2 =
                    reinterpret_cast<const FloatAt20*>(static_cast<std::uintptr_t>(unaff_EDI->limitObject))->value_20;
            }
            while ((iVar4 = iVar4 + 1),
                   iVar4 <=
                       ((*(int*)(static_cast<std::uintptr_t>(unaff_EDI->segmentBoundaries) + iVar6 * 4 + 4) +
                         *(int*)(static_cast<std::uintptr_t>(unaff_EDI->segmentBoundaries) + iVar6 * 4)) >>
                        1) -
                           iVar3) {
                iVar1 = iVar5 * 4;
                iVar5 = iVar5 + 1;
                if (((_DAT_012BF314 < *reinterpret_cast<float*>(param_1 + 4 + iVar1)) &&
                     (*reinterpret_cast<float*>(param_1 + iVar5 * 4) < fVar2)) ||
                    (fVar2 == _DAT_012BF314)) {
                    fVar2 = *reinterpret_cast<float*>(param_1 + iVar5 * 4);
                }
            }
            if (iVar6 < unaff_EDI->segmentCount) {
                do {
                    if (iVar3 + iVar5 <
                        *reinterpret_cast<const int*>(static_cast<std::uintptr_t>(unaff_EDI->segmentBoundaries) +
                                                     iVar6 * 4)) {
                        break;
                    }
                    if (*reinterpret_cast<float*>(param_2 + iVar6 * 4) < fVar2) {
                        *reinterpret_cast<float*>(param_2 + iVar6 * 4) = fVar2;
                    }
                    iVar6 = iVar6 + 1;
                } while (iVar6 < unaff_EDI->segmentCount);
            }
        } while (iVar6 + 1 < unaff_EDI->segmentCount);
    }
    fVar2 = *reinterpret_cast<float*>(param_1 + -4 + unaff_EDI->sampleCount * 4);
    if (iVar6 < unaff_EDI->segmentCount) {
        do {
            if (*reinterpret_cast<float*>(param_2 + iVar6 * 4) < fVar2) {
                *reinterpret_cast<float*>(param_2 + iVar6 * 4) = fVar2;
            }
            iVar6 = iVar6 + 1;
        } while (iVar6 < unaff_EDI->segmentCount);
    }
}