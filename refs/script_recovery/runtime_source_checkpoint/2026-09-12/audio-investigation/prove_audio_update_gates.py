"""Execute full native C1C460 update with controlled gates and voice lists.

Runs entry/SEH setup, eligibility, matrix math, every voice iteration and exit.
Locks, voice lookup, backend position submission and backend tick are doubled.
No audible gain, backend device selection or live script-line claim.
"""
import argparse,hashlib,itertools,json,math,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path('work/runtime_re_tools').resolve()))
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

exe=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe')
digest=hashlib.sha256(exe.read_bytes()).hexdigest()
assert digest=='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
pe=pefile.PE(str(exe));image=pe.get_memory_mapped_image()
ranges=[(0xC1C460,0xC1C61D),(0xC1BFB0,0xC1BFC3),(0xC1BF00,0xC1BFAE),
 (0xC25860,0xC25899),(0xC256A0,0xC256BC),(0xC1D1C0,0xC1D31B),
 (0xC34600,0xC34621),(0xC1C330,0xC1C45E),(0xC1BFF0,0xC1C025)]

def run(name,proxy_gate,mixer_gate,voices,yaw=0,origin=(10,20,30)):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0,0x1000)
    u.mem_map(0x400000,(len(image)+4095)&~4095);u.mem_write(0x400000,image)
    u.mem_map(0x3000000,0x200000)
    mixer,proxy,stack,stop=0x3001000,0x3002000,0x31f0000,0x3009000
    output=[];events=[]
    def w(a,v):u.mem_write(a,struct.pack('<I',v))
    def r(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def vec(a,v):u.mem_write(a,struct.pack('<fff',*v))
    def ret(pop=0,value=0):
        sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EIP,r(sp));u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EAX,value)
    w(0,0xdeadbeef);w(stack,stop);w(proxy+4,mixer)
    u.mem_write(proxy+0x19,bytes([proxy_gate]));u.mem_write(mixer+0x885,bytes([mixer_gate]))
    forward=(math.sin(math.radians(yaw)),math.cos(math.radians(yaw)),0)
    vec(mixer+8,origin);vec(mixer+0x14,forward);vec(mixer+0x20,(0,0,1))
    w(mixer+0x228,len(voices))
    for i,v in enumerate(voices):
        wrapper=0x3010000+i*0x200;voice=wrapper+0x100
        w(wrapper+4,voice if v['present'] else 0);w(wrapper+0xc,123+i)
        u.mem_write(wrapper+0x25,bytes([v['wrapper_gate']]))
        w(voice+0x48,v['voice_gate']);vec(voice+0x38,v.get('position',(0,0.5,0)))
        u.mem_write(voice+0x44,bytes([v['relative']]))
    registers={UC_X86_REG_EBX:0xabcdef01,UC_X86_REG_ESI:0xabcdef02,UC_X86_REG_EDI:0xabcdef03,UC_X86_REG_EBP:0xabcdef04}
    for reg,value in registers.items():u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,proxy)
    def hook(uc,a,size,data):
        sp=u.reg_read(UC_X86_REG_ESP)
        if a==stop:u.emu_stop();return
        if a in (0xC0FFC0,0xC0FFE0):events.append('lock' if a==0xC0FFC0 else 'unlock');ret(4 if a==0xC0FFC0 else 0);return
        if a==0xC13700:
            index=r(sp+4);assert index<len(voices);events.append('lookup_'+str(index));ret(4,0x3010000+index*0x200);return
        if a==0xC33FD0:
            handle=r(sp+4);position=struct.unpack('<fff',u.mem_read(r(sp+8),12))
            output.append({'index':handle-123,'position':position});events.append('submit_'+str(handle-123));ret(8);return
        if a==0xC33690:events.append('backend_tick');ret();return
        if not any(lo<=a<hi for lo,hi in ranges):raise AssertionError(f'unexpected native path {a:#x}')
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xC1C460,0,count=50000)
    assert u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==stack+4 and r(0)==0xdeadbeef
    assert all(u.reg_read(reg)==value for reg,value in registers.items())
    eligible=[i for i,v in enumerate(voices) if proxy_gate and mixer_gate and v['present'] and v['voice_gate'] and v['wrapper_gate']]
    assert [p['index'] for p in output]==eligible
    assert events[-1]=='backend_tick' and events.count('backend_tick')==1
    assert events.count('lock')==events.count('unlock')==int(bool(proxy_gate and mixer_gate))
    for submitted in output:
        v=voices[submitted['index']]
        source=v.get('position',(0,0.5,0));listener=(origin[0],origin[2],origin[1]);facing=(forward[0],0,forward[1])
        if v['relative']:
            # This inventory's relative source remains the retail default (0,.5,0).
            assert tuple(source)==(0,0.5,0)
            expected=tuple(a+0.5*b for a,b in zip(listener,facing))
        else:
            # C1C37A compares listener-local depth to zero; C1C387 multiplies
            # negative depth by -1 before conversion back to backend axes.
            world=(source[0],source[2],source[1])
            projection=sum((a-b)*f for a,b,f in zip(world,listener,facing))
            expected=tuple(a-2*min(projection,0)*f for a,f in zip(world,facing))
        assert all(abs(x-y)<0.00002 for x,y in zip(submitted['position'],expected)),(name,submitted,expected)
        submitted['distance_to_listener']=math.dist(submitted['position'],listener)
        submitted['forward_projection']=sum((a-b)*f for a,b,f in zip(submitted['position'],listener,facing))
        assert submitted['forward_projection']>=-0.00002
        if not v['relative']:assert abs(submitted['distance_to_listener']-math.dist(world,listener))<0.00002
    return dict(name=name,proxy_19=proxy_gate,mixer_885=mixer_gate,voices=voices,yaw=yaw,origin=origin,events=events,submitted=output)

cases=[]
for n,values in enumerate(itertools.product((0,1),repeat=6)):
    proxy,mixer,present,voice,wrapper,relative=values
    cases.append(run('gates_'+str(n),proxy,mixer,[dict(present=present,voice_gate=voice,wrapper_gate=wrapper,relative=relative)]))
for relative,yaw in itertools.product((0,1),(0,30,90,180,270)):
    cases.append(run(f'geometry_{relative}_{yaw}',1,1,[dict(present=1,voice_gate=1,wrapper_gate=1,relative=relative)],yaw))
cases.append(run('no_voices',1,1,[]))
cases.append(run('mixed_voices',1,1,[dict(present=1,voice_gate=1,wrapper_gate=1,relative=1),
    dict(present=0,voice_gate=1,wrapper_gate=1,relative=1),dict(present=1,voice_gate=0,wrapper_gate=1,relative=0),
    dict(present=1,voice_gate=1,wrapper_gate=1,relative=0)],90))
for n,position in enumerate([(10,25,30),(10,15,30),(15,20,35),(5,20,25),(10,20,30)]):
    for yaw in (0,90,180):
        cases.append(run(f'world_position_{n}_{yaw}',1,1,[dict(present=1,voice_gate=1,wrapper_gate=1,relative=0,position=position)],yaw))
assert struct.unpack('<ff',pe.get_data(0x122dedc-0x400000,8))==(0.0,-1.0)
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,default=Path('refs/script_recovery/new_oakvale_intro/runtime_evidence/native-audio-update-gates-20260912.json'))
target=parser.parse_args().output
if target.exists():raise FileExistsError(target)
target.write_text(json.dumps(dict(scope=__doc__,exe_sha256=digest,cases=cases,
    native_bytes={hex(a):pe.get_data(a-0x400000,b-a).hex() for a,b in ranges}),indent=2)+'\n')
print(json.dumps({'cases':len(cases),'all_passed':True,'path':str(target)}))
