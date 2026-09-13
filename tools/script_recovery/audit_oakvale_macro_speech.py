"""Archive installed Oakvale macro speech and reviewed native operand slices.

This is a pinned-binary static audit, not parser execution or an audio test.
Instruction decoding starts at the known interpreter entry; reviewed slices
assert scalar pushes and preserve bytes for independent inspection.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import sys
import capstone
import pefile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.parse_frontend import load_all, decode_entry


def strings(value):
    raw = bytes.fromhex(value)
    if len(raw) < 4:
        raise ValueError('Missing vector count')
    count = int.from_bytes(raw[:4], 'little')
    offset = 4
    result = []
    for _ in range(count):
        end = raw.index(0, offset)
        result.append(raw[offset:end].decode('latin1'))
        offset = end + 1
    if offset != len(raw):
        raise ValueError('Trailing vector bytes')
    return result


def audit(game):
    source = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
    sources = sorted(source.rglob('*.lua'))
    tokens = set()
    for path in sources:
        tokens.update(re.findall(r'\bCS_[A-Z0-9_]+\b', path.read_text()))
    entries, schema, _ = load_all(str(game), str(ROOT / 'ghidra_out/def_schema.json'), 'script.bin')
    macros = []
    speech = []
    for entry in entries:
        if entry['name'] not in tokens or entry['definition'] != 'CCutsceneDef':
            continue
        decoded, leftover = decode_entry(entry, schema)
        assert not leftover
        record = dict(name=entry['name'], index=entry['index'],
                      payload_sha256=hashlib.sha256(entry['payload']).hexdigest())
        for field in ('Macro', 'SkipCond'):
            commands = strings(decoded[field])
            record[field] = commands
            # This command changes Speak's final boolean; none is authored here.
            assert not any('EnableBlackScreenSubtitles' in cmd for cmd in commands)
            for number, command in enumerate(commands):
                match = re.match(r'([^ .]+)\.(\w*Speak\w*)\s+(.+)', command)
                if match:
                    speech.append(dict(macro=entry['name'], field=field,
                                       index=number, command=command,
                                       actor=match[1], verb=match[2]))
        macros.append(record)
    counts = dict(Counter(item['verb'] for item in speech))
    assert len(macros) == 9 and counts == dict(Speak=17, InteractiveSpeak=8, DialogadSpeak=2, DialogSpeak=1)
    exe = game / 'Fable.exe'
    exe_hash = hashlib.sha256(exe.read_bytes()).hexdigest()
    assert exe_hash == '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
    pe = pefile.PE(str(exe))
    image = pe.get_memory_mapped_image()
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    instructions = list(decoder.disasm(image[0xcbfb7d-0x400000:0xcd1900-0x400000], 0xcbfb7d))
    by_address = {ins.address: ins for ins in instructions}
    checks = {
        0xcbfc56: ('mov', 'byte ptr [ebp - 0x234], 0'),
        0xcc5ef1: ('mov', 'byte ptr [ebp - 0x234], 1'),
        0xcc268c: ('and', 'dword ptr [ebp + 0x14], 0'),
        0xcc26f3: ('mov', 'dword ptr [ebp + 0x14], 1'),
        0xcc2746: ('mov', 'dword ptr [ebp + 0x14], 2'),
        0xcc27ea: ('push', 'dword ptr [ebp - 0x234]'),
        0xcc27f2: ('push', '1'), 0xcc27f4: ('push', '0'),
        0xcc2882: ('push', 'dword ptr [ebp - 0x234]'),
        0xcc288a: ('push', '1'), 0xcc288c: ('push', '0'),
        0xcc3517: ('push', '0'), 0xcc3519: ('push', '1'), 0xcc351b: ('push', '0'),
        0xcc35a6: ('push', '0'), 0xcc35a8: ('push', '1'), 0xcc35aa: ('push', '0'),
        0xcc2f50: ('push', '1'), 0xcc2f52: ('push', '1'),
        0xcc32bf: ('push', '1'), 0xcc32c1: ('push', '1'),
    }
    line_flags = [0xcc2f92, 0xcc2fc1, 0xcc2ff0, 0xcc301f, 0xcc304e, 0xcc307d, 0xcc30ac, 0xcc3301]
    checks.update({address: ('push', '1') for address in line_flags})
    for address, expected in checks.items():
        ins = by_address[address]
        assert (ins.mnemonic, ins.op_str) == expected, hex(address)
    ranges = [(0xcbfc52, 0xcbfc65), (0xcc268c, 0xcc2816), (0xcc2882, 0xcc28a2),
              (0xcc2f29, 0xcc30b9), (0xcc32b7, 0xcc3310),
              (0xcc3515, 0xcc353d), (0xcc35a4, 0xcc35c0), (0xcc5ea0, 0xcc5efd)]
    slices = []
    for start, end in ranges:
        assert start in by_address and end in by_address
        slices.append(dict(start=hex(start), end_exclusive=hex(end),
                           bytes=image[start-0x400000:end-0x400000].hex(),
                           instructions=[f'{ins.address:08X} {ins.mnemonic} {ins.op_str}'
                                         for ins in instructions if start <= ins.address < end]))
    return dict(scope=__doc__, exe_sha256=exe_hash,
                script_sha256=hashlib.sha256((game / 'data/CompiledDefs/script.bin').read_bytes()).hexdigest(),
                source_sha256={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},
                macros=macros, speech=speech, counts=counts,
                native_scalar_assertions=len(checks), native_slices=slices,
                limitations=['Static reviewed slices, not full parser execution.',
                             'Macro calls retain native parsing; no audible gain or rendered fade proof.'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, default=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters'))
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise SystemExit('Output exists')
    report = audit(args.game)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(macros=len(report['macros']), speech=report['counts'], native_scalar_assertions=report['native_scalar_assertions'])))
