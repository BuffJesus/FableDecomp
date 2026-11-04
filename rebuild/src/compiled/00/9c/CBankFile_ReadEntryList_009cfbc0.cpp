// Retail009CFBC0 (near_match, 691 instructions both sides, 0.952 opcode similarity):
// read the per-type counts and every entry record (BIG.md section 7: magic, index,
// type, size, offset, CRC, name, timestamp, dependency filenames, info), filling the
// runtime table and the per-flag arrays, then finish the indices. The CRC is the
// update record's state-block CRC and the timestamp fills Checksums. Retail keeps
// filenames.Data in a register across the reads; the filename walk below is the
// closest VC7.1 shape found (register allocation and stack slots differ).
#include <new>
#include <string.h>
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_bank_entry_update.h"
#include "fable_bank_file_data_type_count.h"
#include "fable_stlport_vector_impl.h"
bool CBankFile::ReadEntryList(CFileDataInputStream& stream, unsigned long max_index, unsigned long entry_count)
{
    AllocateBuffers(max_index + 1);
    unsigned long type_count = stream.ReadULONG();
    CArray<CBankFileDataTypeCount> type_counts(type_count);
    for (unsigned long i = 0; i < type_count; ++i) {
        type_counts[i].DataType = stream.ReadULONG();
        type_counts[i].Count = stream.ReadULONG();
    }
    NotifyBankSize(Size, type_counts);
    for (unsigned long e = 0; e < entry_count; ++e) {
        unsigned long magic = stream.ReadULONG();
        unsigned long index = stream.ReadULONG();
        unsigned long type = stream.ReadULONG();
        unsigned long size = stream.ReadULONG();
        unsigned long offset = stream.ReadULONG();
        unsigned long crc = stream.ReadULONG();
        CCharString symbol = stream.ReadPresizedString();
        unsigned long timestamp = stream.ReadULONG();
        unsigned long filename_count = stream.ReadULONG();
        CSmallVector<CCharString, 8> filenames;
        filenames.Allocate(filename_count);
        CCharString* name = filenames.Data;
        for (unsigned long f = 0; f < filename_count; ++f, ++name)
            *name = stream.ReadPresizedString();
        unsigned long info_size = stream.ReadULONG();
        CArray<char> info(info_size);
        stream.Read(info.GetAsCArray(), info_size);
        CRuntimeData& data = RuntimeData[index];
        data.DataType = static_cast<unsigned char>(type);
        data.DataSize = size;
        data.DataOffset = offset;
        data.Valid = true;
        if (OpenFlags & 8) Symbols[index] = symbol;
        if (OpenFlags & 16) Checksums[index] = timestamp;
        if (OpenFlags & 32) {
            CBankFileEntryUpdateData* update = new CBankFileEntryUpdateData;
            update->StateBlockCRC = crc;
            if (update->InfoBuffer) {
                delete[] update->InfoBuffer;
                update->InfoBuffer = 0;
            }
            update->InfoSize = 0;
            update->InfoSize = info.size();
            if (update->InfoSize) {
                update->InfoBuffer = new char[update->InfoSize];
                memcpy(update->InfoBuffer, info.GetAsCArray(), update->InfoSize);
            }
            if (&update->Filenames != &filenames) update->Filenames.Copy(filenames);
            UpdateData[index] = update;
        }
        CCharString filename = !filenames.Size ? CCharString("") : filenames.Data[0];
        InitEntry(index, symbol, filename);
        unsigned long position = stream.GetPosition();
        NotifyEntryAdded(type, index, info);
        stream.SetPosition(position);
        delete[] filenames.Data;
        filenames.Data = 0;
        filenames.Size = 0;
    }
    SymbolCRCIndices.Update();
    SymbolCRCIndices.DeallocateUnusedMemory();
    if (ReadOnly) PackRuntimeData();
    NotifyPostEntriesLoaded();
    FileValid = true;
    return true;
}
