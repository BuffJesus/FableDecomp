"""Full native shader-entry notification and pointer-vector overflow replay."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import oracle_native_shader_bank_add as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-bank-add-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CShaderDataBank_NotifyEntryAdded_00a623d0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderBankAdd_test.cpp'
parent='?NotifyEntryAdded@CShaderDataBank@@MAEXKKABV?$CArray@D@@@Z';overflow='?_M_insert_overflow@?$vector@PAVCShaderBankEntry@@V?$allocator@PAVCShaderBankEntry@@@_STL@@@_STL@@QAEXPAPAVCShaderBankEntry@@ABQAV3@ABU__true_type@2@I_N@Z'
targets={overflow:0xa62740,'??2@YAPAXI@Z':0xbfea1a,'??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z':0xa62120,'?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z':0x9cbc20,'_malloc':0xbfea0e,'_memmove':0xbfeae6,'_free':0xbfea14}
objects=[]
for name,path,extra in [('production',source,[]),('replay',source,['/Dmalloc=FableShaderMalloc','/Dfree=FableShaderFree','/Dmemmove=FableShaderMemmove']),('fixture',fixture,[])]:
 obj=w/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=[check(objects[0],parent,0xa623d0,174,4,targets),check(objects[0],overflow,0xa62740,216,4,targets)]
original,renamed=Coff(objects[0]),Coff(objects[1]);rename={'_malloc':'_FableShaderMalloc','_free':'_FableShaderFree','_memmove':'_FableShaderMemmove'}
for name in [parent,overflow]:
 sec,off=original.symbols[name];other,otheroff=renamed.symbols[name];body,rel=original.sections[sec];otherbody,otherrel=renamed.sections[other]
 assert body==otherbody and off==otheroff and {p:(rename.get(t,t),k)for p,(t,k)in rel.items()}==otherrel
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[2])],env=env,capture_output=True,text=True);assert r.returncode,'native source required'
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));stride=oracle.WIDTH+20;raise AssertionError((w,difference,difference//stride,difference%stride,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual typed notification and generic pointer overflow. Full16KiB at every allocation/copy/free/constructor/base notification and return. Notification: supported/unsupported types, gaps/spare capacity, entry allocation failure and constructor storage swap. Overflow independently covers all positions0..6, count0/1/3, at-end flag, aliased fill and mutating malloc/free. CRT dependencies are recorded and audited renamed relocations; shader entry construction/base callback remain boundaries.')
(ROOT/'rebuild/compile-gate/native-shader-bank-add.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
