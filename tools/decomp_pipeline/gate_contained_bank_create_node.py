"""Original-exact contained-bank allocation/copy and strict allocator provenance."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
ROOT=Path(__file__).resolve().parents[2]
SOURCE=ROOT/'rebuild/src/compiled/00/9a/CContainedBankMap_CreateNode_009aa9e0.cpp'
FIXTURE=ROOT/'rebuild/tests/00/9a/CContainedBankMap_CreateNode_009aa9e0_test.cpp'
REPORT=ROOT/'rebuild/compile-gate/contained-bank-create-node.json'
SYMBOL='?CreateNode@CContainedBankMap@NBankFileManager@@QAEPAUCNode@12@ABU?$pair@$$CBVCCharString@@UCContainedBank@NBankFileManager@@@std@@@Z'

def audit(obj):
 c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert not off and len(body)==76
 assert [rels[site][0] for site in sorted(rels)]==['_malloc','??0CCharString@@QAE@ABV0@@Z']
 fixed=bytearray(body);targets={'_malloc':0xbfea0e,'??0CCharString@@QAE@ABV0@@Z':0x99ec30}
 for site,(name,kind) in rels.items():
  assert kind==20;struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(0x9aa9e0+site+4))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),0x9aa9e0,76)
 return body,rels

def main():
 work=Path(tempfile.mkdtemp(prefix='contained-bank-create-node-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
 inputs={SOURCE,FIXTURE,Path(__file__),EXE};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs}
 def compile(source,name,flags=()):
  obj=work/(name+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT',*flags,'/Fo'+str(obj),str(source)],env,work);assert not result.returncode,result.stdout+result.stderr;return obj
 provider=compile(SOURCE,'primary');host=compile(FIXTURE,'fixture');body,rels=audit(provider)
 raw=EXE.read_bytes();thunk=read_virtual_data(raw,0xbfea0e,6);assert thunk[:2]==b'\xff\x25' and import_names(raw)[struct.unpack_from('<I',thunk,2)[0]]=='malloc'
 hooked=compile(SOURCE,'hooked',['/Dmalloc=FixtureContainedMalloc']);c=Coff(hooked);sec,off=c.symbols[SYMBOL];hook_body,hook_rels=c.sections[sec];assert not off and hook_body==body
 assert [hook_rels[site][0] for site in sorted(hook_rels)]==['_FixtureContainedMalloc','??0CCharString@@QAE@ABV0@@Z']
 text=SOURCE.read_text();assert text.count('malloc(')==1;fault=work/'wrong-allocator.cpp';fault.write_text(text.replace('malloc(', '::operator new('));wrong=compile(fault,'wrong');rejected=False
 try:audit(wrong)
 except AssertionError:rejected=True
 assert rejected,'operator new accepted as malloc'
 def link(name,objects):return subprocess.run([str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/(name+'.exe')),*map(str,objects)],cwd=work,env=env,capture_output=True,text=True)
 result=link('omitted',[host]);assert result.returncode,'omitted provider linked'
 for name,obj,code in [('positive',hooked,0),('wrong-allocator',wrong,80)]:
  result=link(name,[obj,host]);assert not result.returncode,result.stdout+result.stderr
  result=subprocess.run([str(work/(name+'.exe'))],cwd=work,env=env,capture_output=True,text=True,timeout=30)
  (work/(name+'.run.json')).write_text(json.dumps(dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2));assert result.returncode==code,(name,result.returncode,result.stdout,result.stderr)
  assert 'CONTAINED_BANK_CREATE_NODE_PASS' in result.stdout if code==0 else 'CONTAINED_BANK_WRONG_ALLOCATOR' in result.stderr
 for p,h in pins.items():assert sha(Path(p))==h
 report=dict(grade='RELOCATION_MATCH',address='009aa9e0',bytes=76,missing_provider_checks=1,allocator_import='malloc',allocator_target_control=True,allocator_runtime_control=True,build=str(work),source_sha256=sha(SOURCE),fixture_sha256=sha(FIXTURE),input_sha256=pins,object_sha256={str(p):sha(p) for p in (provider,hooked,host,wrong)},boundary='Primary native COFF fully relocated to original bytes;malloc import identity from PE. Host hook has byte-identical body and renamed allocation call;typed value copy and allocation-size/counter checked. Wrong new call rejects original-byte proof and exits80 through explicit host guard. No OOM/unwind claim.')
 REPORT.write_text(json.dumps(report,indent=2)+'\n');print('CONTAINED_BANK_CREATE_NODE_PASS 76/76 omitted=1 allocator=malloc controls=2')
if __name__=='__main__':main()
