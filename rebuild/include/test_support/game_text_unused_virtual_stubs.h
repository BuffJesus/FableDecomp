#pragma once

// Only for unopened game-text bank constructor/title fixtures. Their typed
// vtables require these slots to link; reaching any of them is a fixture error.
// These boundaries are not native reconstruction providers.
#include <stdlib.h>
#include "fable_game_text_bank.h"
#include "fable_date_and_time.h"

void CBankFile::AddEntryMultipleFiles(const CCharString&, unsigned long,
    const CArray<CCharString>&, CCountedPointer<CBankStateBlock>) { abort(); }
void CBankFile::Update() { abort(); }
unsigned long CBankFile::AdjustUpdateEntries() { abort(); }
void CBankFile::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&) { abort(); }
void CBankFile::NotifyBankOpened() { abort(); }
void CBankFile::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryRemoved(unsigned long) { abort(); }
void CBankFile::NotifyPostEntriesLoaded() { abort(); }
void CBankFile::PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
    unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&) { abort(); }
bool CBankFile::IsDiskFile(unsigned long, unsigned long) { abort(); }
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&) { abort(); }
CCharString CBankFile::GetEnumerationType(unsigned long) const { abort(); }

unsigned long NGameText::CDataBank::GetDataFormatVersion() { abort(); }
void NGameText::CDataBank::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&) { abort(); }
void NGameText::CDataBank::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&) { abort(); }
CDateAndTime NGameText::CDataBank::GetLastModifiedTime(const CCharString&) { abort(); }
CCharString NGameText::CDataBank::GetEnumerationType(unsigned long) const { abort(); }
