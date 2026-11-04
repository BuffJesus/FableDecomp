"""Verify typed const/mutable global definition-index lookups and post-load reference acquisition."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from combat_def_helpers_oracle import compare
ROOT=check_one.ROOT
SPECS=[{'address': '00669640', 'file': 'CDefinitionManager_GetDefCombatTypeIndex_00669640.cpp', 'symbol': '??$GetDef@VCCombatTypeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCCombatTypeDef@@@@@Z', 'size': 108}, {'address': '00669770', 'file': 'CDefinitionManager_GetDefCombatTypeName_00669770.cpp', 'symbol': '??$GetDef@VCCombatTypeDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCCombatTypeDef@@@@@Z', 'size': 104}]

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CombatDefHelpers_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/combat-def-helpers.json');a=ap.parse_args()
    work=Path(tempfile.mkdtemp(prefix='combat-def-helpers-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];providers=[];candidate={};native={};entries=[]
    with pefile.PE(str(EXE)) as pe:
        for s in SPECS:
            source=(a.source_dir or ROOT/'rebuild/src/compiled/00/66')/s['file'];obj=work/(s['address']+'.obj')
            r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[s['symbol']];b,rels=c.sections[sec];assert off==0 and len(b)==s['size'];patched=bytearray(b);addr=int(s['address'],16)
            for pos,(name,kind) in rels.items():
                assert kind==20;target=0x9ad9e0 if name.startswith('?GetPDefFromGlobalIndex@') else 0x9ada40 if name.startswith('?GetPDefFromInstantiationName@') else None;assert target is not None;struct.pack_into('<I',patched,pos,(target-addr-pos-4)&0xffffffff)
            raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['size']);assert patched==raw,s
            candidate[addr]=patched;native[addr]=raw;entries.append(dict(s,grade='RELOCATION_MATCH'));providers.append(dict(source=str(source.resolve()),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    cases=compare(candidate,native);test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout
    assert not ({s['symbol'] for s in SPECS}&Coff(test).symbols.keys())
    cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'f.exe'),str(test)]
    for omit in range(2):r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission linked',omit)
    r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout
    r=subprocess.run([str(work/'f.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'COMBAT_DEF_HELPERS_BEHAVIOR_PASS cases=12288' in r.stdout,(r.returncode,r.stdout)
    report=dict(boundaries=["definition pointer acquisition", "virtual deletion callback"],entries=entries,providers=providers,comparative_scenarios=cases,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_definition_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('COMBAT_DEF_HELPERS_PASS comparative=%d compiled=12288 omissions=2'%cases)
if __name__=='__main__':main()
