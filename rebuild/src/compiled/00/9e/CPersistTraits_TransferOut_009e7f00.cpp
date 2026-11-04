// lib_definition_types.cpp, retail 009E7F00-009E8110: text output of the
// value types as "Type(a,b,c)". Floats go through GFFloatToCharString with
// six digits; colours print R, G, B, A.
#include "fable_definition_type_traits.h"
#include "fable_string.h"

// Retail 009E7F00.
void CPersistTraits<C2DVector>::TransferOut(CPersistContext& persist_context, CCharString& string_out, C2DVector& val)
{
    string_out += "C2DCoordF(";
    string_out += GFFloatToCharString(val.X, 6);
    string_out += ",";
    string_out += GFFloatToCharString(val.Y, 6);
    string_out += ")";
}

// Retail 009E7F90.
void CPersistTraits<C3DVector>::TransferOut(CPersistContext& persist_context, CCharString& string_out, C3DVector& val)
{
    string_out += "C3DCoordF(";
    string_out += GFFloatToCharString(val.X, 6);
    string_out += ",";
    string_out += GFFloatToCharString(val.Y, 6);
    string_out += ",";
    string_out += GFFloatToCharString(val.Z, 6);
    string_out += ")";
}

// Retail 009E8050.
void CPersistTraits<C3DAngles>::TransferOut(CPersistContext& persist_context, CCharString& string_out, C3DAngles& val)
{
    string_out += "C3DAngles(";
    string_out += GFFloatToCharString(val.AngleXY, 6);
    string_out += ",";
    string_out += GFFloatToCharString(val.AngleYZ, 6);
    string_out += ",";
    string_out += GFFloatToCharString(val.AngleXZ, 6);
    string_out += ")";
}

// Retail 009E8110.
void CPersistTraits<CRGBColour>::TransferOut(CPersistContext& persist_context, CCharString& string_out, CRGBColour& val)
{
    string_out += "CRGBColour(";
    string_out += GFIntToCharString(val.R);
    string_out += ",";
    string_out += GFIntToCharString(val.G);
    string_out += ",";
    string_out += GFIntToCharString(val.B);
    string_out += ",";
    string_out += GFIntToCharString(val.A);
    string_out += ")";
}
