"""Native const-bound regression gate. Every run requires a fresh artifact directory.

Defaults compile current canonical providers and headers. Frozen pre-migration
source fixtures rebuild genuine obsolete-signature negative controls freshly.
No retries, optimizer fitting, symbol aliases, fallback links or assertion edits.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'rebuild').is_dir())
sys.path.insert(0, str(ROOT / 'tools/decomp_pipeline'))
import check_one as V
import link_closure as closure


def validate():
    cp = subprocess.run([sys.executable, str(Path(__file__))], capture_output=True,
                        text=True, timeout=120)
    assert cp.returncode == 0, cp.stdout + cp.stderr
    prefix = 'NATIVE_CONST_BOUND_REGRESSION_PASS '
    outputs = [line[len(prefix):] for line in cp.stdout.splitlines() if line.startswith(prefix)]
    assert len(outputs) == 1, cp.stdout
    directory = Path(outputs[0])
    report = json.loads((directory / 'result.json').read_text())
    assert report['status'] == 'NATIVE_CONST_BOUND_REGRESSION_PASS'
    assert report['root_bytes_and_typed_fixups_preserved']
    assert len(report['fixtures']) == 6
    pins = json.loads((directory / 'after.json').read_text())
    assert not pins['mismatches']
    assert json.loads((directory / 'before.json').read_text()) == pins['input_hashes']
    for path, digest in {**pins['input_hashes'], **pins['object_hashes']}.items():
        assert sha(Path(path)) == digest, path
    return directory


def index_symbols(source, obj):
    baseline = json.loads((HERE / 'native_const_tree_bounds_baseline.json').read_text())
    spec = next(p for p in baseline['providers'].values() if p['source'] == source)
    inv = inventory(obj)
    assert contribution(inv, spec['symbol']) == spec['runtime']
    assert not any(s['name'] == spec['old_symbol'] for s in inv['symbols'])
    allowed = {spec['symbol'], *spec['published_companions']}
    emitted = {s['name'] for s in inv['symbols'] if s['section'] > 0}
    assert allowed <= emitted, allowed - emitted
    return allowed


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inventory(path):
    raw = path.read_bytes()
    machine, count, _, pointer, symbols_count, optional, _ = struct.unpack_from('<HHIIIHH', raw)
    assert machine == 0x14c and optional == 0
    strings = pointer + symbols_count * 18
    symbols, names, i = [], {}, 0
    while i < symbols_count:
        name, value, section, typ, storage, auxiliaries = struct.unpack_from('<8sIhHBB', raw, pointer + i * 18)
        if name[:4] == bytes(4):
            at = strings + struct.unpack_from('<I', name, 4)[0]
            name = raw[at:raw.index(0, at)]
        name = name.rstrip(b'\0').decode('ascii')
        symbols.append(dict(name=name, value=value, section=section, type=typ, storage=storage))
        names[i] = name
        i += 1 + auxiliaries
    sections = []
    for i in range(count):
        _, _, _, length, data, reloc, _, number, _, flags = struct.unpack_from('<8sIIIIIIHHI', raw, 20 + i * 40)
        fixes = []
        for j in range(number):
            offset, target, kind = struct.unpack_from('<IIH', raw, reloc + j * 10)
            fixes.append(dict(offset=offset, kind=kind, target=names[target]))
        sections.append(dict(flags=flags, payload=raw[data:data + length].hex() if data else '', fixups=fixes))
    return dict(raw_sha256=sha(path), symbols=symbols, sections=sections)


def contribution(inv, symbol):
    found = [s for s in inv['symbols'] if s['name'] == symbol and s['section'] > 0]
    assert len(found) == 1, (symbol, found)
    sym = found[0]
    return dict(value=sym['value'], storage=sym['storage'], **inv['sections'][sym['section'] - 1])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path)
    parser.add_argument('--include', type=Path, default=ROOT / 'rebuild/include')
    parser.add_argument('--string-source', type=Path, default=ROOT / 'rebuild/src/compiled/00/47/STLStringLongTree_Mlowerbound_0047547d.cpp')
    parser.add_argument('--checksum-source', type=Path, default=ROOT / 'rebuild/src/compiled/00/a6/STLChecksumCacheTree_LowerBound_00a609c0.cpp')
    args = parser.parse_args()
    out = args.out.resolve() if args.out else Path(tempfile.mkdtemp(prefix='fable_const_bound_')) / 'artifacts'
    include = args.include.resolve()
    assert not out.exists(), 'Preserve first artifacts: choose a new output directory'
    assert not os.environ.get('CL') and not os.environ.get('_CL_')
    baseline = json.loads((HERE / 'native_const_tree_bounds_baseline.json').read_text())
    sources = {'string': args.string_source.resolve(), 'checksum': args.checksum_source.resolve()}
    catalog = (ROOT / 'rebuild/build_candidates.ps1').read_text(encoding='utf-8-sig')
    assert "else { @('/O2','/Oy','/W3') }" in catalog
    assert '/nologo /c /Od /W3 /Fo$testObject $testSource' in catalog
    for provider in baseline['providers'].values():
        needle = "Source = '" + provider['source'] + "'"
        assert catalog.count(needle) == 1
        at = catalog.index(needle)
        block = catalog[catalog.rfind('[pscustomobject]@{', 0, at):catalog.index('\n    }', at)]
        override = re.search(r"CompilerFlags = '([^']+)'", block)
        actual = override.group(1).split() if override else ['/O2', '/Oy', '/W3']
        assert actual == provider['catalog_flags'], 'Catalog provider recipe drift'
        assert closure.flags_for(provider['source']) == provider['runtime_flags'], 'Runtime provider recipe drift'
    # Reject overlays that silently fall back to canonical headers.
    for header in include.glob('*.h'):
        for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"', header.read_text(), re.M):
            assert (header.parent / name).is_file() or (include / name).is_file(), (header, name)
    unlock_names = ['00/9d/CDefStringTable_Unlock_009d4f10.cpp', '00/51/STLMapCCharStringlong_operator_00519230.cpp', '00/51/STLMapCCharStringlong_insert_unique_hint_00516af0.cpp', '00/43/STL_insertunique_00431606.cpp', '00/43/STL_Minsert_004316cf.cpp', '00/43/global_Alloc_00431773.cpp', '00/42/STL_Rb_global_Rebalance_0042971d.cpp', '00/43/STL_DefStringMapErase_0043020a.cpp']
    test_names = ['00/47/STLStringLongTree_Mlowerbound_0047547d_test.cpp', '00/a6/STLChecksumCacheTree_LowerBound_00a609c0_test.cpp', '00/51/STLMapCCharStringlong_operator_00519230_test.cpp', '00/a6/STLChecksumCacheMap_Index_00a617e0_test.cpp', 'integration/DefStringUnlock_composed_test.cpp', '00/a6/CChecksumCacheFile_GetChecksum_00a60fb0_test.cpp']
    checksum_original = (ROOT / 'rebuild/tests' / test_names[-1]).read_text()
    for relative in test_names:
        text = (ROOT / 'rebuild/tests' / relative).read_text()
        for old, new in (
            ('template<> Tree::_Base_ptr Tree::_M_lower_bound(const CCharString& k)', 'template<> Tree::_Link_type Tree::_M_lower_bound(const CCharString& k) const'),
            ('template<> Tree::_Base_ptr Tree::_M_lower_bound(const CWideString& key)', 'template<> Tree::_Link_type Tree::_M_lower_bound(const CWideString& key) const')):
            text = text.replace(new, old)
        assert hashlib.sha256(text.encode()).hexdigest() == baseline['original_test_text_sha256'][relative], ('Original test logic changed', relative)
    embedded_sources = {ROOT / 'rebuild/src/compiled' / name[len('../src/compiled/'):] for name in re.findall(r'^#include "([^"]+)"', checksum_original, re.M) if name.startswith('../src/compiled/')}
    production_sources = embedded_sources | {ROOT / 'rebuild/src/compiled' / n for n in unlock_names} | {ROOT / 'rebuild/src/compiled/00/a6/CChecksumCacheFile_GetChecksum_00a60fb0.cpp'}
    files = {Path(__file__), HERE / 'native_const_tree_bounds_baseline.json', ROOT / 'rebuild/build_candidates.ps1', Path(V.__file__), *sources.values(), *production_sources, *(ROOT / 'rebuild/tests' / n for n in test_names)}
    files.add(Path(closure.__file__))
    adjacent_headers = set()
    for source in sources.values():
        adjacent = source.parent / 'fable_stlport_tree_bounds.h'
        if adjacent.is_file():
            assert adjacent.read_bytes() == (include / adjacent.name).read_bytes()
            adjacent_headers.add(adjacent)
            files.add(adjacent)
    for directory in (include, V.VC / 'include', V.VC / 'lib'):
        files.update(p for p in directory.rglob('*') if p.is_file())
    files.update(V.VC / 'bin' / n for n in ('cl.exe', 'c1xx.dll', 'c2.dll', 'link.exe', 'mspdb71.dll'))
    before = {str(p): sha(p) for p in sorted(files)}
    out.mkdir(parents=True)
    def save(name, value):
        with (out / name).open('x', encoding='utf-8') as stream:
            json.dump(value, stream, indent=2)
            stream.write('\n')
    save('before.json', before)
    objects, rows = {}, []
    env = V.env()
    env['INCLUDE'] = str(V.VC / 'include') + ';' + str(include)

    def compile_one(source, label, flags, selected_include=None):
        selected_include = include if selected_include is None else selected_include
        obj = out / (label + '.obj')
        command = ['/nologo', '/c', *[f for f in flags if f not in ('/nologo', '/c')], '/showIncludes', '/I' + str(selected_include), '/Fo' + str(obj), str(source)]
        selected_env = dict(env)
        selected_env['INCLUDE'] = str(V.VC / 'include') + ';' + str(selected_include)
        cp = V.cl(command, selected_env, out)
        save(label + '.compile.json', dict(command=command, exit_code=cp.returncode, stdout=cp.stdout, stderr=cp.stderr))
        assert cp.returncode == 0, cp.stdout + cp.stderr
        included = [Path(line.split('including file:', 1)[1].strip()).resolve() for line in cp.stdout.splitlines() if 'including file:' in line]
        assert all(p.is_relative_to(selected_include) or p.is_relative_to(V.VC / 'include') or p in embedded_sources or p in adjacent_headers for p in included), included
        inv = inventory(obj)
        save(label + '.inventory.json', inv)
        objects[str(obj)] = inv['raw_sha256']
        return obj, inv

    def staged(text, label):
        path = out / (label + '.cpp')
        path.write_text(text, encoding='utf-8')
        return path

    def native(name):
        return ROOT / 'rebuild/src/compiled' / name

    def test(name):
        return ROOT / 'rebuild/tests' / name

    def link_run(label, objs, required, provider, pattern, wrong=None):
        # The fixture/other providers must request the selected production provider.
        requests = [f['target'] for obj in objs for sec in inventory(obj)['sections'] for f in sec['fixups']]
        assert required in requests, (label, required)
        defs = [s for obj in objs for s in inventory(obj)['symbols'] if s['name'] == required and s['section'] > 0]
        assert not defs, 'The omitted provider must not be replaced by fixture or COMDAT definitions'
        def link(suffix, additions):
            command = [str(V.VC / 'bin/link.exe'), '/nologo', '/out:' + str(out / (label + suffix + '.exe')), *map(str, objs + additions)]
            cp = subprocess.run(command, env=env, cwd=out, capture_output=True, text=True)
            save(label + suffix + '.link.json', dict(command=command, exit_code=cp.returncode, stdout=cp.stdout, stderr=cp.stderr))
            return cp
        for suffix, additions in [('_omission', [])] + ([('_wrong_old_signature', [wrong])] if wrong else []):
            cp = link(suffix, additions)
            assert cp.returncode != 0 and any(required in line and re.search(r'LNK(?:2001|2019)', line) for line in (cp.stdout + cp.stderr).splitlines()), cp.stdout + cp.stderr
        cp = link('_control', [provider])
        assert cp.returncode == 0, cp.stdout + cp.stderr
        cp = subprocess.run([str(out / (label + '_control.exe'))], cwd=out, capture_output=True, text=True, timeout=30)
        save(label + '.run.json', dict(exit_code=cp.returncode, stdout=cp.stdout, stderr=cp.stderr))
        assert cp.returncode == 0 and pattern in cp.stdout, cp.stdout + cp.stderr
        rows.append(dict(fixture=label, pass_pattern=pattern, production_symbol=required, wrong_old_signature_control=bool(wrong)))

    try:
        # Build actual frozen pre-migration production bodies against their original
        # declarations. These are reviewable negative fixtures, not ABI aliases.
        old_include = out / 'old_include'
        for header in include.rglob('*'):
            if header.is_file():
                copied = old_include / header.relative_to(include)
                copied.parent.mkdir(parents=True, exist_ok=True)
                copied.write_bytes(header.read_bytes())
        (old_include / 'fable_stlport_tree.h').write_text(baseline['old_tree_header_text'], encoding='utf-8')
        providers = {}
        for name, spec in baseline['providers'].items():
            for profile in ('catalog', 'runtime'):
                obj, inv = compile_one(sources[name], name + '_' + profile, spec[profile + '_flags'])
                assert contribution(inv, spec['symbol']) == spec[profile], (name, profile, 'Root contribution drift')
                assert not any(s['name'] == spec['old_symbol'] for s in inv['symbols']), 'No old-signature alias allowed'
                providers[name, profile] = obj
            old_path, old_inv = compile_one(staged(spec['original_source_text'], name + '_old_production'), name + '_old_production', spec['catalog_flags'], old_include)
            assert contribution(old_inv, spec['old_symbol']) == spec['catalog'], 'Frozen genuine old-provider contribution drift'
            assert not any(s['name'] == spec['symbol'] for s in old_inv['symbols']), 'Negative old provider unexpectedly defines migrated identity'
            providers[name, 'old'] = old_path
            lower_test = test('00/47/STLStringLongTree_Mlowerbound_0047547d_test.cpp' if name == 'string' else '00/a6/STLChecksumCacheTree_LowerBound_00a609c0_test.cpp')
            obj, _ = compile_one(lower_test, name + '_lower_fixture', ['/Od', '/W3'])
            link_run(name + '_lower', [obj], spec['symbol'], providers[name, 'catalog'], 'STRING_TREE_LOWER_BOUND_PASS' if name == 'string' else 'CACHE_LOWER_BOUND_00a609c0_PASS', providers[name, 'old'])

        # Signature-only fixture changes: all existing call-order/temporary/reference assertions stay intact.
        for name, relative, old, new, pattern in (
            ('string', '00/51/STLMapCCharStringlong_operator_00519230_test.cpp', 'template<> Tree::_Base_ptr Tree::_M_lower_bound(const CCharString& k)', 'template<> Tree::_Link_type Tree::_M_lower_bound(const CCharString& k) const', 'FABLETLC_STLMAP_CCHARSTRING_LONG_INDEX_00519230_PASS'),
            ('checksum', '00/a6/STLChecksumCacheMap_Index_00a617e0_test.cpp', 'template<> Tree::_Base_ptr Tree::_M_lower_bound(const CWideString& key)', 'template<> Tree::_Link_type Tree::_M_lower_bound(const CWideString& key) const', 'CACHE_MAP_INDEX_00a617e0_PASS')):
            original = test(relative).read_text()
            assert (original.count(old), original.count(new)) in ((1, 0), (0, 1))
            migrated = original.replace(old, new)
            assert hashlib.sha256(migrated.replace(new, old).encode()).hexdigest() == baseline['original_test_text_sha256'][relative]
            fixture, _ = compile_one(staged(migrated, name + '_map_fixture'), name + '_map_fixture', ['/Od', '/W3'])
            if name == 'checksum':
                source, inv = compile_one(native('00/a6/STLChecksumCacheMap_Index_00a617e0.cpp'), 'checksum_map_source', ['/O2', '/Oy', '/W3'])
                sym = next(s['name'] for s in inv['symbols'] if s['name'].startswith('??A?$map@') and s['section'] > 0)
                link_run(name + '_map', [fixture], sym, source, pattern)
            else:
                # Existing string fixture instantiates the actual map implementation from its header.
                exe = out / 'string_map.exe'
                cp = subprocess.run([str(V.VC / 'bin/link.exe'), '/nologo', '/out:' + str(exe), str(fixture)], env=env, cwd=out, capture_output=True, text=True)
                save('string_map.link.json', dict(exit_code=cp.returncode, stdout=cp.stdout, stderr=cp.stderr))
                assert cp.returncode == 0, cp.stdout + cp.stderr
                cp = subprocess.run([str(exe)], cwd=out, capture_output=True, text=True, timeout=30)
                save('string_map.run.json', dict(exit_code=cp.returncode, stdout=cp.stdout, stderr=cp.stderr))
                assert cp.returncode == 0 and pattern in cp.stdout
                rows.append(dict(fixture='string_map', pass_pattern=pattern, boundary='Mock bound/insertion; actual map header implementation'))

        compiled = [compile_one(native(n), 'unlock_provider_' + str(i), ['/O2', '/MT'])[0] for i, n in enumerate(unlock_names)]
        compiled.append(compile_one(test('integration/DefStringUnlock_composed_test.cpp'), 'unlock_fixture', ['/O2', '/MT'])[0])
        link_run('unlock_composed', compiled, baseline['providers']['string']['symbol'], providers['string', 'runtime'], 'DEFSTRING_UNLOCK_COMPOSED_OK cases=14', providers['string', 'old'])

        original = test('00/a6/CChecksumCacheFile_GetChecksum_00a60fb0_test.cpp').read_text()
        def transform(match):
            name = match.group(1)
            if not name.startswith('../src/compiled/'):
                return match.group(0)
            name = name[len('../src/compiled/'):]
            if name == baseline['providers']['checksum']['source']:
                return '// Production lower-bound provider is separately linked.'
            return '#include "' + native(name).as_posix() + '"'
        changed = re.sub(r'^#include "([^"]+)"', transform, original, flags=re.M)
        # Only include routing changes; all original test code after includes is identical.
        assert changed[changed.index('#include <stdio.h>'):] == original[original.index('#include <stdio.h>'):]
        fixture, _ = compile_one(staged(changed, 'checksum_composed_fixture'), 'checksum_composed_fixture', ['/O2', '/MT'])
        get_checksum, _ = compile_one(native('00/a6/CChecksumCacheFile_GetChecksum_00a60fb0.cpp'), 'get_checksum_source', ['/O2', '/MT'])
        link_run('checksum_composed', [fixture, get_checksum], baseline['providers']['checksum']['symbol'], providers['checksum', 'runtime'], 'CACHE_GET_CHECKSUM_00a60fb0_PASS', providers['checksum', 'old'])
        result = dict(status='NATIVE_CONST_BOUND_REGRESSION_PASS', profiles=['catalog /O2 /Oy /W3', 'recorded runtime /O2 /Oy /GS /MT', 'catalog fixture /Od /W3', 'composed behavior /O2 /MT'], root_bytes_and_typed_fixups_preserved=True, fixtures=rows, boundary='Substantive native composition with existing mocked strings/files/streams; no physical lifetime, whole-engine closure or retail parity claim.')
    finally:
        after = {str(p): sha(p) for p in sorted(files)}
        save('after.json', dict(input_hashes=after, mismatches=[p for p in set(before) | set(after) if before.get(p) != after.get(p)], object_hashes=objects))
        assert before == after, 'Input drift'
        assert all(sha(Path(p)) == digest for p, digest in objects.items()), 'First object changed'
    save('result.json', result)
    print('NATIVE_CONST_BOUND_REGRESSION_PASS', out, flush=True)


if __name__ == '__main__':
    main()
