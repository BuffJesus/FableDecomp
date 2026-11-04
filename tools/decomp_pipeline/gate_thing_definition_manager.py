"""Verify genuine CThing singleton/definition-manager providers and their composed behavior."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE
from thing_definition_manager_oracle import compare
ROOT=check_one.ROOT
SPECS=[('CThing_PeekDefinitionManager_004c7990.cpp','004c7990','?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ',9),
       ('CThing_SetCurrentThingManager_004c74f0.cpp','004c74f0','?SetCurrentThingManager@CThing@@SIXAAVCThingManager@@@Z',7)]
DATA='?PCurrentThingManager@CThing@@0PAVCThingManager@@A'
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ThingDefinitionManager_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/thing-definition-manager.json');args=ap.parse_args()
    work=Path(tempfile.mkdtemp(prefix='thing-definition-manager-'));env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    objects=[];coffs=[];providers=[]
    def compile(source,name):
        obj=work/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work)
        assert not r.returncode,r.stdout+r.stderr
        return obj
    for file,addr,symbol,size in SPECS:
        source=args.source_dir/file if args.source_dir else ROOT/'rebuild/src/compiled/00/4c'/file
        obj=compile(source,file+'.obj');objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source.resolve()),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    candidate={};native={};entries=[]
    with pefile.PE(str(EXE)) as pe:
        for (file,addr,symbol,size),c in zip(SPECS,coffs):
            sec,off=c.symbols[symbol];built,rels=c.sections[sec];assert off==0 and len(built)==size
            patched=bytearray(built)
            for pos,(name,kind) in rels.items():
                assert name==DATA and kind==6,(name,kind);struct.pack_into('<I',patched,pos,0x13b8a1c)
            raw=pe.get_data(int(addr,16)-0x400000,size);assert patched==raw,symbol
            candidate[int(addr,16)]=patched;native[int(addr,16)]=raw
            entries.append(dict(address=addr,symbol=symbol,bytes=size,grade='RELOCATION_MATCH'))
        sec,off=coffs[1].symbols[DATA];built,rels=coffs[1].sections[sec];assert not rels
        initial=coff_data(coffs[1],DATA,4);assert initial==b'\0'*4,(DATA,initial)
        # Retail singleton is in .data's loader-zeroed virtual tail.
        rva=0x13b8a1c-pe.OPTIONAL_HEADER.ImageBase
        section=next(s for s in pe.sections if s.VirtualAddress<=rva and rva+4<=s.VirtualAddress+s.Misc_VirtualSize)
        assert section.Name.rstrip(b'\0')==b'.data'
        assert rva>=section.VirtualAddress+section.SizeOfRawData
        retail=bytes(4);assert retail==initial,(DATA,retail)
    cases=compare(candidate,native)
    test=compile(args.test,'test.obj');t=Coff(test);assert all(s[2] not in t.symbols for s in SPECS) and DATA not in t.symbols
    command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'fixture.exe'),str(test)]
    for omit in range(2):
        r=subprocess.run(command+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True)
        assert r.returncode,('omission linked',omit)
    r=subprocess.run(command+[str(obj) for obj in objects],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout
    r=subprocess.run([str(work/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60)
    assert not r.returncode and 'THING_DEFINITION_MANAGER_BEHAVIOR_PASS cases=2048' in r.stdout,(r.returncode,r.stdout)
    report=dict(entries=entries,providers=providers,comparative_scenarios=cases,missing_provider_checks=2,behavior=r.stdout.strip(),
                data_symbol=DATA,data_bytes=initial.hex(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('THING_DEFINITION_MANAGER_PASS comparative=%d compiled=2048 omissions=2'%cases)
if __name__=='__main__':main()
