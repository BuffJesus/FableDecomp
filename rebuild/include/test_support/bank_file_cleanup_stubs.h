#pragma once
#include <stdlib.h>
#include "fable_game_text_bank.h"

// Isolated constructor/lifetime fixtures create empty bank-owned maps. Their
// external cleanup dependencies are not the function under test; reject an
// unexpected populated map instead of silently discarding its contents.
CPackedUIntArray::~CPackedUIntArray() {}
#ifndef FABLE_FIXTURE_OWNS_CHECKSUM_DTOR
CChecksumCacheFile::~CChecksumCacheFile() {}
#endif
typedef _STL::map<CCharString, unsigned long>::_Rep_type FixtureBankStringTree;
typedef _STL::map<CCharString, unsigned long>::value_type FixtureBankStringValue;
template<> void FixtureBankStringTree::_M_erase(_STL::_Rb_tree_node<FixtureBankStringValue>*) { abort(); }
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::_Rep_type FixtureBankCacheTree;
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type FixtureBankCacheValue;
template<> void FixtureBankCacheTree::_M_erase(_STL::_Rb_tree_node<FixtureBankCacheValue>*) { abort(); }
