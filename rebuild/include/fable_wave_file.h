#pragma once
// Ego_r CWaveFile / CWaveLoader (lib_wave_loader.cpp, retail 0x00AAD110-
// 0x00AADC80): 16-bit PCM audio held as interleaved samples, loaded from and
// saved to RIFF WAVE files. Retail's STLport CArray makes CWaveFile 0x18
// bytes (Ego_r 0x1C).
#include "fable_carray.h"
#include "fable_counted_pointer.h"
#include "fable_exception.h"

class CDataInputStream;
class CDataOutputStream;

class CWaveFile
{
public:
    CWaveFile();  // 0x00AAD320 (the sampling frequency is left unset)

    // FableWin ?GetChannels@CWaveFile@@QBEFXZ / ?GetSamplingFrequency@CWaveFile@@QBEKXZ; inline in retail.
    short GetChannels() const { return (short)Channels; }
    unsigned long GetSamplingFrequency() const { return SamplingFrequency; }

    unsigned long GetByteLength() const;                                              // 0x00AAD120
    const short* PeekAudioData() const;                                               // 0x00AAD130
    void Initialise(unsigned long chan, unsigned long freq, const CArray<short>& data); // 0x00AAD590
    void ConvertToMono();                                                             // 0x00AAD5D0
    void ConvertSamplingRate(unsigned long rate);                                     // 0x00AAD340

    unsigned long Channels;           // +0x00
    unsigned long SampleLength;       // +0x04 (samples per channel)
    unsigned long SamplingFrequency;  // +0x08
    CArray<short> AudioData;          // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CWaveFile) == 0x18);

// Ego_r CWaveFileException (empty; its message is dropped).
class CWaveFileException : public CExceptionBase
{
public:
    explicit CWaveFileException(const char*) {}
};

class CWaveLoader
{
public:
    // Ego_r CWaveLoader::CWaveFormatHeader: the "fmt " chunk (WAVEFORMAT + bits).
    class CWaveFormatHeader
    {
    public:
        CWaveFormatHeader() : FormatTag(0) {}
        unsigned short FormatTag;       // +0x00
        unsigned short Channels;        // +0x02
        unsigned long SamplesPerSec;    // +0x04
        unsigned long AvgBytesPerSec;   // +0x08
        unsigned short BlockAlign;      // +0x0C
        unsigned short BitsPerSample;   // +0x0E
    };

    CWaveLoader() {}
    CCountedPointer<CWaveFile> LoadWaveFile(CDataInputStream& is);           // 0x00AAD650
    void SaveWaveFile(const CWaveFile& wavefile, CDataOutputStream& os);      // 0x00AAD150
};
