"""Verify the native definition-symbol and compile path scans."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CDefinitionManager_CreateSymbolsFromPathList_009aef60.cpp'
COMPILE = ROOT / 'rebuild/src/compiled/00/9a/CDefinitionManager_DoCompilePathList_009af480.cpp'
COMMENTS = ROOT / 'rebuild/src/compiled/00/a6/Global_GFRemoveComments_00a60410.cpp'
TEST = ROOT / 'rebuild/tests/00/9a/CDefinitionManager_CreateSymbolsFromPathList_009aef60_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/definition-create-symbols.json'
SYMBOL = '?CreateSymbolsFromPathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@_STL@@@_STL@@_N@Z'
COMPILE_SYMBOL = '?DoCompilePathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@_STL@@@_STL@@ABVCCharString@@@Z'
COMMENT_SYMBOL = '?GFRemoveComments@@YIXPAD00@Z'
ADDRESS = 0x009AEF60
SIZE = 625
COMPILE_ADDRESS = 0x009AF480
COMPILE_SIZE = 605


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-create-symbols-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, COMPILE, COMMENTS, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider, compiler, comments, fixture = (Coff(obj) for obj in objects)
    section, offset = provider.symbols[SYMBOL]
    body, relocations = provider.sections[section]
    body = body[offset:]
    assert len(body) == 616 and len(relocations) == 43
    compile_section, compile_offset = compiler.symbols[COMPILE_SYMBOL]
    compile_body, compile_relocations = compiler.sections[compile_section]
    compile_body = compile_body[compile_offset:]
    assert len(compile_body) == 589 and len(compile_relocations) == 41
    assert COMMENT_SYMBOL in comments.symbols and SYMBOL not in fixture.symbols
    assert sum(symbol == COMMENT_SYMBOL for symbol, kind in relocations.values()) == 3
    assert sum(symbol == COMMENT_SYMBOL for symbol, kind in compile_relocations.values()) == 3
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, SIZE)
        compile_retail = pe.get_data(COMPILE_ADDRESS - pe.OPTIONAL_HEADER.ImageBase, COMPILE_SIZE)
    similarity = compare(body, retail, relocations.keys())
    compile_similarity = compare(compile_body, compile_retail, compile_relocations.keys())
    assert similarity.complete_decode and similarity.opcode_similarity >= 0.8, similarity
    assert compile_similarity.complete_decode and compile_similarity.opcode_similarity >= 0.77, compile_similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in (0, 1, 2):
        remaining = [str(obj) for index, obj in enumerate(objects) if index != omitted]
        result = subprocess.run(link + remaining, env=env, capture_output=True, text=True)
        assert result.returncode != 0, ('omitted provider linked', omitted)
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CREATE_SYMBOLS_PATH_LIST_OK' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='BEHAVIORAL',
                                      symbols=dict(address='009aef60', retail_bytes=SIZE,
                                                   compiled_bytes=len(body), relocations=len(relocations),
                                                   similarity=similarity.to_dict()),
                                      compile=dict(address='009af480', retail_bytes=COMPILE_SIZE,
                                                   compiled_bytes=len(compile_body), relocations=len(compile_relocations),
                                                   similarity=compile_similarity.to_dict()),
                                      omitted_provider_checks=3,
                                      linked_scan_patterns=6, parsed_files=6, empty_path_cases=2,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      compile_sha256=hashlib.sha256(COMPILE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_PATH_LISTS_PASS symbols=616/625 compile=589/605 files=6 omitted=3')


if __name__ == '__main__':
    main()
