#pragma once
#include "fable_counted_pointer.h"
class CBankFileAsyncEntry;
// Opt-in declarations for callers which use the retail out-of-line forms.
template<> CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer&);
template<> __declspec(noinline) CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer();
template<> CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer&);
