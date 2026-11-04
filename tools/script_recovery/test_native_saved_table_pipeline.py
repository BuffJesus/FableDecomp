from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
R=Path(__file__).resolve().parents[2]
F=Path(__file__).resolve().parent/'fixtures/saved_table/book_main'
from tools.script_recovery import convert_quest_unit,native_reviewed_saved_table_staging as staging
from tools.script_recovery.script_units import UNITS
from tools.script_recovery.readable_lua import tokens
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
    definition=UNITS['book_collecting'];results={}
    for name,cache,older_copies in [('disabled',False,True),('enabled',True,True),('no_tracing',False,False),('cache_tracing_only',True,False)]:
        unit=read(F/'candidate_unit.json')
        if not cache:unit.pop('reviewedSavedTableStaging')
        if not older_copies:unit['reviewedInterfaceTableStaging']['0x00e54e90'].pop('copies')
        if name=='cache_tracing_only':
            unit['reviewedSavedTableStaging']['0x00e54e90']['luaPhaseSha256']=sha(results['no_tracing']['phase'].encode())
        out=tmp_path_factory.mktemp('saved_table_'+name);calls=[];captured=[];original=staging.classify
        def observe(*args,**kwargs):
            calls.append((int(args[2]['address'],16),len(args[6]),args[3] is not None))
            if int(args[2]['address'],16)==0xe54e90:captured.append(args[0])
            return original(*args,**kwargs)
        staging.classify=observe
        try:
            report=convert_quest_unit.UnitConverter(definition['evidence']/'translation_unit_typed.json').convert(unit,out)
            convert_quest_unit.write_reports([report],out,'Saved table pipeline test')
        finally:staging.classify=original
        assert len(captured)==1
        results[name]=dict(out=out,calls=calls,phase=captured[0],report=read(out/'CONVERSION_REPORT.json'))
    return results
def test_disabled_metadata_preserves_current_lua_and_report(generated):
    result=generated['disabled'];baseline=F/'baseline_draft'
    assert result['report']==read(baseline/'CONVERSION_REPORT.json')
    expected={p.relative_to(baseline).as_posix():sha(p.read_bytes()) for p in (baseline/'FSE').rglob('*.lua')}
    actual={p.relative_to(result['out']).as_posix():sha(p.read_bytes()) for p in (result['out']/'FSE').rglob('*.lua')}
    assert actual==expected and len(actual)==4
@pytest.mark.parametrize('name,count,reviewed',[
    ('disabled',7,False),('enabled',7,True),('no_tracing',0,False),('cache_tracing_only',7,True),
])
def test_copy_trace_is_requested_only_for_main_and_cleared_between_functions(generated,name,count,reviewed):
    calls=generated[name]['calls'];assert len(calls)>1
    assert [(n,r) for address,n,r in calls if address==0xe54e90]==[(count,reviewed)]
    assert all(n==0 and not r for address,n,r in calls if address!=0xe54e90)
@pytest.mark.parametrize('old_name,new_name',[('disabled','enabled'),('no_tracing','cache_tracing_only')])
def test_review_changes_only_four_annotations_and_exact_report_field(generated,old_name,new_name):
    old,new=generated[old_name],generated[new_name]
    before={p.relative_to(old['out']).as_posix():p for p in (old['out']/'FSE').rglob('*.lua')}
    after={p.relative_to(new['out']).as_posix():p for p in (new['out']/'FSE').rglob('*.lua')}
    assert before.keys()==after.keys() and len(before)==4
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    for name,path in before.items():
        a,b=path.read_text(encoding='utf-8'),after[name].read_text(encoding='utf-8')
        if name!='FSE/V_BookCollecting/Entities/BS_Teacher.lua':assert a==b
        else:
            assert a.count('-- TODO(native): iVar8 = *xStack_7c')==4
            assert b.count('-- reviewed native saved-table staging: iVar8 = *xStack_7c')==4
            assert significant(a)==significant(b)
    expected=deepcopy(old['report']);unit=expected['units'][0]
    main=next(f for f in unit['functions'] if f['address'].lower()=='0x00e54e90')
    actual=next(f for f in new['report']['units'][0]['functions'] if f['address'].lower()=='0x00e54e90')
    diagnostic='unlifted: iVar8 = *xStack_7c';assert main['todo'].count(diagnostic)==4
    main['todo']=[s for s in main['todo'] if s!=diagnostic]
    main['reviewedSavedTableStaging']=actual['reviewedSavedTableStaging']
    unit['summary']['todo']-=4;expected['summary']['todo']-=4
    assert expected==new['report']
