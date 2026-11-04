#include "fable_language.h"
#include "fable_retail_banks.h"

extern unsigned long g_FableLanguage_013B8684;
extern CBankFileManager g_FableBankFileManager_013CA79C;

namespace NLocalisation
{
    unsigned long FABLE_FASTCALL GetLanguage()
    {
        if (g_FableLanguage_013B8684 != 0)
            return g_FableLanguage_013B8684;

        bool hasENGLISH;
        {
            CCharString bank("TEXT_ENGLISH_MAIN", -1);
            hasENGLISH = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasENGLISH)
            return g_FableLanguage_013B8684 = 9;
        bool hasFRENCH;
        {
            CCharString bank("TEXT_FRENCH_MAIN", -1);
            hasFRENCH = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasFRENCH)
            return g_FableLanguage_013B8684 = 12;
        bool hasJAPANESE;
        {
            CCharString bank("TEXT_JAPANESE_MAIN", -1);
            hasJAPANESE = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasJAPANESE)
            return g_FableLanguage_013B8684 = 17;
        bool hasGERMAN;
        {
            CCharString bank("TEXT_GERMAN_MAIN", -1);
            hasGERMAN = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasGERMAN)
            return g_FableLanguage_013B8684 = 7;
        bool hasKOREAN;
        {
            CCharString bank("TEXT_KOREAN_MAIN", -1);
            hasKOREAN = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasKOREAN)
            return g_FableLanguage_013B8684 = 18;
        bool hasCHINESE;
        {
            CCharString bank("TEXT_CHINESE_MAIN", -1);
            hasCHINESE = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasCHINESE)
            return g_FableLanguage_013B8684 = 4;
        bool hasSPANISH;
        {
            CCharString bank("TEXT_SPANISH_MAIN", -1);
            hasSPANISH = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasSPANISH)
            return g_FableLanguage_013B8684 = 10;
        bool hasITALIAN;
        {
            CCharString bank("TEXT_ITALIAN_MAIN", -1);
            hasITALIAN = g_FableBankFileManager_013CA79C.BankExists(bank);
        }
        if (hasITALIAN)
            return g_FableLanguage_013B8684 = 16;
        return g_FableLanguage_013B8684;
    }
}
