#pragma once

// Fixture-only construction for the typed 0x110-byte CBankFile base. Tests
// that record their own string/base lifetimes provide these methods locally.
#include "fable_game_text_bank.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "test_support/bank_file_cleanup_stubs.h"

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CPackedUIntArray::CPackedUIntArray()
    : PackedInts(0), Size(0), Bits(0), Bias(0) {}
