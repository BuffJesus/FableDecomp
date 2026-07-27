#include <cstddef>
#include <cstdint>

extern float _DAT_0122ded8;

struct AudioFilterCoefficients
{
    float outputScale;   // 0x00
    float unused_04;     // 0x04
    float feedback0;     // 0x08
    float feedback1;     // 0x0C
    float feedforward0;  // 0x10
    float feedforward1;  // 0x14
    float feedforward2;  // 0x18
};

static_assert(offsetof(AudioFilterCoefficients, outputScale) == 0x00);
static_assert(offsetof(AudioFilterCoefficients, feedback0) == 0x08);
static_assert(offsetof(AudioFilterCoefficients, feedback1) == 0x0C);
static_assert(offsetof(AudioFilterCoefficients, feedforward0) == 0x10);
static_assert(offsetof(AudioFilterCoefficients, feedforward1) == 0x14);
static_assert(offsetof(AudioFilterCoefficients, feedforward2) == 0x18);

struct AudioFilterState
{
    float z0; // 0x00
    float z1; // 0x04
};

static_assert(offsetof(AudioFilterState, z0) == 0x00);
static_assert(offsetof(AudioFilterState, z1) == 0x04);

void __cdecl Audio_ProcessFilter4Channel(
    float* output,
    float* input,
    int* statePointers,
    int* coefficientPointers,
    int sampleCount)
{
    auto* const coeff0 = reinterpret_cast<AudioFilterCoefficients*>(coefficientPointers[0]);
    const float c0_feedback0 = coeff0->feedback0;
    const float c0_feedback1 = coeff0->feedback1;
    const float c0_bias = _DAT_0122ded8 - (coeff0->feedback1 + coeff0->feedback0);
    const float c0_feedforward0 = coeff0->feedforward0;
    const float c0_feedforward1 = coeff0->feedforward1;
    const float c0_feedforward2 = coeff0->feedforward2;
    const float c0_negSum = -(coeff0->feedforward0 + coeff0->feedforward1 + coeff0->feedforward2);
    const float c0_outputScale = coeff0->outputScale;

    auto* const coeff1 = reinterpret_cast<AudioFilterCoefficients*>(coefficientPointers[1]);
    const float c1_feedback0 = coeff1->feedback0;
    const float c1_feedback1 = coeff1->feedback1;
    const float c1_bias = _DAT_0122ded8 - (coeff1->feedback0 + coeff1->feedback1);
    const float c1_feedforward0 = coeff1->feedforward0;
    const float c1_feedforward1 = coeff1->feedforward1;
    const float c1_feedforward2 = coeff1->feedforward2;
    const float c1_negSum = -(coeff1->feedforward0 + coeff1->feedforward1 + coeff1->feedforward2);
    const float c1_outputScale = coeff1->outputScale;

    auto* const coeff2 = reinterpret_cast<AudioFilterCoefficients*>(coefficientPointers[2]);
    const float c2_feedback0 = coeff2->feedback0;
    const float c2_feedback1 = coeff2->feedback1;
    const float c2_bias = _DAT_0122ded8 - (coeff2->feedback1 + coeff2->feedback0);
    const float c2_feedforward0 = coeff2->feedforward0;
    const float c2_feedforward1 = coeff2->feedforward1;
    const float c2_feedforward2 = coeff2->feedforward2;
    const float c2_negSum = -(coeff2->feedforward1 + coeff2->feedforward2 + coeff2->feedforward0);
    const float c2_outputScale = coeff2->outputScale;

    auto* const coeff3 = reinterpret_cast<AudioFilterCoefficients*>(coefficientPointers[3]);
    const float c3_feedback0 = coeff3->feedback0;
    const float c3_feedback1 = coeff3->feedback1;
    const float c3_bias = _DAT_0122ded8 - (coeff3->feedback0 + coeff3->feedback1);
    const float c3_feedforward0 = coeff3->feedforward0;
    const float c3_feedforward1 = coeff3->feedforward1;
    const float c3_feedforward2 = coeff3->feedforward2;
    const float c3_negSum = -(coeff3->feedforward2 + coeff3->feedforward0 + coeff3->feedforward1);
    const float c3_outputScale = coeff3->outputScale;

    auto* const state0 = reinterpret_cast<AudioFilterState*>(statePointers[0]);
    auto* const state1 = reinterpret_cast<AudioFilterState*>(statePointers[1]);
    auto* const state2 = reinterpret_cast<AudioFilterState*>(statePointers[2]);
    auto* const state3 = reinterpret_cast<AudioFilterState*>(statePointers[3]);

    float y0 = state0->z0;
    float y1 = state1->z0;
    float y2 = state2->z0;
    float y3 = state3->z0;
    float delay0 = state0->z1;
    float delay1 = state1->z1;
    float delay2 = state2->z1;
    float delay3 = state3->z1;

    float prev0;
    float prev1;
    float prev2;
    float prev3;

    do
    {
        prev3 = y3;
        prev2 = y2;
        prev1 = y1;
        prev0 = y0;

        const float in0 = *input;
        float* const input1 = input + 0x80;
        float* const input2 = input + 0x100;
        float* const input3 = input + 0x180;
        input = input + 1;

        y0 = in0 + c0_bias + c0_feedback1 * delay0 + c0_feedback0 * prev0;
        y1 = *input1 + c1_bias + c1_feedback1 * delay1 + c1_feedback0 * prev1;
        y2 = *input2 + c2_bias + c2_feedback1 * delay2 + c2_feedback0 * prev2;
        y3 = *input3 + c3_bias + c3_feedback1 * delay3 + c3_feedback0 * prev3;

        *output =
            (c0_feedforward0 * y0 + c0_feedforward2 * delay0 + c0_feedforward1 * prev0 + c0_negSum) *
            c0_outputScale;
        output[0x80] =
            (c1_feedforward0 * y1 + c1_feedforward2 * delay1 + c1_feedforward1 * prev1 + c1_negSum) *
            c1_outputScale;
        output[0x100] =
            (c2_feedforward0 * y2 + c2_feedforward2 * delay2 + c2_feedforward1 * prev2 + c2_negSum) *
            c2_outputScale;
        output[0x180] =
            (c3_feedforward0 * y3 + c3_feedforward2 * delay3 + c3_feedforward1 * prev3 + c3_negSum) *
            c3_outputScale;

        output = output + 1;
        sampleCount = sampleCount + -1;

        delay0 = prev0;
        delay1 = prev1;
        delay2 = prev2;
        delay3 = prev3;
    } while (sampleCount != 0);

    state0->z0 = y0;
    state0->z1 = prev0;
    state1->z0 = y1;
    state1->z1 = prev1;
    state2->z0 = y2;
    state2->z1 = prev2;
    state3->z0 = y3;
    state3->z1 = prev3;
}