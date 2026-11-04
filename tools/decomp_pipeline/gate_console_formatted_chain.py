"""Link and exercise the real console formatter, string addition and suffix."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
from gate_game_component_abi import Coff

ROOT = Path(__file__).resolve().parents[2]
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandBase_GetFormattedNameCConsoleCommandBaseMBEAVCCharStr_009ea3a0.cpp',
    ROOT / 'rebuild/src/compiled/00/99/CCharString_AddCharStringAndCharString_0099f570.cpp',
    ROOT / 'rebuild/src/compiled/01/3c/ConsoleCommandNameSuffix_013caa48.cpp',
]
TEST = ROOT / 'rebuild/tests/integration/ConsoleCommandFormattedChain_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-formatted-chain.json'
SYMBOLS = [
    '?GetFormattedName@CConsoleCommandBase@@MBE?AVCCharString@@XZ',
    '??H@YI?AVCCharString@@ABV0@0@Z',
    '?g_013caa48@@3VCCharString@@B',
]


def main():
    work = Path(tempfile.mkdtemp(prefix='console-formatted-chain-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES + [TEST]:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    providers = [Coff(obj) for obj in objects[:3]]
    fixture = Coff(objects[3])
    for i, symbol in enumerate(SYMBOLS):
        assert symbol in providers[i].symbols and symbol not in fixture.symbols
        assert all(symbol not in provider.symbols for j, provider in enumerate(providers) if j != i)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in range(3):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'provider omission linked: ' + str(omitted)
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONSOLE_COMMAND_FORMATTED_CHAIN_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(providers=[str(p.relative_to(ROOT)) for p in SOURCES],
                                      compiled_cases=2, missing_provider_checks=3,
                                      source_sha256=[hashlib.sha256(p.read_bytes()).hexdigest() for p in SOURCES],
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_FORMATTED_CHAIN_PASS compiled=2 omitted=3')


if __name__ == '__main__':
    main()
