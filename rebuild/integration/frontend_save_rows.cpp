/* frontend_save_rows.cpp — native port of tools/save_metadata.py.
 *
 * Reads Profile.bin + .sav HEADERs and feeds live rows to the frontend Load Game
 * list. No STL, Win32 file APIs only, hand-rolled parsing per the project's
 * integration style (see visual_boot_checkpoint.cpp). crc0 is implemented inline
 * (proven byte-identical to the compiled FableCRC_Calc_004014A0) to avoid a
 * duplicate-symbol link with the visual runtime. See docs/SAVE_ROW_FEEDER_SPEC.md.
 */
#include "frontend_save_rows.h"
#include "fable_inflate.h"
#include "fable_visual_d3d9.h"

#include <string.h>
#include <stdlib.h>

/* Minimal Win32 file surface, hand-declared (the visual build links no windows.h,
 * matching the __declspec(dllimport)/FABLE_STDCALL style in visual_boot_checkpoint.cpp). */
typedef void*         FableHandle;
typedef unsigned long FableDword32;
typedef int           FableBool32;
#define FABLE_GENERIC_READ           0x80000000u
#define FABLE_FILE_SHARE_READ        0x00000001u
#define FABLE_OPEN_EXISTING          3
#define FABLE_FILE_ATTRIBUTE_NORMAL  0x00000080u
#define FABLE_INVALID_HANDLE_VALUE   ((FableHandle)(-1))
#define FABLE_INVALID_FILE_SIZE      0xFFFFFFFFu
#define FABLE_INVALID_FILE_ATTRS     0xFFFFFFFFu
#define FABLE_ATTR_DIRECTORY         0x00000010u
extern "C" {
    __declspec(dllimport) FableHandle FABLE_STDCALL CreateFileW(
        const wchar_t* name, FableDword32 access, FableDword32 share,
        void* sec, FableDword32 disp, FableDword32 flags, FableHandle tmpl);
    __declspec(dllimport) FableBool32 FABLE_STDCALL ReadFile(
        FableHandle h, void* buf, FableDword32 n, FableDword32* got, void* ov);
    __declspec(dllimport) FableDword32 FABLE_STDCALL GetFileSize(FableHandle h, FableDword32* hi);
    __declspec(dllimport) FableBool32 FABLE_STDCALL CloseHandle(FableHandle h);
    __declspec(dllimport) FableDword32 FABLE_STDCALL GetFileAttributesW(const wchar_t* name);
}

/* ---- crc0: reflected poly 0xEDB88320, caller seed, NO final inversion ---- */
static fable_u32 g_crc0_table[256];
static int       g_crc0_ready = 0;

static void crc0_init(void)
{
    fable_u32 n, k, c;
    for (n = 0; n < 256; ++n) {
        c = n;
        for (k = 0; k < 8; ++k)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        g_crc0_table[n] = c;
    }
    g_crc0_ready = 1;
}

static fable_u32 crc0(fable_u32 seed, const void* data, fable_u32 len)
{
    const unsigned char* p = (const unsigned char*)data;
    fable_u32 c = seed, i;
    if (!g_crc0_ready) crc0_init();
    for (i = 0; i < len; ++i)
        c = g_crc0_table[(c ^ p[i]) & 0xFF] ^ (c >> 8);
    return c;
}

/* ---- little-endian readers ---- */
static fable_u32 rd_u32(const unsigned char* p) {
    return (fable_u32)p[0] | ((fable_u32)p[1] << 8) |
           ((fable_u32)p[2] << 16) | ((fable_u32)p[3] << 24);
}
static fable_u16 rd_u16(const unsigned char* p) {
    return (fable_u16)(p[0] | (p[1] << 8));
}

/* ---- HEADER 23-field schema (kinds mirror save_metadata.py) ---- */
enum { K_ASCII, K_U32, K_BOOL, K_WIDE, K_FLOAT, K_FLOAT3 };
struct HField { const char* name; int kind; };
static const struct HField HEADER_SCHEMA[23] = {
    { "WorldName", K_ASCII }, { "WorldFrame", K_U32 },
    { "TeleportingEnabled", K_BOOL }, { "SavingEnabled", K_BOOL },
    { "ExperienceSpendingEnabled", K_BOOL }, { "CreatureGenerationEnabled", K_BOOL },
    { "CreatureGenerationDisabledGroups", K_U32 }, { "HeroSleepingEnabled", K_BOOL },
    { "MapTableShowQuestCardsOnUsed", K_BOOL }, { "MiniMapEnabled", K_BOOL },
    { "MiniMapActiveBeforeDisabled", K_BOOL }, { "GuildMasterMessagesEnabled", K_BOOL },
    { "SummonerDeathExplosionAffectsHero", K_BOOL }, { "MostRecentSaveType", K_U32 },
    { "MostRecentSaveTypeBeforeManualSave", K_U32 }, { "MostRecentManualSaveName", K_WIDE },
    { "SaveGameMarkerPos", K_FLOAT3 }, { "SaveGameMarkerAngleXY", K_FLOAT },
    { "GuildSealRecallPos", K_FLOAT3 }, { "GuildSealRecallAngleXY", K_FLOAT },
    { "CurrentRegionName", K_ASCII }, { "CurrentRegionMinimapGraphicName", K_ASCII },
    { "TotalTimePlayed", K_FLOAT }
};

/* Decode + validate the inflated HEADER chunk. Returns 0 if valid. */
static int decode_header(const unsigned char* d, unsigned int n)
{
    unsigned int off, end, seclen, i;
    /* magic "HEADER\0" */
    if (n < 7 || memcmp(d, "HEADER", 6) != 0 || d[6] != 0) return -1;
    if (7 + 4 > n) return -2;
    seclen = rd_u32(d + 7);
    off = 11;
    end = off + seclen;
    if (end > n) return -3;

    for (i = 0; i < 23; ++i) {
        fable_u32 tag, want;
        if (off + 4 > end) return -10;
        tag = rd_u32(d + off); off += 4;
        want = crc0(0, HEADER_SCHEMA[i].name, (fable_u32)strlen(HEADER_SCHEMA[i].name));
        if (tag != want) return -11;
        switch (HEADER_SCHEMA[i].kind) {
        case K_ASCII: {
            unsigned int s = off;
            while (off < end && d[off] != 0) ++off;
            if (off >= end) return -12;   /* unterminated */
            ++off;                        /* consume NUL */
            (void)s;
            break;
        }
        case K_WIDE: {
            while (off + 2 <= end && rd_u16(d + off) != 0) off += 2;
            if (off + 2 > end) return -13;
            off += 2;                     /* consume 0x0000 */
            break;
        }
        case K_U32:    if (off + 4 > end) return -14; off += 4;  break;
        case K_BOOL:   if (off + 1 > end) return -15; off += 1;  break;
        case K_FLOAT:  if (off + 4 > end) return -16; off += 4;  break;
        case K_FLOAT3: if (off + 12 > end) return -17; off += 12; break;
        default: return -18;
        }
    }
    if (off != end) return -19;           /* trailing bytes -> reject */
    return 0;
}

int FableValidateSaveForTest(const unsigned char* f, unsigned int n)
{
    fable_u32 sig, c0u, c0c, c1c, trailer_pos, trailer, want;
    unsigned int hdr_cap; unsigned char* hdr; unsigned int got = 0; int rc;
    if (n < 0x20 || memcmp(f, "FableSave!", 10) != 0) return -1;
    sig = rd_u32(f + 0x0C);
    c0u = rd_u32(f + 0x10);
    /* c1u = rd_u32(f + 0x14) — not needed; trailer CRC covers it */
    c0c = rd_u32(f + 0x18);
    if (0x1Cu + c0c + 4u > n) return -2;
    c1c = rd_u32(f + 0x1C + c0c);
    trailer_pos = 0x1Cu + c0c + 4u + c1c;
    if (trailer_pos + 4u > n) return -3;
    if (sig != trailer_pos) return -4;            /* +0x0C must equal trailer offset */
    trailer = rd_u32(f + trailer_pos);
    want = crc0(0, f, trailer_pos);
    if (want != trailer) return -5;               /* corrupt file */

    /* inflate chunk0 (HEADER) and validate length + fields */
    if (c0u == 0 || c0u > 0x100000u) return -6;
    hdr_cap = c0u;
    hdr = (unsigned char*)malloc(hdr_cap);
    if (!hdr) return -7;
    rc = FableZlibInflate(f + 0x1C, c0c, hdr, hdr_cap, &got);
    if (rc != 0 || got != c0u) { free(hdr); return -8; }
    rc = decode_header(hdr, got);
    free(hdr);
    return rc == 0 ? 0 : -20;
}

/* ---- Profile.bin registry parse ---- */
/* Locate the FablePro text region and validate the container. Returns the text
 * pointer + length via out params, or 0 on a bad container. */
static const unsigned char* profile_text(const unsigned char* b, unsigned int len,
                                         unsigned int* text_len)
{
    fable_u32 tdl, trailer, want;
    if (len < 0x14 || memcmp(b, "FablePro", 8) != 0) return 0;
    tdl = rd_u32(b + 0x0C);
    if (tdl < 0x10 || tdl + 4u > len) return 0;
    trailer = rd_u32(b + tdl);
    want = crc0(0, b, tdl);
    if (want != trailer) return 0;
    *text_len = tdl - 0x10;
    return b + 0x10;
}

/* Parse `SaveGameNamesN "value";` lines from the latin1 text. */
static unsigned int parse_registry(const unsigned char* text, unsigned int len,
                                   FableSaveSlot* slots, unsigned int cap)
{
    unsigned int i = 0, count = 0;
    const char* KEY = "SaveGameNames";
    unsigned int KEYLEN = 13;
    while (i < len) {
        /* start of line */
        unsigned int ls = i;
        while (i < len && text[i] != '\n' && text[i] != '\r') ++i;
        {
            unsigned int le = i;   /* [ls,le) is one line */
            if (le - ls > KEYLEN &&
                memcmp(text + ls, KEY, KEYLEN) == 0) {
                unsigned int p = ls + KEYLEN;
                int idx = 0; int have = 0;
                while (p < le && text[p] >= '0' && text[p] <= '9') {
                    idx = idx * 10 + (text[p] - '0'); have = 1; ++p;
                }
                if (have && idx >= 1 && idx <= 50) {
                    /* find first quote */
                    while (p < le && text[p] != '"') ++p;
                    if (p < le) {
                        unsigned int vs = ++p;
                        while (p < le && text[p] != '"') ++p;
                        if (p < le) {
                            unsigned int vlen = p - vs;
                            if (vlen > 0 && count < cap && vlen < sizeof(slots[0].name)) {
                                slots[count].index = idx;
                                memcpy(slots[count].name, text + vs, vlen);
                                slots[count].name[vlen] = 0;
                                ++count;
                            }
                        }
                    }
                }
            }
        }
        /* consume line terminators */
        while (i < len && (text[i] == '\n' || text[i] == '\r')) ++i;
    }
    return count;
}

unsigned int FableParseProfileRegistryForTest(
    const unsigned char* b, unsigned int len, FableSaveSlot* slots, unsigned int cap)
{
    unsigned int tlen = 0;
    const unsigned char* text = profile_text(b, len, &tlen);
    if (!text) return 0;
    return parse_registry(text, tlen, slots, cap);
}

/* ---- small helpers ---- */
static int read_whole_file(const wchar_t* path, unsigned char** out, unsigned int* outlen)
{
    FableHandle h; FableDword32 sz, rd; unsigned char* buf;
    h = CreateFileW(path, FABLE_GENERIC_READ, FABLE_FILE_SHARE_READ, 0,
                    FABLE_OPEN_EXISTING, FABLE_FILE_ATTRIBUTE_NORMAL, 0);
    if (h == FABLE_INVALID_HANDLE_VALUE) return -1;
    sz = GetFileSize(h, 0);
    if (sz == FABLE_INVALID_FILE_SIZE || sz == 0) { CloseHandle(h); return -2; }
    buf = (unsigned char*)malloc(sz);
    if (!buf) { CloseHandle(h); return -3; }
    if (!ReadFile(h, buf, sz, &rd, 0) || rd != sz) { free(buf); CloseHandle(h); return -4; }
    CloseHandle(h);
    *out = buf; *outlen = (unsigned int)sz;
    return 0;
}

static int file_exists(const wchar_t* path)
{
    FableDword32 a = GetFileAttributesW(path);
    return (a != FABLE_INVALID_FILE_ATTRS) && !(a & FABLE_ATTR_DIRECTORY);
}

/* Build profileDir\<asciiName> as a wide path. */
static void join_wide(const wchar_t* dir, const char* name, wchar_t* out, unsigned int cap)
{
    unsigned int i = 0, j = 0;
    while (dir[i] && j + 1 < cap) out[j++] = dir[i++];
    if (j + 1 < cap) out[j++] = L'\\';
    for (i = 0; name[i] && j + 1 < cap; ++i)
        out[j++] = (wchar_t)(unsigned char)name[i];
    out[j] = 0;
}

/* Validate a save file on disk; returns the row action code. */
static fable_u32 action_for_file(const wchar_t* profileDir, const char* filename)
{
    wchar_t path[1024];
    unsigned char* buf; unsigned int len; int rc;
    join_wide(profileDir, filename, path, 1024);
    if (!file_exists(path)) return FABLE_SAVE_ACTION_INVALID;
    if (read_whole_file(path, &buf, &len) != 0) return FABLE_SAVE_ACTION_INVALID;
    rc = FableValidateSaveForTest(buf, len);
    free(buf);
    return rc == 0 ? FABLE_SAVE_ACTION_LOADABLE : FABLE_SAVE_ACTION_INVALID;
}

/* Append one row: `label` is what the frontend shows (retail-faithful "AutoSave" /
 * "Save N"); `filename` is the on-disk .sav referenced by the registry. Only rows
 * whose file is present are emitted; dedup is by on-disk filename. */
static void push_row(FableSaveRowOut* rows, unsigned int* n, unsigned int cap,
                     const char* label, const char* filename, const wchar_t* profileDir)
{
    unsigned int k;
    wchar_t path[1024];
    if (*n >= cap) return;
    join_wide(profileDir, filename, path, 1024);
    if (!file_exists(path)) return;         /* only rows whose file is present */
    for (k = 0; k < *n; ++k)                /* dedup by on-disk filename */
        if (strcmp(rows[k].filename, filename) == 0) return;
    strncpy(rows[*n].name, label, sizeof(rows[0].name) - 1);
    rows[*n].name[sizeof(rows[0].name) - 1] = 0;
    strncpy(rows[*n].filename, filename, sizeof(rows[0].filename) - 1);
    rows[*n].filename[sizeof(rows[0].filename) - 1] = 0;
    rows[*n].action = action_for_file(profileDir, filename);
    ++(*n);
}

/* Format "Save <n>" into buf without <stdio.h> in the hot path. */
static void save_label(char* buf, unsigned int cap, int index)
{
    char digits[12]; int d = 0, i, j = 0;
    const char* pfx = "Save ";
    while (pfx[j] && (unsigned)j + 1 < cap) { buf[j] = pfx[j]; ++j; }
    if (index <= 0) index = 1;
    while (index > 0 && d < 12) { digits[d++] = (char)('0' + index % 10); index /= 10; }
    for (i = d - 1; i >= 0 && (unsigned)j + 1 < cap; --i) buf[j++] = digits[i];
    buf[j] = 0;
}

unsigned int FableBuildSaveRowsForTest(
    const wchar_t* profileDir, FableSaveRowOut* rows, unsigned int cap)
{
    FableSaveSlot slots[50];
    unsigned int n = 0, nslots, i, s;
    wchar_t pbpath[1024];
    unsigned char* pb; unsigned int pblen;

    /* single AutoSave row (retail shows one; AutoSave.qs quicksave is not listed) */
    push_row(rows, &n, cap, "AutoSave", "AutoSave", profileDir);

    /* manual slots from Profile.bin, ascending registry index, labeled "Save N" */
    join_wide(profileDir, "Profile.bin", pbpath, 1024);
    if (read_whole_file(pbpath, &pb, &pblen) == 0) {
        nslots = FableParseProfileRegistryForTest(pb, pblen, slots, 50);
        free(pb);
        for (i = 0; i < nslots; ++i)        /* selection sort by registry index */
            for (s = i + 1; s < nslots; ++s)
                if (slots[s].index < slots[i].index) {
                    FableSaveSlot t = slots[i]; slots[i] = slots[s]; slots[s] = t;
                }
        for (i = 0; i < nslots && n < cap; ++i) {
            char label[32];
            save_label(label, sizeof(label), slots[i].index);
            push_row(rows, &n, cap, label, slots[i].name, profileDir);
        }
    }
    return n;
}

void FABLE_FASTCALL FableFeedVisualFrontendSaveRows(const wchar_t* profileDir)
{
    FableSaveRowOut rows[4];
    const char* names[4];
    fable_u32 actions[4];
    unsigned int n, i;

    if (profileDir == 0) { FableSetVisualFrontendSaveRows(0, 0, 0); return; }
    n = FableBuildSaveRowsForTest(profileDir, rows, 4);
    if (n == 0) { FableSetVisualFrontendSaveRows(0, 0, 0); return; }
    for (i = 0; i < n; ++i) { names[i] = rows[i].name; actions[i] = rows[i].action; }
    FableSetVisualFrontendSaveRows(names, actions, n);
}
