"""Check the complete C++ GFMain against retail instructions and control flow.

The machine-code comparison checks all bytes and relocation targets. Execution
uses explicit simulated callees/imports; it does not launch the game or claim
that the full startup dependency graph is integrated.
"""
import argparse
import json
import tempfile
from pathlib import Path
import check_one
import native_gfmain_harness as H
from gate_game_component_abi import Coff

ROOT = Path(__file__).resolve().parents[2]


def callback_proof(work, include):
    """Verify the registered callback bodies independently of simulation."""
    entries = [
        ('Global_FileErrorCallbacks_00401b60.cpp', '?GFHardDiskError@@YIXPBD@Z', 0x00401b60, 1),
        ('Global_FileErrorCallbacks_00401b60.cpp', '?DoDirtyDisc@@YIXXZ', 0x0041a710, 1),
        ('CIME_MessageCallbacks_00404c70.cpp', '?ProcessWinMessage@CIME@NUISystem@@SI_NPAUHWND__@@IIJAAJ@Z', 0x00404c70, 107),
        ('CIME_MessageCallbacks_00404c70.cpp', '?FilterMessage@CIME@NUISystem@@SI_NPAUtagMSG@@@Z', 0x00404ce0, 5),
        ('Global_GFIsSystemErrorFailure_00401bc0.cpp', '?GFIsSystemErrorFailure@@YI_NW4ESystemCode@@@Z', 0x00401bc0, 50),
    ]
    targets = {
        '?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A': 0x013b7cd8,
        '?ImeUi_ProcessMessage@@YIJPAUHWND__@@IIJPA_N@Z': 0x00cb48a0,
        '?ImeUi_ToggleLanguageBar@@YIXH@Z': 0x00cb3570,
        '?ImeUi_IgnoreHotKey@@YI_NPBUtagMSG@@@Z': 0x00cb2b90,
    }
    objects, report = {}, []
    for source, symbol, address, size in entries:
        if source not in objects:
            obj = work / (source + '.obj')
            result = check_one.cl(['/nologo', '/c', '/O2', '/Oy', '/GS', '/MT',
                                  '/I'+str(include.resolve()), '/Fo'+str(obj),
                                  str(ROOT / 'rebuild/src/compiled/00/40' / source)],
                                 check_one.env(), work)
            if result.returncode:
                raise RuntimeError(result.stdout + result.stderr)
            objects[source] = Coff(obj)
        coff = objects[source]
        section, start = coff.symbols[symbol]
        built, relocs = coff.sections[section]
        native = H.PE.get_data(address-H.BASE, size)
        assert start == 0 and len(built) == size, (symbol, 'body size')
        assert check_one.mask(built, relocs) == check_one.mask(native, relocs), symbol
        for offset, (target, kind) in relocs.items():
            assert kind in (6, 20)
            actual = int.from_bytes(native[offset:offset+4], 'little', signed=kind == 20)
            if kind == 20:
                actual += address + offset + 4
            addend = int.from_bytes(built[offset:offset+4], 'little', signed=True)
            if target in coff.symbols:
                target_section, target_offset = coff.symbols[target]
                assert target_section == section, (symbol, target, 'external section')
                expected = address + target_offset + addend
            else:
                expected = targets[target] + addend
            assert actual == expected, (symbol, target)
        report.append({'address': f'{address:08x}', 'symbol': symbol, 'bytes': size,
                       'relocations': len(relocs), 'grade': 'RELOCATION_MATCH' if relocs else 'MATCH'})
    return report


def relocation_proof():
    oracle = H.CandidateOracle()
    native = H.NATIVE_BODY
    assert len(H.BUILT) == len(native) == 3952
    left, right = bytearray(native), bytearray(H.BUILT)
    proof = []
    for offset, (symbol, kind) in sorted(H.RELOCS.items()):
        assert kind in (6, 20)
        value = int.from_bytes(native[offset:offset+4], 'little', signed=kind == 20)
        actual = 0x00402510 + offset + 4 + value if kind == 20 else value
        addend = int.from_bytes(H.BUILT[offset:offset+4], 'little')
        if symbol.startswith('??_C@'):
            section, start = H.COFF.symbols[symbol]
            data, relocs = H.COFF.sections[section]
            assert not relocs
            literal = data[start+addend:]
            assert H.PE.get_data(actual - H.BASE, len(literal)) == literal, (offset, symbol)
            role = 'literal-content'
        else:
            expected = oracle.resolve(symbol) + addend
            if symbol == '?g_FableEmptyNarrowString_0129AAF4@@3PADA':
                assert H.PE.get_data(actual-H.BASE, 1) == H.PE.get_data(expected-H.BASE, 1) == b'\0'
                role = 'folded-empty-literal'
            else:
                assert actual == expected, (offset, symbol, hex(actual), hex(expected))
                role = 'direct-call' if kind == 20 else 'global-or-import'
        proof.append({'offset': offset, 'symbol': symbol, 'target': f'{actual:08x}', 'role': role})
        left[offset:offset+4] = right[offset:offset+4] = bytes(4)
    differences = [(i, a, b) for i, (a, b) in enumerate(zip(left, right)) if a != b]
    # Same stack address, different volatile temporary register. The target Get()
    # takes no arguments; its return replaces EAX before SetProfileName consumes it.
    allowed = [(0xe96, 0x44, 0x4c), (0xe99, 0x50, 0x51)]
    assert differences in ([], allowed), differences
    assert len(proof) == 391
    return proof, differences


def normalized(result):
    return [(event['target'], event.get('semantic', [])) for event in result['events']]


def init_state(oracle):
    if not hasattr(oracle, 'init_snapshot'):
        return None
    data = bytearray.fromhex(oracle.init_snapshot)
    strings = {}
    for offset, wide in [(12, True), (44, False), (48, False), (148, True), (184, True), (188, True)]:
        storage = int.from_bytes(data[offset:offset+4], 'little')
        strings[offset] = oracle.literal(oracle.u32(storage), wide) if storage else ''
        data[offset:offset+4] = bytes(4)
    return {'bytes': data.hex(), 'strings': strings}


def run_cases():
    cases = json.loads((H.WORK / 'scenarios.json').read_text())
    results = []
    for case in cases:
        scenario = dict(case['scenario'])
        if 'settings' in scenario:
            scenario['settings'] = {int(key): value for key, value in scenario['settings'].items()}
        native, candidate = H.Oracle(**scenario), H.CandidateOracle(**scenario)
        expected, actual = native.run(), candidate.run()
        name = case['name']
        assert expected['exit'] == case['exit'], (name, 'native exit changed')
        assert expected['exit'] is not None or expected['return'] == case['return'], (name, 'native return changed')
        assert expected['exit'] == actual['exit'], (name, 'exit mismatch')
        assert expected['exit'] is not None or expected['return'] == actual['return'], (name, 'return mismatch')
        assert normalized(expected) == normalized(actual), (name, 'call/argument/cleanup sequence mismatch')
        assert init_state(native) == init_state(candidate), (name, 'system init mismatch')
        assert native.global_writes == candidate.global_writes, (name, 'global write footprint mismatch')
        for address in native.global_writes:
            assert native.uc.mem_read(address, 1) == candidate.uc.mem_read(address, 1), (name, 'global value', hex(address))
        results.append({'name': name, 'pass': True, 'calls': len(expected['events'])})
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--include', type=Path, default=ROOT / 'rebuild/include')
    parser.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/40/GFMain_00402510.cpp')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='native-gfmain-') as temporary:
        work = Path(temporary)
        obj = work / 'GFMain.obj'
        result = check_one.cl(['/nologo', '/c', '/O2', '/Oy', '/GS', '/MT', '/W3',
                               '/I' + str(args.include.resolve()), '/Fo' + str(obj),
                               str(args.source.resolve())], check_one.env(), work)
        if result.returncode:
            raise RuntimeError(result.stdout + result.stderr)
        H.configure_candidate(obj)
        relocations, differences = relocation_proof()
        callbacks = callback_proof(work, args.include)
        cases = run_cases()
        grade = 'NEAR_MATCH' if differences else 'RELOCATION_MATCH'
        if args.report:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps({'grade': grade, 'bytes': len(H.BUILT),
                'scope': 'GFMain machine code; callees and OS imports simulated',
                'relocations': relocations, 'differences': differences, 'scenarios': cases,
                'callbacks': callbacks}, indent=2) + '\n')
        print(f'NATIVE_GFMAIN_PASS grade={grade} bytes={len(H.BUILT)} '
              f'relocations={len(relocations)} scenarios={len(cases)} callbacks={len(callbacks)}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
