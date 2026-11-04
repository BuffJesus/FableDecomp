"""Link bank enumeration with the actual character search and packed-array reader."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve()
while not(ROOT/"rebuild").is_dir():ROOT=ROOT.parent
SOURCES=['rebuild/src/compiled/00/9c/CBankFile_CreateSymbolMap_009cc530.cpp','rebuild/src/compiled/00/99/CCharString_FindCharPos_0099e5a0.cpp','rebuild/src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp']
TARGETS=['?CreateSymbolMap@CBankFile@@QBEXAAVCSymbolMap@@_N@Z','?FindCharPos@CCharString@@QBEJD@Z','??ACPackedUIntArray@@QBEKK@Z']

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BankSymbolMapChain_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-symbol-map-chain.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='bank-symbol-map-chain-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];records=[]
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
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'BANK_SYMBOL_MAP_CHAIN_BEHAVIOR_PASS scenarios=576' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(sources=records,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),external_boundaries=['CCharString construction/destruction','CSymbolMap mutation','virtual bank enumeration'],missing_provider_checks=3),indent=2)+'\n');print('BANK_SYMBOL_MAP_CHAIN_PASS compiled=576 providers=3')
if __name__=='__main__':main()
