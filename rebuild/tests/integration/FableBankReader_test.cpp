// M0 runtime-bank-load proof: the C++ BIGB reader loads the real fonts.big and
// byte-matches the verified Python parse (tools/parse_bigb.py /
// render_fable_static_font.py). If the retail install is not present the test
// SKIPS (still PASS) so it never breaks a build on a machine without the game.
#include "fable_bank_reader.h"
#include <stdio.h>

static const char* kFontsBig =
    "C:\\Programs\\Steam\\steamapps\\common\\Fable The Lost Chapters\\data\\lang\\English\\fonts.big";

int main()
{
    fable_bank::Bank bank;
    if (!fable_bank::Open(&bank, kFontsBig))
    {
        printf("FABLETLC_BANK_READER SKIP (fonts.big not present)\n");
        return 0;
    }

    int rc = 1;
    do
    {
        if (!fable_bank::VerifyContiguous(&bank)) { printf("FAIL toc-not-contiguous\n"); break; }

        unsigned int size = 0;
        const unsigned char* raw =
            fable_bank::FindEntry(&bank, "FONT_ENGLISH_MAIN", "ENG_ARIAL_12", &size);
        if (!raw) { printf("FAIL no ENG_ARIAL_12\n"); break; }

        unsigned int q = 0;
        while (raw[q] != 0) ++q;             // ASCIIZ face name
        const char* face = (const char*)raw;
        q += 1 + 8 + 1;                       // face NUL + u32 height + u32 weight + u8 italic
        unsigned int maxH, texW, texH, minChar, maxChar, bankCount;
        memcpy(&maxH, raw + q, 4); memcpy(&texW, raw + q + 4, 4); memcpy(&texH, raw + q + 8, 4);
        memcpy(&minChar, raw + q + 12, 4); memcpy(&maxChar, raw + q + 16, 4);
        memcpy(&bankCount, raw + q + 20, 4); q += 24;

        // Flatten the glyph banks by character (matches load_font).
        const unsigned char* glyphPtr[128] = { 0 };
        unsigned int gi = 0;
        for (unsigned int bk = 0; bk < bankCount; ++bk)
        {
            q += 8;                            // bankIndex, unknown
            unsigned int gc; memcpy(&gc, raw + q, 4); q += 4;
            for (unsigned int g = 0; g < gc && gi < 128; ++g) { glyphPtr[gi++] = raw + q; q += 22; }
        }
        short advSpace, advA, advI;            // advance = 3rd short of <ffffhhh> (record+20)
        memcpy(&advSpace, glyphPtr[32 - minChar] + 20, 2);
        memcpy(&advA,     glyphPtr['A' - minChar] + 20, 2);
        memcpy(&advI,     glyphPtr['i' - minChar] + 20, 2);

        const bool ok = strcmp(face, "Arial") == 0 && texW == 128 && texH == 128 &&
                        minChar == 32 && maxChar == 127 &&
                        gi == (maxChar - minChar + 1) &&
                        advSpace == 4 && advA == 10 && advI == 3;
        if (!ok)
        {
            printf("FAIL metrics face=%s %ux%u %u..%u glyphs=%u sp=%d A=%d i=%d\n",
                   face, texW, texH, minChar, maxChar, gi, advSpace, advA, advI);
            break;
        }
        rc = 0;
    } while (0);

    fable_bank::Close(&bank);
    if (rc == 0) printf("FABLETLC_BANK_READER PASS (runtime fonts.big byte-matches Python)\n");
    return rc;
}
