"""Build every evidence file for a unit already entered in script_units.py (the Gameflow / Trader Escort recipe).

Steps, all read-only against the Ghidra project and the retail/debug images:
  1. export_guild_training --unit U       untyped translation unit + inventory + define_addresses (anchor passes)
  2. pdb-locals.exe <msdia> Ego_r.pdb PAT  debug-build symbols for the unit's classes (CRLF -> LF)
  3. quest_unit_evidence --unit U         units/<Script>.json
  4. ghidra_typing_spec --unit U          typing_spec.json (FSE typedefs + PDB stack parameters)
  5. infer_helper_prototypes --unit U
  6. ExportTypedTranslationUnit.java      translation_unit_typed.json over the registered range
Conversion is separate: convert_quest_unit / build_readable_unit / smoke_run_unit.
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402

PDB_LOCALS = ROOT / 'work/pdb_locals_20260913/pdb-locals.exe'
MSDIA = Path(r'C:/Program Files/dotnet/sdk/10.0.301/TestHostNetFramework/x86/msdia140.dll')
HEADLESS = Path(r'D:/Subuwu/tools/ghidra-public/support/analyzeHeadless.bat')


def run(cmd, log, **kw):
    log.parent.mkdir(parents=True, exist_ok=True)
    with log.open('w', encoding='utf-8') as fh:
        subprocess.run([str(c) for c in cmd], cwd=ROOT, stdout=fh, stderr=subprocess.STDOUT, check=True, **kw)


def register(name, steps):
    spec = script_unit(name)
    ev = spec['evidence']
    logs = ROOT / f'work/{name}_export'
    py = [sys.executable, '-X', 'utf8', '-m']
    if 'export' in steps:
        run(py + ['tools.script_recovery.export_guild_training', '--unit', name], logs / 'register_export.log')
    if 'pdb' in steps:
        (ev / 'pdb').mkdir(parents=True, exist_ok=True)
        out = subprocess.run([str(PDB_LOCALS), str(MSDIA), str(ROOT / 'debug_build/Ego_r.pdb'), spec['pdb_pattern']],
                             cwd=ROOT, capture_output=True, check=True).stdout
        (ev / 'pdb/Ego_r-pdb-locals.tsv').write_bytes(out.replace(b'\r\n', b'\n'))
    if 'evidence' in steps:
        run(py + ['tools.script_recovery.quest_unit_evidence', '--unit', name], logs / 'register_evidence.log')
    if 'typing' in steps:
        run(py + ['tools.script_recovery.ghidra_typing_spec', '--unit', name, '--out', ev / 'typing_spec.json'],
            logs / 'register_typing.log')
    if 'prototypes' in steps:
        run(py + ['tools.script_recovery.infer_helper_prototypes', '--unit', name], logs / 'register_prototypes.log')
    if 'typed' in steps:
        defs = ev / 'define_addresses.txt'
        run([HEADLESS, ROOT / 'ghidra_proj', 'FableTLC', '-process', 'Fable.exe', '-readOnly', '-noanalysis',
             '-scriptPath', ROOT / 'tools/ghidra_scripts', '-postScript', 'ExportTypedTranslationUnit.java',
             f"0x{spec['lo']:08X}", f"0x{spec['hi']:08X}", ev / 'translation_unit_typed.json',
             defs if defs.exists() else '-', ev / 'typing_spec.json'], logs / 'typed_pass_1.log')
    return ev


STEPS = ('export', 'pdb', 'evidence', 'typing', 'prototypes', 'typed')

if __name__ == '__main__':
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('units', nargs='+')
    a.add_argument('--steps', default=','.join(STEPS))
    args = a.parse_args()
    for u in args.units:
        print(u, register(u, set(args.steps.split(','))), flush=True)
