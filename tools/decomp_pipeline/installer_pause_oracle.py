"""Installer Pause: volatile counter, dirty-disc bypass and unlocked APC wait."""
import hashlib
import itertools
import re
import struct
import tempfile
from pathlib import Path

import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ
from unicorn.x86_const import (UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,
    UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EIP)

R = Path(__file__).resolve().parents[2]
ENTRY,SIZE,SYMBOL = 0x9c31e0,103,'?Pause@CFileInstaller@@QAEXXZ'
SOURCE = R/'rebuild/src/compiled/00/9c/CFileInstaller_Pause_009c31e0.cpp'
ADDRESS = {'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,
           '?Release@CCriticalSectionLock@@QAEXXZ':0xa62960,'??1CCriticalSectionLock@@QAE@XZ':0xa629a0,
           '__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,
           '__imp__LeaveCriticalSection@4':0x143fe10,'__imp__SetEvent@4':0x143fde0,
           '__imp__WaitForSingleObjectEx@12':0x143fe08,'?DirtyDisc@CFileInstaller@@1_NA':0x13ca814}
ARENA,LENGTH,OBJECT = 0x2000000,0x4000,0x2000100
STACK,STOP,API,DIRTY = 0x3000000,0x4000000,0x5000000,0x13ca814
INITIAL = {seed:bytes((i*37+seed*19)&255 for i in range(LENGTH)) for seed in (0,7)}


def providers(source=SOURCE):
    roots = [dict(name='pause',source=source,symbol=SYMBOL,address=ENTRY,size=SIZE)]
    for method,address,symbol in (
        ('Constructor',0xa62920,'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z'),
        ('Release',0xa62960,'?Release@CCriticalSectionLock@@QAEXXZ'),
        ('Destructor',0xa629a0,'??1CCriticalSectionLock@@QAE@XZ')):
        roots.append(dict(name=method,source=R/'rebuild/src/compiled/00/a6'/('CCriticalSectionLock_%s_%08x.cpp'%(method,address)),symbol=symbol,address=address))
    return roots,dict(ADDRESS)


def build_codes(source=SOURCE,include_dir=None):
    roots,addresses = providers(source)
    includes = ([Path(include_dir)] if include_dir else [])+[R/'rebuild/include']
    codes = {}
    raw = EXE.read_bytes()
    with tempfile.TemporaryDirectory(prefix='installer-pause-') as tmp:
        for index,root in enumerate(roots):
            obj = Path(tmp)/('provider%d.obj'%index)
            result = V.cl(['/nologo','/c','/O2','/MT',*['/I'+str(p) for p in includes],'/Fo'+str(obj),str(root['source'])],V.env(),Path(tmp))
            assert result.returncode == 0,result.stdout+result.stderr
            c = Coff(obj)
            section,offset = c.symbols[root['symbol']]
            assert offset == 0
            body,relocations = c.sections[section]
            original = read_virtual_data(raw,root['address'],len(body))
            patched = bytearray(body)
            for site,(symbol,kind) in relocations.items():
                assert kind in (6,20) and struct.unpack_from('<i',body,site)[0] == 0
                value = struct.unpack_from('<i' if kind == 20 else '<I',original,site)[0]
                assert (root['address']+site+4+value if kind == 20 else value) == addresses[symbol]
                target = addresses[symbol]-(root['address']+site+4 if kind == 20 else 0)
                struct.pack_into('<I',patched,site,target&0xffffffff)
            assert bytes(patched) == original
            codes[root['address']] = bytes(patched)
    assert len(codes[ENTRY]) == SIZE
    return codes


def execute(codes,args):
    paused,running,dirty,enter_mutation,unlock_mutation,callback_mode,wait_result,seed = args
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0x9c3000,0x1000),(0xa62000,0x1000),(0x143f000,0x1000),(0x13ca000,0x1000),
                         (ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
        u.mem_map(address,size)
        if address in (0x9c3000,0xa62000):u.mem_write(address,b'\xcc'*size)
    for address,body in codes.items():u.mem_write(address,body)
    u.mem_write(ARENA,INITIAL[seed]);u.mem_write(0x13ca000,b'\xa5'*0x1000)
    def put(address,value):u.mem_write(address,struct.pack('<I',value&0xffffffff))
    def get(address):return struct.unpack('<I',u.mem_read(address,4))[0]
    initial_event = 0 if seed == 0 else 0x12345678
    initial_wait = initial_event
    put(OBJECT+0x7c,paused);u.mem_write(OBJECT+0x7b,bytes([running]));u.mem_write(DIRTY,bytes([dirty]))
    put(OBJECT+0x84,initial_event);put(OBJECT+0x8c,initial_wait)
    entered_paused = paused if enter_mutation == 0 else 0xffffffff if enter_mutation == 1 else 0
    entered_running = running if enter_mutation == 0 else 1
    entered_dirty = dirty if enter_mutation == 0 else 0
    after_count = (entered_paused+1)&0xffffffff
    waiting = bool(entered_running and after_count <= 1 and not entered_dirty)
    final_count,final_running,final_dirty = after_count,entered_running,entered_dirty
    event,wait_handle = initial_event,initial_wait
    if unlock_mutation:
        final_count,final_running,final_dirty = 5,0,255
        event,wait_handle = 0x11223344,0x99aabbcc
    final_event,final_wait = event,wait_handle
    if waiting and callback_mode == 1:
        final_count,final_running,final_dirty = 9,0,1
        wait_handle = final_wait = 0xabcdef01
    if waiting and callback_mode == 2:
        final_count,final_running,final_dirty = 0x76543210,1,255
    expected = bytearray(u.mem_read(ARENA,LENGTH))
    struct.pack_into('<I',expected,OBJECT-ARENA+0x7c,final_count)
    expected[OBJECT-ARENA+0x7b] = final_running
    struct.pack_into('<I',expected,OBJECT-ARENA+0x84,final_event)
    struct.pack_into('<I',expected,OBJECT-ARENA+0x8c,final_wait)
    expected_global = bytearray(u.mem_read(0x13ca000,0x1000));expected_global[DIRTY-0x13ca000] = final_dirty
    expected_reads = [(OBJECT+0x7c,4),(OBJECT+0x7b,1)]
    if entered_running:expected_reads.append((OBJECT+0x7c,4))
    expected_trace = ['enter','leave']+([('event',event),('wait',wait_handle,0xffffffff,1)] if waiting else [])
    for slot,target in ((0x143fe0c,API),(0x143fe10,API+16),(0x143fd30,API+32),(0x143fde0,API+48),(0x143fe08,API+64)):put(slot,target)
    trace,reads,seen = [],[],set()
    depth = 0
    def memory_read(uc,access,address,size,value,user):
        if address in (OBJECT+0x7b,OBJECT+0x7c):reads.append((address,size))
    def hook(uc,address,size,user):
        nonlocal depth
        seen.add(address)
        if address not in (API,API+16,API+32,API+48,API+64):return
        sp = uc.reg_read(UC_X86_REG_ESP);pop,result = 4,0 if seed == 0 else 0xffffffff
        assert address != API+32,'Pause must take a blocking scoped lock'
        if address == API:
            assert not trace and not depth and get(sp+4) == OBJECT+4
            depth = 1;trace.append('enter')
            put(OBJECT+0x7c,entered_paused);u.mem_write(OBJECT+0x7b,bytes([entered_running]));u.mem_write(DIRTY,bytes([entered_dirty]))
        elif address == API+16:
            assert depth == 1 and trace == ['enter'] and get(sp+4) == OBJECT+4
            depth = 0;trace.append('leave')
            if unlock_mutation:
                put(OBJECT+0x7c,5);u.mem_write(OBJECT+0x7b,b'\0');u.mem_write(DIRTY,b'\xff')
                put(OBJECT+0x84,event);put(OBJECT+0x8c,final_wait if not waiting or callback_mode != 1 else 0x99aabbcc)
        elif address == API+48:
            assert waiting and not depth and trace == ['enter','leave'] and get(sp+4) == event
            trace.append(('event',event))
            if callback_mode == 1:
                put(OBJECT+0x7c,9);u.mem_write(OBJECT+0x7b,b'\0');u.mem_write(DIRTY,b'\1');put(OBJECT+0x8c,wait_handle)
        else:
            assert waiting and not depth and trace == expected_trace[:-1]
            assert [get(sp+i*4) for i in (1,2,3)] == [wait_handle,0xffffffff,1]
            trace.append(('wait',wait_handle,0xffffffff,1));pop,result = 12,wait_result
            if callback_mode == 2:
                put(OBJECT+0x7c,final_count);u.mem_write(OBJECT+0x7b,bytes([final_running]));u.mem_write(DIRTY,bytes([final_dirty]))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_READ,memory_read)
    sp = STACK+0x8000;put(sp,STOP)
    saved = (UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for index,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+index)
    u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
    u.emu_start(ENTRY,STOP,count=500)
    assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == sp+4
    assert all(u.reg_read(reg) == 0x55660000+seed+index for index,reg in enumerate(saved))
    assert not depth and trace == expected_trace
    state = bytes(u.mem_read(ARENA,LENGTH));global_state = bytes(u.mem_read(0x13ca000,0x1000))
    assert state == bytes(expected) and global_state == bytes(expected_global)
    assert reads == expected_reads,('volatile_reads',reads,expected_reads)
    return (state,global_state,tuple(trace),tuple(reads)),seen


def compare_pause(source=SOURCE,include_dir=None):
    roots,_ = providers(source)
    includes = ([Path(include_dir)] if include_dir else [])+[R/'rebuild/include']
    inputs = {Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
    visited = set()
    def closure(path):
        if path in visited:return
        visited.add(path);inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q = next((q for q in [path.parent/name,*[inc/name for inc in includes]] if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    codes = build_codes(source,include_dir);raw = EXE.read_bytes()
    native = {a:read_virtual_data(raw,a,len(body)) for a,body in codes.items()}
    assert codes == native
    cases = 0;witnesses = {}
    for args in itertools.product((0,1,2,0x7fffffff,0xffffffff),(0,1,255),(0,1,255),range(3),(False,True),range(3),(0,0xc0,0xffffffff),(0,7)):
        try:
            actual,seen = execute(codes,args);original,_ = execute(native,args)
            assert actual == original
        except Exception as error:raise AssertionError(args) from error
        cases += 1
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%1215 == 0:print('installer pause cases=%d'%cases,flush=True)
    for root in roots:
        assert root['address'] in witnesses
        detected = False
        try:execute({a:b for a,b in codes.items() if a != root['address']},witnesses[root['address']])
        except Exception:detected = True
        assert detected,root['name']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    return dict(status='PASS',cases=cases,providers=len(roots),missing_provider_checks=len(roots),input_sha256=pins,
                boundaries=['Genuine scoped-lock constructor/Release/destructor execute; waits/events/locks controlled, no actual APC or worker.',
                            'Independent full storage/ABI/call-order/volatile-read model, DirtyDisc bypass, unsigned overflow and callback-sensitive handle reload.',
                            'WaitForSingleObjectEx is called once with infinite timeout/alertable true; failure/APC return values ignored as in original.'])
