"""Validate the native system-font character table initialization."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0xa6c170
SYMBOL='?Initialise@CSystemFont@@QAE_NXZ'
TARGETS={'?Initialise@CInitBaseClass@@IAE_NXZ':0x99a350,'?FableSystemFontInitialized_013d2cf0@@3_NA':0x13d2cf0,'?FableSystemFontBlank_013d2cf4@@3PADA':0x13d2cf4,'?FableSystemFontCharacters_013d28f0@@3PAVCSystemFontCharacter@@A':0x13d28f0,'?FableSystemFontGlyphs_01397c60@@3PAY0CE@DA':0x1397c60}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a6/CSystemFont_Initialise_00a6c170.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a6/CSystemFont_Initialise_00a6c170_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-font-init.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='system-font-init-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/O2'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==805 and len(relocs)==154
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,805)
    candidate=bytearray(built);checked=[]
    for site,(name,kind) in sorted(relocs.items()):
        assert kind in [6,20];addend=int.from_bytes(built[site:site+4],'little');target=TARGETS[name]
        value=target+addend if kind==6 else target+addend-ENTRY-site-4
        struct.pack_into('<I',candidate,site,value&0xffffffff)
        checked.append(dict(offset=site,symbol=name,target=hex(target),addend=addend))
    assert candidate==native
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'SYSTEM_FONT_INIT_BEHAVIOR_PASS scenarios=512' in r.stdout,r.stdout+r.stderr
    proof=dict(address='00a6c170',grade='RELOCATION_MATCH',built_bytes=805,retail_bytes=805,relocations=checked,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('SYSTEM_FONT_INIT_PASS grade=RELOCATION_MATCH compiled=512')

if __name__=='__main__':main()
