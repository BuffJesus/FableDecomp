"""Execute retail GFMain instructions with explicit, in-memory leaf boundaries.

This is a control-flow oracle, not execution of the complete game or its callees.
No emulated import touches host files, the registry, windows, or processes.
"""
from pathlib import Path
import json
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from gate_game_component_abi import Coff
import struct
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / "rebuild/integration/native_gfmain"
SPEC = json.loads((WORK / 'abi.json').read_text())
HELPERS = {int(h['address'], 16): h for h in SPEC['helpers']}
PE = pefile.PE(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe')
IMAGE = PE.get_memory_mapped_image()
BASE = PE.OPTIONAL_HEADER.ImageBase
NATIVE_BODY = PE.get_data(0x00402510 - BASE, 3952)
assert hashlib.sha256(NATIVE_BODY).hexdigest() == SPEC['body_sha256'], 'Unsupported retail GFMain image'
OPS = {i.address: {'instruction': i.mnemonic + ' ' + i.op_str}
       for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(NATIVE_BODY, 0x00402510)}


class Oracle:
    def __init__(self, **scenario):
        self.scenario = scenario
        self.ops = OPS
        self.entrypoint = 0x00402510
        self.site_alias = {}
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.uc.mem_map(BASE, (len(IMAGE) + 4095) & ~4095)
        self.uc.mem_write(BASE, IMAGE)
        self.uc.mem_map(0x02000000, 0x200000)
        self.uc.mem_map(0x03000000, 0x100000)
        self.next_heap = 0x02001000
        self.events = []
        self.global_writes = set()
        self.visited = set()
        self.edges = set()
        self.previous = None
        self.permission_calls = 0
        self.file_calls = 0
        self.exit_code = None
        self.init_address = None
        self.profile_address = None
        self.system = self.alloc(0x100)
        self.p32(self.system + 0x94, 0x1234)
        self.singleton = self.alloc(0x100)
        # Scenario-owned settings; all other bytes retain their retail image value.
        defaults = {0x0137548f: 0, 0x01375459: 0, 0x013b8615: 0,
                    0x01375447: 0, 0x013b8634: 0, 0x013b8616: 0,
                    0x013b8605: 0, 0x013b8617: 0, 0x013b8648: 0,
                    0x013b861d: 0, 0x01375449: 0}
        defaults.update(scenario.get('settings', {}))
        for address, value in defaults.items():
            self.uc.mem_write(address, bytes([value]))
        self.p32(0x013ca818, 0)
        self.p32(0x013ca81c, 0)
        if scenario.get('initial_owner_refs', 0):
            info = self.alloc(12)
            self.p32(info, scenario['initial_owner_refs'])
            self.p32(info + 8, self.alloc(16))
            self.p32(0x013ca818, self.u32(info + 8))
            self.p32(0x013ca81c, info)
        self.initial_sp = 0x030ff000
        self.p32(self.initial_sp, 0x02000000)
        self.p32(self.initial_sp + 4, 7)
        self.uc.reg_write(UC_X86_REG_ESP, self.initial_sp)
        self.uc.reg_write(UC_X86_REG_ECX, 0x9876)
        self.command_line = self.alloc(16)
        self.uc.mem_write(self.command_line, b'-oracle\0')
        self.uc.reg_write(UC_X86_REG_EDX, self.command_line)
        self.saved = {UC_X86_REG_EBX: 0x11111111, UC_X86_REG_ESI: 0x22222222,
                      UC_X86_REG_EDI: 0x33333333, UC_X86_REG_EBP: 0x44444444}
        for register, value in self.saved.items():
            self.uc.reg_write(register, value)
        self.uc.hook_add(UC_HOOK_CODE, self.instruction)
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self.memory_write)

    def memory_write(self, uc, access, address, size, value, unused):
        if BASE <= address < BASE + len(IMAGE):
            self.global_writes.update(range(address, address + size))

    def alloc(self, size):
        address = self.next_heap
        self.next_heap += (size + 15) & ~15
        assert self.next_heap < 0x02200000
        self.uc.mem_write(address, bytes(size))
        return address

    def u32(self, address):
        return struct.unpack('<I', self.uc.mem_read(address, 4))[0]

    def p32(self, address, value):
        self.uc.mem_write(address, struct.pack('<I', value & 0xffffffff))

    def literal(self, address, wide=False):
        unit = 2 if wide else 1
        data = bytearray()
        if not address:
            return ''
        for offset in range(0, 8192, unit):
            part = bytes(self.uc.mem_read(address + offset, unit))
            if part == bytes(unit):
                return data.decode('utf-16le' if wide else 'latin1')
            data.extend(part)
        raise AssertionError('Unterminated emulated string')

    def string(self, owner, wide=False):
        storage = self.u32(owner)
        return self.literal(self.u32(storage), wide) if storage else ''

    def set_string(self, owner, text, wide=False):
        encoded = text.encode('utf-16le' if wide else 'latin1')
        unit = 2 if wide else 1
        data = self.alloc(len(encoded) + unit)
        self.uc.mem_write(data, encoded + bytes(unit))
        storage = self.alloc(20)
        self.p32(storage, data)
        self.p32(storage + 4, data + len(encoded) if wide else len(encoded))
        self.p32(storage + 8, data + len(encoded) + 2 if wide else len(encoded) + 1)
        self.p32(storage + (12 if wide else 13), 1)
        self.p32(owner, storage)
        return owner

    def arguments(self, helper):
        sp = self.uc.reg_read(UC_X86_REG_ESP)
        args = []
        regs = []
        if helper['cc'] == '__fastcall':
            regs = [UC_X86_REG_ECX, UC_X86_REG_EDX]
        elif helper['cc'] == '__thiscall':
            args.append(self.uc.reg_read(UC_X86_REG_ECX))
        for index in range(len(helper['params'])):
            args.append(self.uc.reg_read(regs[index]) if index < len(regs)
                        else self.u32(sp + 4 * (index - len(regs))))
        stack_size = 4 * max(0, len(helper['params']) - len(regs))
        return args, 0 if helper['cc'] == '__cdecl' else stack_size

    def leaf(self, target, a):
        cfg = self.scenario
        if target == 0x00403b10:
            self.init_address = a[0]
            self.uc.mem_write(a[0], bytes(200))
            return a[0]
        if target == 0x0099ebf0:
            return self.set_string(a[0], self.literal(a[1]))
        if target in (0x0099e4b0, 0x0099aed0):
            self.p32(a[0], 0)
            if target == 0x0099aed0 and self.profile_address is None:
                self.profile_address = a[0]
            return a[0]
        if target in (0x0099eae0, 0x0099b510):
            return 0
        if target in (0x0099b6b0, 0x0099b800):
            return self.set_string(a[0], self.literal(a[1], True), True)
        if target in (0x0099efb0, 0x0099b7d0):
            self.p32(a[0], self.u32(a[1]))
            return a[0]
        if target == 0x0099e480:
            return self.set_string(a[1], self.string(a[0]), True)
        if target == 0x0099af10:
            storage = self.u32(a[0])
            return self.u32(storage) if storage else 0x0122d70c
        if target == 0x0099b150:
            return int(self.string(a[0], True) != self.literal(a[1], True))
        if target in (0x0099be70, 0x0099bf30, 0x0099bff0):
            left = self.literal(a[1], True) if target == 0x0099bff0 else self.string(a[1], True)
            right = self.literal(a[2], True) if target == 0x0099bf30 else self.string(a[2], True)
            return self.set_string(a[0], left + right, True)
        if target in (0x00415260, 0x00415440, 0x00415530):
            return self.set_string(a[0], 'english')
        if target in (0x00997510, 0x004023f0, 0x0041a0a0, 0x0041a0c0,
                      0x0041a100, 0x0041a120, 0x0041a180, 0x0041a1e0,
                      0x0041a330, 0x0041a430, 0x0041a4c0, 0x0041a540):
            return self.set_string(a[0], f'directory_{target:08x}/', True)
        if target == 0x00999230:
            return int(cfg.get('settings_exist', False))
        if target == 0x0099ad80:
            self.uc.mem_write(a[0], bytes(28))
            self.uc.mem_write(a[0] + 20, bytes([cfg.get('settings_open', True)]))
            return a[0]
        if target in (0x00404720, 0x009badd0):
            self.uc.mem_write(a[0], bytes(40 if target == 0x00404720 else 44))
            return a[0]
        if target == 0x0099f2e0:
            return self.set_string(a[0], '')
        if target == 0x004045c0:
            self.uc.mem_write(a[2], bytes(self.uc.mem_read(a[3], 1)))
            return 0
        if target == 0x00402130:
            values = cfg.get('permissions', [0, 0])
            value = values[min(self.permission_calls, len(values) - 1)]
            self.permission_calls += 1
            return value
        if target in (0x00401fe0, 0x004017b0, 0x004022b0):
            # An explicit boundary mutation exercises the otherwise empty
            # startup-profile branch, including the draft's two register bytes.
            if target == 0x004022b0 and cfg.get('startup_profile'):
                self.set_string(self.profile_address, cfg['startup_profile'], True)
            return int(cfg.get({0x00401fe0: 'eula', 0x004017b0: 'config', 0x004022b0: 'game_init'}[target], True))
        if target == 0x009a6610:
            self.init_snapshot = bytes(self.uc.mem_read(a[1], 200)).hex()
            return cfg.get('system_code', 1)
        if target == 0x009a4ec0:
            return self.system
        if target in (0x00404440, 0x00404a80, 0x0040d2a0):
            return self.singleton
        if target == 0x009c85a0:
            self.p32(a[0], self.alloc(32))
            return a[0]
        if target == 0x00403e40:
            self.p32(a[0], a[1])
            info = self.alloc(12)
            self.p32(info, cfg.get('bank_refs', 1))
            self.p32(info + 8, a[1])
            self.p32(a[0] + 4, info)
            return 0
        if target == 0x009cc410:
            return 7 if cfg.get('text', True) else 0
        if target == 0x009c9070:
            if not cfg.get('entry', True):
                self.p32(a[1], 0)
                return a[1]
            entry = self.alloc(68)
            self.p32(entry, self.alloc(32))
            self.p32(entry + 4, cfg.get('entry_refs', 1))
            self.p32(a[1], entry)
            return a[1]
        if target == 0x009c7a40:
            return self.set_string(a[1], 'localized error', True)
        if target == 0x004014a0:
            crc = a[0]
            for byte in self.uc.mem_read(a[1], a[2]):
                crc ^= byte
                for _ in range(8):
                    crc = (crc >> 1) ^ (0xedb88320 if crc & 1 else 0)
            return crc
        return 0

    def semantic_arguments(self, target, args):
        helper = HELPERS[target]
        offset = int('this' in helper)
        result = []
        for parameter, value in zip(helper['params'], args[offset:]):
            name, typ = parameter['name'], parameter['type']
            if name == 'result':
                continue
            if typ in ('NativeCharString *', 'NativeWideString *'):
                result.append(self.string(value, typ == 'NativeWideString *') if value else None)
            elif typ in ('char *', 'ushort *') and name in ('text', 'name', 'left', 'right'):
                result.append(self.literal(value, typ == 'ushort *'))
            elif typ == 'bool':
                result.append(value & 255)
            elif typ in ('int', 'uint'):
                result.append(value)
            elif typ == 'char *' and name in ('value', 'defaultValue'):
                result.append(self.uc.mem_read(value, 1)[0])
            else:
                result.append('pointer' if value else None)
        if target in (0x0099eae0, 0x0099b510):
            result.append(self.string(args[0], target == 0x0099b510))
        return result

    def instruction(self, uc, address, size, unused):
        assert address in self.ops, f'Execution escaped GFMain: {address:08x}'
        if self.previous is not None:
            self.edges.add((self.previous, address))
        self.previous = address
        self.visited.add(address)
        op = self.ops[address]['instruction']
        if not op.startswith('call '):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        site = self.site_alias.get(address, address)
        cleanup = 0
        result = 0
        semantic = []
        if op.startswith('call 0x'):
            target = int(op.split()[1], 16)
            if target == 0x00401284:
                self.exit_code = self.u32(sp)
                self.events.append({'site': f'{address:08x}', 'target': 'exit', 'args': [self.exit_code]})
                uc.emu_stop()
                return
            if target in HELPERS:
                args, cleanup = self.arguments(HELPERS[target])
                semantic = self.semantic_arguments(target, args)
                result = self.leaf(target, args)
            elif target == 0x00bfea1a:
                args = [self.u32(sp)]
                result = self.alloc(args[0])
            elif target in (0x00bfe9bc, 0x00bfe9f9):
                args = []
                # Cookie verification must preserve the return value in EAX.
                if target == 0x00bfe9f9:
                    assert uc.reg_read(UC_X86_REG_ECX) == self.u32(0x0139c8a8)
                result = uc.reg_read(UC_X86_REG_EAX)
            else:
                raise AssertionError(f'Unmodelled direct call {target:08x}')
            label = f'{target:08x}'
        else:
            args = []
            label = f'indirect_{site:08x}'
            if site in (0x00402d93, 0x00402ded):
                cleanup = 28
                args = [self.u32(sp + i * 4) for i in range(7)]
                semantic = [self.literal(args[0], True)] + args[1:]
                values = self.scenario.get('file_open', [True, True])
                result = 0x5678 if values[self.file_calls] else 0xffffffff
                self.file_calls += 1
            elif site in (0x00402db0, 0x00402e00):
                cleanup, result = 4, 1
            elif site == 0x00402ea5:
                cleanup, result = 8, 1
            elif site == 0x00402fce:
                cleanup, result = 16, 1
                args = [self.u32(sp + i * 4) for i in range(4)]
                semantic = [args[0], self.literal(args[1], True), self.literal(args[2], True), args[3]]
            else:
                assert site in (0x00402648, 0x00402f15, 0x00402faa, 0x00402ffe)
        self.events.append({'site': f'{address:08x}', 'target': label, 'args': args, 'semantic': semantic})
        uc.reg_write(UC_X86_REG_EAX, result & 0xffffffff)
        uc.reg_write(UC_X86_REG_ESP, sp + cleanup)
        uc.reg_write(UC_X86_REG_EIP, address + size)

    def run(self):
        self.uc.emu_start(self.entrypoint, 0x02000000, count=10000)
        if self.exit_code is None:
            assert self.uc.reg_read(UC_X86_REG_EIP) == 0x02000000
            assert self.uc.reg_read(UC_X86_REG_ESP) == self.initial_sp + 8
            for register, value in self.saved.items():
                assert self.uc.reg_read(register) == value
        return {'return': self.uc.reg_read(UC_X86_REG_EAX), 'exit': self.exit_code,
                'events': self.events, 'visited': sorted(self.visited),
                'edges': sorted(self.edges)}



SYMBOLS = {name: int(value, 16) for name, value in json.loads((WORK / 'symbols.json').read_text()).items()}


def configure_candidate(object_path):
    global COFF, SECTION, START, BUILT, RELOCS
    COFF = Coff(Path(object_path))
    SECTION, START = COFF.symbols['@GFMain@12']
    BUILT, RELOCS = COFF.sections[SECTION]
    assert START == 0


class CandidateOracle(Oracle):
    def __init__(self, **scenario):
        super().__init__(**scenario)
        self.entrypoint = 0x04000000
        self.uc.mem_map(self.entrypoint, 0x100000)
        self.data_cursor = self.entrypoint + 0x10000
        self.literal_addresses = {}
        code = bytearray(BUILT)
        for offset, (symbol, kind) in RELOCS.items():
            target = self.resolve(symbol)
            addend = int.from_bytes(code[offset:offset + 4], 'little')
            value = target + addend - (self.entrypoint + offset + 4 if kind == 20 else 0)
            assert kind in (6, 20)
            struct.pack_into('<I', code, offset, value & 0xffffffff)
        self.uc.mem_write(self.entrypoint, bytes(code))
        self.ops = {}
        for instruction in Cs(CS_ARCH_X86, CS_MODE_32).disasm(code, self.entrypoint):
            self.ops[instruction.address] = {'instruction': instruction.mnemonic + ' ' + instruction.op_str}
        # Both bodies retain the same ten indirect call roles in physical order.
        # Their arguments and effects are compared independently in the matrix.
        native_indirect = [a for a, row in OPS.items()
                           if row['instruction'].startswith('call ') and not row['instruction'].startswith('call 0x')]
        draft_indirect = [a for a, row in self.ops.items()
                          if row['instruction'].startswith('call ') and not row['instruction'].startswith('call 0x')]
        assert len(native_indirect) == len(draft_indirect), draft_indirect
        self.site_alias = dict(zip(draft_indirect, native_indirect))

    def resolve(self, name):
        if name in SYMBOLS:
            return SYMBOLS[name]
        # Every address-labelled external is bound to the corresponding retail
        # object; addends still select member offsets inside that same object.
        match = re.search(r'(?:g_|callback_)[A-Za-z0-9_]*?([0-9a-fA-F]{8})(?:@|$)', name)
        if match:
            return int(match.group(1), 16)
        special = {
            '?GUseRetailBanks@@3_NA': 0x013b8616,
            '___security_cookie': 0x0139c8a8,
            '?GFIsSystemErrorFailure@@YI_NW4ESystemCode@@@Z': 0x00401bc0,
            '?g_Game_013B83D0@@3VCGame@@A': 0x013b83d0,
            '?GConfigOptions@@3UCConfigOptions@@A': 0x01375070,
            '?GAME_REG_KEY@CSystemRegistry@@2VCWideString@@B': 0x013bca28,
            '?GFX_RESET_KEY_NAME@CSystemRegistry@@2VCWideString@@B': 0x013bca2c,
            '?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A': 0x013ca818,
            '?g_FableEmptyNarrowString_0129AAF4@@3PADA': 0x0129aaf4,
            '??_7CDiskFileWin32@@6B@': 0x0122d06c,
            '?g_CreateFileW@@3P6GPAXPBGKKPAXKK1@ZA': 0x0143fe2c,
            '?g_CloseHandle@@3P6GHPAX@ZA': 0x0143fe30,
            '?g_MessageBoxW@@3P6GHPAUHWND__@@PBG1I@ZA': 0x014402c8,
            '__imp__CreateFileW@28': 0x0143fe2c,
            '__imp__CloseHandle@4': 0x0143fe30,
            '__imp__MessageBoxW@16': 0x014402c8,
        }
        if name in special:
            return special[name]
        if name in self.literal_addresses:
            return self.literal_addresses[name]
        if name.startswith('??_C@'):
            section, start = COFF.symbols[name]
            data, relocs = COFF.sections[section]
            assert not relocs
            address = self.data_cursor
            self.data_cursor += (len(data) + 15) & ~15
            self.uc.mem_write(address, data)
            self.literal_addresses[name] = address + start
            return address + start
        raise AssertionError('Unmapped GFMain data symbol: ' + name)


