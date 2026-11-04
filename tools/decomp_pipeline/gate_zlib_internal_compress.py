import pathlib
import subprocess
import struct
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[2] / 'tools/decomp_pipeline'))
import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

root = pathlib.Path(__file__).resolve().parents[2]
source = root / 'rebuild/src/compiled/00/a3/CCompressorZlib_InternalCompress_00a3aa40.cpp'
work = pathlib.Path(tempfile.mkdtemp(prefix='internal-compress-'))
obj = work / 'candidate.obj'
env = check_one.env()
env['INCLUDE'] = str(root / 'rebuild/include') + ';' + env['INCLUDE']
result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
assert result.returncode == 0, result.stdout + result.stderr
coff = Coff(obj)
name = '?InternalCompress@CCompressorZlib@@QAEKPBXKPAXK@Z'
section, offset = coff.symbols[name]
body, relocs = coff.sections[section]
address = 0x00A3AA40
with pefile.PE(str(EXE)) as pe:
    retail = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, len(body))
print('length', len(body), 'offset', offset, 'relocations', relocs)
candidate = bytearray(body)
for pos, (symbol, kind) in relocs.items():
    if symbol.startswith('??_C@'):
        target = 0x0129CFE0
    else:
        target = {
            '?Alloc@CCompressorZlib@@CAPAXPAXII@Z': 0x00A3A9C0,
            '?Free@CCompressorZlib@@CAXPAX0@Z': 0x00A3AA00,
            '_z_deflateInit_': 0x00C021C0,
            '_z_deflate': 0x00C006A0,
            '_z_deflateEnd': 0x00C00AE0,
        }[symbol]
    struct.pack_into('<I', candidate, pos, (target - address - pos - 4 if kind == 20 else target) & 0xffffffff)
print('match', candidate == retail)
assert offset == 0 and len(body) == 192 and candidate == retail
for index, (actual, expected) in enumerate(zip(candidate, retail)):
    if actual != expected:
        print('first difference', index, hex(actual), hex(expected))
        break
test = root / 'rebuild/tests/00/a3/CCompressorZlib_InternalCompress_00a3aa40_test.cpp'
test_obj = work / 'test.obj'
result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test_obj), str(test)], env, work)
assert result.returncode == 0, result.stdout + result.stderr
exe = work / 'fixture.exe'
link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(exe)]
result = subprocess.run(link + [str(test_obj)], env=env, capture_output=True, text=True)
assert result.returncode != 0, 'omitted provider linked'
result = subprocess.run(link + [str(obj), str(test_obj)], env=env, capture_output=True, text=True)
assert result.returncode == 0, result.stdout + result.stderr
result = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
assert result.returncode == 0 and 'ZLIB_INTERNAL_COMPRESS_PASS cases=16' in result.stdout, (result.returncode, result.stdout, result.stderr)
print(result.stdout.strip(), 'omission=PASS')
