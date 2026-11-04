#define Z_PREFIX
#include "third_party/zlib121/zlib.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char output[8192];
    static unsigned char input[4096];
    const unsigned sizes[] = {0, 1, 16, 127, 1024, 4096};
    for (unsigned case_index = 0; case_index < 6; ++case_index)
    {
        for (unsigned i = 0; i < sizes[case_index]; ++i)
            input[i] = (unsigned char)((i * 19 + case_index * 37) & 255);
        z_stream stream;
        memset(&stream, 0, sizeof(stream));
        int result = deflateInit_(&stream, 6, ZLIB_VERSION, sizeof(stream));
        if (result != Z_OK) return 10 + case_index;
        stream.next_in = input;
        stream.avail_in = sizes[case_index];
        stream.next_out = output;
        stream.avail_out = sizeof(output);
        do { result = deflate(&stream, Z_FINISH); }
        while (result == Z_OK);
        if (result != Z_STREAM_END) return 20 + case_index;
        printf("%u %lu ", sizes[case_index], stream.total_out);
        for (unsigned i = 0; i < stream.total_out; ++i) printf("%02x", output[i]);
        puts("");
        if (deflateEnd(&stream) != Z_OK) return 30 + case_index;
    }
    for (unsigned i = 0; i < 512; ++i)
        input[i] = (unsigned char)((i * 19 + 6 * 37) & 255);
    uLongf compressed = sizeof(output);
    if (compress2(output, &compressed, input, 512, 6) != Z_OK) return 40;
    printf("512 %lu ", compressed);
    for (unsigned i = 0; i < compressed; ++i) printf("%02x", output[i]);
    puts("");
    return 0;
}
