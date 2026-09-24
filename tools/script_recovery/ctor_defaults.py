"""Constant member values a retail script-class constructor leaves behind, read by emulating it (read-only).

The converter has no constructor body: Init stood in only for the CTimer registrations. The constructor also
zero-initialises scalar members Init never touches (Q_OrchardFarmRaid 0x00DCC040: each CCrateTeamManager's
StateCounter[0..5] and MemberCount). The sidecar keeps quest state across an in-process load, so a counter that
Init does not reset keeps a stale value (live 2026-09-24: Teams_1_MemberCount stayed 1 after a reload, and wave 2
never spawned).

The constructor is run with unicorn over the mapped retail image on an object pre-filled with a sentinel byte.
Every call is skipped: EAX gets a sentinel, and ESP drops by the callee's proven stack operands (a direct
callee's RET, `convert_quest_unit.callee_stack_words`; an interface vcall's slot prototype). An unprovable
purge aborts the whole emulation (no evidence rather than wrong evidence). A field counts as initialised when
every byte of it was written and its final value is not a sentinel (call result, constructor argument, a read
through either, a writable global), an image address (a vtable) or a pointer into the object. Limitation: a
one-byte Bool copied out of tainted memory cannot be told apart by value (constructors seen so far set bools
from immediates); a Bool field is therefore only taken when the constructor's store was an immediate.
"""
from __future__ import annotations

import struct

FILL = 0xCC
OBJ_BASE = 0x20000000
OBJ_SIZE = 0x4000
STACK_BASE = 0x30000000
STACK_SIZE = 0x10000
LOW_SIZE = 0x100000          # the null page reads TAINT (dereferences of unset pointers stay mapped)
RETURN_SENTINEL = 0x0CAFE000
CALL_SENTINEL = 0xDEAD0000   # EAX after a skipped call: 0xDEAD0000 + n
ARG_SENTINEL = 0xDEAD1000    # constructor stack arguments: argument i points at ARG_SENTINEL + i * 0x100
MAX_INSNS = 200000


TAINT = 0xDEAD8000
IMMEDIATE_KEY = -1           # emulate()'s result carries the immediate-stored byte offsets under this key


def _sections(rdata):
    """(va, virtual size, raw offset, raw size, writable) per section, from the PE section table."""
    data = rdata.data
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    nsec = struct.unpack_from('<H', data, pe + 6)[0]
    opt = struct.unpack_from('<H', data, pe + 20)[0]
    out = []
    for i in range(nsec):
        off = pe + 24 + opt + i * 40
        vsize, va, rsize, raw = struct.unpack_from('<IIII', data, off + 8)
        flags = struct.unpack_from('<I', data, off + 36)[0]
        out.append((va, vsize, raw, rsize, bool(flags & 0x80000000)))
    return out


def _image():
    from tools.script_recovery.lift_native_lua import RData
    rdata = RData()
    return rdata if rdata.ok else None


def emulate(ctor: int, slot_words, *, rdata=None, allocators=frozenset()) -> dict | None:
    """{object offset: byte value} per byte the code wrote into the object (plus IMMEDIATE_KEY: the offsets
    last stored from an immediate), or None when emulation is not provable. `slot_words(disp)` gives an indirect vcall's stack operand count (None = unknown). With
    `allocators` (operator new's call targets) the function is an allocator with the constructor inlined
    (`this_00 = operator_new(n); ... *this_00 = &vtable`): the allocation returns the object, and ECX (the
    factory's own receiver) is tainted instead."""
    try:
        from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE, UcError
        from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        from capstone.x86 import X86_OP_IMM, X86_OP_MEM
    except ImportError:
        return None
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    rdata = rdata or _image()
    if rdata is None:
        return None
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    # Everything whose runtime value the image does not fix -- writable sections (.data/.bss globals such as the
    # GSI interface pointer), the null page, constructor arguments and call results -- reads as TAINT, a dword
    # that points back into the tainted pages: dereference chains stay mapped, and any value that came from
    # there is recognisably not a constant (an entity ctor copies its parent quest's +0x40 into this+4).
    taint = struct.pack('<I', TAINT) * 0x400
    for va, vsize, raw, rsize, writable in _sections(rdata):
        start = rdata.base + va
        page = start & ~0xFFF
        end = (start + max(vsize, rsize, 1) + 0xFFF) & ~0xFFF
        uc.mem_map(page, end - page)
        if writable:
            for addr in range(page, end, 0x1000):
                uc.mem_write(addr, taint)
        else:
            uc.mem_write(start, rdata.data[raw:raw + rsize])
    uc.mem_map(0, LOW_SIZE)
    for addr in range(0, LOW_SIZE, 0x1000):
        uc.mem_write(addr, taint)
    uc.mem_map(OBJ_BASE, OBJ_SIZE)
    uc.mem_write(OBJ_BASE, bytes([FILL]) * OBJ_SIZE)
    uc.mem_map(STACK_BASE, STACK_SIZE)
    uc.mem_map(CALL_SENTINEL, 0x10000)
    for addr in range(CALL_SENTINEL, CALL_SENTINEL + 0x10000, 0x1000):
        uc.mem_write(addr, taint)
    esp = STACK_BASE + STACK_SIZE - 0x100
    frame = struct.pack('<I', RETURN_SENTINEL) + b''.join(struct.pack('<I', ARG_SENTINEL + 0x100 * i) for i in range(8))
    uc.mem_write(esp, frame)
    uc.reg_write(UC_X86_REG_ESP, esp)
    uc.reg_write(UC_X86_REG_ECX, TAINT if allocators else OBJ_BASE)
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    cs.detail = True
    written: dict[int, int] = {}
    immediate: set[int] = set()     # object bytes whose last store came from an immediate operand
    state = {'calls': 0, 'abort': None, 'count': 0, 'imm': False}

    def on_write(uc_, access, address, size, value, user):
        if OBJ_BASE <= address < OBJ_BASE + OBJ_SIZE:
            data = (value & ((1 << (8 * size)) - 1)).to_bytes(size, 'little')
            for i in range(size):
                written[address - OBJ_BASE + i] = data[i]
                (immediate.add if state['imm'] else immediate.discard)(address - OBJ_BASE + i)

    def on_code(uc_, address, size, user):
        state['count'] += 1
        if state['count'] > MAX_INSNS:
            state['abort'] = 'instruction limit'
            uc_.emu_stop()
            return
        code = bytes(uc_.mem_read(address, size))
        ins = next(cs.disasm(code, address), None)
        state['imm'] = bool(ins is not None and ins.mnemonic == 'mov' and len(ins.operands) == 2
                            and ins.operands[1].type == X86_OP_IMM)
        if ins is None or ins.mnemonic != 'call':
            return
        op = ins.operands[0]
        if op.type == X86_OP_IMM:
            words = callee_stack_words(op.imm)
            if words is None and _import_thunk(uc_, op.imm) and _caller_cleans(uc_, cs, address + size):
                words = 0       # a __cdecl import (`jmp [__imp_??2@YAPAXI@Z]`, operator new): the caller's `add esp`
        elif op.type == X86_OP_MEM and op.mem.base and not op.mem.index:
            words = slot_words(op.mem.disp)
        else:
            words = None
        if words is None:
            state['abort'] = f'unproven purge at {address:#x} ({ins.op_str})'
            uc_.emu_stop()
            return
        state['calls'] += 1
        # only the first allocation is the object: an entity factory allocates its 12-byte shared holder next
        allocation = op.type == X86_OP_IMM and op.imm in allocators and not state.get('allocated')
        state['allocated'] = state.get('allocated') or allocation
        uc_.reg_write(UC_X86_REG_EAX, OBJ_BASE if allocation else CALL_SENTINEL + state['calls'])
        uc_.reg_write(UC_X86_REG_ESP, uc_.reg_read(UC_X86_REG_ESP) + 4 * words)
        uc_.reg_write(UC_X86_REG_EIP, address + size)

    uc.hook_add(UC_HOOK_CODE, on_code)
    uc.hook_add(UC_HOOK_MEM_WRITE, on_write)
    try:
        uc.emu_start(ctor, RETURN_SENTINEL)
    except UcError as e:
        if uc.reg_read(UC_X86_REG_EIP) != RETURN_SENTINEL:
            return None
    if state['abort'] or uc.reg_read(UC_X86_REG_EIP) != RETURN_SENTINEL:
        return None
    written[IMMEDIATE_KEY] = frozenset(immediate)
    return written


def _import_thunk(uc, target: int) -> bool:
    """`jmp dword ptr [abs32]` (FF 25 imm32): an import thunk."""
    try:
        return bytes(uc.mem_read(target, 2)) == b'\xff\x25'
    except Exception:
        return False


def _caller_cleans(uc, cs, after_call: int) -> bool:
    """An `add esp, N` follows the call within a few register moves (`call new | mov esi, eax | add esp, 4`,
    Q_TraderConflictEvil Alloc 0x00DFA029): a caller-cleaned (__cdecl) call."""
    for k, ins in enumerate(cs.disasm(bytes(uc.mem_read(after_call, 32)), after_call)):
        if ins.mnemonic == 'add' and ins.op_str.startswith('esp, '):
            return True
        if k >= 3 or ins.mnemonic in ('call', 'ret', 'push', 'pop') or ins.mnemonic.startswith('j') or 'esp' in ins.op_str:
            return False
    return False


def _plausible_constant(value: int, size: int) -> bool:
    if size == 4:
        if CALL_SENTINEL <= value < CALL_SENTINEL + 0x10000:    # call results, constructor arguments, reads through them
            return False
        if 0x400000 <= value < 0x2000000:                      # image addresses (vtables, globals)
            return False
        if OBJ_BASE <= value < OBJ_BASE + OBJ_SIZE:            # pointers into the object
            return False
    return True


def constant_fields(written: dict[int, int], fields: dict[str, list]) -> dict[str, tuple[str, object]]:
    """{field name: (kind, value)} for Int / Bool / Float fields the constructor fully wrote with a constant."""
    out = {}
    for off_hex, (name, kind) in fields.items():
        off = int(off_hex, 16)
        size = 1 if kind == 'Bool' else 4 if kind in ('Int', 'Float') else None
        if size is None or not all(off + i in written for i in range(size)):
            continue
        if kind == 'Bool' and off not in written.get(IMMEDIATE_KEY, frozenset()):
            continue            # a byte copied from tainted memory is not recognisable by value
        raw = bytes(written[off + i] for i in range(size))
        value = int.from_bytes(raw, 'little')
        if not _plausible_constant(value, size):
            continue
        if kind == 'Bool':
            out[name] = (kind, value != 0)
        elif kind == 'Float':
            out[name] = (kind, struct.unpack('<f', raw)[0])
        else:
            out[name] = (kind, value - (1 << 32) if value & 0x80000000 else value)
    return out


def find_constructor(functions: list[dict], vtable: int, exclude: set[int]) -> list[tuple[int, frozenset]]:
    """(address, operator-new targets) of the unit function that stores `vtable` into a fresh object: a
    constructor (`*(undefined ***)this = &PTR_..._<vtable>;`, no allocator targets) or an allocator with the
    constructor inlined (the store goes to the result of `operator_new`), for every such function. Destructors
    are excluded."""
    import re
    hits = []
    for f in functions:
        address = int(f['address'], 16)
        text = f.get('decompile') or ''
        if address in exclude:
            continue
        # (`*(undefined ***)this = &PTR_..`, or an entity factory's `*puVar2 = &PTR_..` on operator new's result)
        store = re.search(r'\*(?:\((?:undefined \*\*\*|void \*\*|int \*)\))?\(?(\w+)\)? = &PTR_\w*?_%08x;' % vtable, text)
        if not store:
            continue
        if store.group(1) == 'this':
            hits.append((address, frozenset()))
            continue
        news = frozenset(int(c['target'], 16) for c in f.get('calls', [])
                         if 'operator_new' in (c.get('currentName') or '').replace(' ', '_') and c.get('target'))
        if news and re.search(r'\b%s = (?:\([^;]*?\))?(?:::)?operator_new\(' % re.escape(store.group(1)), text):
            hits.append((address, news))
    return hits


def constructor_constants(functions: list[dict], vtable: int, exclude: set[int], fields: dict, slot_words,
                          *, rdata=None) -> dict[str, tuple[str, object]]:
    """Constant fields of the class constructed with `vtable`. VC7.1 may emit the constructor out of line AND
    inline it into the allocator (Guild's CV_AmbushScamScript 0x00D3B390 and its Alloc 0x00D50600): every
    candidate is emulated and the result is used only when all of them agree ({} otherwise)."""
    results = []
    for address, allocators in find_constructor(functions, vtable, exclude):
        written = emulate(address, slot_words, rdata=rdata, allocators=allocators)
        if written is None:
            return {}
        results.append(constant_fields(written, fields))
    if not results or any(r != results[0] for r in results[1:]):
        return {}
    return results[0]
