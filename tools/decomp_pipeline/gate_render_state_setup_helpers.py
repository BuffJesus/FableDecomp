"""Check typed render-state registration and field preservation."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('CRenderStateManager_StateSetup_00a04580','helpers_test.cpp','/O2','RENDER_STATE_SETUP_HELPERS_BEHAVIOR_PASS scenarios=1680',[
 (0xa04580,47,'?SetupRenderState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DRENDERSTATETYPE@@@Z'),
 (0xa045b0,54,'?SetupTextureStageState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DTEXTURESTAGESTATETYPE@@J@Z'),
 (0xa045f0,54,'?SetupSamplerStageState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DSAMPLERSTATETYPE@@J@Z')])]
TARGETS={}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-setup-helpers.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='render-state-setup-helpers-'));pe=pefile.PE(str(EXE));reports=[]
    for stem,test_name,opt,marker,functions in SPECS:
        shard=f'{functions[0][0]:08x}';shard=shard[:2]+'/'+shard[2:4]
        source=args.stage/(stem+'.cpp') if args.stage else ROOT/'rebuild/src/compiled'/shard/(stem+'.cpp')
        test=args.stage/test_name if args.stage else ROOT/'rebuild/tests/00/a0/CRenderStateManager_SetupStateHelpers_00a04580_test.cpp'
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
        # A neutral harness filename avoids Windows installer detection for 'Setup'.
        exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
        r=subprocess.run(cmd+[str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
        r=subprocess.run(cmd+[str(obj),str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
        r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and marker in r.stdout,r.stdout+r.stderr
        reports.append(dict(source=stem,functions=rows,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(test.read_bytes()).hexdigest()))
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(dict(components=reports),indent=2)+'\n')
    print('RENDER_STATE_SETUP_HELPERS_PASS functions=3 bytes=155 compiled=1680')

if __name__=='__main__':main()
