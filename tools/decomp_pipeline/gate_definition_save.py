"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_save




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_SaveBinaryDefinitions_009af7a0.cpp','00/99/CMemoryDataOutputStream_Clear_00996b60.cpp','00/9b/CPersistContext_Init_Save_009baa50.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']
def main():
    retail=oracle_definition_save.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_SaveBinaryDefinitions_009af7a0_test.cpp';obj=w/'enum.obj'
    fixture=test.read_text();outputs=[bytes.fromhex(row['output']) for row in retail];table='static const unsigned sizes[7]={'+','.join(str(len(v))for v in outputs)+'};\nstatic const unsigned char retailOutput[7][256]={'+','.join('{'+','.join(str(v)for v in output)+'}' for output in outputs)+'};'
    table+='\nstatic const unsigned expectedBlocks[7]={'+','.join(str(len(row['blocks'])) for row in retail)+'};'
    for label,field in [('expectedFirst','first'),('expectedCounts','count'),('expectedPayloads','payload')]:
        table+='\nstatic const unsigned '+label+'[7][4]={'+','.join('{'+','.join(str(block[field])for block in row['blocks'])+'}'for row in retail)+'};'
    table+='\nstatic const unsigned expectedOffsets[7][4][3]={'+','.join('{'+','.join('{'+','.join(str(v)for v in block['offsets'])+'}'for block in row['blocks'])+'}'for row in retail)+'};'
    fixture=fixture.replace('// RETAIL_EXPECTATIONS',table).replace('// RETAIL_COMPARE','if(output.GetLength()!=sizes[k]||memcmp(output.PeekData(),retailOutput[k],sizes[k]))return 3;if(blockCalls!=expectedBlocks[k])return 4;for(unsigned q=0;q<blockCalls;++q){if(firstIndices[q]!=expectedFirst[k][q]||blockCounts[q]!=expectedCounts[k][q]||blockSizes[q]!=expectedPayloads[k][q])return 5;for(unsigned z=0;z<blockCounts[q];++z)if(blockOffsets[q][z]!=expectedOffsets[k][q][z])return 6;}');test=w/'fixture.cpp';test.write_text(fixture)
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?SaveBinaryDefinitions@CDefinitionManager'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_SAVE_OK cases=7' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-save.json').write_text(json.dumps(dict(address='009af7a0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=1007,linked_cases=7,retail_cases=retail,mocked_boundaries='streams, string table save, def virtual transfer, block compressor, offset sort/shrink and allocator stats'),indent=2)+'\n')
    print('DEFINITION_SAVE_PASS cases=7 bytes='+str(len(b)))
if __name__=='__main__':main()

