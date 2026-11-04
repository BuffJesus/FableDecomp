"""Exercise native definition saving through its recovered stream/storage chain."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_save
from gate_compressed_offset_sort import NAMES as OFFSET_SORT_PROVIDERS
from gate_compressed_save_shrink import NAMES as SAVE_SHRINK_PROVIDERS
from gate_allocator_statistics import NAMES as ALLOCATOR_STATISTICS_PROVIDERS
from gate_memory_allocator_data import SOURCE as ALLOCATOR_DATA_SOURCE




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_SaveBinaryDefinitions_009af7a0.cpp','00/99/CMemoryDataOutputStream_Clear_00996b60.cpp','00/9b/CPersistContext_Init_Save_009baa50.cpp','00/9a/CDefinitionManager_CompressDefinitionBlock_009ae390.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']
NAMES+=['00/9d/CDefStringTable_SaveTable_009d4ce0.cpp','00/9d/CDefStringTable_WriteTable_009d4a50.cpp']
NAMES+=['00/99/CFileDataOutputStream_'+name+'.cpp' for name in ['Constructor_00994400','Destructor_00994430','SetPosition_00994450','GetPosition_00994460','Close_00994470','Write_00994480']]
NAMES+=OFFSET_SORT_PROVIDERS
NAMES+=SAVE_SHRINK_PROVIDERS
NAMES+=ALLOCATOR_STATISTICS_PROVIDERS
NAMES+=[ALLOCATOR_DATA_SOURCE]
NAMES+=['00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp']
NAMES+=['00/99/'+name+'.cpp' for name in ['CDataInputStream_CDataInputStream_00993b20','CMemoryDataOutputStream_CMemoryDataOutputStream_00996af0','CMemoryDataOutputStream_Destructor_00996b30','CMemoryDataOutputStream_SetPosition_00994b10','CMemoryDataOutputStream_GetPosition_009945b0','CMemoryDataOutputStream_Close_009945c0','CDataOutputStream_WriteEBOOL_00993e30','CDataOutputStream_WriteCBYTE_00993ea0','CDataOutputStream_WriteCBYTE_00993eb0']]
NAMES+=['00/41/STLVectorChar_resize_004122d0.cpp','00/41/STLVectorChar_Mfillinsert_004110d0.cpp']
def main():
    retail=oracle_definition_save.collect()
    w=Path(tempfile.mkdtemp(prefix='definition-save-composed-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_SaveComposed_009af7a0_test.cpp';obj=w/'enum.obj'
    fixture=test.read_text();outputs=[bytes.fromhex(row['output']) for row in retail];table='static const unsigned sizes[7]={'+','.join(str(len(v))for v in outputs)+'};\nstatic const unsigned char retailOutput[7][256]={'+','.join('{'+','.join(str(v)for v in output)+'}' for output in outputs)+'};'
    table+='\nstatic const unsigned expectedBlocks[7]={'+','.join(str(len(row['blocks'])) for row in retail)+'};'
    for label,field in [('expectedFirst','first'),('expectedCounts','count'),('expectedPayloads','payload')]:
        table+='\nstatic const unsigned '+label+'[7][4]={'+','.join('{'+','.join(str(block[field])for block in row['blocks'])+'}'for row in retail)+'};'
    table+='\nstatic const unsigned expectedOffsets[7][4][3]={'+','.join('{'+','.join('{'+','.join(str(v)for v in block['offsets'])+'}'for block in row['blocks'])+'}'for row in retail)+'};'
    fixture=fixture.replace('// RETAIL_EXPECTATIONS',table).replace('// RETAIL_COMPARE','if(output.GetLength()!=sizes[k]||memcmp(output.PeekData(),retailOutput[k],sizes[k]))return 3;if(blockCalls!=expectedBlocks[k])return 4;for(unsigned q=0;q<blockCalls;++q){if(firstIndices[q]!=expectedFirst[k][q]||blockCounts[q]!=expectedCounts[k][q]||blockSizes[q]!=expectedPayloads[k][q])return 5;for(unsigned z=0;z<blockCounts[q];++z)if(blockOffsets[q][z]!=expectedOffsets[k][q][z])return 6;}');test=w/'fixture.cpp';test.write_text(fixture)
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?SaveBinaryDefinitions@CDefinitionManager'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test),'kernel32.lib'],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_SAVE_OK cases=7' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-save-composed.json').write_text(json.dumps(dict(address='009af7a0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=1007,linked_cases=7,retail_cases=retail,native_providers=NAMES,mocked_boundaries='disk operations, string/base lifetime, def virtual transfer, compressor runtime and recording size-class allocators; unused input-stream slots abort if called; memory/file output streams, scalar output, string-table save, offset sorting, data/offset compaction and allocator statistics are native; allocator locking uses real Win32 critical sections'),indent=2)+'\n')
    print('DEFINITION_SAVE_COMPOSED_PASS cases=7 bytes='+str(len(b)))
if __name__=='__main__':main()

