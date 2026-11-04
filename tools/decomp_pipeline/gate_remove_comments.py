"""Check the recovered native GFRemoveComments and its retail instruction shape."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/a6/Global_GFRemoveComments_00a60410.cpp'
TEST = ROOT / 'rebuild/tests/00/a6/Global_GFRemoveComments_00a60410_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/remove-comments.json'
SYMBOL = '?GFRemoveComments@@YIXPAD00@Z'
ADDRESS = 0x00A60410
SIZE = 245

CASES = (
    (b'alpha', b'/*', b'*/', b'alpha'),
    (b'a/* x */b', b'/*', b'*/', b'ab'),
    (b'a/* x\ny */b', b'/*', b'*/', b'a\nb'),
    (b'a/* x */b/* y */c', b'/*', b'*/', b'abc'),
    (b'a/* unfinished', b'/*', b'*/', b'a/* unfinished'),
    (b'a// x\nb', b'//', b'\n', b'a\nb'),
    (b'a/* x */\nb', b'/*', b'*/', b'a\n\nb'),
    (b'a#x\ny\n', b'#', b'', b'a\n\n'),
)


def retail_case(image, source, opening, closing):
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
    from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EIP, UC_X86_REG_ESP

    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x00A60000, 0x1000)
    machine.mem_write(0x00A60000, image)
    machine.mem_map(0x00BFE000, 0x2000)
    machine.mem_write(0x00BFEB3A, b'\xc3')
    machine.mem_write(0x00BFEAE6, b'\xc3')
    machine.mem_map(0x00200000, 0x1000)
    machine.mem_map(0x00300000, 0x1000)
    machine.mem_map(0x00DEAD0000, 0x1000)
    machine.mem_write(0x00200100, source + b'\0')
    machine.mem_write(0x00200200, opening + b'\0')
    machine.mem_write(0x00200300, closing + b'\0')
    stack = 0x00300800
    machine.mem_write(stack, (0xDEAD0000).to_bytes(4, 'little') + (0x00200300).to_bytes(4, 'little'))
    machine.reg_write(UC_X86_REG_ESP, stack)
    machine.reg_write(UC_X86_REG_ECX, 0x00200100)
    machine.reg_write(UC_X86_REG_EDX, 0x00200200)

    def word(address):
        return int.from_bytes(machine.mem_read(address, 4), 'little')

    def cstring(address):
        result = bytearray()
        while True:
            value = machine.mem_read(address, 1)[0]
            if value == 0:
                return bytes(result)
            result.append(value)
            address += 1

    def crt(uc, address, size, data):
        if address not in (0x00BFEB3A, 0x00BFEAE6):
            return
        esp = uc.reg_read(UC_X86_REG_ESP)
        left, right = word(esp + 4), word(esp + 8)
        if address == 0x00BFEB3A:
            haystack = cstring(left)
            needle = cstring(right)
            index = haystack.find(needle)
            value = left + index if index >= 0 else 0
        else:
            count = word(esp + 12)
            uc.mem_write(left, bytes(uc.mem_read(right, count)))
            value = left
        uc.reg_write(UC_X86_REG_EAX, value)
        uc.reg_write(UC_X86_REG_ESP, esp + 4)
        uc.reg_write(UC_X86_REG_EIP, word(esp))

    machine.hook_add(UC_HOOK_CODE, crt)
    machine.emu_start(ADDRESS, 0xDEAD0000, count=10000)
    return cstring(0x00200100)


def main():
    work = Path(tempfile.mkdtemp(prefix='remove-comments-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = provider.symbols[SYMBOL]
    body, relocations = provider.sections[section]
    body = body[offset:]
    assert len(body) == 262 and len(relocations) == 4
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, SIZE)
        image = pe.get_data(0x00A60000 - pe.OPTIONAL_HEADER.ImageBase, 0x1000)
    for source, opening, closing, expected in CASES:
        actual = retail_case(image, source, opening, closing)
        assert actual == expected, (source, actual, expected)
    similarity = compare(body, retail, relocations.keys())
    assert similarity.complete_decode and similarity.opcode_similarity >= 0.8, similarity
    assert all(symbol in ('_strstr', '_memmove') for symbol, kind in relocations.values())
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GF_REMOVE_COMMENTS_OK' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00a60410', grade='BEHAVIORAL',
                                      retail_bytes=SIZE, compiled_bytes=len(body),
                                      relocations=len(relocations), compiled_cases=8, retail_cases=len(CASES),
                                      missing_provider_checks=1,
                                      similarity=similarity.to_dict(),
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('GF_REMOVE_COMMENTS_PASS retail=245 compiled=262 cases=8 omitted=1')


if __name__ == '__main__':
    main()
