"""Complete native shader-bank entry constructor and retail state replay."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from shader_bank_entry_targets import targets
import oracle_native_shader_bank_entry as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-bank-entry-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CShaderBankEntry_Constructor_00a62120.cpp';fixture=ROOT/'rebuild/tests/integration/NativeShaderBankEntry_test.cpp'
boundaries={0x99a2f0:('BaseCtor',4),0x99e4b0:('CharDefault',4),0x9cc240:('EntrySize',12),0x9cc2a0:('EntryRead',24),0x994930:('MemoryCtor',16),0x993ca0:('ReadOverflow',16),0x464931:('ResizeCode',16),0x995d30:('ReadString',12),0x99ec30:('CopyString',12),0x433530:('GrowStrings',28),0x99eae0:('DestroyString',4),0x99efb0:('AssignString',12),0x993b10:('MemoryDtor',4)}
aliases=[]
for name,address in targets.items():
 if address in boundaries:
  target,count=boundaries[address];aliases.append('#pragma comment(linker,"/alternatename:'+name+'=@'+target+'@'+str(count)+'")')
aliases.append('#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_UnusedShaderEntryBoundary")')
(w/'shader_entry_aliases.h').write_text('\n'.join(aliases)+'\n')
objects=[]
for name,path,extra in [('production',source,[]),('replay',source,['/Dmalloc=FableEntryMalloc','/Dfree=FableEntryFree']),('fixture',fixture,[])]:
 obj=w/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)]+extra,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
symbol='??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z';proof=check(objects[0],symbol,0xa62120,678,20,targets)
original,renamed=Coff(objects[0]),Coff(objects[1]);sec,off=original.symbols[symbol];other,otheroff=renamed.symbols[symbol];body,rel=original.sections[sec];otherbody,otherrel=renamed.sections[other]
rename={'_malloc':'_FableEntryMalloc','_free':'_FableEntryFree'};assert body==otherbody and off==otheroff and {p:(rename.get(t,t),k)for p,(t,k)in rel.items()}==otherrel
original.table('??_7CShaderBankEntry@@6B@',['??_ECShaderBankEntry@@UAEPAXI@Z'])
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[2])],env=env,capture_output=True,text=True);assert r.returncode,'native source required'
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));stride=oracle.WIDTH+48;raise AssertionError((w,difference,difference//stride,difference%stride,len(actual),len(expected)))
report.update(providers=[proof],omission_checks=1,compared_bytes=len(actual),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual native constructor and inline memory zero/read paths. Full16KiB plus28-byte local stream at every dependency/return; vtable words normalized. Bank/base/string/vector/stream-construction and overflow boundaries, allocation callbacks. Zero/nonzero bank sizes, code lengths0/1/3 words with remainders, constant counts0/1/3, spare/full string vectors, chunk limits and signed-position suppression/wraparound. No shader execution.')
(ROOT/'rebuild/compile-gate/native-shader-bank-entry.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
