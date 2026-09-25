"""Combat assists must preserve real fights and avoid region-exit volumes."""
from pathlib import Path
import unittest

from lupa.lua54 import LuaRuntime


class CombatTests(unittest.TestCase):
    def setUp(self):
        self.lua = LuaRuntime()
        self.lua.execute('''
            function actor(x, y, hp)
                local a = {pos={x=x,y=y,z=40}, hp=hp}
                function a:GetPos() return self.pos end
                function a:IsAlive() return true end
                function a:IsNull() return false end
                function a:IsUnconscious() return false end
                function a:IsEqualTo(b) return self == b end
                function a:GetDefName() return 'BANDIT' end
                function a:GetCurrentMapName() return 'OrchardFarm' end
                function a:FadeOutAndKillEntity() error('unexpected fade') end
                return a
            end
            hero = actor(3210,3200,100)
            targets = {}
            drains = 0
            quest = {}
            function quest:GetHero() return hero end
            function quest:GetAllThingsWithScriptName() return targets end
            function quest:GetAllCreaturesInAreaWithScriptName() return targets end
            function quest:GetHealth(a) return a.hp end
            function quest:ModifyThingHealth(a) drains=drains+1; a.hp=1 end
            function quest:GetDistanceBetweenThings(a,b)
                return math.sqrt((a.pos.x-b.pos.x)^2+(a.pos.y-b.pos.y)^2)
            end
            function quest:EntityTeleportToPosition(a,p) landing=p end
            function quest:EntitySetFacingAngleTowardsThing() end
            function quest:AreEntitiesEnemies() return true end
            function quest:GetFollowingEntityList() return {} end
            function quest:IsInCutscene() return false end
            function quest:IsInMovieSequence() return false end
            function quest:GetHeroHealthPercentage() return 1 end
        ''')
        self.lua.execute(Path(__file__).with_name('ingame_runner.lua').read_text(encoding='utf-8'))
        self.lua.execute("Runner.fightNames={'BanditTeamMember'}; Runner.fightBounds={3202,3170,3293,3293}")

    def test_spawn_in_exit_volume_is_not_approached(self):
        self.lua.execute('targets={actor(3301,3292,20)}')
        self.assertEqual(self.lua.eval('Runner.fightStep(quest)'), 'none')
        self.assertIsNone(self.lua.globals().landing)
        self.assertEqual(self.lua.globals().drains, 0)

    def test_landing_stays_inside_bounds_even_when_hero_is_outside(self):
        self.lua.execute('hero.pos.x=3305; hero.pos.y=3200; targets={actor(3292,3200,20)}')
        self.lua.eval('Runner.fightStep(quest)')
        self.assertLess(self.lua.globals().landing.x, 3293)

    def test_knocked_back_bandit_is_attacked_from_inside_boundary(self):
        self.lua.execute('Runner.fightDrain=false; targets={actor(3295,3277.2,5)}')
        self.assertNotEqual(self.lua.eval('Runner.fightStep(quest)'), 'none')
        self.assertLess(self.lua.globals().landing.x, 3293)
        self.assertEqual(self.lua.globals().drains, 0)

    def test_real_damage_mode_never_drains_in_fight_or_area_clear(self):
        self.lua.execute('Runner.fightDrain=false; targets={actor(3250,3200,20)}')
        self.lua.eval('Runner.fightStep(quest)')
        self.lua.execute('Runner.frame=7; Runner.tick(quest)')
        self.assertEqual(self.lua.globals().drains, 0)
        self.assertEqual(self.lua.eval('targets[1].hp'), 20)

    def test_default_assist_leaves_one_health_for_real_finishing_hit(self):
        self.lua.execute('targets={actor(3250,3200,20)}')
        self.lua.eval('Runner.fightStep(quest)')
        self.assertEqual(self.lua.globals().drains, 1)
        self.assertEqual(self.lua.eval('targets[1].hp'), 1)


if __name__ == '__main__':
    unittest.main()
