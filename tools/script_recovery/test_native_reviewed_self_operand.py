import copy
import json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import _text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_self_operand import restore

ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
    unit=json.loads((ROOT/'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8'))
    row=unit['reviewedSelfOperands']['0x00e02e50']
    fn=next(f for f in json.loads((ROOT/'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text(encoding='utf-8-sig'))['functions'] if int(f['address'],16)==0xe02e50)
    return fn,row,RData(),_text_order_sites(fn['decompile'],fn)
def test_restores_marker_without_renaming_other_uses(evidence):
    fn,row,data,entries=evidence
    text=fn['decompile']
    result=restore(text,fn,entries,data,row)
    assert result==text.replace('(CScriptThing_bv *)p0,false,true)',
                               '(CScriptThing *)((int)this + 8),false,true)')
    assert restore(text,fn,entries,data,None)==text
@pytest.mark.parametrize('change',['native','source','anchor','missing','ambiguous','receiver','argument'])
def test_stale_or_ambiguous_evidence_rejected(evidence,change):
    fn,row,data,entries=evidence
    row=copy.deepcopy(row);entries=copy.deepcopy(entries)
    if change=='native':row['nativeSha256']='0'*64
    if change=='source':row['sourceSha256']='0'*64
    if change=='anchor':row['anchors']['0xe0336b']='90'
    if change=='missing':entries=[]
    target=next((e for e in entries if int(e[3]['site'],16)==0xe0336c),None)
    if change=='ambiguous':entries.append(copy.deepcopy(target))
    if change=='receiver':target[2][0]='other'
    if change=='argument':target[2][-3]='different'
    with pytest.raises(ValueError):restore(fn['decompile'],fn,entries,data,row)
