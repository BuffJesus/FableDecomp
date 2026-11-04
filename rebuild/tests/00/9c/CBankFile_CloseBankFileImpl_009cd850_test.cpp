#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_game_text_bank.h"

void __fastcall CloseBankFileImpl(CBankFile*);
#include "fable_small_vector.h"
// Allocation prefix consumed by the candidate. Use raw storage so no fixture
// destructor can substitute for candidate cleanup.
struct UpdatePrefix {
    unsigned long StateBlockCRC, InfoSize;
    char* InfoBuffer;
    CSmallVector<CCharString, 8> Filenames;
};
static CBankFile* bank;
static UpdatePrefix* update;
static void* infoBuffer;
static CCPPointerInfo* ownership;
static char events[64];
static unsigned count;
static void record(char c) { events[count++] = c; events[count] = 0; }
static void require(bool condition) { if (!condition) { printf("failed after %s\n", events); exit(1); } }
void operator delete[](void* p) { require(p == infoBuffer); record('I'); free(p); }
void operator delete(void* p) {
    if (p == update) record('U');
    else { require(p == ownership); record('O'); }
    free(p);
}
template<> void CSmallVector<CCharString, 8>::Clear() {
    require(this == &update->Filenames && update->InfoBuffer == 0 && update->InfoSize == 0);
    record('F');
    Data = 0; Size = 0;
}
void __fastcall ReleaseBankSymbols(CArrayVector<CCharString>* symbols) {
    require(symbols == &bank->Symbols);
    require(bank->UpdateData.begin() == 0 && bank->UpdateData.end() == 0 && bank->UpdateData.capacity() == 0);
    require(bank->RuntimeData.begin() == 0 && bank->RuntimeData.end() == 0 && bank->RuntimeData.capacity() == 0);
    record('S');
}
void __fastcall ClearPackedBankArray(CPackedUIntArray* packed) {
    require(packed >= &bank->PackedDataOffset && packed <= &bank->PackedValid);
    record("1234"[packed - &bank->PackedDataOffset]);
    packed->PackedInts = 0; packed->Size = 0; packed->Bits = 0; packed->Bias = 0;
}
void __fastcall releaseOwner(void* p) { require(p == bank); record('R'); }
typedef _STL::map<CCharString, unsigned long>::_Rep_type StringTree;
template<> void StringTree::_M_erase(_STL::_Rb_tree_node<_STL::pair<const CCharString, unsigned long> >* node) {
    require(node == reinterpret_cast<_STL::_Rb_tree_node<_STL::pair<const CCharString, unsigned long> >*>(1));
    if (this == &bank->SymbolIndices._M_t) record('A');
    else if (this == &bank->OldSymbolIndices._M_t) record('B');
    else { require(this == &bank->FilenameIndices._M_t); record('C'); }
}
static void setupTree(_STL::map<CCharString, unsigned long>& map, _STL::_Rb_tree_node_base& header) {
    memset(&header, 0, sizeof(header));
    header._M_parent = reinterpret_cast<_STL::_Rb_tree_node_base*>(1);
    map._M_t._M_header._M_data = reinterpret_cast<_STL::_Rb_tree_node<_STL::pair<const CCharString, unsigned long> >*>(&header);
    map._M_t._M_node_count = 1;
}
int main() {
    unsigned char storage[sizeof(CBankFile)];
    memset(storage, 0, sizeof(storage));
    bank = reinterpret_cast<CBankFile*>(storage);
    bank->Size = 11;
    bank->OpenFlags = 0x48;
    bank->Alignment = 16;
    update = static_cast<UpdatePrefix*>(malloc(sizeof(UpdatePrefix)));
    memset(update, 0, sizeof(*update));
    infoBuffer = update->InfoBuffer = static_cast<char*>(malloc(7));
    update->InfoSize = 7;
    CBankFileEntryUpdateData** updates = static_cast<CBankFileEntryUpdateData**>(malloc(8));
    updates[0] = reinterpret_cast<CBankFileEntryUpdateData*>(update); updates[1] = 0;
    bank->UpdateData._M_set(updates, updates + 2, updates + 2);
    CBankFile::CRuntimeData* runtime = static_cast<CBankFile::CRuntimeData*>(malloc(24));
    bank->RuntimeData._M_set(runtime, runtime + 1, runtime + 2);
    ownership = new CCPPointerInfo(releaseOwner, bank);
    bank->BankFile.Info = ownership;
    bank->BankFile.Data = reinterpret_cast<CDiskFileWin32*>(bank);
    _STL::_Rb_tree_node_base headers[3];
    setupTree(bank->SymbolIndices, headers[0]);
    setupTree(bank->OldSymbolIndices, headers[1]);
    setupTree(bank->FilenameIndices, headers[2]);
    _STL::pair<unsigned long, unsigned long> pairs[2];
    bank->SymbolCRCIndices._M_set(pairs, pairs + 2, pairs + 2);
    CloseBankFileImpl(bank);
    require(strcmp(events, "IFUSROABC1234") == 0);
    require(bank->Size == 0 && bank->BankFile.Info == 0 && bank->BankFile.Data == 0);
    require(bank->OpenFlags == 0x48 && bank->Alignment == 16);
    require(bank->SymbolCRCIndices.begin() == pairs && bank->SymbolCRCIndices.end() == pairs && bank->SymbolCRCIndices.capacity() == 2);
    for (unsigned i = 0; i < 3; ++i) require(headers[i]._M_parent == 0 && headers[i]._M_left == &headers[i] && headers[i]._M_right == &headers[i]);
    count = 0;
    CloseBankFileImpl(bank);
    require(strcmp(events, "S1234") == 0);
    ownership = new CCPPointerInfo(releaseOwner, bank);
    ownership->RefCount = 2;
    bank->BankFile.Info = ownership;
    bank->BankFile.Data = reinterpret_cast<CDiskFileWin32*>(bank);
    count = 0;
    CloseBankFileImpl(bank);
    require(strcmp(events, "S1234") == 0 && ownership->RefCount == 1 && bank->BankFile.Info == 0 && bank->BankFile.Data == 0);
    delete ownership;
    puts("BANK_CLOSE_IMPL_009cd850_PASS");
    return 0;
}
