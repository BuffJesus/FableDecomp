"""Whole existing SickChild pipeline: opt-in and absent-metadata contracts."""
from pathlib import Path
import hashlib,json,tempfile
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter,write_reports
from tools.script_recovery.build_readable_unit import build
from tools.script_recovery.script_units import UNITS
F=Path(__file__).resolve().parent/'fixtures/member_table/pipeline'
@pytest.mark.parametrize('mode',['disabled','enabled'])
def test_member_table_metadata_preserves_full_unit_pipeline(mode):
    root=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists())
    tmp_path=Path(tempfile.mkdtemp(prefix='codex_lua_member_table_pipeline_',dir=root/'work'))
    expected=json.loads((F/'expected.json').read_text())[mode]
    unit=json.loads((F/('candidate_unit.json' if mode=='enabled' else 'baseline_unit.json')).read_text())
    definition=UNITS['sick_child']
    draft=tmp_path/'draft'
    report=UnitConverter(definition['evidence']/'translation_unit_typed.json').convert(unit,draft)
    write_reports([report],draft,'Native member-table comment pipeline regression')
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
