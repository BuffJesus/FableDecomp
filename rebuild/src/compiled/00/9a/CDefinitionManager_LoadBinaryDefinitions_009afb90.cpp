#include <new>
#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "fable_progress_display.h"
#include "fable_stlport_vector_impl.h"

bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& stream)
{
    if (stream.IsEOF())
        return false;

    bool safeBinaryFormat = stream.ReadValue<char>() != 0;
    unsigned long dependencyCRC = stream.ReadValue<unsigned long>();
    if (DependencyCheckCRC != 0 && dependencyCRC != DependencyCheckCRC)
        return false;
    if (stream.ReadValue<unsigned long>() != GDefStringTable.RandomID)
        return false;

    UseSafeBinaryFormat = safeBinaryFormat;
    ClearInstantiatedDefs();

    unsigned long defCount = stream.ReadValue<unsigned long>();
    InstantiatedDefs.resize(defCount, 0);
    for (unsigned long globalIndex = 0; globalIndex < defCount; ++globalIndex)
    {
        {
            CCharString text("Definition Manager: Load Binary Def");
            NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
        }
        InstantiatedDefs[globalIndex] = new CInstantiatedDefInfo;
        CInstantiatedDefInfo* info = InstantiatedDefs[globalIndex];
        info->ClassName.TransferBinaryIn(stream);
        info->InstantiationName.TransferBinaryIn(stream);
        info->ClassIndex = stream.ReadValue<unsigned long>();

        CDefClassInfo* classInfo = GetPDefClassInfo(GDefStringTable.GetStringCRC(info->ClassName.TablePos));
        if (classInfo)
        {
            classInfo->GlobalIndices.push_back(globalIndex);
            NameCRCToIndexMap.Insert(GDefStringTable.GetStringCRC(info->InstantiationName.TablePos), globalIndex);
        }
    }

    CompressedOffsets.resize(stream.ReadValue<unsigned long>());
    stream.Read(CompressedOffsets.GetAsCArray(), CompressedOffsets.size() * sizeof(CompressedOffsets[0]));
    CompressedDefData.resize(stream.ReadValue<unsigned long>(), 0);
    stream.Read(CompressedDefData.GetAsCArray(), CompressedDefData.size());
    NameCRCToIndexMap.Update();
    return true;
}