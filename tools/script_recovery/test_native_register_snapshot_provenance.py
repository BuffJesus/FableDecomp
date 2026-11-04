import pytest
from tools.script_recovery.native_register_snapshot_provenance import analyze

def final(code,reg='ebx'):
    raw=bytes.fromhex(code+' c3');return analyze(raw,0x1000)['states'][0x1000+len(raw)-1].get(reg)

def test_memory_write_retains_snapshot_bits():
    assert final('89 ce 8b 5e 04 c7 06 00 00 00 00')==('interface_snapshot',0x1002)

def test_call_preserves_saved_register_only():
    code='89 ce 8b 5e 04 89 d8 e8 00 00 00 00'
    assert final(code)==('interface_snapshot',0x1002)
    assert final(code,'eax') is None

@pytest.mark.parametrize('write',['b3 00','66 bb 00 00','31 db','5b','8b 5c 24 14'])
def test_partial_full_and_stack_reload_destroy_origin(write):
    assert final('89 ce 8b 5e 04 '+write) is None

def test_bypass_join_does_not_inherit_load():
    assert final('89 ce 85 c0 74 03 8b 5e 04') is None

def test_different_load_sites_do_not_merge_snapshots():
    assert final('89 ce 85 c0 74 05 8b 5e 04 eb 03 8b 5e 04') is None

def test_table_snapshot_remembers_pointer_load_and_read_sites():
    assert final('89 ce 8b 5e 04 8b 03','eax')==('table_snapshot',0x1002,0x1005)

def test_wrong_field_is_unknown():
    assert final('89 ce 8b 5e 08') is None

@pytest.mark.parametrize('raw',['ff e0','eb 7f','8b'])
def test_unsupported_cfg_fails_closed(raw):
    with pytest.raises(ValueError):analyze(bytes.fromhex(raw),0x1000)

@pytest.mark.parametrize('raw',['cd80c3','ccc3','cec3','cfc3','0f34c3','0f35c3','0f05c3','0f07c3','f4c3','0f0bc3','cbc3'])
def test_unsupported_system_and_far_transfers_reject(raw):
    with pytest.raises(ValueError):analyze(bytes.fromhex(raw),0x1000)


from tools.script_recovery import native_register_snapshot_provenance as P

def state(code,**options):
 raw=bytes.fromhex(code+' c3');return P.analyze(raw,0x1000,**options)['states'][0x1000+len(raw)-1]

def test_explicit_parent_pointer_keeps_load_site_identity_at_bool_store():
 result=state('89 ce 8b 46 14 c6 40 54 01',parent_offsets=(0x14,))
 assert result['eax']==('parent_snapshot',0x14,0x1002)
 assert 'ecx' in result and result['ecx']==('entry_this',)

def test_parent_pointer_is_not_a_resource_table_profile():
 result=state('89 ce 8b 46 14 8b 48 54',parent_offsets=(0x14,))
 assert result['eax']==('parent_snapshot',0x14,0x1002) and 'ecx' not in result

def test_parent_pointer_alone_does_not_invent_member_address_or_table():
 for tail,reg in [('8d 58 54','ebx'),('89 c3 83 c3 54','ebx'),('8b 58 54','ebx')]:
  result=state('89 ce 8b 46 14 '+tail,parent_offsets=(0x14,))
  assert reg not in result

@pytest.mark.parametrize('reg_opcode,reg,kept',[('8b 5e 14','ebx',True),('8b 46 14','eax',False)])
def test_call_only_keeps_parent_pointer_in_callee_saved_register(reg_opcode,reg,kept):
 result=state('89 ce '+reg_opcode+' e8 00 00 00 00',parent_offsets=(0x14,))
 assert (result.get(reg)==('parent_snapshot',0x14,0x1002)) is kept

def test_different_parent_reload_sites_do_not_merge():
 assert 'ebx' not in state('89 ce 85 c0 74 05 8b 5e 14 eb 03 8b 5e 14',parent_offsets=(0x14,))

def test_new_parent_profile_does_not_change_default_mode():
 raw=bytes.fromhex('89 ce 8b 46 14 c6 40 54 01 c3')
 result=P.analyze(raw,0x1000)
 assert 'eax' not in result['states'][0x1009]

def test_empty_new_profile_does_not_consume_existing_member_offset_iterator():
 raw=bytes.fromhex('89 ce 8d 5e 08 c3')
 result=P.analyze(raw,0x1000,member_offsets=iter((8,)))
 assert result['states'][0x1005]['ebx']==('member_address',8)

def test_explicit_parent_and_existing_member_iterator_both_keep_their_origins():
 raw=bytes.fromhex('89 ce 8d 5e 08 8b 46 14 c3')
 result=P.analyze(raw,0x1000,member_offsets=iter((8,)),parent_offsets=(0x14,))['states'][0x1008]
 assert result['ebx']==('member_address',8) and result['eax']==('parent_snapshot',0x14,0x1005)

@pytest.mark.parametrize('offsets',[True,20,'0x14',[True],[0],[-4],[0x15],[4096],[0x14,0x14],[4]])
def test_invalid_or_conflicting_parent_offsets_refused(offsets):
 with pytest.raises(ValueError,match='parent pointer offsets'):state('89 ce 8b 46 14',parent_offsets=offsets)

@pytest.mark.parametrize('other',[dict(member_offsets=(0x14,)),dict(parent_resource_profiles=((0x14,(0x40,)),))])
def test_parent_pointer_profile_cannot_override_an_existing_member_profile(other):
 with pytest.raises(ValueError,match='parent pointer offsets'):state('89 ce 8b 46 14',parent_offsets=(0x14,),**other)

