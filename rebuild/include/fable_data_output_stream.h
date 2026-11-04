#pragma once

// Lionhead's output streams. Retail vtables (RTTI): CDataOutputStream
// 0x0129A6E0 (0 its destructor 0x00996F00, 1-4 _purecall),
// CMemoryDataOutputStream 0x0129A788 (0 0x009973C0, 1 SetPosition 0x00994B10,
// 2 GetPosition 0x009945B0, 3 Close 0x009945C0 (empty), 4 Write 0x00996CC0).
// Ego_r layouts: CDataOutputStream 8 bytes (vptr, NewLine); the memory
// stream adds Data (a CArray<char>: a 12-byte STLport vector in retail, +8)
// and CurrentPosition (+0x14), 0x18 bytes in retail (Ego_r 0x1C).
//
// Declared `struct` (FableWin: class) as the older forward declarations
// were, so existing parameter manglings stay as they are.

#include "rebuild_abi.h"
#include "fable_data_stream.h"
#include "fable_string.h"
#include "fable_stlport_vector.h"
#include "fable_carray.h"

// A CBaseClassNonCopyable (CMemoryDataOutputStream 0x00996AF0 calls its ctor
// 0x0099A2F0 first).
class CDataInputStream;
class CMemoryDataOutputStream;

// Retail RTTI .?AVCDataOutputStream@@ and FableWin decorations (AAVCDataOutputStream):
// a class, not a struct; the class key is part of every parameter decoration.
class CDataOutputStream : public CDataStream
{
public:
    // Inline in retail (CMemoryDataOutputStream 0x00996AF0): new lines are
    // "\r\n" (retail literal 0x012397F8).
    CDataOutputStream()
        : NewLine("\r\n")
    {
    }
    // Implicit destruction preserves the derived vptr during NewLine cleanup.
    virtual void SetPosition(unsigned long position) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
    // FableWin ?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z is the override.
    virtual void Write(const void* data, long size) = 0;

    void Write(const CMemoryDataOutputStream& source); // 0x00995EB0
    void WriteDateAndTime(const class CDateAndTime& value);
    void WriteUBYTE(unsigned char value);
    void WriteUWORD(unsigned short value);
    void WriteEBOOL(bool value);                                   // 0x993E30
    // FableWin ?WriteRepeatedByte@CDataOutputStream@@QAEXEJ@Z, retail 0x00994230.
    void WriteRepeatedByte(unsigned char value, long count);
    // FableWin ?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z,
    // retail 0x00995AE0.
    void WriteNullTerminatedString(const CCharString& text);
    // Ego_r, retail 0x009959F0 / 0x00995A20 / 0x00995A80: the text alone, the text
    // and NewLine, and a length-prefixed text.
    void WriteString(const CCharString& text);
    void WriteStringNewLine(const CCharString& text);
    void WritePresizedString(const CCharString& text);
    // FableWin ?WriteULONG@CDataOutputStream@@QAEXK@Z: out of line in most
    // retail units (0x00993EA0; CPersistContext::CheckCRC 0x00404500 calls it,
    // which only matches without the body in sight). Units that expand it
    // include fable_data_output_stream_inline.h (CDefStringTable::AddString
    // 0x009D4F90 calls Write through the vtable).
    void WriteULONG(unsigned long value);
    // Ego_r ?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z (retail 0x00996610): size bytes, compressed.
    void WriteCompressed(const void* data, long size);
    // FableWin ?WriteSLONG@CDataOutputStream@@QAEXJ@Z, retail 0x00993EB0
    // (Write(&value, 4) through the vtable; the def members' binary transfer
    // 0x00431102 calls it).
    void WriteSLONG(long value);
    // Ego_r lib_data_stream.cpp writers, retail 0x00993E50-0x009942D0
    // (CDataOutputStream_Writers_00993e50.cpp).
    void WriteSBYTE(signed char value);
    void WriteSWORD(short value);
    void WriteUHUGE(unsigned __int64 value);
    void WriteCBYTE(char value);
    void WriteSHUGE(__int64 value);
    void WriteFloat(float value);
    void WriteDouble(double value);
    void WriteNullTerminatedWString(const class CWideString& text);
    void WriteColour(const class CRGBColour& colour);
    // Ego_r ?WritePixelFormat@CDataOutputStream@@QAEXABVCPixelFormat@@@Z, retail 0x00993F60.
    void WritePixelFormat(const class CPixelFormat& format);
    // Ego_r ?Write2DVector@CDataOutputStream@@QAEXABVC2DVector@@@Z, retail 0x00993F90.
    void Write2DVector(const class C2DVector& vector);
    void Write3DVector(const class C3DVector& vector);
    void Write3DAngles(const class C3DAngles& angles);
    // Ego_r ?WriteMatrix3x4@CDataOutputStream@@QAEXABVCMatrix3x4@@@Z, retail 0x00994070.
    void WriteMatrix3x4(const class CMatrix3x4& matrix);
    void AlignData(long alignment);
    // FableWin ?CopyFrom@CDataOutputStream@@QAEXAAVCDataInputStream@@K@Z, retail
    // 0x00996970 (CDefStringTable::ReadTable 0x009D4AB0 copies names.bin's text).
    void CopyFrom(CDataInputStream& stream, unsigned long size);

    CCharString NewLine;                                           // +0x04
};

// Ego_r CBufferedFileDataOutputStream (0x24), RTTI vtable 0x0129A710: writes
// go through a buffer covering [DataStartFilePtr, + BufferDataSize) of the
// file and are flushed when a write falls outside it or does not fit
// (CBufferedFileDataOutputStream_00994520.cpp).
class CBufferedFileDataOutputStream : public CDataOutputStream
{
public:
    CBufferedFileDataOutputStream(CAFile& file, long bufferSize);      // 0x009944A0
    virtual ~CBufferedFileDataOutputStream();                           // 0x009947A0
    virtual void SetPosition(unsigned long position);                   // 0x00994500
    virtual unsigned long GetPosition() const;                          // 0x00994510
    virtual void Close();                                               // 0x00994810
    virtual void Write(const void* data, long size);                    // 0x00994870
    void Flush();                                                       // 0x00994520

    CAFile* File;
    unsigned char* Buffer;
    long BufferSize;
    long BufferDataSize;
    long DataStartFilePtr;
    long FileSize;
    long FilePos;
};

class CMemoryDataOutputStream : public CDataOutputStream
{
public:
    // FableWin ??0CMemoryDataOutputStream@@QAE@XZ, retail 0x00996AF0 (out of
    // line; CDefStringTable 0x009D4DB0 calls it): empty, at position 0.
    CMemoryDataOutputStream();
    virtual ~CMemoryDataOutputStream();                            // 0x009973C0 (scalar deleting)

    virtual void SetPosition(unsigned long position);             // 0x00994B10
    virtual unsigned long GetPosition() const;                     // 0x009945B0
    virtual void Close();                                          // 0x009945C0
    virtual void Write(const void* data, long size);              // 0x00996CC0

    // FableWin ?GetLength@CMemoryDataOutputStream@@QBEKXZ, retail 0x00994B40
    // (out of line: Data's size).
    unsigned long GetLength() const;
    // FableWin ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ, retail 0x00995E70.
    const void* PeekData() const;
    // Ego_r ?CopyDataToString@CMemoryDataOutputStream@@QBEXAAVCCharString@@@Z, retail 0x00995E80.
    void CopyDataToString(CCharString& string) const;
    // FableWin ?ReserveSize@CMemoryDataOutputStream@@QAEXK@Z, retail 0x00996BC0.
    void ReserveSize(unsigned long size);
    void Clear();                                                // 0x00996B60

    CArrayVector<char> Data;                                       // +0x08
    unsigned long CurrentPosition;                                 // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CMemoryDataOutputStream) == 0x18);
