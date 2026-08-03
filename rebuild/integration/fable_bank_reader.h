#ifndef FABLE_BANK_READER_H
#define FABLE_BANK_READER_H

// Runtime BIGB (.big) bank reader for the reconstruction -- the first piece of
// loading frontend assets from the game data banks at RUNTIME instead of baking
// them at build time. Ported byte-for-byte from tools/parse_bigb.py and proven
// against the real data\lang\English\fonts.big (M0): the C++ TOC/footer/glyph
// parse matches the verified Python parse exactly.
//
// Header-only + freestanding on <stdio.h>/<string.h>/<stdlib.h> so it can be
// unit-tested standalone today; the runtime integration swaps the fopen/fread
// load for the reconstruction's narrow-Win32 ReadFile path (M1), leaving the
// baked-resource path as fallback behind FABLE_RUNTIME_BANKS.
//
// BIGB layout (per tools/parse_bigb.py / docs/TEXTBIG_FORMAT.md):
//   header: "BIGB", u32 version, u32 footerOffset, u32 footerSize
//   footer(@footerOffset): u32 subBankCount, then per sub-bank:
//       cstr name, u32 version, u32 entryCount, u32 tocOffset, u32 tocSize, u32 align
//   TOC(@tocOffset): optional stats header (u32 count<1000, then count*2 u32),
//       then entryCount * { u32 magic(=42), u32 id, i32 type, u32 size,
//       u32 offset, u32 crc, lpstr name, u32 timestamp, u32 depCount,
//       depCount*lpstr, u32 infoSize, u8[infoSize] info }
//   Sub-bank TOCs are contiguous: each ends where the next begins; the last
//   ends exactly at footerOffset.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fable_bank
{
    struct Cursor
    {
        const unsigned char* b;
        unsigned int size;
        unsigned int p;
        unsigned int u32v() { unsigned int v; memcpy(&v, b + p, 4); p += 4; return v; }
        // u32-length-prefixed string; returns pointer + length with trailing NUL trimmed.
        const unsigned char* lpstr(unsigned int* outLen)
        {
            unsigned int n = u32v();
            const unsigned char* s = b + p;
            p += n;
            while (n > 0 && s[n - 1] == 0) --n;
            *outLen = n;
            return s;
        }
        const char* cstr()
        {
            const char* s = (const char*)(b + p);
            while (p < size && b[p] != 0) ++p;
            ++p;
            return s;
        }
    };

    struct SubBank { char name[64]; unsigned int version, entryCount, tocOffset, tocSize, align; };

    struct Bank
    {
        unsigned char* buf;
        unsigned int   size;
        unsigned int   footerOffset;
        SubBank        subs[32];
        unsigned int   subCount;
    };

    // Load + parse the BIGB header/footer. Returns false on I/O error or non-BIGB.
    inline bool Open(Bank* bank, const char* path)
    {
        memset(bank, 0, sizeof(*bank));
        FILE* f = fopen(path, "rb");
        if (!f) return false;
        fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
        if (n <= 16) { fclose(f); return false; }
        bank->buf = (unsigned char*)malloc(n);
        if (!bank->buf) { fclose(f); return false; }
        bool readOk = fread(bank->buf, 1, n, f) == (size_t)n;
        fclose(f);
        if (!readOk) { free(bank->buf); bank->buf = 0; return false; }
        bank->size = (unsigned int)n;
        if (memcmp(bank->buf, "BIGB", 4) != 0) { free(bank->buf); bank->buf = 0; return false; }
        memcpy(&bank->footerOffset, bank->buf + 8, 4);
        Cursor r; r.b = bank->buf; r.size = bank->size; r.p = bank->footerOffset;
        bank->subCount = r.u32v();
        if (bank->subCount > 32) { free(bank->buf); bank->buf = 0; return false; }
        for (unsigned int i = 0; i < bank->subCount; ++i)
        {
            SubBank& s = bank->subs[i];
            strncpy(s.name, r.cstr(), 63); s.name[63] = 0;
            s.version = r.u32v(); s.entryCount = r.u32v();
            s.tocOffset = r.u32v(); s.tocSize = r.u32v(); s.align = r.u32v();
        }
        return true;
    }

    inline void Close(Bank* bank)
    {
        if (bank->buf) { free(bank->buf); bank->buf = 0; }
    }

    inline int FindSubBank(const Bank* bank, const char* name)
    {
        for (unsigned int i = 0; i < bank->subCount; ++i)
            if (strcmp(bank->subs[i].name, name) == 0) return (int)i;
        return -1;
    }

    // Walk a sub-bank's TOC. If wantName is non-null and matches an entry, its
    // payload offset/size are written to *outOffset/*outSize and *found=true.
    // Returns the byte position where the walk ended (for the contiguity check).
    inline unsigned int WalkToc(const Bank* bank, const SubBank& s,
                                const char* wantName,
                                unsigned int* outOffset, unsigned int* outSize,
                                bool* found)
    {
        Cursor r; r.b = bank->buf; r.size = bank->size; r.p = s.tocOffset;
        unsigned int save = r.p;
        unsigned int statsCount = r.u32v();
        if (statsCount < 1000) { for (unsigned int k = 0; k < statsCount; ++k) { r.u32v(); r.u32v(); } }
        else r.p = save;
        if (found) *found = false;
        for (unsigned int i = 0; i < s.entryCount; ++i)
        {
            r.u32v();               // magic (=42)
            r.u32v();               // id
            r.u32v();               // type (i32)
            unsigned int size = r.u32v();
            unsigned int off = r.u32v();
            r.u32v();               // crc
            unsigned int nlen; const unsigned char* nm = r.lpstr(&nlen);
            r.u32v();               // timestamp
            unsigned int depc = r.u32v();
            for (unsigned int d = 0; d < depc; ++d) { unsigned int dl; r.lpstr(&dl); }
            unsigned int infoSize = r.u32v(); r.p += infoSize;
            if (wantName && found && !*found &&
                nlen == (unsigned int)strlen(wantName) && memcmp(nm, wantName, nlen) == 0)
            {
                *found = true; if (outOffset) *outOffset = off; if (outSize) *outSize = size;
            }
        }
        return r.p;
    }

    // Convenience: locate an entry's payload by sub-bank + entry name.
    inline const unsigned char* FindEntry(const Bank* bank, const char* subBank,
                                          const char* name, unsigned int* outSize)
    {
        int si = FindSubBank(bank, subBank);
        if (si < 0) return 0;
        unsigned int off = 0, size = 0; bool found = false;
        WalkToc(bank, bank->subs[si], name, &off, &size, &found);
        if (!found) return 0;
        if (outSize) *outSize = size;
        return bank->buf + off;
    }

    // Verify the sub-bank TOCs are contiguous and the last ends at the footer.
    inline bool VerifyContiguous(const Bank* bank)
    {
        for (unsigned int i = 0; i < bank->subCount; ++i)
        {
            unsigned int end = WalkToc(bank, bank->subs[i], 0, 0, 0, 0);
            unsigned int expected = (i + 1 < bank->subCount)
                ? bank->subs[i + 1].tocOffset : bank->footerOffset;
            if (end != expected) return false;
        }
        return true;
    }
}

#endif
