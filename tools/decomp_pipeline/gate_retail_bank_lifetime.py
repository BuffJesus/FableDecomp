"""Check the retail-bank destructor and its contained-bank map cleanup."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
CASES = [
    ('009ac7c0', 'CRetailBank_Destructor_009ac7c0',
     '??1CRetailBank@NBankFileManager@@UAE@XZ', 161, 149,
     'RETAIL_BANK_DESTRUCTOR_PASS', 0.93),
    ('009aa780', 'CContainedBankMap_Erase_009aa780',
     '?Erase@CContainedBankMap@NBankFileManager@@QAEXPAUCNode@12@@Z',
     59, 59, 'CONTAINED_BANK_ERASE_PASS', 1.0),
]
REPORT = ROOT / 'rebuild/compile-gate/retail-bank-lifetime.json'


def main():
    work = Path(tempfile.mkdtemp(prefix='retail-bank-lifetime-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    checks = []
    with pefile.PE(str(EXE)) as pe:
        assert struct.unpack('<I', pe.get_data(0x0129B010 - pe.OPTIONAL_HEADER.ImageBase, 4))[0] == 0x009AC7A0
        for address, name, symbol, retail_size, built_size, marker, min_score in CASES:
            source = ROOT / 'rebuild/src/compiled/00/9a' / (name + '.cpp')
            fixture = ROOT / 'rebuild/tests/00/9a' / (name + '_test.cpp')
            objects = []
            for path in (source, fixture):
                obj = work / (path.stem + '.obj')
                result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(path)], env, work)
                assert result.returncode == 0, result.stdout + result.stderr
                objects.append(obj)
            provider, test = Coff(objects[0]), Coff(objects[1])
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(body) == built_size
            assert symbol not in test.symbols
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, retail_size)
            similarity = compare(body, retail, relocs.keys())
            assert similarity.near_match and similarity.opcode_similarity >= min_score, similarity
            if address == '009ac7c0':
                scalar_symbol = '??_GCRetailBank@NBankFileManager@@UAEPAXI@Z'
                scalar_section, scalar_offset = provider.symbols[scalar_symbol]
                scalar, scalar_relocs = provider.sections[scalar_section]
                assert scalar_offset == 0 and len(scalar) == 30
                retail_scalar = pe.get_data(0x009AC7A0 - pe.OPTIONAL_HEADER.ImageBase, 30)
                assert set(scalar_relocs) == {4, 17}
                masked = bytearray(scalar)
                retail_masked = bytearray(retail_scalar)
                for site in scalar_relocs:
                    masked[site:site + 4] = b'\0' * 4
                    retail_masked[site:site + 4] = b'\0' * 4
                assert masked == retail_masked
            link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / (name + '.exe'))]
            result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
            assert result.returncode != 0, 'omitted provider linked'
            result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
            assert result.returncode == 0, result.stdout + result.stderr
            result = subprocess.run([str(work / (name + '.exe'))], env=env, capture_output=True, text=True, timeout=30)
            assert result.returncode == 0 and marker in result.stdout, (result.returncode, result.stdout, result.stderr)
            checks.append(dict(address=address, source=name + '.cpp', candidate_bytes=len(body),
                               retail_bytes=retail_size, similarity=similarity.to_dict(),
                               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                               fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest()))
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', providers=checks,
                                      retail_vtable='0129b010',
                                      missing_provider_checks=2), indent=2) + '\n')
    print('RETAIL_BANK_LIFETIME_PASS scalar=30/30 dtor=149/161 erase=59/59 omitted=2')


if __name__ == '__main__':
    main()
