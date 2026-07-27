#include <malloc.h>

void __cdecl DSP_ComputeRunningMinEnvelope(float* samples, int window, int sampleCount)
{
    auto* indices = static_cast<int*>(_alloca(static_cast<size_t>(sampleCount) * sizeof(int)));
    auto* values = static_cast<float*>(_alloca(static_cast<size_t>(sampleCount) * sizeof(float)));

    int writePos = 0;
    int candidateCount = 0;
    int i = 0;

    if (0 < sampleCount) {
        do {
            if (candidateCount < 2) {
                indices[candidateCount] = i;
            } else {
                const float sample = samples[i];
                float candidateValue = values[candidateCount - 1];

                if (candidateValue <= sample) {
                    int scan = candidateCount - 2;

                    do {
                        if ((indices[scan + 1] + window <= i) ||
                            (candidateCount < 2) ||
                            ((candidateValue < values[scan]) == (candidateValue == values[scan])) ||
                            (indices[scan] + window <= i)) {
                            indices[candidateCount] = i;
                            goto store_value;
                        }

                        candidateValue = values[scan];
                        --candidateCount;
                        --scan;
                    } while (candidateValue <= sample);
                }

                indices[candidateCount] = i;
            }

        store_value:
            values[candidateCount] = samples[i];
            ++candidateCount;
            ++i;
        } while (i < sampleCount);
    }

    if (0 < candidateCount) {
        int segment = 0;

        do {
            int end;
            if ((candidateCount - 1 <= segment) || (values[segment + 1] <= values[segment])) {
                end = indices[segment] + 1 + window;
            } else {
                end = indices[segment + 1];
            }

            if (sampleCount < end) {
                end = sampleCount;
            }

            for (; writePos < end; ++writePos) {
                samples[writePos] = values[segment];
            }

            ++segment;
        } while (segment < candidateCount);
    }
}