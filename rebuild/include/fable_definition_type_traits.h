#pragma once

// lib_definition_types.cpp: the text persist traits of the engine's value
// types. Each is an empty class with thiscall members (Ego_r
// CPersistTraits<C2DVector/C3DVector/C3DAngles/CRGBColour>); the text names are
// the old ones (C2DCoordF / C3DCoordF).

#include "fable_persist_context.h"
#include "fable_2d_vector.h"
#include "fable_rgb_colour.h"

class CCharString;
class CStringParser;

// Ego_r C3DAngles (12 bytes): AngleXY, AngleYZ, AngleXZ.
class C3DAngles
{
public:
    float AngleXY;
    float AngleYZ;
    float AngleXZ;
};

// Ego_r CPersistTraits<float>; the text reader is retail 0x0040F7E0 (not landed).
template <>
class CPersistTraits<float>
{
public:
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, float& val);
};

template <>
class CPersistTraits<C2DVector>
{
public:
    // Retail 0x009E7F00.
    void TransferOut(CPersistContext& persist_context, CCharString& string_out, C2DVector& val);
    // Retail 0x009E81E0.
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, C2DVector& val);
};

template <>
class CPersistTraits<C3DVector>
{
public:
    // Retail 0x009E7F90.
    void TransferOut(CPersistContext& persist_context, CCharString& string_out, C3DVector& val);
    // Retail 0x009E84D0.
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, C3DVector& val);
};

template <>
class CPersistTraits<C3DAngles>
{
public:
    // Retail 0x009E8050.
    void TransferOut(CPersistContext& persist_context, CCharString& string_out, C3DAngles& val);
    // Retail 0x009E8870.
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, C3DAngles& val);
};

template <>
class CPersistTraits<CRGBColour>
{
public:
    // Retail 0x009E8110.
    void TransferOut(CPersistContext& persist_context, CCharString& string_out, CRGBColour& val);
    // Retail 0x009E8C10.
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, CRGBColour& val);
};
