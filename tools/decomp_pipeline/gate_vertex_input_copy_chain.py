"""Link the recovered input binder and all nested vector bodies in one behavior fixture."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from gate_shader_resource_input import SYMBOL as BIND
from gate_shader_input_vector_assign import ASSIGN as STREAMS
from gate_vertex_declaration_assign import ASSIGN as DECLARATIONS
from gate_vertex_stream_elements import ASSIGN as ELEMENTS,CTOR
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['00/a5/CShaderResource_SetVertexShaderInput_00a5d8b0.cpp','00/a5/CVertexShaderInput_VectorAssignment_00a5e9c0.cpp','00/a5/CVertexDeclaration_VectorAssignment_00a5e2e0.cpp','00/a5/CVertexStreamFormat_ElementVectors_00a5e0a0.cpp']
TARGETS=[BIND,STREAMS,DECLARATIONS,ELEMENTS,CTOR]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/VertexInputCopyChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-input-copy-chain.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-input-copy-chain-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];records=[]
 for index,relative in enumerate(SOURCES):
  source=ROOT/'rebuild/src/compiled'/relative;obj=w/('source%d.obj'%index)
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
  objects.append(obj);records.append(dict(source=relative,sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 fixture=Coff(test);assert all(symbol not in fixture.symbols for symbol in TARGETS)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omitted in range(len(objects)):
  r=subprocess.run(command+[str(obj) for i,obj in enumerate(objects) if i!=omitted]+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'missing provider must fail link: '+SOURCES[omitted]
 r=subprocess.run(command+list(map(str,objects))+[str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_INPUT_COPY_CHAIN_BEHAVIOR_PASS scenarios=2160' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(sources=records,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),external_boundaries=['CBaseClass destructor','CRT allocation/free'],missing_provider_checks=4),indent=2)+'\n');print('VERTEX_INPUT_COPY_CHAIN_PASS compiled=2160 providers=4')
if __name__=='__main__':main()
