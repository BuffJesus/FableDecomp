import argparse,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
import oracle_native_shader_lookup as oracle
from variable_allocator_parity import check
from gate_game_component_abi import Coff
parser=argparse.ArgumentParser();parser.add_argument('--bank-symbols',action='store_true');compose=parser.parse_args().bank_symbols
w=Path(tempfile.mkdtemp(prefix='native-shader-lookup-'));env=V.env()
targets={'?GetVertexShader@CShaderDataBank@@QBEPAVCShaderBankEntry@@K@Z':0xa61fa0,'??2@YAPAXI@Z':0xbfea1a,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z':0x9cc1d0,'??0CVertexShader@@AAE@ABV?$CArrayVector@K@@PBU_D3DVERTEXELEMENT9@@ABVCCharString@@2@Z':0xa63020,'??1CCharString@@QAE@XZ':0x99eae0,'??_7CVertexShader@@6B@':0x129b3d0,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0,'?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@K@Z':0xa5d510}
specs=[('00/a5/CShaderResource_GetVertexShaderIndex_00a5d510.cpp','?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@K@Z',0xa5d510,212,9),('00/a5/CShaderResource_GetVertexShaderName_00a5d5f0.cpp','?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@ABVCCharString@@@Z',0xa5d5f0,37,2),('00/a6/CShaderDataBank_GetVertexShader_00a61fa0.cpp','?GetVertexShader@CShaderDataBank@@QBEPAVCShaderBankEntry@@K@Z',0xa61fa0,16,0)]
if compose:
 specs.append(('00/9c/CBankFile_GetEntrySymbol_009cc1d0.cpp','?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z',0x9cc1d0,57,3))
 targets.update({'??_C@_09ONJBBFFA@?$CIUnknown?$CJ?$AA@':0x129a03c,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0})
objects=[];reports=[]
for i,(source,symbol,address,length,count) in enumerate(specs):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,w);assert not r.returncode,r.stdout+r.stderr
 reports.append(check(obj,symbol,address,length,count,targets));objects.append(obj)
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeShaderLookup_test.cpp')]+(['/DFABLE_COMPOSE_BANK_SYMBOLS']if compose else[]),env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(len(specs)):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission',omit)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
coff=Coff(objects[0]);coff.table('??_7CVertexShader@@6B@',['??_ECVertexShader@@UAEPAXI@Z','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CVertexShader@@EAEKXZ'])
sec,offset=coff.symbols[specs[0][1]];body,relocs=coff.sections[sec];patched=bytearray(body)
for site,(name,kind) in relocs.items():
 value=targets[name]+int.from_bytes(body[site:site+4],'little')-(0xa5d510+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff)
fault_cases=oracle.allocation_failure(patched)
inputs,expected,report=oracle.collect(compose);(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected)
 difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
 raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12),len(actual),len(expected)))
report.update(allocation_failure_comparisons=fault_cases,providers=reports,omission_checks=len(specs),boundary='Actual typed resource lookups and bank indexing. String lookup/copy/destruction, entry symbol (native when --bank-symbols), allocation and vertex creation boundaries. Callback-mutated bank/vector/declaration. Cached and new handles, null data, reference wraparound. Vtable words normalized; all other arena bytes compared.')
(ROOT/('rebuild/compile-gate/native-shader-lookup-bank-symbols.json'if compose else'rebuild/compile-gate/native-shader-lookup.json')).write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
