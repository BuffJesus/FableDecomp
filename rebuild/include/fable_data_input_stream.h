#pragma once

#include <string.h>

#include "fable_data_stream.h"

class CAFile;
class CDateAndTime;
class CCharString;
class CWideString;
class CDataOutputStream;
class CMemoryDataOutputStream;

// RTTI: CDataInputStream : CDataStream : CBaseClassNonCopyable. CDataStream's
// vtable (0x129A69C) is the dtor plus three pure slots.

// Ego_r.pdb layouts (CDataInputStream 0x18, CFileDataInputStream 0x24,
// CMemoryDataInputStream 0x1C). Only the members reconstructed callers use
// are declared.
class CDataInputStream : public CDataStream
{
public:
    // FableWin ??0CDataInputStream@@QAE@XZ, retail 0x00993B20 (out of line:
    // no source, every field zero; CDefinitionManager::LoadDef 0x009AD6E0
    // calls it for its local CMemoryDataInputStream).
    CDataInputStream();
    // FableWin ??0CDataInputStream@@QAE@PBEJJ@Z: positioned at 0 over `data`,
    // `streamSize` long with `chunkBytes` readable (expanded into
    // CMemoryDataInputStream 0x00994930; no standalone retail copy).
    CDataInputStream(const unsigned char* data, long streamSize, long chunkBytes);
    // Inline and empty in retail (its vptr store is dead in every derived dtor).
    virtual ~CDataInputStream() {}

    // FableWin ?GetLength@CDataInputStream@@QBEKXZ (inline).
    unsigned long GetLength() const { return StreamSize; }

    // Vtable 0x129A6B0 (CDataStream's first three slots are pure): 1
    // SetPosition 0x993BC0, 2 GetPosition (inline; COMDAT 0x405FA0), 3 Close
    // 0x993C80, 4 ReadCompressed 0x995FB0, 5 ReadZlibCompressed 0x996230, 6
    // ReadNullTerminatedString 0x994C80, 7 ReadNullTerminatedWString 0x994D20,
    // then the three private source hooks below (0x993B90/0x993BA0/0x993BB0).
    // Fixtures that emit a stream vtable define the out-of-line slots through
    // test_support/data_input_stream_stubs.h.
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const { return StreamPos; }
    virtual void Close();
    virtual void ReadCompressed(void* data, long size);
    virtual void ReadZlibCompressed(void* data, long size);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();

    bool IsEOF() const { return StreamPos == (unsigned long)StreamSize; }

    // FableWin ?CopyTo@CDataInputStream@@QAEXAAVCDataOutputStream@@K@Z, retail
    // 0x009964E0 (CDataOutputStream::CopyFrom 0x00996970 forwards to it).
    void CopyTo(CDataOutputStream& stream, unsigned long size);
    CCharString ReadLine();                                    // 0x994B50
    CCharString ReadUntilCharacter(char character, bool& found); // 0x994DC0
    CCharString ReadUntilWhiteSpace();                         // 0x994E90
    CCharString ReadWhiteSpace();                              // 0x994F60 (FableWin)
    void ReadWithSrcChunkOverflow(void* data, long size);      // 0x993CA0

    // The general inlined read (CBankFileManager::OpenRetailBank 0x9A8840):
    // a positive size, the position guard, then the in-window copy or the
    // chunk-overflow reader. Retail inlines it even inside large callers.
    __forceinline void Read(void* data, long size)
    {
        if (size > 0)
        {
            __int64 next = (unsigned long)(StreamPos + size);
            if (next >= 0 && next <= 0x7FFFFFFF)
            {
                if (size <= SourceChunkBytesRemaining)
                {
                    memcpy(data, CurrentSourcePtr, size);
                    CurrentSourcePtr += size;
                    SourceChunkBytesRemaining -= size;
                    StreamPos += size;
                }
                else
                {
                    ReadWithSrcChunkOverflow(data, size);
                }
            }
        }
    }

    template<typename T>
    __forceinline CDataInputStream& operator>>(T& value)
    {
        Read(&value, sizeof(T));
        return *this;
    }

    // Reads into a temporary the caller copies out; its stack slot is
    // released at once, which is how retail's frames share read slots.
    template<typename T>
    __forceinline T ReadValue()
    {
        T value;
        Read(&value, sizeof(T));
        return value;
    }

    // Inlined by retail callers: a position guard, then the in-window fast
    // path or the chunk-overflow reader.
    // Signed 32-bit scalar read; expanded by CPersistTraits<long> (0x0040FE60).
    // Ego_r ?OpenStream@CDataInputStream@@QAEXPBEJJ@Z, retail 0x00993C60.
    void OpenStream(const unsigned char* data, long streamSize, long chunkBytes);

    // Ego_r ?Read2DVector@...@@QAE?AVC2DVector@@XZ, ?Read3DVector@...@@QAE?AVC3DVector@@XZ,
    // ?Read3DAngles@...@@QAE?AVC3DAngles@@XZ; retail 0x00995030 / 0x009950F0 / 0x00995210.
    class C2DVector Read2DVector();
    class C3DVector Read3DVector();
    class C3DAngles Read3DAngles();
    // Ego_r ?ReadMatrix3x4@CDataInputStream@@QAE?AVCMatrix3x4@@XZ, retail 0x00995340.
    class CMatrix3x4 ReadMatrix3x4();
    // Ego_r ?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ, retail 0x00996390.
    CCharString ReadPresizedString();
    // Ego_r ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ, retail 0x00994670.
    class CPixelFormat ReadPixelFormat();
    // Ego_r ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ, retail 0x009945E0.
    class CRGBColour ReadColour();
    long ReadSLONG();
    // Ego_r inline readers (no out-of-line body in retail): a stored byte
    // tested against zero, and the 32-bit unsigned / float scalars
    // (C3DAnimFileRootChunk::ReadFromFile 0x00A98AF0).
    __forceinline bool ReadEBOOL() { return ReadValue<char>() != 0; }
    __forceinline unsigned long ReadULONG() { return ReadValue<unsigned long>(); }
    __forceinline float ReadFloat() { return ReadValue<float>(); }
    // Ego_r inline 16/8-bit readers (CWaveLoader::LoadWaveFile 0x00AAD650).
    __forceinline unsigned short ReadUWORD() { return ReadValue<unsigned short>(); }
    __forceinline short ReadSWORD() { return ReadValue<short>(); }
    __forceinline unsigned char ReadUBYTE() { return ReadValue<unsigned char>(); }
    // Ego_r ?ReadString@CDataInputStream@@QAE?AVCCharString@@K@Z, retail 0x00996BE0:
    // `length` characters, not terminated.
    CCharString ReadString(unsigned long length);
    CDateAndTime ReadDateAndTime();

    __forceinline char ReadCBYTE()
    {
        char value;
        __int64 next = (unsigned long)(StreamPos + 1);
        if (next >= 0 && next <= 0x7FFFFFFF)
        {
            if (SourceChunkBytesRemaining >= 1)
            {
                value = *CurrentSourcePtr;
                ++CurrentSourcePtr;
                --SourceChunkBytesRemaining;
                StreamPos = (unsigned long)next;
            }
            else
            {
                ReadWithSrcChunkOverflow(&value, 1);
            }
        }
        return value;
    }

    unsigned long StreamPos;
    long StreamSize;
    fable_u8* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;

protected:
    // Ego_r hierarchy helpers, retail 0x00993C10 / 0x00993C40.
    void SetPositionAssumeWithinSourceChunk(unsigned long position);
    bool CanSeekToPosWithinSourceChunk(long position);

private:
    // Fixtures reach the private hooks through this (test-only) friend.
    friend class CDataInputStreamTestAccess;

    // Ego_r private virtuals (?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z,
    // ?NeedsBufferedRead@...@@EAE_NJ@Z, ?InternalReadUnbuffered@...@@EAEXPAEJ@Z):
    // how ReadWithSrcChunkOverflow 0x993CA0 refills the source window. The
    // base stream has no source (empty bodies; NeedsBufferedRead is false).
    virtual void InternalReadNextSourceChunk(const fable_u8*& source, long& size);
    virtual bool NeedsBufferedRead(long size);
    virtual void InternalReadUnbuffered(fable_u8* data, long size);
};

class CFileDataInputStream : public CDataInputStream
{
public:
    // 0x00994700 / 0x00994780.
    CFileDataInputStream(CAFile* file, unsigned long bufferSize);
    virtual ~CFileDataInputStream();
    // Vtable 0x129A728 overrides: Close 0x994300 and the source hooks
    // 0x994360 / 0x9943B0 / 0x9943D0.
    virtual void Close();

private:
    friend class CDataInputStreamTestAccess;
    virtual void InternalReadNextSourceChunk(const fable_u8*& source, long& size);
    virtual bool NeedsBufferedRead(long size);
    virtual void InternalReadUnbuffered(fable_u8* data, long size);

public:

    CAFile* File;
    fable_u8* Buffer;
    long BufferSize;
};

class CMemoryDataInputStream : public CDataInputStream
{
public:
    // 0x00995F50; the dtor folds with ~CDataStream at 0x00993B10.
    CMemoryDataInputStream(const CCharString& source);
    CMemoryDataInputStream(const CMemoryDataOutputStream& source); // 0x00995EE0
    CMemoryDataInputStream(const void* data, unsigned long size); // 0x994930
    // FableWin ??0CMemoryDataInputStream@@QAE@XZ; inline in retail
    // (CDefinitionManager::LoadDef 0x009AD6E0): no memory source yet.
    CMemoryDataInputStream()
        : StreamSource(0)
    {
    }
    virtual ~CMemoryDataInputStream();
    void OpenMemorySource(const void* data, unsigned long size); // 0x00994570
    // FableWin ?OpenStringSource@CMemoryDataInputStream@@QAEXABVCCharString@@@Z;
    // inline in retail (the CCharString constructor 0x00995F50 expands it).
    void OpenStringSource(const CCharString& source);

    // 0x995D30: the override of CDataInputStream's slot 6 (vtable 0x123675C).
    // Callers on a concrete stream call it directly.
    // Ego_r virtual methods and retail table also override slots4 and7.
    virtual void ReadCompressed(void* data, long size); // 0x995B10
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString(); // 0x994960

    // Ego_r.pdb: inline memory-source access used by ReadCompressed.
    __forceinline const unsigned char* ReadConstData(long size)
    {
        unsigned long position = GetPosition();
        __int64 next = position + (unsigned long)size;
        if (next >= 0 && next <= 0x7fffffff) {
            long newPosition = (long)next;
            long oldPosition = StreamPos;
            CurrentSourcePtr += newPosition - oldPosition;
            SourceChunkBytesRemaining += oldPosition - newPosition;
            StreamPos = newPosition;
            return StreamSource + position;
        }
        return 0;
    }

    fable_u8* StreamSource;
};

FABLE_STATIC_ASSERT(sizeof(CDataInputStream) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CFileDataInputStream) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream) == 0x1C);
