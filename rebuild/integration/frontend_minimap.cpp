/* frontend_minimap.cpp — see frontend_minimap.h. Runtime container read, no baking. */
#include "frontend_minimap.h"
#include "fable_bank_reader.h"
#include "fable_texture_decode.h"

#include <string.h>

namespace
{
    // Cached bank: keyed by path so a changed bank re-opens; the ring reloads on
    // every selection so caching the open+parse is the whole win.
    fable_bank::Bank g_MinimapBank;
    bool             g_MinimapBankOpen = false;
    char             g_MinimapBankPath[520] = {};

    const char* const kSubBank = "GBANK_FRONT_END_PC";
    const char* const kSuffix  = "_FRONT_END";

    bool EnsureBank(const char* path)
    {
        if (g_MinimapBankOpen && strcmp(g_MinimapBankPath, path) == 0)
            return true;
        if (g_MinimapBankOpen)
        {
            fable_bank::Close(&g_MinimapBank);
            g_MinimapBankOpen = false;
        }
        if (!fable_bank::Open(&g_MinimapBank, path))
            return false;
        g_MinimapBankOpen = true;
        strncpy(g_MinimapBankPath, path, sizeof(g_MinimapBankPath) - 1);
        g_MinimapBankPath[sizeof(g_MinimapBankPath) - 1] = 0;
        return true;
    }
}

extern "C" int FableLoadRegionMinimap(
    const char* bankPath, const char* graphicName,
    unsigned char* outRGBA, unsigned int outCap,
    unsigned int* outW, unsigned int* outH)
{
    char asset[128];
    unsigned int glen, slen, size = 0, infoLen = 0;
    const unsigned char* pay; const unsigned char* info = 0;

    if (bankPath == 0 || graphicName == 0 || outRGBA == 0)
        return -1;
    if (!EnsureBank(bankPath))
        return -2;

    // Build the asset name: HEADER "MINIMAP_<REGION>" -> "MINIMAP_<REGION>_FRONT_END".
    glen = (unsigned int)strlen(graphicName);
    slen = (unsigned int)strlen(kSuffix);
    if (glen == 0 || glen + 1 > sizeof(asset)) return -3;
    strncpy(asset, graphicName, sizeof(asset) - 1);
    asset[sizeof(asset) - 1] = 0;
    if (glen < slen || memcmp(asset + glen - slen, kSuffix, slen) != 0)
    {
        if (glen + slen + 1 > sizeof(asset)) return -3;
        memcpy(asset + glen, kSuffix, slen + 1);
    }

    pay = fable_bank::FindEntry(&g_MinimapBank, kSubBank, asset, &size, &info, &infoLen);
    if (pay == 0 || info == 0 || infoLen < 34)
        return -4;

    return FableDecodeBankTexture(info, pay, size, outRGBA, outCap, outW, outH);
}

extern "C" void FableReleaseRegionMinimapBank(void)
{
    if (g_MinimapBankOpen)
    {
        fable_bank::Close(&g_MinimapBank);
        g_MinimapBankOpen = false;
        g_MinimapBankPath[0] = 0;
    }
}
