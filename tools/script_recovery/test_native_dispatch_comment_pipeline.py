from pathlib import Path
from copy import deepcopy
import hashlib,json
import pytest
from tools.script_recovery import convert_quest_unit
from tools.script_recovery.script_units import UNITS
F=Path(__file__).resolve().parent/'fixtures/dispatch_staging/book_wrong_time'
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
    results={};definition=UNITS['book_collecting']
    for mode,unit_name in [('disabled','baseline_unit.json'),('enabled','candidate_unit.json')]:
        unit=read(F/unit_name);out=tmp_path_factory.mktemp('comment_staging_'+mode)
        report=convert_quest_unit.UnitConverter(definition['evidence']/'translation_unit_typed.json').convert(unit,out)
        convert_quest_unit.write_reports([report],out,'Comment-only dispatch pipeline')
        results[mode]=out
    return results
@pytest.mark.parametrize('mode,baseline',[('disabled','baseline_draft'),('enabled','candidate_draft')])
def test_output_and_report_match_pinned_pipeline(generated,mode,baseline):
    actual=generated[mode];expected=F/baseline
    assert read(actual/'CONVERSION_REPORT.json')==read(expected/'CONVERSION_REPORT.json')
    assert {p.relative_to(actual).as_posix():sha(p) for p in (actual/'FSE').rglob('*.lua')}=={p.relative_to(expected).as_posix():sha(p) for p in (expected/'FSE').rglob('*.lua')}
def test_only_one_diagnostic_and_its_proof_are_changed(generated):
    old=read(generated['disabled']/'CONVERSION_REPORT.json');new=read(generated['enabled']/'CONVERSION_REPORT.json')
    expected=deepcopy(old);unit=expected['units'][0]
    main=next(f for f in unit['functions'] if f['address'].lower()=='0x00e54e90')
    actual=next(f for f in new['units'][0]['functions'] if f['address'].lower()=='0x00e54e90')
    main['todo'].remove('unlifted: xStack_7c = *pCVar11;');main['reviewedDispatchStaging']=actual['reviewedDispatchStaging']
    unit['summary']['todo']-=1;expected['summary']['todo']-=1
    assert expected==new
