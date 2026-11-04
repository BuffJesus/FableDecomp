"""Optional table-read facts require a receiver on every incoming native path."""
import pytest
from tools.script_recovery.test_native_resource_cfg import Code, proof


def reads(code, **options):
    return {site: row for site,row in proof(code, **options).items()
            if row.get('kind') == 'resource_vtable_load'}


def test_table_reads_are_opt_in_and_keep_existing_calls_identical():
    code=Code();code.emit('8bf1 8d7e34 8bcf 8b07 8d542410 52 ff5030 c3')
    normal=proof(code)
    extended=proof(code,include_resource_table_loads=True)
    assert not reads(code)
    assert {site: row for site,row in extended.items() if row.get('kind')!='resource_vtable_load'}==normal
    assert reads(code,include_resource_table_loads=True)=={
        0x1007: dict(family='self',offset=0x34,kind='resource_vtable_load',
                     receiverRegister='edi',tableRegister='eax')}


@pytest.mark.parametrize('load',['668b07','8b4704','648b07','8b0437'])
def test_wrong_width_offset_segment_or_index_has_no_table_fact(load):
    code=Code();code.emit('8bf1 8d7e34 '+load+' c3')
    assert not reads(code,include_resource_table_loads=True)


@pytest.mark.parametrize('alternative',['','8d7e38','bf00000000','66bf0000','8937'])
def test_resource_disagreement_at_join_has_no_table_fact(alternative):
    code=Code();code.emit('8bf1 85c0');code.jump('74','alternative')
    code.emit('8d7e34');code.jump('eb','join');code.label('alternative')
    code.emit(alternative);code.label('join');code.emit('8b07 c3')
    assert not reads(code,include_resource_table_loads=True)


def test_identical_resources_at_join_keep_table_fact():
    code=Code();code.emit('8bf1 85c0');code.jump('74','alternative')
    code.emit('8d7e34');code.jump('eb','join');code.label('alternative')
    code.emit('8d7e34');code.label('join');code.emit('8b07 c3')
    assert len(reads(code,include_resource_table_loads=True))==1
