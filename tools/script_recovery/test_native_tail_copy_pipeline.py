from pathlib import Path
from copy import deepcopy
import hashlib,json
import pytest
from tools.script_recovery import convert_quest_unit, native_reviewed_interface_table_staging as staging
from tools.script_recovery.script_units import UNITS
W=Path(__file__).resolve().parent/'fixtures/tail_copy/book_main'
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
    results={};definition=UNITS['book_collecting']
    for enabled in [False,True]:
        unit=read(W/'candidate_unit.json')
        if not enabled:unit['reviewedInterfaceTableStaging']['0x00e54e90'].pop('copies')
        out=tmp_path_factory.mktemp('tail_copy_'+str(enabled));calls=[];original=staging.classify
        def observed(*args,**kwargs):
            calls.append((int(args[2]['address'],16),len(kwargs['copy_trace'])))
            return original(*args,**kwargs)
        staging.classify=observed
        try:
            report=convert_quest_unit.UnitConverter(definition['evidence']/'translation_unit_typed.json').convert(unit,out)
            convert_quest_unit.write_reports([report],out,'Copy provenance test')
        finally:staging.classify=original
        results[enabled]=dict(out=out,calls=calls,report=read(out/'CONVERSION_REPORT.json'))
    return results

@pytest.mark.parametrize('enabled',[False,True])
def test_trace_is_enabled_only_for_the_requested_function(generated,enabled):
    calls=generated[enabled]['calls'];assert len(calls)>1
    assert [n for address,n in calls if address==0xe54e90]==([7] if enabled else [0])
    assert all(n==0 for address,n in calls if address!=0xe54e90)

def test_copy_metadata_changes_only_the_reviewed_annotations_and_report(generated):
    old,new=generated[False],generated[True]
    before={p.relative_to(old['out']).as_posix():p for p in old['out'].rglob('*.lua')}
    after={p.relative_to(new['out']).as_posix():p for p in new['out'].rglob('*.lua')}
    assert before.keys()==after.keys() and len(before)==4
    teacher='FSE/V_BookCollecting/Entities/BS_Teacher.lua';copy=read(W/'candidate_row.json')['copies'][0]
    region=copy['luaRegion'];replacement=region.replace('-- TODO(native): ','-- reviewed native interface-table staging: ',1).replace('--[[unresolved native value]]','--[[reviewed native interface-table staging]]',1)
    for name,path in before.items():
        if name!=teacher:assert sha(path)==sha(after[name])
        else:
            source=path.read_text(encoding='utf-8');assert source.count(region)==1
            assert source.replace(region,replacement,1)==after[name].read_text(encoding='utf-8')
    expected=deepcopy(old['report']);unit=expected['units'][0]
    main=next(row for row in unit['functions'] if row['address'].lower()=='0x00e54e90')
    actual=next(row for row in new['report']['units'][0]['functions'] if row['address'].lower()=='0x00e54e90')
    main['todo'].remove('unlifted: iVar8 = *CVar12');main['reviewedInterfaceTableStaging'][0]['copies']=actual['reviewedInterfaceTableStaging'][0]['copies']
    unit['summary']['todo']-=1;expected['summary']['todo']-=1
    assert expected==new['report']

def test_lifter_reset_clears_prior_function_copy_events():
    from tools.script_recovery.test_lift_native_lua import make
    lifter=make();lifter.native_tail_copy_trace=[dict(previous=True)];lifter.reset()
    assert lifter.native_tail_copy_trace==[]
