#pragma once

// Link-only bodies for the out-of-line virtual slots of the input streams, for
// fixtures whose objects emit a CDataInputStream-family vtable (a stream
// constructor defined or inlined in the fixture's link) without testing those
// slots. Each body aborts: a fixture that reaches one is testing the wrong
// thing. Include in exactly one fixture TU. A fixture that links a real body
// (or defines its own stand-in) defines the matching FABLE_STUBS_NO_* macro
// before including this header.
#include <stdlib.h>

#include "fable_data_input_stream.h"
#include "fable_string.h"

#ifndef FABLE_STUBS_NO_DATA_INPUT_STREAM_CLOSE
void CDataInputStream::Close() { abort(); }
#endif
#ifndef FABLE_STUBS_NO_DATA_INPUT_STREAM_READS
void CDataInputStream::ReadCompressed(void*, long) { abort(); }
void CDataInputStream::ReadZlibCompressed(void*, long) { abort(); }
CCharString CDataInputStream::ReadNullTerminatedString() { abort(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { abort(); }
#endif
#ifndef FABLE_STUBS_NO_DATA_INPUT_STREAM_SOURCE
void CDataInputStream::InternalReadNextSourceChunk(const fable_u8*&, long&) { abort(); }
bool CDataInputStream::NeedsBufferedRead(long) { abort(); }
void CDataInputStream::InternalReadUnbuffered(fable_u8*, long) { abort(); }
#endif
#ifndef FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
CCharString CMemoryDataInputStream::ReadNullTerminatedString() { abort(); }
#endif
#ifndef FABLE_STUBS_NO_MEMORY_COMPRESSED_READ
void CMemoryDataInputStream::ReadCompressed(void*, long) { abort(); }
#endif
#ifndef FABLE_STUBS_NO_MEMORY_WIDE_STRING_READ
CWideString CMemoryDataInputStream::ReadNullTerminatedWString() { abort(); }
#endif
#if !defined(FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM) && !defined(FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM_CLOSE)
void CFileDataInputStream::Close() { abort(); }
#endif
#if !defined(FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM) && !defined(FABLE_STUBS_NO_FILE_NEXT_SOURCE_CHUNK)
void CFileDataInputStream::InternalReadNextSourceChunk(const fable_u8*&, long&) { abort(); }
#endif
#if !defined(FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM) && !defined(FABLE_STUBS_NO_FILE_NEEDS_BUFFERED_READ)
bool CFileDataInputStream::NeedsBufferedRead(long) { abort(); }
#endif
#if !defined(FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM) && !defined(FABLE_STUBS_NO_FILE_READ_UNBUFFERED)
void CFileDataInputStream::InternalReadUnbuffered(fable_u8*, long) { abort(); }
#endif
