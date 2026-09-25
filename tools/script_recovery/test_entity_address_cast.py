import unittest

from tools.script_recovery.annotate_interface_slots import annotate, load_thing_slots
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, strip_receiver_arguments


class EntityAddressCastTests(unittest.TestCase):
    def convert(self, source, entity=True):
        slots = load_thing_slots()
        unit = {'quest': {'fields': {}}, 'entities': {'E': {'fields': {}}}}
        spec = LoweringSpec(unit, 'E', entity=entity, thing_slots=slots)
        lowered, _ = lower(source, spec)
        return annotate(lowered, {}, slots, entity=entity)

    def test_wrong_string_cast_preserves_own_thing_through_saved_alias(self):
        source = ('p0 = (CCharString)(this + 8);\n'
                  'xStack_24 = p0;\n'
                  'p = (**(code **)(*(int *)p0 + 0x18))((void *)p0);\n'
                  'p0 = xStack_24;\n'
                  'p = (**(code **)(*(int *)p0 + 0x18))((void *)p0);\n')
        self.assertEqual(self.convert(source).count('CScriptThing::GetPos('), 2)

    def test_other_offset_and_quest_layout_are_not_entity_receivers(self):
        for offset, entity in [(12, True), (8, False)]:
            with self.subTest(offset=offset, entity=entity):
                source = (f'p0 = (CCharString)(this + {offset});\n'
                          'p = (**(code **)(*(int *)p0 + 0x18))((void *)p0);\n')
                self.assertNotIn('CScriptThing::GetPos(', self.convert(source, entity))

    def test_scalar_redefinition_ends_entity_alias(self):
        source = ('p0 = (CCharString)(this + 8);\n'
                  'p0 = 4;\n'
                  'p = (**(code **)(*(int *)p0 + 0x18))((void *)p0);\n')
        self.assertNotIn('CScriptThing::GetPos(', self.convert(source))

    def test_void_cast_receiver_does_not_take_the_ability_argument(self):
        source = ('p0 = (CCharString)(this + 8);\n'
                  'b = (**(code **)(*(int *)p0 + 0xa4))((void *)p0,0xe,"SCRIPT_NAME_HERO");\n')
        result = strip_receiver_arguments(self.convert(source))
        self.assertIn('CScriptThing::MsgIsHitBySpecialAbilityFrom((CScriptThing *)(this + 8), 0xe,"SCRIPT_NAME_HERO")', result)
        self.assertNotIn('(void *)p0', result)


if __name__ == '__main__':
    unittest.main()
