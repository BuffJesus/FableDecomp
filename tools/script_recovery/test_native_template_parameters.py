"""Declaration templates must not split parameters or reinterpret expressions."""
import json
from pathlib import Path

import pytest

from tools.script_recovery.native_arguments import split_arguments
from tools.script_recovery.native_function_parameters import function_parameters


def test_native_book_helper_retains_index_and_argument_map():
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/book_collecting/translation_unit_typed.json').read_text(encoding='utf-8-sig'))
    fn = next(f for f in unit['functions'] if f['address'].lower() == '0x00e57020')
    signature = function_parameters(fn['decompile'], member=True)
    assert 'problem' not in signature
    assert [p['native'] for p in signature['parameters']] == ['param_1', 'param_2']
    assert signature['parameters'][0]['type'] == 'long'
    assert signature['parameters'][1]['type'].startswith('map<CCharString,CCharString,')
    assert signature['parameters'][1]['type'].endswith('*')
    assert signature['returnKind'] is None


@pytest.mark.parametrize('close', ['>>', '> >'])
def test_nested_type_commas_and_adjacent_parameters(close):
    nested = 'map<Key, vector<Value' + close + ' *items'
    assert split_arguments('long index, ' + nested + ', bool enabled', template_types=True) == [
        'long index', nested, 'bool enabled']


def test_call_comparisons_and_shifts_keep_expression_semantics():
    assert split_arguments('a < b, c > d, bits >> 2, f(a, b)') == [
        'a < b', 'c > d', 'bits >> 2', 'f(a, b)']


@pytest.mark.parametrize('operand', ['map<A,B *items', 'map<A,B>> *items'])
def test_malformed_template_signature_fails_closed(operand):
    result = function_parameters('void __thiscall Fn(Owner *this, ' + operand + ') {}', member=True)
    assert result['parameters'] == []
    assert result['problem'] == 'unparsed native signature'


def test_reviewed_nested_type_does_not_create_phantom_parameters():
    source = '''/* [bsim sim=1 <- ego_r]
    void __thiscall Owner::Fn(long,class map<Key,Value> &) */
    void __thiscall Fn(Owner *this, long index, map<Key,Value> *values) {}'''
    result = function_parameters(source, member=True)
    assert [p['native'] for p in result['parameters']] == ['index', 'values']


def test_literal_angle_brackets_are_not_delimiters():
    assert split_arguments('"<,>", map<A,B> *items', template_types=True) == ['"<,>"', 'map<A,B> *items']
