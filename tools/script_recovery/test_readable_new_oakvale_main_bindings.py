import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import RAW
from tools.script_recovery.readable_new_oakvale_main_bindings import lower
from tools.script_recovery.lift_native_lua import RData


class MainBindingTests(unittest.TestCase):
    def test_dead_father_binding_and_post_attack_zero_survive_startup(self):
        source = (RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        source, report = lower(source)
        self.assertEqual(report['bindingNames'][-1], 'OVI_DeadFather')
        for attack, terminate in ((False, False), (True, False), (True, True)):
            lua = LuaRuntime(); lua.execute(source)
            lua.globals().attack = attack; lua.globals().terminate = terminate
            lua.execute('''
                local bindings, finalized, deactivated, continued = {}, false, false, false
                local quest = {
                    AddEntityBinding=function(_, name, path)
                        assert(not finalized and path=="NewOakValeIntro/Entities/"..name)
                        bindings[#bindings+1]=name
                    end,
                    FinalizeEntityBindings=function() finalized=true end,
                    GetStateBool=function(_, name) assert(finalized and name=="AttackOver");return attack end,
                    IsActiveThreadTerminating=function() return terminate end,
                    DeactivateQuest=function(_, name, value)
                        assert(name=="Q__OakValeIntro_PostAttack" and value==0);deactivated=true
                    end,
                    GetActiveQuestName=function()return 'quest' end,
                    SetQuestCardObjective=function()end,
                    CreateThread=function(_, name)assert(name=='StartBarrelTimer')end,
                }
                DoMission=function()continued=true end
                Main(quest)
                assert(#bindings==16 and bindings[16]=='OVI_DeadFather')
                assert(deactivated==(attack and not terminate))
                assert(continued==not(attack and terminate))
            ''')

    def test_binding_correspondence_changes_fail_closed(self):
        source = (RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        with self.assertRaises(ValueError):lower(source.replace('NOVI_CreatedBeetle', 'MissingBeetle'))
        # the binding block now lifts by itself; the deactivation operand is the remaining draft-shape witness
        with self.assertRaises(ValueError):lower(source.replace('Q__OakValeIntro_PostAttack", nil', 'Q__OakValeIntro_PostAttack", 7'))
        class Changed(RData):
            def bytes_at(self, address, size):
                raw = super().bytes_at(address, size)
                if address <= 0xdac119 < address+size:
                    raw=bytearray(raw);raw[0xdac119-address]^=1;raw=bytes(raw)
                return raw
        with self.assertRaises(ValueError):lower(source, Changed())


if __name__ == '__main__':unittest.main()
