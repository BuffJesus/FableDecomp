"""Admit the qualified font prepare method and its one additional lookup cell."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data

ROOT = Path(__file__).resolve().parents[2]
SOURCE = '00/ab/CStaticFontBank_PrepareToRender_00ab83a0.cpp'
METHOD = '?PrepareToRender@CStaticFontBank@@UBEX_N@Z'
DATA = '?FableRenderDefaultValue_01396dc8@@3KA'


def qualify(source, obj, rows):
    if source != SOURCE:
        return rows
    receipt = json.loads((ROOT/'rebuild/compile-gate/native-static-font-prepare.json').read_text())
    assert receipt['status'] == 'PASS' and receipt['repeat'] and len(receipt['controls']) == 28
    assert receipt['cases'] == 2304 and receipt['paths'] == 4608
    proof = receipt['qualification']
    assert proof['grade']['near_match'] and proof['helper_original_bytes_exact'] == 23
    assert proof['table_original_final_bytes_exact'] == 176 and proof['final_pe_function_bytes_exact'] == 2695
    for name, digest in receipt['input_sha256'].items():
        path = Path(name)
        if path.suffix in ('.h', '.cpp', '.py'):
            assert hashlib.sha256(path.read_bytes()).hexdigest() == digest, name
    reference = next(Path(path) for path, name in receipt['object_sources'].items()
                     if Path(name).resolve() == (ROOT/'rebuild/src/compiled'/SOURCE).resolve())
    assert hashlib.sha256(reference.read_bytes()).hexdigest() == receipt['objects'][str(reference)]
    fresh, accepted = Coff(obj), Coff(reference)
    fs, fo = fresh.symbols[METHOD]; rs, ro = accepted.symbols[METHOD]
    assert fo == ro == 0 and fresh.sections[fs] == accepted.sections[rs], METHOD
    assert coff_data(fresh, DATA, 4) == coff_data(accepted, DATA, 4) == bytes((1, 0, 0, 0))
    allowed = {METHOD: '00ab83a0', DATA: '01396dc8'}
    result = [(name, allowed[name], path, kind) for name, _, path, kind in rows if name in allowed]
    assert len(result) == 2 and {row[0] for row in result} == set(allowed)
    directory = Path(receipt['build'])/('index-provider-'+datetime.now(timezone.utc).strftime('%H%M%S%f'))
    directory.mkdir()
    retained = directory/'provider.obj'
    retained.write_bytes(Path(obj).read_bytes())
    (directory/'receipt.json').write_text(json.dumps(dict(source=source, rows=result,
        object=str(retained), object_sha256=hashlib.sha256(retained.read_bytes()).hexdigest(),
        qualifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        indexer_sha256=hashlib.sha256((ROOT/'tools/decomp_pipeline/symbol_index.py').read_bytes()).hexdigest()), indent=2))
    return result
