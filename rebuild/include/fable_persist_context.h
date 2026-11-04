#pragma once

#include "rebuild_abi.h"

class CCharString;
class CStringParser;
class CMemoryDataInputStream;
#include "fable_data_output_stream.h"   // class CDataOutputStream
#include "fable_stlport_vector.h"
class CDefinitionManager;
class CSymbolMap;

class CPersistContext;

class CPersistContext_TransferableComponentGenericType
{
public:
    virtual void TransferOut(CPersistContext& context, CCharString& text) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
};

template <class T>
class CPersistContext_TransferableComponent
    : public CPersistContext_TransferableComponentGenericType
{
public:
    CPersistContext_TransferableComponent(T& value) : TransVal(&value) {}
    virtual void TransferOut(CPersistContext& context, CCharString& text);   // <bool> 0x403F90
    virtual void TransferIn(CPersistContext& context, CStringParser& parser); // <bool> 0x403FC0
    T* TransVal;
};

template <class T>
class CPersistTraits;

template <>
class CPersistTraits<bool>
{
public:
    // Retail Transfer<bool> (004045C0) constructs this temporary without a
    // DefaultVal store; boolean input reports invalid tokens rather than
    // falling back to that member. Do not add an observable initialization.
    CPersistTraits() {}
    ~CPersistTraits() {}
    void TransferIn(CPersistContext& context, CStringParser& parser, bool& value);
    void TransferBinaryIn(CMemoryDataInputStream& stream, bool& value);  // 0x403EB0
    void TransferBinaryOut(CDataOutputStream& stream, const bool& value)
    {
        stream.WriteEBOOL(value);
    }

    bool DefaultVal;
};

template <>
class CPersistTraits<unsigned char>
{
public:
    CPersistTraits() : DefaultVal(0) {}
    void TransferIn(CPersistContext& context, CStringParser& parser, unsigned char& value); // 0x009E9080
    void TransferBinaryIn(CMemoryDataInputStream& stream, unsigned char& value);
    unsigned char DefaultVal;
};

// Ego_r: { long DefaultVal; } (4 bytes); the binary member transfer
// 0x00431102 zeroes it (`and [ebp-4], 0`) before choosing a path.
template <>
class CPersistTraits<long>
{
public:
    CPersistTraits()
        : DefaultVal(0)
    {
    }
    // Retail 0x0040FE60 (one body for the 4-byte traits: <float> too).
    void TransferBinaryIn(CMemoryDataInputStream& stream, long& value);
    void TransferBinaryOut(CDataOutputStream& stream, const long& value)
    {
        stream.WriteSLONG(value);
    }

    long DefaultVal;
};

// Ego_r: { CCharString DefaultVal; } (4 bytes). Strings travel as
// null-terminated text.
template <>
class CPersistTraits<CCharString>
{
public:
    void TransferBinaryIn(CMemoryDataInputStream& stream, CCharString& value);
    void TransferBinaryOut(CDataOutputStream& stream, const CCharString& value)
    {
        stream.WriteNullTerminatedString(value);
    }

    CCharString DefaultVal;
};

// Ego_r ??$GFSerialiseVectorBinaryIn@...@@YIXAAVCMemoryDataInputStream@@AAV?$vector@...@Z
// and ...Out@...@@YIXAAVCDataOutputStream@@AAV?$vector@...@Z (fastcall free
// templates; <CCharString>: retail 0x00433273 / 0x00433227).
template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, _STL::vector<T>& value);
template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& stream, _STL::vector<T>& value);

// Ego_r CPersistContextWriteSection (8 bytes): a named block in a save, whose
// binary length is patched in when the section ends.
class CPersistContextWriteSection
{
public:
    CCharString Name;
    long StartPosition;
};

// CPersistContext (Ego_r PDB, 0x2C bytes).
class CPersistContext
{
public:
    // Enumerator names are ours (the PDB gives only the type); values follow
    // the Transfer dispatch tables.
    enum EMode
    {
        MODE_TEXT_LOAD,
        MODE_TEXT_SAVE,
        MODE_BINARY_LOAD,
        MODE_BINARY_SAVE,
        MODE_DEFAULTS
    };

    // An uninitialised context: callers pick the mode with an Init* member
    // (AddDefClass 0x009B0AC0 calls InitClearMode on a fresh local).
    CPersistContext()
    {
    }

    // FableWin ?InitClearMode@CPersistContext@@QAEXXZ, retail 0x009BAA70:
    // defaults mode, no definition manager, no CRC check.
    void InitClearMode();

    // FableWin ?Init@CPersistContext@@QAEXAAVCMemoryDataInputStream@@_N@Z,
    // retail 0x009BAA30: binary load from the stream (CDefinitionManager::
    // LoadDef 0x009AD6E0 passes UseSafeBinaryFormat, which is not stored).
    void Init(CMemoryDataInputStream& stream, bool safeBinaryFormat);
    // FableWin ?Init@CPersistContext@@QAEXAAVCDataOutputStream@@_N@Z, retail
    // 0x009BAA50: binary save to the stream.
    void Init(CDataOutputStream& stream, bool safeBinaryFormat);

    // Text-load context @ 0x009BADD0.
    CPersistContext(
        CStringParser& parser,
        void (__fastcall* errorCallback)(const CCharString&),
        bool useDefaults,
        const CDefinitionManager* definitionManager,
        const CSymbolMap* symbolMap,
        CCharString* stringToClean);

    // Defined (and explicitly instantiated) per type in its landed source:
    // Transfer<bool> is 0x004045C0.
    template <class T>
    void Transfer(const char* name, T& value, const T& defaultValue);

    // Retail's def Transfer bodies (CFrontEndDef::Transfer 0x004331C0) send
    // each member through one-argument, binary-only forms: CheckCRC(""), then
    // the load or save path by Mode (retail defs are always compiled). Names
    // not recovered: Ego_r's defs call the named Transfer / TransferVector
    // instead. <long>: 0x00431102; vector<CCharString>: 0x004331F9.
    template <class T>
    void TransferBinaryMember(T& value);
    template <class T>
    void TransferBinaryVector(_STL::vector<T>& value);

    // CIME::LoadIMESettings reaches 0x004045C0 with byte fields; which
    // instantiation its source named is unresolved (see the 2026-09-25 journal).

    void TransferSignedChar(
        const char* name,
        signed char& value,
        const signed char& defaultValue);

protected:
    void CheckCRC(const char* name);                               // 0x404500
    void TransferObjectSaveText(                                   // 0x9BAD60
        const char* name,
        CPersistContext_TransferableComponentGenericType& object);
    void TransferObjectLoadText(                                   // 0x9BBA40
        const char* name,
        CPersistContext_TransferableComponentGenericType& object);

public:
    void PersistError(const CStringParser& parser, const CCharString& message) const;
    // Ego_r ?StartSection@CPersistContext@@QAEXAAVCPersistContextWriteSection@@ABVCCharString@@@Z,
    // retail 0x009BAB60, and the matching EndSection, retail 0x009BABF0.
    void StartSection(CPersistContextWriteSection& section, const CCharString& name);
    void EndSection(CPersistContextWriteSection& section);
    void CheckUnknownSymbols();

    const CDefinitionManager* PDefinitionManager;
    const CSymbolMap* PSymbolMap;
    CStringParser* PParser;
    void (__fastcall* ErrorCallback)(const CCharString&);
    CCharString* PStringToClean;
    bool UseDefaults;
    bool UseCRCCheck;
    bool SingleTransfer;
    bool Finished;
    EMode Mode;
    bool Binary;
    CCharString* PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
};

template <class T>
void CPersistContext::TransferBinaryMember(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}

template <class T>
void CPersistContext::TransferBinaryVector(_STL::vector<T>& value)
{
    CheckCRC("");
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        GFSerialiseVectorBinaryIn(*PLoadStream, value);
        break;
    default:
        GFSerialiseVectorBinaryOut(*PSaveStream, value);
        break;
    }
}

// The element count (WriteSLONG), then each element through its traits
// (Ego_r and retail keep a traits object, hence a default-constructed
// DefaultVal, for the whole call).
template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& stream, _STL::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, value[i]);
}

// Donor static SLONG text converter; retail 0x009E2590.
class CPersistTraitsSLONG
{
public:
    static void FABLE_FASTCALL TransferIn(CPersistContext& context, CStringParser& parser, long& value);
    static void FABLE_FASTCALL TransferOut(CPersistContext& context, CCharString& text, const long& value);
};
