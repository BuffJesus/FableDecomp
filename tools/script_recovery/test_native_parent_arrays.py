import json
import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import load_entity_parent_state
from tools.script_recovery.test_lift_native_lua import make


class ParentArrayTests(unittest.TestCase):
    def test_unresolved_reassignment_invalidates_parent_alias(self):
        for replacement in ('FUN_00123456()', '&local_40', 'DAT_00123456'):
            with self.subTest(replacement=replacement):
                lifter = make(entity=True)
                lifter.parent_state = {'0x58': ('BadDeedsPerformed', 'Int')}
                source = ('{\niVar8 = *(int *)(this + 0x14);\n'
                          f'iVar8 = {replacement};\n'
                          'iVar12 = *(int *)(iVar8 + 0x58);\n}')
                body = '\n'.join(lifter.lift('Main', source))
                self.assertNotIn('GetStateInt("BadDeedsPerformed")', body)
                self.assertNotIn('iVar8', lifter.parent_aliases)

    def test_nested_branch_invalidation_reaches_outer_join(self):
        lifter = make(entity=True)
        lifter.parent_state = {'0x58': ('BadDeedsPerformed', 'Int')}
        source = ('{\niVar8 = *(int *)(this + 0x14);\n'
                  'if (outer) {\nif (inner) {\niVar8 = 7;\n}\n'
                  'else {\niVar12 = *(int *)(iVar8 + 0x58);\nreturn iVar12;\n}\n}\n'
                  'else {\niVar12 = *(int *)(iVar8 + 0x58);\nreturn iVar12;\n}\nreturn 0;\n}')
        body = '\n'.join(lifter.lift('Main', source))
        lua = LuaRuntime()
        quest = lua.table_from({'GetStateInt': lambda _q, name: 3})
        run = lua.execute('return function(quest,outer,inner)\n' + body + '\nend')
        for outer, inner, expected in ((True, True, 0), (True, False, 3), (False, True, 3), (False, False, 3)):
            self.assertEqual(run(quest, outer, inner), expected)
        self.assertNotIn('iVar8', lifter.parent_aliases)
        self.assertEqual(lifter.todo, [])

    def test_sibling_branch_restores_parent_alias_but_join_does_not_assume_it(self):
        for alternative in ('else {', 'else if (*(int *)(iVar8 + 0x58) > 0) {'):
            lifter = make(entity=True)
            lifter.parent_state = {'0x58': ('BadDeedsPerformed', 'Int')}
            source = '{\niVar8 = *(int *)(this + 0x14);\nif (choose) {\niVar8 = 7;\n}\n' + alternative + '\niVar12 = *(int *)(iVar8 + 0x58);\nreturn iVar12;\n}\nreturn 0;\n}'
            body = '\n'.join(lifter.lift('Main', source))
            lua = LuaRuntime()
            q = lua.table_from({'GetStateInt': lambda _q, name: 3})
            run = lua.execute('return function(quest,choose)\n' + body + '\nend')
            self.assertEqual(run(q, False), 3)
            self.assertEqual(run(q, True), 0)
            self.assertNotIn('iVar8', lifter.parent_aliases)
            self.assertEqual(lifter.todo, [])

    def test_parent_alias_reads_shared_state_and_expires_on_reassignment(self):
        lifter = make(entity=True)
        lifter.parent_state = {'0x58': ('BadDeedsPerformed', 'Int')}
        source = '{\niVar8 = *(int *)(this + 0x14);\niVar12 = *(int *)(iVar8 + 0x58);\nreturn iVar12;\n}'
        body = '\n'.join(lifter.lift('Main', source))
        lua = LuaRuntime()
        q = lua.table_from({'GetStateInt': lambda _q, name: 3 if name == 'BadDeedsPerformed' else None})
        self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(q), 3)
        self.assertEqual(lifter.todo, [])
        lifter.statement('Main', 'iVar8 = 9;')
        self.assertNotIn('iVar8', lifter.parent_aliases)

    def test_parent_alias_does_not_escape_conditional_scope(self):
        lifter = make(entity=True)
        lifter.parent_state = {'0x58': ('BadDeedsPerformed', 'Int')}
        source = '{\nif (choose) {\niVar8 = *(int *)(this + 0x14);\n}\niVar12 = *(int *)(iVar8 + 0x58);\n}'
        body = '\n'.join(lifter.lift('Main', source))
        self.assertNotIn('GetStateInt("BadDeedsPerformed")', body)

    def test_guard_array_uses_same_keys_as_quest_deed_helpers(self):
        fields = load_entity_parent_state('NOVI_Guard')
        lifter = make(entity=True)
        lifter.parent_state = fields
        lua, values = LuaRuntime(), {f'WhichBadDeedsPerformed_{i}': False for i in range(5)}
        quest = lua.table_from({'GetStateBool': lambda _q, name: values[name]})
        for index in range(5):
            offset = hex(0xfc + index)
            self.assertEqual(fields[offset], (f'WhichBadDeedsPerformed_{index}', 'Bool'))
            expression = lifter.expr(f'*(char *)(*(int *)(this + 0x14) + {offset}) != 0')
            run = lua.execute('return function(quest) return ' + expression + ' end')
            self.assertIs(run(quest), False)
            values[f'WhichBadDeedsPerformed_{index}'] = True
            self.assertIs(run(quest), True)
        self.assertEqual(lifter.todo, [])

    def test_array_requires_matching_owner_declaration_and_byte_extent(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            inventories = root / 'entities'
            inventories.mkdir()
            inventory = {'nativeClass': 'NScript::CQuestScript::Actor', 'parentFieldsRead': ['Flags[0..4]']}
            field = {'name': 'Flags', 'type': 'bool[5]', 'retailOffset': '0xfc..0x100'}
            def load(changes=None, declaration=None, owner=None):
                (root / 'persistence_manifest.json').write_text(json.dumps({'script': 'Quest', 'fields': [dict(field, **(changes or {}))]}))
                payload = dict(inventory)
                if declaration is not None:
                    payload['parentFieldsRead'] = declaration
                if owner is not None:
                    payload['nativeClass'] = owner
                (inventories / 'Actor.json').write_text(json.dumps(payload))
                return load_entity_parent_state('Actor', inventories)
            self.assertEqual(len(load()), 5)
            self.assertEqual(load({'retailOffset': '0xfc..0x101'}), {})
            self.assertEqual(load({'type': 'int[5]'}), {})
            self.assertEqual(load(declaration=['Flags[0..3]']), {})
            self.assertEqual(load(owner='NScript::COtherScript::Actor'), {})


if __name__ == '__main__':
    unittest.main()
