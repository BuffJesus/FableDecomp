"""Link and exercise the real console help and parameter-type providers."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
from gate_game_component_abi import Coff

ROOT = Path(__file__).resolve().parents[2]
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandBase_GetHelp_009ea7d0.cpp',
    ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandParameters_GetParameterTypeAsString_009ea740.cpp',
]
TEST = ROOT / 'rebuild/tests/integration/ConsoleCommandHelpParameterChain_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-help-parameter-chain.json'
SYMBOLS = [
    '?GetHelp@CConsoleCommandBase@@MAE?AVCCharString@@XZ',
    '?GetParameterTypeAsString@CConsoleCommandParameters@@QBE?AVCCharString@@J@Z',
]


def main():
    work = Path(tempfile.mkdtemp(prefix='console-help-parameter-chain-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES + [TEST]:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    providers = [Coff(obj) for obj in objects[:2]]
    fixture = Coff(objects[2])
    for i, symbol in enumerate(SYMBOLS):
        assert symbol in providers[i].symbols and symbol not in fixture.symbols
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in range(2):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'provider omission linked: ' + str(omitted)
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONSOLE_COMMAND_HELP_PARAMETER_CHAIN_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(providers=[str(p.relative_to(ROOT)) for p in SOURCES],
                                      compiled_cases=3, missing_provider_checks=2,
                                      source_sha256=[hashlib.sha256(p.read_bytes()).hexdigest() for p in SOURCES],
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_HELP_PARAMETER_CHAIN_PASS compiled=3 omitted=2')


if __name__ == '__main__':
    main()
