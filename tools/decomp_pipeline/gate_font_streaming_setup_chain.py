"""Link the complete recovered streaming-font setup with actual bank providers."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['rebuild/src/compiled/00/a5/CStreamingFontDataBank_InitStreamingEntry_00a5ef50.cpp','rebuild/src/compiled/00/9c/CBankFile_FindIndexBySymbol_009ccdc0.cpp','rebuild/src/compiled/00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp','rebuild/src/compiled/00/9c/CBankFile_EntryAccessors_009cc210.cpp','rebuild/src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp']
from gate_font_streaming_entry import SYMBOL
from gate_bank_symbol_index import LOOKUP,FIND
from gate_bank_async_entry import FACTORY,ALIGN
TARGETS=[SYMBOL,LOOKUP,FIND,FACTORY,ALIGN,'?GetEntryDataSize@CBankFile@@QBEKK@Z','??ACPackedUIntArray@@QBEKK@Z']

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/FontStreamingSetupChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/font-streaming-setup-chain.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='font-streaming-setup-chain-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];records=[]
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
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'FONT_STREAMING_SETUP_CHAIN_BEHAVIOR_PASS scenarios=360' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(sources=records,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),external_boundaries=['allocation/free','CCharString construction/destruction','CBaseClassNonCopyable lifetime'],missing_provider_checks=5),indent=2)+'\n');print('FONT_STREAMING_SETUP_CHAIN_PASS compiled=360 providers=5')
if __name__=='__main__':main()
