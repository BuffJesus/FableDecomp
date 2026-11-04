"""Generated initialization copies definitions; readers retain the owned values."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_book_reaction_lowering import generated
from types import SimpleNamespace
from tools.script_recovery.native_string_vectors import lower_string_vectors


def lowering_spec():
    return SimpleNamespace(entity=False, native_address=0xe54990,
        call_labels={'ctor': 1, 'resize': 2, 'dtor': 3, 'assign': 4},
        parent_string_snapshots={0x94: {
            'storage': 'questStringVector', 'element': 'CCharString',
            'name': 'BookDef', 'global': '0x4d4',
            'initialization': {'functions': ['0x00e54990'], 'stride': 4,
                'emptyConstructor': '0x1', 'resize': '0x2',
                'destructor': '0x3', 'assign': '0x4'}}})


RESIZE = '  temp = ctor((CCharString *)&slot);\n  resize((vector *)(this + 0x94), count, temp);\n  dtor((CCharString *)&slot);'
ASSIGN = 'assign((CCharString *)(*(int *)(this + 0x94) + index * 4), (CCharString_bv *)(*(int *)(DAT_0143e90c + 0x4d4) + index * 4));'


def test_exact_evidenced_operations_lower():
    assert lower_string_vectors(RESIZE, lowering_spec()).strip() == 'GSI->StateStringVectorResize("BookDef", count);'
    assert lower_string_vectors(ASSIGN, lowering_spec()) == 'GSI->SetStateStringVectorAt("BookDef", index, ENGINE_GlobalGameDataStringAt(0x4d4, index));'


@pytest.mark.parametrize('source', [
    RESIZE.replace('&slot);', '&other);', 1),
    RESIZE.replace('count, temp', 'count, other'),
    RESIZE.replace('  resize', '  mutate(&slot);\n  resize'),
    RESIZE.replace('  dtor', '  if (flag) {}\n  dtor'),
    RESIZE.replace('0x94', '0x98'),
    ASSIGN.replace('0x4d4', '0x4d8'),
    ASSIGN.replace('index * 4));', 'other * 4));'),
    ASSIGN.replace('* 4', '* 8'),
])
def test_ambiguous_operands_remain_native(source):
    assert lower_string_vectors(source, lowering_spec()) == source


@pytest.mark.parametrize('attribute,value', [('entity', True), ('native_address', 0xe57530)])
def test_wrong_owner_or_function_remains_native(attribute, value):
    spec = lowering_spec()
    setattr(spec, attribute, value)
    assert lower_string_vectors(RESIZE + '\n' + ASSIGN, spec) == RESIZE + '\n' + ASSIGN


@pytest.mark.parametrize('definitions', [[], ['A'], ['', 'A', 'A', 'Z']])
def test_init_snapshot_survives_global_replacement_and_reinitializes(generated, definitions):
    _, roots, teachers = generated
    for root, teacher in zip(roots, teachers):
        master, entity = LuaRuntime(), LuaRuntime()
        master.execute(root)
        entity.execute(teacher)
        state, strings, flags, observed = {}, {}, {}, []
        global_values = list(definitions)
        def resize(_, name, size):
            strings[name] = (strings.get(name, []) + [''] * size)[:size]
        def read(_, name, index):
            assert name == 'BookDef'
            return strings[name][index]
        def possession(_, name, hero):
            assert hero == 'hero'
            observed.append(name)
            return False
        methods = {
            'SnapshotGlobalConversations': lambda *_: None,  # BookDef is the subject of this fixture.
            'SetStateInt': lambda _, k, v: state.__setitem__(k, v),
            'SetStateBool': lambda _, k, v: state.__setitem__(k, v),
            'GetStateInt': lambda _, k: state.get(k, 0),
            'ReadGlobalGameData': lambda _, off: len(global_values) if off == 0x4ec else 0,
            'ReadGlobalGameDataFloat': lambda *_: 0,
            'ReadGlobalGameDataStringAt': lambda _, off, i: global_values[i],
            'StateStringVectorResize': resize,
            'SetStateStringVectorAt': lambda _, k, i, v: strings[k].__setitem__(i, v),
            'GetStateStringVectorAt': read,
            'StateFlagVectorResize': lambda _, k, n: flags.__setitem__(k, [False] * n),
            'SetStateFlagVectorAt': lambda _, k, i, v: flags[k].__setitem__(i, v),
            'RetailResources': lambda _: entity.table(),
            'GetHero': lambda _: 'hero',
            'IsObjectInThingsPossession': possession,
            'IsActiveThreadTerminating': lambda _: True,
        }
        mq, eq = master.table_from(methods), entity.table_from(methods)
        master.globals().Init(mq)
        assert strings['BookDef'] == definitions
        global_values[:] = ['REPLACED'] * len(definitions)
        entity.globals().Init(eq, 'teacher')
        lookup = entity.globals().LookForBook or entity.globals().helper_E57530
        lookup(eq, 'teacher')
        assert observed == definitions
        # A second native Init overwrites every retained entry, then a smaller
        # Init destroys the tail; neither reads stale entries from the old copy.
        for replacement in [['NEW', '', 'NEW'], ['SHORT'], []]:
            global_values[:] = replacement
            master.globals().Init(mq)
            assert strings['BookDef'] == replacement
            assert state['BooksInGame'] == len(replacement)
            observed.clear()
            lookup(eq, 'teacher')
            assert observed == replacement
