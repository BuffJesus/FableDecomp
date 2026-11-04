#include <new>
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_persist_context.h"
#include "fable_memory_allocator.h"
#include "fable_stlport_vector_impl.h"

void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream& output)
{
    GDefStringTable.SaveTable(false);
    output.WriteEBOOL(UseSafeBinaryFormat);
    output.WriteULONG(DependencyCheckCRC);
    output.WriteULONG(GDefStringTable.RandomID);
    unsigned long count = InstantiatedDefs.size();
    output.WriteULONG(count);

    CMemoryDataOutputStream block;
    CArrayVector<unsigned short> offsets;
    CompressedDefData.clear();
    CompressedOffsets.clear();
    unsigned long firstGlobalIndex = 0;
    for (unsigned long i = 0; i < count; ++i)
    {
        CInstantiatedDefInfo* info = InstantiatedDefs[i];
        output.WriteSLONG(info->ClassName.TablePos);
        output.WriteSLONG(info->InstantiationName.TablePos);
        output.WriteULONG(info->ClassIndex);
        CMemoryDataOutputStream data;
        info->PDef->InternalSaveBinary(data);
        CMemoryDataOutputStream fields;
        CPersistContext context;
        context.Init(fields, UseSafeBinaryFormat);
        info->PDef->Transfer(context);
        data.Write(fields.PeekData(), fields.GetLength());

        unsigned long currentBytes = block.GetLength() + offsets.size() * 2;
        if (currentBytes && currentBytes + data.GetLength() + 2 > 0x8000)
        {
            CompressDefinitionBlock(block, offsets, firstGlobalIndex);
            firstGlobalIndex = i;
        }
        offsets.push_back(static_cast<unsigned short>(block.GetLength()));
        block.Write(data.PeekData(), data.GetLength());
    }
    CompressDefinitionBlock(block, offsets, firstGlobalIndex);
    CompressedOffsets.Insert(count, CompressedDefData.size());
    CompressedOffsets.Update();
    CompressedDefData.DeallocateUnusedMemory();
    CompressedOffsets.DeallocateUnusedMemory();
    output.WriteULONG(CompressedOffsets.size());
    output.Write(CompressedOffsets.GetAsCArray(), CompressedOffsets.size() * sizeof(CompressedOffsets[0]));
    output.WriteSLONG(CompressedDefData.size());
    output.Write(CompressedDefData.GetAsCArray(), CompressedDefData.size());

    if (CMemoryAllocator::Get())
    {
        CMemoryAllocator::Get()->GetTotalMemoryAllocated();
        CMemoryAllocator::Get()->GetNoAllocatedAreas();
    }
    for (unsigned long i = 0; i < count; ++i)
        InstantiatedDefs[i]->PDef.Reset(0);
    if (CMemoryAllocator::Get())
    {
        CMemoryAllocator::Get()->GetTotalMemoryAllocated();
        CMemoryAllocator::Get()->GetNoAllocatedAreas();
    }
}
