import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-managed-array-helpers-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CArray_ManagedTextureData_DeallocateMemory_00a6be40.cpp';obj=w/'helpers.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj)
copy=[s for s in c.symbols if '__copy@' in s];destroy=[s for s in c.symbols if '__destroy_aux' in s];assert len(copy)==len(destroy)==1;proofs=[];body=bytearray(192)
for symbol,address,length,offset in [(copy[0],0xa6b080,82,0),(destroy[0],0xa6b700,50,128)]:proofs.append(check(obj,symbol,address,length,0,{}));body[offset:offset+length]=read_virtual_data(EXE.read_bytes(),address,length)
(w/'managed_array_helpers_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'managed_array_helpers_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@CopyRoot@20',copy[0]),('@DestroyRoot@12',destroy[0])])+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureArrayHelpers_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=61440 snapshots=' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CPreallocTexturePool.h','fable_managed_texture_data.h','fable_stlport_vector.h','fable_stlport_algobase.h','fable_iv_counted_pointer.h','fable_resource_list.h','fable_carray.h','rebuild_abi.h']]
report=dict(cases=61440,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Supporting original82B class-element copy/50B destruction helpers execute actual emitted source, fullMATCH and allno-relocation bodies.61440cases/full16KiB virtualdelete/final snapshots,empty/nonempty ranges0..4,NULL/shared/distinct element owners,forward/backward/identity/disjoint overlaps,refcounts unsigned endpoints,cached-bound callbacks that replace source/destination slots andnext counts,two placements-fills. Original forward-copy behavior preserved, no memmove overlap safety invented. Virtualownerdeletion remainsfunctionalboundary. One sourceomission fails; strengthening two supportinghelper proofs only,zero additionalfunction/data coverage.')
(ROOT/'rebuild/compile-gate/native-managed-texture-array-helpers.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_ARRAY_HELPERS_GATE_PASS')
