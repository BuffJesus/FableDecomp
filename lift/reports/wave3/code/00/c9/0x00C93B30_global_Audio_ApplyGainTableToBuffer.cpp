extern float DAT_013a3948[];

void __fastcall Audio_ApplyGainTableToBuffer(
    int* sampleCount,
    const float* inputBuffer,
    const int* gainIndexBuffer,
    float* outputBuffer,
    int maxSamples)
{
    int totalSamples = *sampleCount;
    if (totalSamples < maxSamples) {
        maxSamples = totalSamples;
    }

    int processed = 0;

    if (maxSamples > 3) {
        const float* in = inputBuffer + 1;
        float* out = outputBuffer + 2;

        do {
            const int base = processed * 4;
            processed += 4;

            out[-2] = DAT_013a3948[gainIndexBuffer[base + 0]] * in[-1];
            out[-1] = DAT_013a3948[gainIndexBuffer[base + 1]] * in[0];
            out[0] = DAT_013a3948[gainIndexBuffer[base + 2]] * in[1];
            out[1] = DAT_013a3948[gainIndexBuffer[base + 3]] * in[2];

            in += 4;
            out += 4;
        } while (processed < maxSamples - 3);
    }

    if (processed < maxSamples) {
        int remaining = maxSamples - processed;
        const float* in = inputBuffer + processed;
        const int* gain = gainIndexBuffer + processed;
        float* out = outputBuffer + processed;

        processed += remaining;
        do {
            remaining -= 1;
            *out = DAT_013a3948[*gain] * *in;
            ++in;
            ++gain;
            ++out;
        } while (remaining != 0);
    }

    if (processed < totalSamples) {
        float* out = outputBuffer + processed;
        for (int tailCount = totalSamples - processed; tailCount != 0; --tailCount) {
            *out = 0.0f;
            ++out;
        }
    }
}