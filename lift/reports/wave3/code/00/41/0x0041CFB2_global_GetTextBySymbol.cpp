#include <cstddef>
#include <new>

struct DataBankAt14Overlay {
    std::byte pad[0x14];
    NGameText::CDataBank* dataBankAt14;
};
static_assert(offsetof(DataBankAt14Overlay, dataBankAt14) == 0x14);

struct DataBankAt60Overlay {
    std::byte pad[0x60];
    NGameText::CDataBank* dataBankAt60;
};
static_assert(offsetof(DataBankAt60Overlay, dataBankAt60) == 0x60);

namespace NGameText {
class CDataBank {
public:
    void GetTextBySymbol(void* outText, CCharString& symbol);
};
}

extern DataBankAt14Overlay* DAT_013b86a0;
extern DataBankAt60Overlay* DAT_013b871c;
extern const char DAT_013bca24[];

CWideString __fastcall GetTextBySymbol(CCharString& symbol)
{
    alignas(CCharString) std::byte local_8_storage[sizeof(CCharString)];

    if (DAT_013b86a0 != nullptr) {
        DAT_013b86a0->dataBankAt14->GetTextBySymbol(local_8_storage, symbol);

        CCharString* const localText =
            std::launder(reinterpret_cast<CCharString*>(local_8_storage));
        if (*localText != DAT_013bca24) {
            CWideString result(*localText);
            localText->~CCharString();
            return result;
        }

        localText->~CCharString();
    }

    if (DAT_013b871c != nullptr) {
        DAT_013b871c->dataBankAt60->GetTextBySymbol(local_8_storage, symbol);

        CCharString* const localText =
            std::launder(reinterpret_cast<CCharString*>(local_8_storage));
        if (*localText != DAT_013bca24) {
            CWideString result(*localText);
            localText->~CCharString();
            return result;
        }

        localText->~CCharString();
    }

    return CWideString(symbol);
}