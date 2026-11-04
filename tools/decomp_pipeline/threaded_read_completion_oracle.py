"""Windows read completion: error policy, status publication and installer wake."""
import hashlib
import itertools
import re
import struct
import tempfile
from pathlib import Path

import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_ESP, UC_X86_REG_EIP)

R = Path(__file__).resolve().parents[2]
ENTRY, SIZE, SYMBOL = 0x98e320, 102, '?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z'
SOURCE = R/'rebuild/src/compiled/00/98/CThreadedFileManager_OnFileReadCompletion_0098e320.cpp'
ADDRESS = {
    '?m_bDirtyDiscError@CThreadedFileManager@@0_NA':0x13bc868,
    '?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA':0x13bc864,
    '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
    '__imp__SetEvent@4':0x143fde0,
    '?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A':0x13ca818,
    '?Get@CFileInstallerSingleton@@SIAAVCFileInstaller@@XZ':0x404440,
    '?Unpause@CFileInstaller@@QAEXXZ':0x9c3250,
}
ARENA, LENGTH = 0x2000000, 0x6000
MANAGER, ALT_MANAGER, FILE, REQUEST, OVERLAPPED = ARENA+0x100, ARENA+0x500, ARENA+0x1000, ARENA+0x2000, ARENA+0x3000
INSTALLER, ALT_INSTALLER = ARENA+0x4000, ARENA+0x4100
STACK, STOP, API = 0x3000000, 0x4000000, 0x5000000
DIRTY, ERROR_CALLBACK, SINGLETON = 0x13bc868, 0x13bc864, 0x13ca818
INITIAL = {seed:bytes((i*37+seed*19)&255 for i in range(LENGTH)) for seed in (0,7)}


def execute(codes, args):
    error, device, callback, dirty, singleton, mutation, seed, old_status = args
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0x98e000,0x1000),(0x404000,0x1000),(0x9c3000,0x1000),
                         (0x122d000,0x1000),(0x143f000,0x1000),(0x13bc000,0x1000),(0x13ca000,0x1000),
                         (ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
        u.mem_map(address,size)
        if address in (0x98e000,0x404000,0x9c3000):
            u.mem_write(address,b'\xcc'*size)
    for address,body in codes.items():
        u.mem_write(address,body)
    u.mem_write(ARENA,INITIAL[seed])
    u.mem_write(0x13bc000,b'\xa5'*0x1000)
    u.mem_write(0x13ca000,b'\x5a'*0x1000)
    u.mem_write(0x122d000,b'\x6b'*0x1000)
    u.mem_write(0x122d70e,b'\0')
    def put(address,value):
        u.mem_write(address,struct.pack('<I',value&0xffffffff))
    def get(address):
        return struct.unpack('<I',u.mem_read(address,4))[0]
    put(OVERLAPPED+16,REQUEST)
    put(REQUEST+0x28,FILE)
    put(REQUEST+0x24,MANAGER)
    put(REQUEST+0x1c,old_status)
    put(FILE+4,device)
    put(MANAGER+0x164,0x12345678)
    put(ALT_MANAGER+0x164,0x87654321)
    put(ERROR_CALLBACK,API+32 if callback else 0)
    u.mem_write(DIRTY,bytes([dirty]))
    put(SINGLETON,INSTALLER if singleton else 0)
    put(0x143fde0,API)
    expected = bytearray(u.mem_read(ARENA,LENGTH))
    global_expected = {page:bytearray(u.mem_read(page,0x1000)) for page in (0x13bc000,0x13ca000)}
    hard_error = bool(error and device != 1 and callback)
    current_singleton = INSTALLER if singleton else 0
    model_manager = MANAGER
    expected_trace = []
    if error and device == 1:
        global_expected[0x13bc000][DIRTY-0x13bc000] = 1
    if hard_error:
        expected_trace.append(('hard_error',0x122d70e))
        if mutation in (1,3):
            model_manager = ALT_MANAGER
            current_singleton = 0 if current_singleton else INSTALLER
            struct.pack_into('<I',expected,REQUEST-ARENA+0x24,ALT_MANAGER)
    event = 0x12345678 if model_manager == MANAGER else 0x87654321
    expected_trace.append(('set_event',event))
    if mutation in (2,3):
        current_singleton = 0 if current_singleton else INSTALLER
    final_status = 0x87654321 if mutation == 3 else 3
    struct.pack_into('<I',expected,REQUEST-ARENA+0x1c,final_status)
    struct.pack_into('<I',global_expected[0x13ca000],SINGLETON-0x13ca000,current_singleton)
    returned_installer = ALT_INSTALLER if mutation in (1,3) else INSTALLER
    if current_singleton:
        expected_trace.extend([('singleton_get',),('unpause',returned_installer)])
    trace, seen = [], set()
    def hook(uc,address,size,user):
        seen.add(address)
        if address not in (API,API+32,0x404440,0x9c3250):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        pop, result = 0, 0xfedcba98
        if address == API+32:
            assert hard_error and not trace
            assert uc.reg_read(UC_X86_REG_ECX) == 0x122d70e
            assert u.mem_read(0x122d70e,1) == b'\0'
            assert get(REQUEST+0x1c) == old_status
            trace.append(('hard_error',0x122d70e))
            if mutation in (1,3):
                put(REQUEST+0x24,ALT_MANAGER)
                put(REQUEST+0x1c,0x55667788)
                put(SINGLETON,0 if get(SINGLETON) else INSTALLER)
        elif address == API:
            assert trace == expected_trace[:int(hard_error)]
            assert get(REQUEST+0x1c) == 3 and get(sp+4) == event
            trace.append(('set_event',event))
            pop, result = 4, 0 if seed == 0 else 0xffffffff
            if mutation in (2,3):
                put(SINGLETON,0 if get(SINGLETON) else INSTALLER)
            if mutation == 3:
                put(REQUEST+0x1c,final_status)
        elif address == 0x404440:
            assert current_singleton and get(SINGLETON) == current_singleton
            trace.append(('singleton_get',))
            result = returned_installer
        else:
            assert trace[-1] == ('singleton_get',)
            assert uc.reg_read(UC_X86_REG_ECX) == returned_installer
            trace.append(('unpause',returned_installer))
        uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp))
        uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp = STACK+0x8000
    transferred = (0,0xffffffff,0x12345678)[old_status//2]
    for index,value in enumerate((STOP,error,transferred,OVERLAPPED)):
        put(sp+index*4,value)
    saved = (UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for index,reg in enumerate(saved):
        u.reg_write(reg,0x55660000+seed+index)
    u.reg_write(UC_X86_REG_ESP,sp)
    u.reg_write(UC_X86_REG_ECX,0xabcdef01)
    u.emu_start(ENTRY,STOP,count=200)
    assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == sp+16
    assert all(u.reg_read(reg) == 0x55660000+seed+index for index,reg in enumerate(saved))
    assert trace == expected_trace
    state = bytes(u.mem_read(ARENA,LENGTH))
    assert state == bytes(expected)
    globals_after = tuple(bytes(u.mem_read(page,0x1000)) for page in global_expected)
    assert globals_after == tuple(bytes(value) for value in global_expected.values())
    return (state,globals_after,tuple(trace)),seen


def compare_completion(source=SOURCE, include_dir=None):
    include_dirs = ([Path(include_dir)] if include_dir else [])+[R/'rebuild/include']
    inputs = {Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
    visited = set()
    def closure(path):
        if path in visited:
            return
        visited.add(path)
        inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q = next((q for q in [path.parent/name,*[inc/name for inc in include_dirs]] if q.is_file()),None)
            assert q,name
            closure(q)
    closure(source)
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='threaded-read-completion-') as tmp:
        obj = Path(tmp)/'provider.obj'
        result = V.cl(['/nologo','/c','/O2','/MT',*['/I'+str(p) for p in include_dirs],'/Fo'+str(obj),str(source)],V.env(),Path(tmp))
        assert result.returncode == 0,result.stdout+result.stderr
        c = Coff(obj)
        section,offset = c.symbols[SYMBOL]
        assert offset == 0
        body,relocations = c.sections[section]
        assert len(body) == SIZE and len(relocations) == 7
        patched = bytearray(body)
        for site,(symbol,kind) in relocations.items():
            assert kind in (6,20) and struct.unpack_from('<i',body,site)[0] == 0
            target = ADDRESS[symbol]-(ENTRY+site+4 if kind == 20 else 0)
            struct.pack_into('<I',patched,site,target&0xffffffff)
    codes = {ENTRY:bytes(patched)}
    native = {ENTRY:read_virtual_data(EXE.read_bytes(),ENTRY,SIZE)}
    assert codes == native
    cases = 0
    for args in itertools.product((0,1,5,0xffffffff),(0,1,2),(False,True),(0,1,255),
                                  (False,True),range(4),(0,7),(0,2,4)):
        try:
            actual,seen = execute(codes,args)
            original,_ = execute(native,args)
            assert actual == original and ENTRY in seen
        except Exception as error:
            raise AssertionError(args) from error
        cases += 1
        if cases%864 == 0:
            print('read completion cases=%d'%cases,flush=True)
    detected = False
    try:
        execute({},(0,0,False,0,False,0,0,0))
    except Exception:
        detected = True
    assert detected
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    return dict(status='PASS',cases=cases,missing_provider_checks=1,input_sha256=pins,
                boundaries=['SetEvent, hard-disk error callback, installer singleton lookup and Unpause controlled; no actual IO/APC/threading.',
                            'Independent call-order/full-storage/ABI model, dirty-disc byte policy and callback-sensitive manager/installer reloads.',
                            'Error callback is fastcall with an empty string; transferred byte count and SetEvent result unused.'])
