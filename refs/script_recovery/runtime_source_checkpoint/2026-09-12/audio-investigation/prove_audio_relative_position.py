"""Execute the native per-voice position-update slice with synthetic listener poses.

Runs C1C49D through C1C5D9, including coordinate conversion, matrix construction,
inverse and both position branches. Lock acquisition, voice lookup and backend
SetPosition are stubs. The update eligibility prefix is not executed. This
establishes submitted geometry, not live selection, gain, mixer or device output.
"""
import hashlib,itertools,json,math,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path('work/runtime_re_tools').resolve()))
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

exe=Path(r'C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe')
pe=pefile.PE(str(exe));image=pe.get_memory_mapped_image()
ranges=[(0xC1C49D,0xC1C5D9),(0xC1BFB0,0xC1BFC3),(0xC1BF00,0xC1BFAE),
        (0xC25860,0xC25899),(0xC256A0,0xC256BC),(0xC1D1C0,0xC1D31B),
        (0xC34600,0xC34621),(0xC1C330,0xC1C45E),(0xC1BFF0,0xC1C025)]


def run(relative,yaw,origin):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x400000,(len(image)+4095)&~4095);u.mem_write(0x400000,image)
    u.mem_map(0x3000000,0x200000)
    mixer,proxy,wrapper,voice,stack=0x3001000,0x3002000,0x3003000,0x3004000,0x31F0000
    output=[];branches=[]
    forward=(math.sin(math.radians(yaw)),math.cos(math.radians(yaw)),0)
    def w(a,v):u.mem_write(a,struct.pack('<I',v))
    def vec(a,v):u.mem_write(a,struct.pack('<fff',*v))
    def r(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def ret(pop,value=0):
        sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EIP,r(sp));u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EAX,value)
    w(proxy+4,mixer);w(mixer+0x228,1)
    vec(mixer+8,origin);vec(mixer+0x14,forward);vec(mixer+0x20,(0,0,1))
    w(wrapper+4,voice);w(wrapper+0xC,123);u.mem_write(wrapper+0x25,b'\1')
    w(voice+0x48,1);vec(voice+0x38,(0,0.5,0));u.mem_write(voice+0x44,bytes([relative]))
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,mixer);u.reg_write(UC_X86_REG_EBP,proxy)
    def hook(u,address,size,data):
        if address==0xC0FFC0:ret(4);return
        if address==0xC13700:ret(4,wrapper);return
        if address==0xC33FD0:
            sp=u.reg_read(UC_X86_REG_ESP);assert r(sp+4)==123
            output.append(struct.unpack('<fff',u.mem_read(r(sp+8),12)));ret(8);return
        if address in (0xC1C3F0,0xC1C330):branches.append(hex(address))
        if not any(lo<=address<hi for lo,hi in ranges):raise AssertionError(f'Unexpected native path {address:#x}')
    u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xC1C49D,0xC1C5D9,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==0xC1C5D9 and u.reg_read(UC_X86_REG_ESP)==stack
    assert len(output)==1 and branches==[hex(0xC1C3F0 if relative else 0xC1C330)]
    # Backend coordinates swap the game's Y/Z axes.
    listener=(origin[0],origin[2],origin[1]);facing=(forward[0],forward[2],forward[1])
    offset=tuple(a-b for a,b in zip(output[0],listener))
    distance=math.sqrt(sum(v*v for v in offset));front=sum(a*b for a,b in zip(offset,facing))
    if relative:
        expected=tuple(a+0.5*b for a,b in zip(listener,facing))
        assert all(abs(a-b)<0.00002 for a,b in zip(output[0],expected)),(output,expected)
        assert abs(distance-0.5)<0.00002 and abs(front-0.5)<0.00002
    else:
        expected_distance=math.dist((0,0,0.5),listener)
        assert abs(distance-expected_distance)<0.00002
    return {'relative':relative,'yaw_degrees':yaw,'listener_game_position':origin,
            'submitted_backend_position':output[0],'listener_distance':distance,
            'forward_projection':front,'native_branch':branches[0]}


cases=[run(*args) for args in itertools.product((0,1),(0,30,90,180,270),((0,0,0),(10,20,30)))]
result={'exe_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'scope':__doc__,'cases':cases,
        'native_bytes':{hex(lo):pe.get_data(lo-0x400000,hi-lo).hex() for lo,hi in ranges}}
path=Path('refs/script_recovery/new_oakvale_intro/runtime_evidence/native-audio-relative-position-20260912.json')
path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(f'{len(cases)} native position-update cases passed: {path}')
