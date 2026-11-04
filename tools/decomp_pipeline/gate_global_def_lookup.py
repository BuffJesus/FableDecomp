"""Verify typed const/mutable global definition-index lookups and post-load reference acquisition."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from global_def_lookup_oracle import compare
ROOT=check_one.ROOT
SPECS=[dict(address='009ad9e0',file='CDefinitionManager_GetPDefFromGlobalIndexConst_009ad9e0.cpp',symbol='?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z',size=44),
       dict(address='009ada10',file='CDefinitionManager_GetPDefFromGlobalIndex_009ada10.cpp',symbol='?GetPDefFromGlobalIndex@CDefinitionManager@@QAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z',size=44)]
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/GlobalDefLookup_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/global-def-lookup.json');a=ap.parse_args()
    work=Path(tempfile.mkdtemp(prefix='global-def-lookup-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];providers=[];candidate={};native={};entries=[]
    with pefile.PE(str(EXE)) as pe:
        for s in SPECS:
            source=(a.source_dir or ROOT/'rebuild/src/compiled/00/9a')/s['file'];obj=work/(s['address']+'.obj')
            r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[s['symbol']];b,rels=c.sections[sec];assert off==0 and len(b)==s['size'];patched=bytearray(b);addr=int(s['address'],16)
            for pos,(name,kind) in rels.items():assert name=='?LoadDef@CDefinitionManager@@IBEXK@Z' and kind==20;struct.pack_into('<I',patched,pos,(0x9ad6e0-addr-pos-4)&0xffffffff)
            raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['size']);assert patched==raw,s
            candidate[addr]=patched;native[addr]=raw;entries.append(dict(s,grade='RELOCATION_MATCH'));providers.append(dict(source=str(source.resolve()),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    cases=compare(candidate,native);test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout
    assert not ({s['symbol'] for s in SPECS}&Coff(test).symbols.keys())
    cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'f.exe'),str(test)]
    for omit in range(2):r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission linked',omit)
    r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout
    r=subprocess.run([str(work/'f.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'GLOBAL_DEF_LOOKUP_BEHAVIOR_PASS cases=1440' in r.stdout,(r.returncode,r.stdout)
    report=dict(entries=entries,providers=providers,comparative_scenarios=cases,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_definition_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('GLOBAL_DEF_LOOKUP_PASS comparative=%d compiled=1440 omissions=2'%cases)
if __name__=='__main__':main()
