"""Whole existing SickChild pipeline: opt-in and absent-metadata contracts."""
from pathlib import Path
import hashlib,json,tempfile
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter,write_reports
from tools.script_recovery.build_readable_unit import build
from tools.script_recovery.script_units import UNITS
F=Path(__file__).resolve().parent/'fixtures/missing_calls/pipeline'
@pytest.fixture(scope='module')
def reused_converter():
    return UnitConverter(UNITS['sick_child']['evidence']/'translation_unit_typed.json')

@pytest.mark.parametrize('mode',['disabled','enabled','disabled_reuse'])
def test_missing_calls_metadata_preserves_full_unit_pipeline(mode,reused_converter):
    root=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists())
    tmp_path=Path(tempfile.mkdtemp(prefix='codex_lua_missing_calls_pipeline_',dir=root/'work'))
    expected=json.loads((F/'expected.json').read_text())['enabled' if mode=='enabled' else 'disabled']
    unit=json.loads((F/('candidate_unit.json' if mode=='enabled' else 'baseline_unit.json')).read_text())
    definition=UNITS['sick_child']
    draft=tmp_path/'draft'
    original_index=reused_converter.by_address
    report=reused_converter.convert(unit,draft)
    assert reused_converter.by_address is original_index
    assert not hasattr(reused_converter,'retained_cstring_reviews')
    write_reports([report],draft,'Native missing-call comment pipeline regression')
    build('sick_child',draft=draft,out=tmp_path/'readable')
    lua=LuaRuntime()
    for stage in ['draft','readable']:
        tree=tmp_path/stage
        files={p.relative_to(tree/'FSE').as_posix():p for p in (tree/'FSE').rglob('*.lua')}
        assert {name:hashlib.sha256(p.read_bytes()).hexdigest() for name,p in files.items()}==expected[stage]['lua']
        for p in files.values():lua.compile(p.read_text())
        name='CONVERSION_REPORT.json' if stage=='draft' else 'READABLE_REPORT.json'
        actual=json.loads((tree/name).read_text())
        if stage=='readable':
            assert actual['draft']==str(draft.relative_to(root))
            actual['draft']=expected[stage]['report']['draft']
        assert actual==expected[stage]['report']
