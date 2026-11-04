// lib_3d_anim_file_timing_event.cpp, retail 0x00AAE190-0x00AAE4BF and
// 0x00AAEAB0 (Ego_r C3DAnimFileTimingEventChunk): the "TMEV" chunk naming an
// event and its time in the sequence. Supersedes the stand-in landings at
// 0x00AAE220 and 0x00AAEAB0.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AAE190.
const CCharString C3DAnimFileTimingEventChunk::GetChunkInfo() const
{
    return CCharString("Name: ") + Name + ", Time: " + GFFloatToCharString(Time, 6);
}

// Retail 00AAE220.
void C3DAnimFileTimingEventChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteNullTerminatedString(Name);
    stream.WriteFloat(Time);
}

// Retail 00AAE250.
CChunkedFileChunk* C3DAnimFileTimingEventChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileTimingEventChunk);
}

// Retail 00AAE440.
void C3DAnimFileTimingEventChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    Name = stream.ReadNullTerminatedString();
    Time = stream.ReadValue<float>();
}

// Retail 00AAEAB0.
const CCharString C3DAnimFileTimingEventChunk::GetChunkDescription() const
{
    return CCharString("Timing Event");
}

C3DAnimFileTimingEventChunk::C3DAnimFileTimingEventChunk() : CChunkedFileChunk("TMEV") {}
