"""Verify allocation tree queries against retail and independent minimum-size bounds."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_tree_queries as oracle
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp'
METHODS=[('GetTreeAreaWithSizeGreaterThanOrEqualTo',0xa64c10,55,'IBEPAVCMemoryArea@1@J@Z'),('GetLargestTreeAreaSize',0xa64c50,27,'IBEJXZ'),('GetSmallestTreeAreaSize',0xa64c70,27,'IBEJXZ')]

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-tree-queries-'));env=check_one.env();obj=work/'queries.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);raw=EXE.read_bytes();sections=pe_sections(raw);reports=[]
    for name,address,length,signature in METHODS:
        section,offset=coff.symbols['?'+name+'@CMemoryAllocatorVariableSize@@'+signature];body,relocs=coff.sections[section]
        at=va_to_off(sections,address);assert offset==0 and not relocs and len(body)==length and body==raw[at:at+length]
        reports.append(dict(address=f'{address:08x}',bytes=length,grade='MATCH'))
    fixture=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(ROOT/'rebuild/tests/integration/VariableTreeQueries_test.cpp')],env,work);assert result.returncode==0,result.stdout+result.stderr
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(bytes.fromhex(row['state']));data.extend(struct.pack('<I',len(row['queries'])))
        data.extend(b''.join(struct.pack('<2I',request&0xffffffff,result)for request,result in row['queries']))
        data.extend(struct.pack('<2I',row['largest'],row['smallest']))
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    result=subprocess.run(link+[str(fixture)],env=env,capture_output=True,text=True);assert result.returncode!=0,'query provider omitted'
    result=subprocess.run(link+[str(obj),str(fixture)],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);calls=sum(len(r['queries'])+2 for r in rows)
    assert result.returncode==0 and f'VARIABLE_TREE_QUERIES_OK states={len(rows)} calls={calls}'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/variable-tree-queries.json').write_text(json.dumps(dict(providers=reports,states=len(rows),linked_calls=calls,retail_calls=calls,omitted_provider_checks=1,compared_unchanged_bytes=oracle.WIDTH,invariants='smallest signed eligible size, returned node membership, signed min/max, full state unchanged; one empty tree plus 530 retail-built insertion states',boundaries='none in tree queries'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_TREE_QUERIES_PASS bytes=109 states={len(rows)} calls={calls} omitted=1')

if __name__=='__main__':main()
