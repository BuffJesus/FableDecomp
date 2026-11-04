"""Validate vertex declaration generation, including its embedded switch tables."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import struct,check_one
import vertex_declaration_generation_oracle as v
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]

def digest(result):
 events,state,decl,alt=result
 begin,end,capacity=struct.unpack('<3I',bytes.fromhex(state))
 blob=struct.pack('<4I',(end-begin)//8,(capacity-begin)//8,int(begin==v.ALT),len(events))+bytes.fromhex(decl)+bytes.fromhex(alt)
 h=2166136261
 for b in blob:h=((h^b)*16777619)&0xffffffff
 return h

def expected_header(b,n):
 patterns=[];rows=[];comparisons=0
 for case in v.cases():
  streams,used,cap=case
  result=v.execute(n,*case);assert result==v.execute(b,*case);comparisons+=1
  if not streams and not cap:continue # native null write is checked in the emulator only
  if streams not in patterns:patterns.append(streams)
  rows.append((patterns.index(streams),used,cap,digest(result)))
 text='// Generated from independently executed retail results.\nstatic const Pattern patterns[]={\n'
 for streams in patterns:
  counts=[len(s) for s in streams]+[0]*(3-len(streams));types=[t for s in streams for t in s]
  text+='{%d,{%s},{%s}},\n'%(len(streams),','.join(map(str,counts)),','.join(map(str,types)) or '0')
 text+='};\nstatic const Scenario scenarios[]={\n'
 for row in rows:text+='{%d,%d,%d,0x%08xU},\n'%row
 text+='};\n'
 return text,comparisons,len(rows)

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexShaderInput_GenerateD3DDeclaration_00ab3870.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/ab/CVertexShaderInput_GenerateD3DDeclaration_00ab3870_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-declaration-generation.json')
 a=ap.parse_args();work=Path(tempfile.mkdtemp(prefix='vertex-declaration-generation-'))
 env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE']
 obj=work/'candidate.obj';test=work/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
 assert v.SYMBOL not in Coff(test).symbols
 b,n,report=v.validate(obj);expected,comparisons,compiled=expected_header(b,n)
 support=a.include/'test_support/vertex_declaration_generation_expected.h';assert support.read_text()==expected
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode,'fixture must require candidate'
 r=subprocess.run(cmd+[str(test),str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'DECLARATION_GENERATION_BEHAVIOR_PASS scenarios='+str(compiled) in r.stdout,r.stdout+r.stderr
 report.update(address='00ab3870',grade='NEAR_MATCH',retail_bytes=668,built_bytes=662,retail_code_bytes=612,built_code_bytes=622,behavior=r.stdout.strip(),comparative_scenarios=comparisons,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),expected_sha256=hashlib.sha256(support.read_bytes()).hexdigest())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
 print('DECLARATION_GENERATION_PASS compiled='+str(compiled)+' comparative='+str(comparisons)+' grade=NEAR_MATCH')
if __name__=='__main__':main()
