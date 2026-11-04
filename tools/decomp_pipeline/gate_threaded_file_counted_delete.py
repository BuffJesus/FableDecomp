"""Replace the generic print-only counted-file deleter with typed native proof."""
import hashlib,json,re,struct,subprocess
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from threaded_file_counted_delete_oracle import compare_deletion
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/9a/gapscan_sub009a9040_009a9040.cpp'
TEST=R/'rebuild/tests/00/9a/gapscan_sub009a9040_009a9040_test.cpp'
SYMBOL='?DeleteData@?$CCountedPointer@VCThreadedFile@@@@SIXPAX@Z'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 build=R/'rebuild/build/native-threaded-file-counted-delete/00/9a/009a9040'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();sources=build/'sources';sources.mkdir()
 inputs={Path(__file__),EXE,R/'ghidra_out/fablewin_pdb_names.tsv'}
 inputs.update(R/'tools/decomp_pipeline'/n for n in ('threaded_file_counted_delete_oracle.py','check_one.py','gate_game_component_abi.py','pe_oracle.py'))
 visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in (SOURCE,TEST):closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
 inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in visited:
  target=include/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else sources/p.relative_to(R)
  target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 (build/'gate-source.py').write_bytes(Path(__file__).read_bytes())
 env=V.env();env['INCLUDE']=';'.join(map(str,(V.VC/'include',include)))
 def compile(name,p):
  obj=build/(name+'.obj');cmd=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/Fo'+str(obj),str(p)];result=V.cl(cmd,env,build)
  save(name+'.compile.json',dict(command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
  for line in result.stdout.splitlines():
   if 'including file:' in line.lower():
    header=Path(line.split(':',2)[-1].strip()).resolve();assert header.is_relative_to(V.VC/'include') or header.is_relative_to(build)
  return obj
 objects=[compile('root',sources/SOURCE.relative_to(R)),compile('fixture',sources/TEST.relative_to(R))]
 c=Coff(objects[0]);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec]
 original=read_virtual_data(EXE.read_bytes(),0x9a9040,11)
 assert off==0 and len(body)==11 and not rels and body==original
 assert SYMBOL not in Coff(objects[1]).symbols
 assert struct.unpack('<I',read_virtual_data(EXE.read_bytes(),0x9a9cd0,4))[0]==0x9a9040
 assert '02fd5cf0\t'+SYMBOL in (R/'ghidra_out/fablewin_pdb_names.tsv').read_text(encoding='utf-8')
 behavior=compare_deletion(original,body);assert behavior['cases']==256
 save('differential.json',behavior)
 def link(name,objs,exports=False):
  exe=build/(name+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),*map(str,objs),*(['/export:DeleteFile='+SYMBOL] if exports else [])]
  result=subprocess.run(cmd,cwd=build,env=env,capture_output=True,text=True,timeout=30);save(name+'.link.json',dict(command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,linked=link('probe',objects,True);assert linked.returncode==0,linked.stdout+linked.stderr
 with pefile.PE(str(exe)) as pe:
  symbols={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
  assert pe.get_data(symbols['DeleteFile'],11)==original
 def run(name,exe):
  result=subprocess.run([str(exe)],cwd=build,env=env,capture_output=True,text=True,timeout=30);save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 positive=run('positive',exe);assert positive.returncode==0 and 'THREADED_FILE_COUNTED_DELETE_PASS cases=48' in positive.stdout
 _,omitted=link('omitted',objects[1:]);assert omitted.returncode and SYMBOL in omitted.stdout+omitted.stderr
 noop=sources/'noop.cpp';noop.write_text('#include "fable_threaded_file.h"\nvoid FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void*) {}\n')
 noop_obj=compile('noop',noop);control,linked=link('control',[noop_obj,objects[1]]);assert linked.returncode==0
 negative=run('control',control);assert negative.returncode==2
 assert all(sha(Path(p))==h for p,h in pins.items()),'Input drift'
 report=dict(status='PASS',build=str(build),address='009a9040',symbol=SYMBOL,grade='MATCH',bytes=11,compiled_cases=48,differential=behavior,required_source_omissions=1,runtime_controls=1,linked_body_verified=True,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),boundary='Retypes existing exact generic root and replaces print-only fixture; controlled deleting dispatch, no new reconstruction credit or file lifetime claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-counted-delete.json').write_text(json.dumps(report,indent=2)+'\n')
 print('THREADED_FILE_COUNTED_DELETE_PASS exact=11 cases=48 differential=256 omissions=1 controls=2');print('RECEIPT',build/'receipt.json')

if __name__=='__main__':main()
