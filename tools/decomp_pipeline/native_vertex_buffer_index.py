"""Admit only qualified vertex bodies and the five-slot Win32 vtable."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
from gate_game_component_abi import Coff

ROOT = Path(__file__).resolve().parents[2]
DISPATCH = '00/a6/CVertexBuffer_Dispatch_00a63240.cpp'
LIFETIME = '00/9e/CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp'
FACTORY = '00/9e/CFontManager_CFontManager_009e2e30.cpp'
TABLE = '??_7CVertexBufferWin32@@6B@'
SCALAR = '??_GCVertexBufferWin32@@UAEPAXI@Z'


def normalized(coff, symbol):
    section, offset = coff.symbols[symbol]
    raw, rels = coff.sections[section]
    if symbol == TABLE:
        assert offset in (0, 4) and len(raw) == offset + 20
        # Optional /GR locator is outside the vtable's five callable slots.
        return raw[offset:], {site-offset: value for site, value in rels.items() if site >= offset}
    assert offset == 0
    return raw, rels


def qualify(source, obj, rows):
    if source not in (DISPATCH, LIFETIME, FACTORY):
        return rows
    receipt = json.loads((ROOT/'rebuild/compile-gate/native-vertex-buffer-dispatch.json').read_text())
    assert receipt['status'] == 'PASS' and receipt['repeat']
    assert receipt['cases'] == 23328 and receipt['original_paths'] == 93312
    assert receipt['original_variants'] == 4 and len(receipt['controls']) == 26
    assert receipt['qualification']['verified_bytes'] == 406
    for name, digest in receipt['input_sha256'].items():
        path = Path(name)
        if path.suffix in ('.h', '.cpp', '.py'):
            assert hashlib.sha256(path.read_bytes()).hexdigest() == digest, name
    reference_source = LIFETIME if source == FACTORY else source
    reference = next(Path(path) for path, name in receipt['object_sources'].items()
                     if Path(name).resolve() == (ROOT/'rebuild/src/compiled'/reference_source).resolve())
    assert hashlib.sha256(reference.read_bytes()).hexdigest() == receipt['objects'][str(reference)]
    allowed = {row['symbol']: row['address'][2:].zfill(8)
               for row in receipt['qualification']['proof'][:8]} if source == DISPATCH else {TABLE: '0129ba20'}
    if source == LIFETIME:
        allowed[SCALAR] = '009e2890'
    fresh, accepted = Coff(obj), Coff(reference)
    for symbol in allowed:
        assert normalized(fresh, symbol) == normalized(accepted, symbol), symbol
    if source == FACTORY:
        # These inline constructor/scalar/base bodies have no established retail
        # addresses. Keep unrelated factory providers, but do not mislabel these
        # companions with the factory's address.
        unlocated = ('??0CVertexBuffer', '??1CVertexBuffer', '??_GCVertexBuffer',
                     '??_ECVertexBuffer', '??_7CVertexBufferBase')
        # The base deleting destructor is located (retail 0x009E2B20, exact in gate_native_font_manager).
        located = {'??_GCVertexBufferBase@@UAEPAXI@Z': '009e2b20'}
        result = [(name, located.get(name, allowed.get(name, address)), path, kind)
                  for name, address, path, kind in rows if name in located or not name.startswith(unlocated)]
        assert sum(row[0] == TABLE for row in result) == 1
    else:
        result = [(name, allowed[name], path, kind)
                  for name, _, path, kind in rows if name in allowed]
        assert len(result) == len(allowed)
    directory = Path(receipt['build'])/('index-provider-'+datetime.now(timezone.utc).strftime('%H%M%S%f'))
    directory.mkdir()
    retained = directory/'provider.obj'
    retained.write_bytes(Path(obj).read_bytes())
    (directory/'receipt.json').write_text(json.dumps(dict(
        source=source, rows=result, object=str(retained),
        object_sha256=hashlib.sha256(retained.read_bytes()).hexdigest(),
        qualifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        indexer_sha256=hashlib.sha256((ROOT/'tools/decomp_pipeline/symbol_index.py').read_bytes()).hexdigest(),
        qualification_build=receipt['build']), indent=2))
    return result
