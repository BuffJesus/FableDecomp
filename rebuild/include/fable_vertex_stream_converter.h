#pragma once
// Hand-owned (retail model): CVertexStreamConverter (lib_vertex_stream_converter), a stateless helper that converts
// vertex streams between formats; ConvertVertexStream_*ToFormat build one on the stack. Ego_r.pdb: no data members.
// Partial view: only the constructor (retail 0x00AC44C0, Ego_r 0x00C4BC90, out of line); ConvertFrom/ConvertTo/
// ConvertStream (retail 0x00AC44D0..) are not modelled yet.
class CVertexStreamConverter
{
public:
    CVertexStreamConverter();
};
