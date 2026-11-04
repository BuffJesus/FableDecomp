"""Check system-font character access and six-byte row addressing."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('CSystemFont_CharacterAccess_00a6c130','leaves_test.cpp','/O2','SYSTEM_FONT_LEAVES_BEHAVIOR_PASS scenarios=9729',[
 (0xa6c130,15,'?GetCharacter@CSystemFont@@QAEPAVCSystemFontCharacter@@E@Z'),
 (0xa6c140,11,'?Initialise@CSystemFontCharacter@@QAE_NPAD@Z'),
 (0xa6c150,26,'?IsElementSolid@CSystemFontCharacter@@QAE_NKK@Z')])]
TARGETS={'?FableSystemFontCharacters_013d28f0@@3PAVCSystemFontCharacter@@A':0x13d28f0}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-font-leaves.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='system-font-leaves-'));pe=pefile.PE(str(EXE));reports=[]
    for stem,test_name,opt,marker,functions in SPECS:
        shard=f'{functions[0][0]:08x}';shard=shard[:2]+'/'+shard[2:4]
        source=args.stage/(stem+'.cpp') if args.stage else ROOT/'rebuild/src/compiled'/shard/(stem+'.cpp')
        test=args.stage/test_name if args.stage else ROOT/'rebuild/tests'/shard/(stem+'_test.cpp')
        obj,test_obj=work/(stem+'.obj'),work/(stem+'.test.obj')
        for path,target,flag in [(source,obj,opt),(test,test_obj,'/O2')]:
            r=check_one.cl(['/nologo','/c',flag,'/Oy','/MT','/Fo'+str(target),str(path.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
        coff=Coff(obj);rows=[]
        for address,length,symbol in functions:
            sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
            native=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);candidate=bytearray(built)
            for site,(name,kind) in relocs.items():
                assert kind==6 and built[site:site+4]==b'\0'*4
                value=TARGETS[name]
                assert int.from_bytes(native[site:site+4],'little')==value
                struct.pack_into('<I',candidate,site,value)
            assert candidate==native
            rows.append(dict(address=f'{address:08x}',grade='RELOCATION_MATCH' if relocs else 'MATCH',bytes=length,relocations=len(relocs)))
        exe=work/(stem+'.exe');cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
        r=subprocess.run(cmd+[str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
        r=subprocess.run(cmd+[str(obj),str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
        r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and marker in r.stdout,r.stdout+r.stderr
        reports.append(dict(source=stem,functions=rows,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(test.read_bytes()).hexdigest()))
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(dict(components=reports),indent=2)+'\n')
    print('SYSTEM_FONT_LEAVES_PASS functions=3 bytes=52 compiled=9729')

if __name__=='__main__':main()
