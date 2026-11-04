"""Check reset dispatch order and the native system/timer callback ABIs."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[
    ('CDisplayManager_DeviceResetPhases_009bf500','phases_test.cpp','/O2','DEVICE_RESET_PHASES_BEHAVIOR_PASS scenarios=1680',[
        (0x9bf500,66,'?PreDeviceReset@CDisplayManager@@AAEXXZ'),
        (0x9bf550,78,'?PostDeviceReset@CDisplayManager@@AAE_NXZ')]),
    ('CSystemManager_DeviceResetCallbacks_009a52b0','system_callbacks_test.cpp','/O2','SYSTEM_RESET_CALLBACKS_PASS scenarios=256',[
        (0x9a52b0,1,'?OnPreDeviceReset@CSystemManager@@QAEXXZ'),
        (0x9a52c0,3,'?OnPostDeviceReset@CSystemManager@@QAE_NXZ')]),
    ('CVsyncCallbackTimer_OnPreDeviceReset_009f73a0','timer_test.cpp','/Od','TIMER_PRE_RESET_PASS scenarios=512',[
        (0x9f73a0,19,'?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ')]),
]
TARGETS={
    '?Uninit@CRenderManagerCore@@QAEXXZ':0xa0a9d0,
    '?Init@CRenderManagerCore@@QAEXPAUIDirect3DDevice9@@@Z':0xa0a940,
    '?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ':0x9f73a0,
    '?OnPreDeviceReset@CSystemManager@@QAEXXZ':0x9a52b0,
    '?OnPostDeviceReset@CSystemManager@@QAE_NXZ':0x9a52c0,
    '?ClearAllQueries@CVsyncCallbackTimer@@AAEXXZ':0x9f7250,
}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/device-reset-phases.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='reset-phases-'));pe=pefile.PE(str(EXE));reports=[]
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
                assert kind==20 and built[site:site+4]==b'\0'*4
                value=(TARGETS[name]-address-site-4)&0xffffffff
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
    print('DEVICE_RESET_PHASES_PASS functions=5 bytes=167 compiled=2448')

if __name__=='__main__':main()
