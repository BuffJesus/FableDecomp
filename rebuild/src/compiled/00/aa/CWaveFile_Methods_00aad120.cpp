// Ego_r lib_wave_loader.cpp, retail 0x00AAD120-0x00AAD650: CWaveFile's
// accessors and in-place conversions, and CWaveLoader's load and save.
#include "fable_wave_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AAD320.
CWaveFile::CWaveFile()
{
    Channels = 0;
    SampleLength = 0;
}

// Retail 00AAD120.
unsigned long CWaveFile::GetByteLength() const
{
    return AudioData.size() * sizeof(short);
}

// Retail 00AAD130.
const short* CWaveFile::PeekAudioData() const
{
    return AudioData.GetAsCArray();
}

// Retail 00AAD590.
void CWaveFile::Initialise(unsigned long chan, unsigned long freq, const CArray<short>& data)
{
    Channels = chan;
    SamplingFrequency = freq;
    AudioData = data;
    SampleLength = AudioData.size() / Channels;
}

// Retail 00AAD5D0: stereo becomes the average of each sample pair.
void CWaveFile::ConvertToMono()
{
    if (Channels == 2)
    {
        CArray<short> new_data(SampleLength);
        for (unsigned long i = 0; i < SampleLength; ++i)
            new_data[i] = (short)((AudioData[i * 2] + AudioData[i * 2 + 1]) >> 1);
        Channels = 1;
        AudioData = new_data;
    }
}

// Retail 00AAD150: a RIFF "WAVE" file with a PCM "fmt " chunk and the
// samples in a "data" chunk.
void CWaveLoader::SaveWaveFile(const CWaveFile& wavefile, CDataOutputStream& os)
{
    CMemoryDataOutputStream frm_chunk;
    frm_chunk.WriteUWORD(1);
    frm_chunk.WriteUWORD(wavefile.GetChannels());
    frm_chunk.WriteULONG(wavefile.GetSamplingFrequency());
    frm_chunk.WriteULONG(wavefile.GetSamplingFrequency() * wavefile.GetChannels() * 2);
    frm_chunk.WriteUWORD(wavefile.GetChannels() * 2);
    frm_chunk.WriteUWORD(16);

    CMemoryDataOutputStream all_chunks;
    {
        const CCharString id("WAVE");
        all_chunks.WriteString(id);
    }
    {
        const CCharString id("fmt ");
        all_chunks.WriteString(id);
    }
    all_chunks.WriteULONG(frm_chunk.GetLength());
    all_chunks.Write(frm_chunk.PeekData(), frm_chunk.GetLength());
    {
        const CCharString id("data");
        all_chunks.WriteString(id);
    }
    all_chunks.WriteULONG(wavefile.GetByteLength());
    all_chunks.Write(wavefile.PeekAudioData(), wavefile.GetByteLength());

    {
        const CCharString id("RIFF");
        os.WriteString(id);
    }
    os.WriteULONG(all_chunks.GetLength());
    os.Write(all_chunks.PeekData(), all_chunks.GetLength());
}

// Retail 00AAD650: a RIFF "WAVE" file; reads its chunks until the end of the
// RIFF data, taking the format from "fmt " and 16- or 8-bit PCM samples from
// "data" (8-bit samples are unsigned and widened to 16 bits).
CCountedPointer<CWaveFile> CWaveLoader::LoadWaveFile(CDataInputStream& is)
{
    CCountedPointer<CWaveFile> wavefile;
    CCharString file_header = is.ReadString(4);
    if (file_header != "RIFF")
        throw CWaveFileException("Not a WAV file!");
    const unsigned long file_size = is.ReadULONG();
    const unsigned long end_pos = is.GetPosition() + file_size;
    CCharString datatype = is.ReadString(4);
    if (datatype != "WAVE")
        throw CWaveFileException("Not a WAV file!");
    CWaveFormatHeader wave_format;
    while (is.GetPosition() < end_pos)
    {
        CCharString chunk_header = is.ReadString(4);
        const unsigned long chunk_size = is.ReadULONG();
        const unsigned long chunk_start = is.GetPosition();
        if (chunk_header == "fmt ")
        {
            wave_format.FormatTag = is.ReadUWORD();
            wave_format.Channels = is.ReadUWORD();
            wave_format.SamplesPerSec = is.ReadULONG();
            wave_format.AvgBytesPerSec = is.ReadULONG();
            wave_format.BlockAlign = is.ReadUWORD();
            wave_format.BitsPerSample = is.ReadUWORD();
        }
        else if (chunk_header == "data")
        {
            if (wave_format.FormatTag != 1)
                throw CWaveFileException("Unsupported WAV compression!");
            const unsigned long samples = chunk_size / wave_format.BlockAlign;
            const unsigned long count = wave_format.Channels * samples;
            CArray<short> audio_data(count);
            const long bits = (wave_format.BlockAlign * 8) / wave_format.Channels;
            if (bits == 16)
            {
                for (unsigned long i = 0; i < count; ++i)
                    audio_data[i] = is.ReadSWORD();
            }
            else if (bits == 8)
            {
                for (unsigned long i = 0; i < count; ++i)
                    audio_data[i] = (short)((is.ReadUBYTE() - 0x80) << 8);
            }
            else
            {
                throw CWaveFileException("WAV data is not 16 or 8 bits!");
            }
            wavefile.Reset(new CWaveFile);
            wavefile->Initialise(wave_format.Channels, wave_format.SamplesPerSec, audio_data);
        }
        is.SetPosition(chunk_start + chunk_size);
    }
    return wavefile;
}
