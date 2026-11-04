import copy
from pathlib import Path

import pytest

from tools.script_recovery import native_random as random

SOURCE = '''iVar1 = rand();
iVar1 = iVar1 % 3;
if (ready) { value = rand() & 3; }
nextValue = rand() % 10;
'''


def native(count=3):
    calls = [dict(currentName='MSVCR71.dll::rand', site=hex(0x1000+16*i),
                  target=hex(random.RAND_THUNK)) for i in range(count)]
    data = {random.RAND_THUNK: random.RAW_THUNK_BYTES}
    for call in calls:
        address = int(call['site'],16)
        data[address] = b'\xe8' + (random.RAND_THUNK-address-5).to_bytes(4,'little',signed=True)
    return dict(calls=calls), data


def test_partial_pairs_keep_their_modulus_and_complete_only_raw_draws():
    fn,data = native()
    result,evidence = random.fold_retail_random_with_raw_fallback(SOURCE,fn,lambda a,n:data.get(a))
    expected = '''iVar1 = GSI->RetailRandModulo(3);
if (ready) { value = GSI->RetailRandModulo(32768) & 3; }
nextValue = GSI->RetailRandModulo(32768) % 10;
'''
    assert result == expected
    assert evidence[-1]['draws'] == 2 and evidence[-1]['pairedDraws'] == 1
    assert evidence[-1]['verifiedFunctionCallSites'] == [c['site'] for c in fn['calls']]


@pytest.mark.parametrize('args', ['(int)p0,(uint)p1,p2', '0', '(undefined4)registerValue'])
def test_pure_stale_arguments_do_not_block_remaining_draws(args):
    fn,data=native();source=SOURCE.replace('value = rand()',f'value = rand({args})')
    result,evidence=random.fold_retail_random_with_raw_fallback(source,fn,lambda a,n:data.get(a))
    assert 'rand(' not in result and evidence[-1]['draws']==2


@pytest.mark.parametrize('args', ['++x', '*pointer', 'pointer[0]', 'callback()', '/* preserve */'])
def test_unproven_raw_source_preserves_existing_pairs_only(args):
    fn,data=native();source=SOURCE.replace('value = rand()',f'value = rand({args})')
    reader=lambda a,n:data.get(a)
    assert random.fold_retail_random_with_raw_fallback(source,fn,reader)==random.fold_retail_random(source,fn,reader)


def test_only_pairs_and_only_raw_calls_keep_previous_behavior():
    fn,data=native(2);reader=lambda a,n:data.get(a)
    pairs='iVar1 = rand(); iVar1 = iVar1 % 3; iVar2 = rand(); iVar2 = iVar2 % 10;'
    assert random.fold_retail_random_with_raw_fallback(pairs,fn,reader)==random.fold_retail_random(pairs,fn,reader)
    raw='value = rand() & 3; nextValue = rand() % 10;'
    assert random.fold_retail_random_with_raw_fallback(raw,fn,reader)==random.recover_raw_retail_random(raw,fn,reader)


@pytest.mark.parametrize('kind', ['opcode','target','missing','extra','duplicate','thunk'])
def test_original_complete_native_inventory_is_required(kind):
    fn,data=native()
    if kind=='opcode':data[0x1020]=b'\x90'*5
    elif kind=='target':fn['calls'][2]['target']='0x100'
    elif kind=='missing':fn['calls'].pop()
    elif kind=='extra':fn['calls'].append(dict(fn['calls'][0],site='0x1030'))
    elif kind=='duplicate':fn['calls'][2]=copy.deepcopy(fn['calls'][1])
    elif kind=='thunk':data[random.RAND_THUNK]=b'\x90'*6
    reader=lambda a,n:data.get(a)
    assert random.fold_retail_random_with_raw_fallback(SOURCE,fn,reader)==random.fold_retail_random(SOURCE,fn,reader)


@pytest.mark.parametrize('status', ['rejected','recovered'])
def test_reviewed_evidence_is_never_reinterpreted_as_generic_pairs(monkeypatch,status):
    fn,data=native();result=SOURCE if status=='rejected' else SOURCE.replace('rand()','GSI->RetailRandModulo(7)',1)
    evidence=[dict(status=status,kind='conditional modulo',reason='pinned witness')]
    monkeypatch.setattr(random,'fold_retail_random',lambda *_:(result,evidence))
    assert random.fold_retail_random_with_raw_fallback(SOURCE,fn,lambda a,n:data.get(a))==(result,evidence)


@pytest.mark.parametrize('kind', ['draw_count','api_count','extra_metadata'])
def test_inconsistent_pair_accounting_does_not_enable_raw_completion(monkeypatch,kind):
    fn,data=native();result,evidence=random.fold_retail_random(SOURCE,fn,lambda a,n:data.get(a))
    if kind=='draw_count':evidence=evidence*2
    elif kind=='api_count':result=result.replace('rand()', '0', 1);evidence=evidence*2
    else:evidence[0]['unreviewed']=True
    monkeypatch.setattr(random,'fold_retail_random',lambda *_:(result,evidence))
    assert random.fold_retail_random_with_raw_fallback(SOURCE,fn,lambda a,n:data.get(a))==(result,evidence)


def test_literal_rand_text_and_existing_native_api_do_not_shift_counts():
    fn,data=native();source='print("rand() GSI->RetailRandModulo(7)");\n'+SOURCE
    result,evidence=random.fold_retail_random_with_raw_fallback(source,fn,lambda a,n:data.get(a))
    assert result.startswith('print("rand() GSI->RetailRandModulo(7)");')
    assert evidence[-1]['draws']==2
