#pragma once
#include "fable_game_text_bank.h"
#include <stdlib.h>
// Preserve the base boundary behavior in isolated derived lifetime fixtures.
bool CBankFileAsync::OpenReadOnly(const CCharString& name,unsigned long flags) { return CBankFile::OpenReadOnly(name,flags); }
void CBankFileAsync::OpenReadWrite(const CCharString& name,unsigned long flags,unsigned long mode) { CBankFile::OpenReadWrite(name,flags,mode); }
bool CBankFileAsync::OpenFileReadOnly(const CWideString& name,unsigned long flags) { return CBankFile::OpenFileReadOnly(name,flags); }
void CBankFileAsync::OpenFileReadWrite(const CWideString& name,unsigned long flags,unsigned long mode) { CBankFile::OpenFileReadWrite(name,flags,mode); }
void CBankFileAsync::Close() { CBankFile::Close(); }
// These fixtures own empty async sets; populated cleanup is tested separately.
typedef _STL::set<CBankFileAsyncData*>::_Rep_type FixtureAsyncBankTree;
template<> void FixtureAsyncBankTree::_M_erase(_STL::_Rb_tree_node<CBankFileAsyncData*>*) { abort(); }
