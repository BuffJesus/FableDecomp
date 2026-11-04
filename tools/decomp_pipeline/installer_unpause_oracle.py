"""Installer pause-count release with genuine scoped locks and volatile reads."""
import hashlib
import itertools
import re
import struct
import tempfile
from pathlib import Path

import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ
from unicorn.x86_const import (UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,
    UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EIP)

R = Path(__file__).resolve().parents[2]
ENTRY, SIZE, SYMBOL = 0x9c3250,89,'?Unpause@CFileInstaller@@QAEXXZ'
SOURCE = R/'rebuild/src/compiled/00/9c/CFileInstaller_Unpause_009c3250.cpp'
ADDRESS = {'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,
           '??1CCriticalSectionLock@@QAE@XZ':0xa629a0,
           '__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,
           '__imp__LeaveCriticalSection@4':0x143fe10,'__imp__SetEvent@4':0x143fde0}
ARENA,LENGTH,OBJECT = 0x2000000,0x4000,0x2000100
STACK,STOP,API = 0x3000000,0x4000000,0x5000000
INITIAL = {seed:bytes((i*37+seed*19)&255 for i in range(LENGTH)) for seed in (0,7)}


def providers(source=SOURCE):
    roots = [dict(name='unpause',source=source,symbol=SYMBOL,address=ENTRY,size=SIZE)]
    for method,address,symbol in (
        ('Constructor',0xa62920,'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z'),
        ('Destructor',0xa629a0,'??1CCriticalSectionLock@@QAE@XZ')):
        roots.append(dict(name=method,source=R/'rebuild/src/compiled/00/a6'/('CCriticalSectionLock_%s_%08x.cpp'%(method,address)),symbol=symbol,address=address))
    return roots,dict(ADDRESS)


def build_codes(source=SOURCE,include_dir=None):
    roots,addresses = providers(source)
    includes = ([Path(include_dir)] if include_dir else [])+[R/'rebuild/include']
    codes = {}
    with tempfile.TemporaryDirectory(prefix='installer-unpause-') as tmp:
        for index,root in enumerate(roots):
            obj = Path(tmp)/('provider%d.obj'%index)
            result = V.cl(['/nologo','/c','/O2','/MT',*['/I'+str(p) for p in includes],'/Fo'+str(obj),str(root['source'])],V.env(),Path(tmp))
            assert result.returncode == 0,result.stdout+result.stderr
            c = Coff(obj)
            section,offset = c.symbols[root['symbol']]
            assert offset == 0
            body,relocations = c.sections[section]
            patched = bytearray(body)
            for site,(symbol,kind) in relocations.items():
                assert kind in (6,20) and struct.unpack_from('<i',body,site)[0] == 0
                if root['address'] == ENTRY and len(body) == SIZE:
                    original = read_virtual_data(EXE.read_bytes(),ENTRY,SIZE)
                    operand = struct.unpack_from('<i' if kind == 20 else '<I',original,site)[0]
                    assert (ENTRY+site+4+operand if kind == 20 else operand) == addresses[symbol]
                target = addresses[symbol]-(root['address']+site+4 if kind == 20 else 0)
                struct.pack_into('<I',patched,site,target&0xffffffff)
            codes[root['address']] = bytes(patched)
    return codes


def execute(codes,args):
    paused,running,handles,enter_mutation,leave_mutation,seed = args
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0x9c3000,0x1000),(0xa62000,0x1000),(0x143f000,0x1000),
                         (ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
        u.mem_map(address,size)
        if address in (0x9c3000,0xa62000):u.mem_write(address,b'\xcc'*size)
    for address,body in codes.items():u.mem_write(address,body)
    u.mem_write(ARENA,INITIAL[seed])
    def put(address,value):u.mem_write(address,struct.pack('<I',value&0xffffffff))
    def get(address):return struct.unpack('<I',u.mem_read(address,4))[0]
    first,second = ((0,0),(0x12345678,0x12345678),(0xffffffff,0),(0x12345678,0x87654321))[handles]
    put(OBJECT+0x7c,paused)
    u.mem_write(OBJECT+0x7b,bytes([running]))
    put(OBJECT+0x90,first)
    put(OBJECT+0x84,second)
    entered_paused = paused if enter_mutation == 0 else 0 if enter_mutation == 1 else 1
    entered_running = running if enter_mutation == 0 else 0 if enter_mutation == 1 else 1
    after_count = entered_paused-1 if entered_paused else 0
    waking = bool(entered_paused and entered_running and not after_count)
    final_count,final_running,final_event = after_count,entered_running,second
    if waking and handles == 3:
        final_count,final_running,final_event = 5,0,0xabcdef01
    event_second = final_event
    if leave_mutation:
        final_count,final_running = 0xfedcba98,1
    expected = bytearray(u.mem_read(ARENA,LENGTH))
    struct.pack_into('<I',expected,OBJECT-ARENA+0x7c,final_count)
    expected[OBJECT-ARENA+0x7b] = final_running
    struct.pack_into('<I',expected,OBJECT-ARENA+0x84,final_event)
    expected_reads = [(OBJECT+0x7c,4)]
    if entered_paused:
        expected_reads.extend([(OBJECT+0x7c,4),(OBJECT+0x7b,1)])
        if entered_running:expected_reads.append((OBJECT+0x7c,4))
    expected_trace = ['enter']+([('event',first),('event',event_second)] if waking else [])+['leave']
    for slot,target in ((0x143fe0c,API),(0x143fe10,API+16),(0x143fd30,API+32),(0x143fde0,API+48)):put(slot,target)
    trace,reads,seen = [],[],set()
    depth = 0
    def memory_read(uc,access,address,size,value,user):
        if address in (OBJECT+0x7b,OBJECT+0x7c):reads.append((address,size))
    def hook(uc,address,size,user):
        nonlocal depth
        seen.add(address)
        if address not in (API,API+16,API+32,API+48):return
        sp = uc.reg_read(UC_X86_REG_ESP)
        assert address != API+32,'Unpause must take a blocking scoped lock'
        if address == API:
            assert not trace and depth == 0 and get(sp+4) == OBJECT+4
            depth = 1
            trace.append('enter')
            put(OBJECT+0x7c,entered_paused)
            u.mem_write(OBJECT+0x7b,bytes([entered_running]))
        elif address == API+16:
            assert depth == 1 and get(sp+4) == OBJECT+4
            assert trace == expected_trace[:-1]
            depth = 0
            trace.append('leave')
            if leave_mutation:
                put(OBJECT+0x7c,final_count)
                u.mem_write(OBJECT+0x7b,bytes([final_running]))
        else:
            assert depth == 1 and waking
            event_index = len(trace)-1
            assert event_index in (0,1) and get(sp+4) == (first if event_index == 0 else event_second)
            trace.append(('event',get(sp+4)))
            if event_index == 0 and handles == 3:
                put(OBJECT+0x7c,5)
                u.mem_write(OBJECT+0x7b,b'\0')
                put(OBJECT+0x84,0xabcdef01)
        uc.reg_write(UC_X86_REG_EAX,0 if seed == 0 else 0xffffffff)
        uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp))
        uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook)
    u.hook_add(UC_HOOK_MEM_READ,memory_read)
    sp = STACK+0x8000
    put(sp,STOP)
    saved = (UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for index,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+index)
    u.reg_write(UC_X86_REG_ESP,sp)
    u.reg_write(UC_X86_REG_ECX,OBJECT)
    u.emu_start(ENTRY,STOP,count=500)
    assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == sp+4
    assert all(u.reg_read(reg) == 0x55660000+seed+index for index,reg in enumerate(saved))
    assert depth == 0 and trace == expected_trace
    state = bytes(u.mem_read(ARENA,LENGTH))
    assert state == bytes(expected)
    assert reads == expected_reads,('volatile_reads',reads,expected_reads)
    return (state,tuple(trace),tuple(reads)),seen


def compare_unpause(source=SOURCE,include_dir=None):
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
    codes = build_codes(source,include_dir)
    raw = EXE.read_bytes()
    native = {a:read_virtual_data(raw,a,SIZE if a == ENTRY else len(body)) for a,body in codes.items()}
    assert all(codes[a] == native[a] for a in codes if a != ENTRY)
    grade = compare(codes[ENTRY],native[ENTRY]).to_dict()
    assert grade['complete_decode'] and grade['near_match'] and grade['opcode_similarity'] >= .97
    assert len(codes[ENTRY]) == SIZE and grade['built_instructions'] == grade['retail_instructions'] == 34
    cases = 0
    witnesses = {}
    for args in itertools.product((0,1,2,0x80000000,0xffffffff),(0,1,255),range(4),range(3),(False,True),(0,7)):
        try:
            actual,seen = execute(codes,args)
            original,_ = execute(native,args)
            assert actual == original
        except Exception as error:raise AssertionError(args) from error
        cases += 1
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
    for root in roots:
        assert root['address'] in witnesses
        detected = False
        try:execute({a:b for a,b in codes.items() if a != root['address']},witnesses[root['address']])
        except Exception:detected = True
        assert detected,root['name']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    return dict(status='PASS',cases=cases,providers=len(roots),missing_provider_checks=len(roots),input_sha256=pins,grade=grade,
                boundaries=['Genuine scoped lock constructor/destructor execute; Win32 lock/event calls controlled, no real concurrency.',
                            'PDB-volatile Running/Paused memory reads checked in order, unsigned decrement, event order, alias/null handles and ignored event failure.',
                            'Full storage/ABI model, lock-entry/exit mutations and first-event mutation of later event and pause state.'])
