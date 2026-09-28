"""Member string vectors Init fills from wide literals become immutable Lua tables (V_ChickenKicking ChickenSign)."""
from tools.script_recovery import native_literal_string_vectors as lsv

INIT = '''void __thiscall FUN_00e68dc0(void *this)\r
\r
{\r
  vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_> *this_00;\r
  int p1;\r
  void *pvStack_4;\r
  \r
  this_00 = (vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_> *)\r
            ((int)this + 0x1c);\r
  pvStack_4 = this;\r
  p1 = CCharString::CCharString((CCharString *)&pvStack_4);\r
  std::vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_>::resize\r
            (this_00,3,p1);\r
  CCharString::~CCharString((CCharString *)&pvStack_4);\r
  CCharString__AssignFromWide(*(void **)this_00,0x100);\r
  CCharString__AssignFromWide((void *)(*(int *)this_00 + 4),0x200);\r
  CCharString__AssignFromWide((void *)(*(int *)this_00 + 8),0x300);\r
  return;\r
}\r
'''
MAIN = '''  pCVar5 = (CWideString_bv *)(*(int *)(this + 0x1c) + *(int *)(*(int *)(this + 0x14) + 0x5c) * 4);\r
  (**(code **)(iVar2 + 0xb24))(*(void **)(this + 4),pCVar4,pCVar5);\r
'''
FIELDS = [{'offset': '0x1c', 'type': 'vector<CWideString,std::allocator<CWideString>_>', 'name': 'TextKeys'}]
WIDE = {0x100: 'A', 0x200: 'B', 0x300: 'C "q"'}.get


def test_literal_vector_becomes_a_table_and_reads_index_it():
    found = lsv.recover(FIELDS, {'Init': INIT, 'Main': MAIN}, WIDE)
    assert list(found) == [0x1c] and found[0x1c][:2] == ('TextKeys', ['A', 'B', 'C "q"'])
    init = lsv.apply(INIT, 'Init', found)
    assert 'resize' not in init and 'AssignFromWide' not in init and 'pvStack_4 = this' not in init
    assert 'pCVar5 = LOCALLIST_StringAt(TextKeys, *(int *)(*(int *)(this + 0x14) + 0x5c));' in lsv.apply(MAIN, 'Main', found)
    assert 'local TextKeys = {"A", "B", "C \\"q\\""}' in lsv.prelude(found, 'CChickenSign')


def test_any_other_use_of_the_member_keeps_the_source():
    written = MAIN + '  *(int *)(this + 0x1c) = 0;\r\n'
    assert lsv.recover(FIELDS, {'Init': INIT, 'Main': written}, WIDE) == {}


def test_a_missing_slot_or_unresolved_literal_keeps_the_source():
    partial = INIT.replace('  CCharString__AssignFromWide((void *)(*(int *)this_00 + 8),0x300);\r\n', '')
    assert lsv.recover(FIELDS, {'Init': partial, 'Main': MAIN}, WIDE) == {}
    assert lsv.recover(FIELDS, {'Init': INIT, 'Main': MAIN}, {0x100: 'A'}.get) == {}
