#include "fable_crc.h"
#include "fable_gfmain.h"
#include "fable_game_text_bank.h"
#include "fable_window_title.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"

// Stand-ins for the text-bank members GetWindowTitle (0x004023F0) reaches.
// The linked NGameText::CDataBank ctor (0x009C85A0) is the reconstructed one;
// the bank itself is never opened here, so the TEXT_GUI_WINDOW_TITLE lookup
// misses and the title stays the built-in "Fable - The Lost Chapters".

// The shared empty narrow text (0x0129AAF4) CCharString hands out for null data.
char g_FableEmptyNarrowString_0129AAF4[1] = {0};

// The CRC-32 table FableCRC_Calc_004014A0 reads (retail static data at
// 0x0129A168: the reflected table for polynomial 0xEDB88320), built at startup.
fable_u32 g_FableCRCTable_0129A168[256];

namespace
{
    GFMainPhase7State* g_ActivePhase7State;

    struct CRCTableBuilder
    {
        CRCTableBuilder()
        {
            for (fable_u32 index = 0; index != 256; ++index)
            {
                fable_u32 value = index;
                for (fable_u32 bit = 0; bit != 8; ++bit)
                {
                    value = (value & 1)
                        ? ((value >> 1) ^ 0xEDB88320)
                        : (value >> 1);
                }
                g_FableCRCTable_0129A168[index] = value;
            }
        }
    };

    CRCTableBuilder g_CRCTableBuilder;
}

// CIVCountedPointeeBase slot 1 deletes the pointee once its last reference
// goes (its ctor/dtor are inline).
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const
{
    delete const_cast<CIVCountedPointeeBase*>(this);
}

// CResourceList's lifetime is the reconstructed one (ctor 0x009FC570, dtor
// 0x009FC5C0, HandleFailedAllocation 0x009FC540). The bank never loads a
// resource here, so the list's deeper members stay stand-ins.
#include "../src/compiled/00/9f/CResourceList_CResourceList_009fc570.cpp"
#include "../src/compiled/00/9f/CResourceList_Destructor_009fc5c0.cpp"
#include "../src/compiled/00/9f/CResourceList_HandleFailedAllocation_009fc540.cpp"

void CResourceList::RemoveResource(CResource&)
{
}

bool CResourceList::UnloadNextResource(unsigned long)
{
    return false;
}

// CResourceBank / CBankFile lifetime (the CDataBank bases).

// CResourceList::SetMaximumMemory: the stand-in list holds nothing.
void CResourceList::SetMaximumMemory(long maximumMemory)
{
    MaximumMemory = maximumMemory;
}

CResourceBank::CResourceBank()
{
}

CResourceBank::~CResourceBank()
{
}

CBankFile::CBankFile()
{
}

// The text bank's unopened stand-in still constructs CBankFile's packed
// members, so keep this constructor on the recovered catalog implementation.
#include "../src/compiled/00/a6/CPackedUIntArray_CPackedUIntArray_00a629c0.cpp"
#include "test_support/bank_file_cleanup_stubs.h"

CBankFile::~CBankFile()
{
}

template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list()
{
}

CBankFileAsync::CBankFileAsync()
{
}

CBankFileAsync::~CBankFileAsync()
{
}

// CBankFile::OpenReadOnly (vtable slot 1): records the open, reads nothing.
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long)
{
    if (g_ActivePhase7State != 0)
        ++g_ActivePhase7State->exactWindowTitleBankOpenCalls;
    return true;
}

// CBankFile::FindIndexBySymbolCRC @ 0x009CC410: the unopened bank has no
// symbols.
unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long) const
{
    if (g_ActivePhase7State != 0)
        ++g_ActivePhase7State->exactWindowTitleLookupCalls;
    return 0;
}

// NGameText::CDataBank::~CDataBank (reached through the deleting destructor).
NGameText::CDataBank::~CDataBank()
{
    if (g_ActivePhase7State != 0)
        ++g_ActivePhase7State->exactWindowTitleBankDeleteCalls;
}

// Text-entry lookup boundary (not reached: no index).
CIVCountedPointer<const NGameText::CTextEntry>
NGameText::CDataBank::GetPTextEntry(unsigned long, bool) const
{
    return CIVCountedPointer<const NGameText::CTextEntry>();
}

// NGameText::CTextEntry::GetText @ 0x009C7A40 (not reached).
CWideString NGameText::CTextEntry::GetText() const
{
    return CWideString();
}

CWideString FABLE_FASTCALL FableRunExactWindowTitleBoundary(
    GFMainPhase7State& state)
{
    ++state.exactWindowTitleCoordinatorCalls;
    g_ActivePhase7State = &state;
    CWideString title = GetWindowTitle();
    g_ActivePhase7State = 0;
    return title;
}

// Unopened bank boundary for this isolated fixture.
void NGameText::CDataBank::Close() { CBankFile::Close(); }
