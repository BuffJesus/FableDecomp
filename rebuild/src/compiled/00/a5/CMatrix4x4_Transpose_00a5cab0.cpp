#include "fable_matrix4x4.h"
void CMatrix4x4::Transpose()
{
    __declspec(align(16)) CMatrix4x4 copy=*this;
    for(int row=0;row<4;++row){
        for(int column=0;column<4;++column)
            Data[row][column]=copy.Data[column][row];
    }
}
