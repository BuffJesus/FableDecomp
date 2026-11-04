// lib_data_stream.cpp, retail 0x00995030 / 0x009950F0 / 0x00995210 / 0x00995340
// (Ego_r CDataInputStream::Read2DVector / Read3DVector / Read3DAngles /
// ReadMatrix3x4): each float
// component through the inline Read, then the value built from them.
#include "fable_data_input_stream.h"
#include "fable_2d_vector.h"
#include "fable_definition_type_traits.h"
#include "engine/CMatrix3x4.h"

// Retail 00995030.
C2DVector CDataInputStream::Read2DVector()
{
    float x;
    float y;
    Read(&x, 4);
    Read(&y, 4);
    return C2DVector(x, y);
}

// Retail 009950F0.
C3DVector CDataInputStream::Read3DVector()
{
    float x;
    float y;
    float z;
    Read(&x, 4);
    Read(&y, 4);
    Read(&z, 4);
    return C3DVector(x, y, z);
}

// Retail 00995210: each angle through one read temporary (ReadValue).
C3DAngles CDataInputStream::Read3DAngles()
{
    C3DAngles angles;
    angles.AngleXY = ReadValue<float>();
    angles.AngleYZ = ReadValue<float>();
    angles.AngleXZ = ReadValue<float>();
    return angles;
}

// Retail 00995340: twelve floats, row by row, each through a read temporary.
CMatrix3x4 CDataInputStream::ReadMatrix3x4()
{
    CMatrix3x4 matrix;
    matrix.E11 = ReadValue<float>();
    matrix.E12 = ReadValue<float>();
    matrix.E13 = ReadValue<float>();
    matrix.E21 = ReadValue<float>();
    matrix.E22 = ReadValue<float>();
    matrix.E23 = ReadValue<float>();
    matrix.E31 = ReadValue<float>();
    matrix.E32 = ReadValue<float>();
    matrix.E33 = ReadValue<float>();
    matrix.E41 = ReadValue<float>();
    matrix.E42 = ReadValue<float>();
    matrix.E43 = ReadValue<float>();
    return matrix;
}
