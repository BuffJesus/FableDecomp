// CParticleProgram::OptimiseConstraints (private; retail 0x00ADFDC0, Ego_r 0x00BEB910): rewrite the compiled constraint
// program so that consecutive records of the same type (other than REPEAT / END_REPEAT, which must stay one per loop)
// become one record whose count and payload size are the sums and whose payload is the concatenation.
#include "fable_particle_program.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

void CParticleProgram::OptimiseConstraints()
{
    CMemoryDataInputStream input(Constraints);
    CMemoryDataOutputStream output;
    unsigned long runType = (unsigned long)-1;
    unsigned long runCount = 0;
    unsigned long runSize = 0;
    CMemoryDataOutputStream runData;
    while (!input.IsEOF())
    {
        unsigned long type = input.ReadULONG();
        unsigned long count = input.ReadULONG();
        unsigned long size = input.ReadULONG();
        if (type == runType && type != CONSTRAINT_REPEAT && type != CONSTRAINT_END_REPEAT)
        {
            runCount += count;
            runSize += size;
        }
        else
        {
            if (runCount)
            {
                output.WriteULONG(runType);
                output.WriteULONG(runCount);
                output.WriteULONG(runSize);
                output.Write(runData.PeekData(), runData.GetLength());
            }
            runType = type;
            runCount = count;
            runSize = size;
            runData.Clear();
        }
        input.CopyTo(runData, size);
    }
    if (runCount)
    {
        output.WriteULONG(runType);
        output.WriteULONG(runCount);
        output.WriteULONG(runSize);
        output.Write(runData.PeekData(), runData.GetLength());
    }
    Constraints.Clear();
    Constraints.Write(output.PeekData(), output.GetLength());
}
