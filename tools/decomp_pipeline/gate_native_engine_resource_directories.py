"""Typed mesh/cache getters, writable cache shell path and temporary lifetime."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
import oracle_native_engine_resource_directories as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-resource-directories-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/41/NResourceDirectoryNames_EngineDirectories_0041a1c0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeEngineResourceDirectories_test.cpp'
specs=[('GetMeshBankDir',0x41a1c0,17,0x122f454,'Data\\Meshes\\'),('GetEngineCacheDir',0x41a200,17,0x122f48c,'Data\\EngineCache\\'),('GetWriteableEngineCacheDir',0x41a220,99,0x122f4b0,'\\Microsoft\\Fable\\')]
symbols=['?'+name+'@NResourceDirectoryNames@@YI?AVCWideString@@XZ'for name,*_ in specs]
aliases=dict(zip(['@GetMesh@4','@GetCache@4','@GetWriteCache@4'],symbols));aliases.update({'??0CWideString@@QAE@PBG@Z':'@ConstructWide@12','??1CWideString@@QAE@XZ':'@DestroyWide@4','??H@YI?AVCWideString@@ABV0@PBG@Z':'@AppendWide@12','_g_FableGetFolderPathW_0041A430':'_FolderAPI'})
(w/'engine_resource_directories_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/GS','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={'??0CWideString@@QAE@PBG@Z':0x99b6b0,'??1CWideString@@QAE@XZ':0x99b510,'??H@YI?AVCWideString@@ABV0@PBG@Z':0x99bf30,'___security_cookie':0x139c8a8,'@__security_check_cookie@4':0xbfe9f9,'_g_FableGetFolderPathW_0041A430':0x1440280}
c=Coff(objects[0]);raw=EXE.read_bytes();proof=[]
for symbol,(_,address,size,literal,text)in zip(symbols,specs):
 sec,off=c.symbols[symbol];body,rel=c.sections[sec]
 for _,(name,_)in rel.items():
  if name.startswith('??_C@'):
   targets[name]=literal;section,offset=c.symbols[name];value=(text+'\0').encode('utf-16le');assert c.sections[section][0][offset:offset+len(value)]==read_virtual_data(raw,literal,len(value))==value
 proof.append(check(objects[0],symbol,address,size,len(rel),targets))
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//856,difference%856,len(actual),len(expected)))
report.update(providers=proof,omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Full256-byte result arena and584bytes of observed UTF16 text. Exact literals and hidden return ownership. Writable path uses recorded SHGetFolderPathW with initialized empty/Unicode/trailing-separator/max259-unit buffers and success/failure HRESULTs, concatenation and temporary destruction. Callback mutations; actual compiled security-cookie check. No OS folder calls or normalized bytes.')
(ROOT/'rebuild/compile-gate/native-engine-resource-directories.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
