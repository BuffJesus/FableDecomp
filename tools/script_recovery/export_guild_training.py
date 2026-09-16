"""Expand the Guild native export from instruction and vtable anchors, read-only."""
from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from tools.script_recovery.export_native_threads import GHIDRA_HOME, PROJECT_DIR, SCRIPT_DIR, Image
from tools.script_recovery.guild_training_inventory import EVIDENCE, ROOT, recover
from tools.script_recovery.script_units import unit as script_unit


def anchors(translation, inventory, image, lo=0xD3B390, hi=0xD68F00):
    addresses = {int(a, 16) for q in inventory['quests'] for e in q['entities']
                 for a in e['missingExports']}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    for function in translation['functions']:
        for call in function['calls']:
            target = int(call['target'], 16)
            if lo <= target < hi:
                addresses.add(target)
        for span in function['bodyRanges']:
            start, end = int(span['start'], 16), int(span['endExclusive'], 16)
            for ins in decoder.disasm(image.bytes_at(start, end-start), start):
                if (ins.mnemonic == 'mov' and len(ins.operands) == 2
                        and ins.operands[0].type == X86_OP_MEM and ins.operands[0].mem.disp == 0x34
                        and ins.operands[1].type == X86_OP_IMM):
                    target = ins.operands[1].imm
                    if lo <= target < hi:
                        addresses.add(target)
    return addresses


def run(max_passes=4, unit_name='guild_training'):
    spec = script_unit(unit_name)
    evidence, lo, hi = spec['evidence'], spec['lo'], spec['hi']
    evidence.mkdir(parents=True, exist_ok=True)
    output = evidence / 'translation_unit.json'
    job = evidence / 'define_addresses.txt'
    logs = ROOT / f'work/{unit_name}_export'
    logs.mkdir(parents=True, exist_ok=True)
    defined = {int(line, 16) for line in job.read_text().splitlines()
               if line and not line.startswith('#')} if job.exists() else set()
    for attempt in range(max_passes):
        if output.exists():
            translation = json.loads(output.read_text(encoding='utf-8-sig'))
            inventory = recover(unit_name=unit_name)
            (evidence / 'inventory.json').write_text(json.dumps(inventory, indent=2)+'\n', encoding='utf-8')
            wanted = anchors(translation, inventory, Image(), lo, hi)
            available = {int(f['address'], 16) for f in translation['functions'] if f.get('decompile')}
            missing = wanted - available
            if not missing:
                print(f'Guild export complete: {len(available)} function bodies; all entity slots exported.')
                return inventory
            defined.update(wanted)
        job.write_text('# Native factory, vtable, callee and thread anchors.\n'+
                       ''.join(f'0x{a:08X}\n' for a in sorted(defined)), encoding='utf-8')
        command = [str(GHIDRA_HOME / 'support/analyzeHeadless.bat'), str(PROJECT_DIR), 'FableTLC',
                   '-process', 'Fable.exe', '-readOnly', '-noanalysis', '-scriptPath', str(SCRIPT_DIR),
                   '-postScript', 'ExportScriptTranslationUnit.java', f'0x{lo:08X}', f'0x{hi:08X}',
                   str(output), str(job)]
        log = logs / f'export_pass_{attempt+1}.log'
        print(f'Export pass {attempt+1}: {len(defined)} anchors; {log}', flush=True)
        with log.open('w', encoding='utf-8') as stream:
            subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT, check=True)
    raise RuntimeError('Guild export needs another expansion pass; inspect logs and missing anchors')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--max-passes', type=int, default=4)
    parser.add_argument('--unit', default='guild_training')
    args = parser.parse_args()
    run(args.max_passes, args.unit)
