"""Validate symbol-map implicit lifetime and owned container cleanup."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from symbol_map_clear_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SET_DTOR='??1?$set@VCCharString@@U?$less@VCCharString@@@_STL@@V?$allocator@VCCharString@@@3@@_STL@@QAE@XZ'
MAP_DTOR='??1?$CVectorMap@VCCharString@@VCSymbolInfo@CSymbolMap@@V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@@QAE@XZ'
ERASE='?_M_erase@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@_STL@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@VCCharString@@@2@@Z'
DESTROY='??$__destroy_aux@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@_STL@@@_STL@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@0ABU__false_type@0@@Z'
CTOR='??0CSymbolMap@@QAE@XZ'
SCALAR='??_GCSymbolMap@@UAEPAXI@Z'
SPECS=[(0x4ad4a0,SET_DTOR,66,0),(0x6fcf10,MAP_DTOR,37,0),(0x4aba70,ERASE,59,0),(0x6fbcc0,DESTROY,51,0),(0x9e5200,CTOR,76,1),(0x9e7bd0,SCALAR,48,1)]
TARGETS={SET_DTOR:0x4ad4a0,MAP_DTOR:0x6fcf10,ERASE:0x4aba70,DESTROY:0x6fbcc0,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CSymbolMap@@6B@':0x129c158,'_malloc':0xbfea0e,'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc}

from gate_symbol_map_reset import RESET,COPY
CLEAR='?Clear@CSymbolMap@@QAEXXZ'
SPECS += [(0x9e79e0,RESET,103,2),(0x9e6600,COPY,108,2),(0x9e5250,CLEAR,62,3)]
TARGETS.update({RESET:0x9e79e0,COPY:0x9e6600,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0})

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/SymbolMapClear_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/symbol-map-clear.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='symbol-map-clear-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];sources=[a.source,ROOT/'rebuild/src/compiled/00/9e/CSymbolMap_CSymbolMap_009e5200.cpp',ROOT/'rebuild/src/compiled/00/9e/CSymbolMap_VectorMemory_009e7680.cpp',ROOT/'rebuild/src/compiled/00/9e/CSymbolMap_Clear_009e5250.cpp'];objects=[]
 for i,source in enumerate(sources+[a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 coffs=[Coff(obj) for obj in objects];assert all(symbol not in coffs[-1].symbols for _,symbol,_,_ in SPECS);candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for address,symbol,length,index in SPECS:
   c=coffs[index];sec,off=c.symbols[symbol];assert off==0;b,rel=c.sections[sec];assert len(b)==length;n=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(b);checked=[]
   for site,(name,kind) in rel.items():
    target=TARGETS[name];assert b[site:site+4]==bytes(4) and kind in (6,20)
    value=target if kind==6 else (target-address-site-4)&0xffffffff
    assert struct.unpack_from('<I',n,site)[0]==value,(hex(address),site,name,hex(address+site+4+struct.unpack_from('<i',n,site)[0]))
    struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target))
   assert patched==n;candidate[address]=patched;native[address]=n;records.append(dict(address='%08x'%address,symbol=symbol,bytes=length,grade='RELOCATION_MATCH',relocations=checked))
 comparisons=compare(candidate,native);exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omitted in range(4):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SYMBOL_MAP_CLEAR_BEHAVIOR_PASS scenarios=1458' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),release_providers=[dict(source=str(p.relative_to(ROOT)),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources[1:]])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('SYMBOL_MAP_CLEAR_PASS compiled=1458 comparative='+str(comparisons))
if __name__=='__main__':main()
