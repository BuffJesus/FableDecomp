// CDefinitionManager::Compile() @ 0x009B08C0 (FableWin
// ?Compile@CDefinitionManager@@QAEXXZ). Checks the def sources' dependencies,
// then loads the compiled def file when compiled defs are in use and it
// exists; otherwise it builds the symbols from the symbol path list and
// compiles the compile path list. After compiling it trims memory and, when
// GFMain asked for it, writes the compiled file. The GFGetTime calls are the
// timing points of Ego_r's debug output, which retail compiles out.
#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_disk_file.h"

double FABLE_FASTCALL GFGetTime();

// ?CreatingCompiledDefs@CDefinitionManager@@1_NA, retail 0x0138E188 (initialised
// to 1; GFMain overwrites it with AllowDataGeneration).
bool CDefinitionManager::CreatingCompiledDefs = true;

void CDefinitionManager::Compile()
{
    GFGetTime();
    CheckDependencies(SymbolPathList);
    GFGetTime();
    if (UsingCompiledDefs && CAFile::PathExists(CompiledFileName))
    {
        CDiskFileWin32 file;
        file.Open(CompiledFileName, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(&file, 0x4000);
        GFGetTime();
        if (LoadBinaryDefinitions(stream))
        {
            GFGetTime();
            return;
        }
    }

    GFGetTime();
    CreateSymbolsFromPathList(SymbolPathList, true);
    {
        CCharString prefix("");
        DoCompilePathList(CompilePathList, prefix);
    }
    GFGetTime();
    RemoveUnusedMemory();
    if (CreatingCompiledDefs)
    {
        CDiskFileWin32 file;
        file.Open(CompiledFileName, FILE_ACCESS_CREATE, 4);
        CMemoryDataOutputStream stream;
        GFGetTime();
        SaveBinaryDefinitions(stream);
        file.Write(stream.PeekData(), stream.GetLength(), 0);
        GFGetTime();
        file.Close();
    }
}