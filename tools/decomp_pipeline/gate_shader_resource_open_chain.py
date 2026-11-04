"""Link resource bank setup through recovered maximum-index, packed-reader and pointer-vector bodies."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from gate_shader_pointer_resize import SYMBOL as RESIZE
from gate_shader_pointer_fill import INSERT
from gate_bank_max_entry import SYMBOL as MAXIMUM
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['rebuild/src/compiled/00/a5/CShaderResource_OpenShaderBank_00a5d7c0.cpp','rebuild/src/compiled/00/9c/CBankFile_GetMaxEntryIndex_009cc350.cpp','rebuild/src/compiled/00/a5/CShaderResource_PointerResize_00a5e6a0.cpp','rebuild/src/compiled/00/a5/CShaderResource_PointerFillInsert_00a5e3b0.cpp','rebuild/src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp']
TARGETS=['?OpenShaderBank@CShaderResource@@QAEXPBVCShaderDataBank@@@Z',MAXIMUM,RESIZE,INSERT,'??ACPackedUIntArray@@QBEKK@Z']
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ShaderResourceOpenChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-resource-open-chain.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='shader-resource-open-chain-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];records=[]
 for index,relative in enumerate(SOURCES):
  source=ROOT/relative;obj=w/('source%d.obj'%index)
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
  objects.append(obj);records.append(dict(source=relative,sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 fixture=Coff(test);assert all(symbol not in fixture.symbols for symbol in TARGETS)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omitted in range(len(objects)):
  r=subprocess.run(command+[str(obj) for i,obj in enumerate(objects) if i!=omitted]+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'missing provider must fail link: '+SOURCES[omitted]
 r=subprocess.run(command+list(map(str,objects))+[str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SHADER_RESOURCE_OPEN_CHAIN_BEHAVIOR_PASS scenarios=1584' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(sources=records,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),external_boundaries=['CRT allocation/free and memmove'],missing_provider_checks=5),indent=2)+'\n');print('SHADER_RESOURCE_OPEN_CHAIN_PASS compiled=1584 providers=5')
if __name__=='__main__':main()
