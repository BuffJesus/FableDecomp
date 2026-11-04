"""Restore Madame's six literal-name camera point calls."""
import hashlib
import json
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


ROOT = Path(__file__).resolve().parents[2]
OLD = [
    'quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, 1)',
    'quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, fVar19)',
    'quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, i_stk_19c)',
    'quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, iVar7, 0x1)',
    'quest:CameraUseCameraPoint(me, pCVar8, -1.0, 0x1, nil --[[missing]])',
    'quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, nil --[[missing]])',
]
NEW = 'quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)'
DIAGNOSTICS = (["CameraUseCameraPoint: missing spTarget (4 args for 5 params)"] * 4
               + ["CameraUseCameraPoint: missing i2 (4 args for 5 params)",
                  "CameraUseCameraPoint: missing spTarget, i2 (3 args for 5 params)"])


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame camera point {fn["address"]}: {why}')

    slots = json.loads((ROOT / 'refs/script_recovery/bordello/typing_spec.json').read_text())
    slot = slots['slots']['0x678'] if 'slots' in slots else slots['0x678']
    require(slot['name'] == 'CameraUseCameraPoint_NameAndTarget'
            and [p['name'] for p in slot['params']] ==
            ['pCamPointName', 'pTarget', 'duration', 'i1', 'i2'],
            'camera point ABI changed')
    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    require(rdata.bytes_at(0x012E402C, 12) == b'CAM_B_MADAM\0', 'camera point literal changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    require(indexed[0xE3BB7E].bytes.hex() == '8d5d08', 'Madame self register changed')
    for site in row['sites']:
        name = indexed.get(int(site['nameLiteralSite'], 16))
        target = indexed.get(int(site['targetPushSite'], 16))
        call = indexed.get(int(site['callSite'], 16))
        require(name is not None and name.bytes.hex() == '682c402e01'
                and target is not None and target.bytes.hex() == '53'
                and call is not None and call.bytes.hex() == site['callBytes'],
                f'camera point call changed at {site["callSite"]}')
        between = [ins for ins in instructions if target.address < ins.address < call.address]
        require(len(between) == 2 and between[0].mnemonic == 'lea'
                and between[1].bytes.hex() in ('50', '52'),
                f'camera point argument order changed at {site["callSite"]}')
        prior = [ins for ins in instructions if target.address - 16 <= ins.address < target.address]
        require([ins.bytes.hex() for ins in prior[-3:]] == ['6aff', '6a00', '68000080bf'],
                f'camera point scalar operands changed at {site["callSite"]}')
    calls = [call['site'].lower() for call in fn['indirectCalls'] if call['slot'] == '0x678']
    require(calls == [site['callSite'].lower() for site in row['sites']],
            'camera point call inventory changed')
    require(source.count('quest:CameraUseCameraPoint(') == 6
            and all(source.count(old) == 1 for old in OLD)
            and all(todo.count(diagnostic) == DIAGNOSTICS.count(diagnostic)
                    for diagnostic in set(DIAGNOSTICS)),
            'lifted camera point calls changed')
    for old in OLD:
        source = source.replace(old, NEW, 1)
    for diagnostic in DIAGNOSTICS:
        todo.remove(diagnostic)
    return source, todo
