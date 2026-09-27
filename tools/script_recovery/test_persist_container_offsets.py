"""Named retail persistence transfers can disambiguate debug container layout."""
import copy
import json
from pathlib import Path

import pytest
from tools.script_recovery.quest_unit_evidence import persisted_container_offsets

ROW = {'name': 'Owned', 'type': 'vector<bool,std::allocator<bool>_>', 'offset': '0xb0'}
LABEL = 'CPersistContext::Transfer<signed_char>'
FN = {'address': '0x100', 'decompile':
      LABEL + '(ctx,"Flag",(void *)((int)this + 0x78),defaultValue);\n' +
      LABEL + '(ctx,0x200,(int)this + 0xac);',
      'strings': [{'address': '0x00000200', 'value': 'Owned'}],
      'calls': [{'currentName': LABEL, 'site': '0x110', 'target': '0x300'},
                {'currentName': LABEL, 'site': '0x120', 'target': '0x400'}]}


def test_pairs_container_with_its_own_native_call_among_scalar_overloads():
    row = persisted_container_offsets([ROW], {'0x78': ['Flag', 'Bool']}, FN)[0]
    assert row['offset'] == '0xac'
    assert row['estimatedOffset'] == '0xb0'
    assert row['offsetEvidence']['site'] == '0x120'
    assert row['offsetEvidence']['callee'] == '0x400'
    assert ROW['offset'] == '0xb0'  # caller metadata remains immutable


@pytest.mark.parametrize('case', ['missing-call', 'unknown-name', 'foreign-owner',
                                  'conflicting-offsets', 'scalar-overlap', 'duplicate-member'])
def test_ambiguous_or_unproven_offsets_are_not_applied(case):
    fn, rows, fields = copy.deepcopy(FN), [dict(ROW)], {}
    if case == 'missing-call': fn['calls'].pop()
    if case == 'unknown-name': fn['strings'][0]['value'] = 'Other'
    if case == 'foreign-owner': fn['decompile'] = fn['decompile'].replace('this + 0xac', 'other + 0xac')
    if case == 'conflicting-offsets':
        fn['decompile'] += '\n' + LABEL + '(ctx,0x200,(int)this + 0xc0);'
        fn['calls'].append(dict(fn['calls'][-1], site='0x130'))
    if case == 'scalar-overlap': fields = {'0xac': ['Other', 'Int']}
    if case == 'duplicate-member': rows.append(dict(ROW))
    assert persisted_container_offsets(rows, fields, fn) == rows


def test_book_owned_native_transfer_corrects_estimated_debug_offset():
    root = Path(__file__).resolve().parents[2]
    evidence = root / 'refs/script_recovery/book_collecting'
    f = next(f for f in json.loads((evidence / 'translation_unit_typed.json').read_text())['functions']
             if f['address'].lower() == '0x00e54880')
    row = persisted_container_offsets([dict(ROW, name='BookOwned')], {}, f)[0]
    assert row['offset'] == '0xac'
    assert row['offsetEvidence']['site'] == '0x00E54981'
    assert row['offsetEvidence']['callee'] == '0x00CDCF80'
