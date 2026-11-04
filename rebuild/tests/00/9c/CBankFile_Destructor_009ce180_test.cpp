#include "test_support/bank_extended_virtual_boundaries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#include "test_support/bank_file_virtual_stubs.h"
static CBankFile* bank;
static void* bankVtable;
static void* heapBank;
static char events[80]; static unsigned count;
static CCPPointerInfo* owner;
static unsigned long* packedPointers[4];
static void record(char c) { events[count++] = c; events[count] = 0; }
static void require(bool condition) { if (!condition) { printf("failed after %s\n", events); exit(1); } }
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { record('B'); }
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long) { return false; }
void __fastcall CloseBankFileImpl(CBankFile* receiver) {
    require(receiver == bank && *reinterpret_cast<unsigned long*>(bank) != 0xA5A5A5A5);
    bankVtable = *reinterpret_cast<void**>(bank);
    record('C');
}
CChecksumCacheFile::~CChecksumCacheFile() { require(this == &bank->ChecksumCache); record('K'); }
CWideString::~CWideString() { record('W'); }
CCharString::~CCharString() {
    if (this == &bank->HeaderEnumerationType) record('H');
    else if (this == &bank->BankHandle) record('N');
    else if (this == bank->Symbols.begin()) record('s');
    else { require(this == bank->Symbols.begin() + 1); record('t'); }
}
void __fastcall releaseOwner(void* p) { require(p == bank); record('R'); }
void operator delete(void* p) { if (p == heapBank) record('D'); else { require(p == owner); record('O'); } free(p); }
void operator delete[](void* p) {
    if (!p) return;
    for (unsigned i = 0; i < 4; ++i) if (p == packedPointers[i]) { record("1234"[i]); free(p); return; }
    abort();
}
CPackedUIntArray::~CPackedUIntArray() { operator delete[](PackedInts); }
typedef _STL::map<CCharString, unsigned long>::_Rep_type StringTree;
typedef _STL::map<CCharString, unsigned long>::value_type StringValue;
typedef _STL::_Rb_tree_node<StringValue> StringNode;
template<> void StringTree::_M_erase(StringNode* node) {
    require(node != 0);
    if (this == &bank->SymbolIndices._M_t) record('1');
    else if (this == &bank->OldSymbolIndices._M_t) record('2');
    else { require(this == &bank->FilenameIndices._M_t); record('3'); }
    free(node);
}
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::_Rep_type CacheTree;
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type CacheValue;
template<> void CacheTree::_M_erase(_STL::_Rb_tree_node<CacheValue>*) { abort(); }
static void setupTree(FableBankOwnedMap<CCharString, unsigned long>& map) {
    StringNode* header = static_cast<StringNode*>(malloc(sizeof(StringNode)));
    memset(header, 0, sizeof(*header));
    header->_M_parent = static_cast<StringNode*>(malloc(sizeof(StringNode)));
    map._M_t._M_header._M_data = header; map._M_t._M_node_count = 1;
}
int main() {
    unsigned char raw[sizeof(CBankFile)]; memset(raw, 0, sizeof(raw));
    bank = reinterpret_cast<CBankFile*>(raw); *reinterpret_cast<unsigned long*>(raw) = 0xA5A5A5A5;
    setupTree(bank->SymbolIndices); setupTree(bank->OldSymbolIndices); setupTree(bank->FilenameIndices);
    bank->ChecksumCache.Entries._M_t._M_header._M_data = static_cast<_STL::_Rb_tree_node<CacheValue>*>(malloc(sizeof(_STL::_Rb_tree_node<CacheValue>)));
    bank->ChecksumCache.Entries._M_t._M_node_count = 0;
    owner = new CCPPointerInfo(releaseOwner, bank);
    bank->BankFile.Info = owner; bank->BankFile.Data = reinterpret_cast<CDiskFileWin32*>(bank);
    CPackedUIntArray* packed[] = { &bank->PackedDataOffset, &bank->PackedDataSize, &bank->PackedDataType, &bank->PackedValid };
    for (unsigned i = 0; i < 4; ++i) packed[i]->PackedInts = packedPointers[i] = static_cast<unsigned long*>(malloc(4));
    CCharString* strings = static_cast<CCharString*>(malloc(8));
    bank->Symbols._M_set(strings, strings + 2, strings + 2);
    bank->CBankFile::~CBankFile();
    require(strcmp(events, "CKWH321NWRO4321stB") == 0);
    count = 0;
    heapBank = malloc(sizeof(CBankFile));
    memset(heapBank, 0, sizeof(CBankFile));
    bank = static_cast<CBankFile*>(heapBank);
    *reinterpret_cast<void**>(bank) = bankVtable;
    delete bank;
    require(strcmp(events, "CKWHNWBD") == 0);
    puts("BANK_DTOR_009ce180_PASS");
    return 0;
}
