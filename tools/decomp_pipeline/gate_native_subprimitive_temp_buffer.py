import hashlib,json,re,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-temp-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b8/SubPrimitiveSort_TemporaryBufferAllocate_00b84e20.cpp';fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveTemporaryBuffer_test.cpp'
root='?_M_allocate_buffer@?$_Temporary_buffer@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@@_STL@@AAEXXZ'
(w/'subprimitive_temp_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Allocate@8='+root+'")\n')
body=read_virtual_data(EXE.read_bytes(),0xb84e20,75);assert body[43]==0xe8
(w/'subprimitive_temp_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
objects=[]
for n,p in enumerate([source,fixture]):
 o=w/(str(n)+'.obj');flags=['/Dmalloc=NativeTempMalloc'] if n==0 else [];r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),*flags,str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb84e20,75,1,{'_NativeTempMalloc':0xbfea0e});assert proof['grade']=='MATCH'
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip());m=re.search(r'cases=(\d+) snapshots=(\d+)',r.stdout);assert m and int(m[1])==512
report=dict(cases=512,snapshots=int(m[2]),providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual temporary-buffer allocator,512cases/full64KiB snapshots;signed negative/zero/positive/extreme lengths,INT_MAX clamp,allocation failures/halving and callback mutation of length/original length/buffer/neighbour fields. CRT malloc is redirected only in fixture build;no stable sorting/drawing claim.')
(ROOT/'rebuild/compile-gate/native-subprimitive-temp-buffer.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SUBPRIMITIVE_TEMP_BUFFER_GATE_PASS')
