"""Composition and outer lifetime checks. Full native dispatcher test still pending."""
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime


class BarrelThugMainBodyTests(unittest.TestCase):
    def test_composed_helpers_skip_intro_and_cancel_at_each_outer_query(self):
        folder=Path(__file__).parent
        modules=('intro_prepare','intro','talk_body','conversation','timed_remarks','hit','main_body')
        source='\n'.join((folder/f'barrel_thug_{name}.lua').read_text().rsplit('\nreturn ',1)[0] for name in modules)
        for cancel in range(1,5):
            with self.subTest(cancel=cancel):
                lua=LuaRuntime();lua.globals().main=lua.execute(source+'\nreturn runBarrelThugMainAfterCondition')
                lua.globals().cancel=cancel
                lua.execute('''
                    local events, terms = {}, 0
                    local function event(e) events[#events+1]=e end
                    local quest = {
                        NewScriptFrame=function() event("frame") end,
                        IsActiveThreadTerminating=function()
                            terms=terms+1; event("term"); return terms==cancel
                        end,
                        GetStateBool=function(_,key)
                            assert(key=="BarrelManSpokenToHeroOnReturn");event("returned");return true
                        end,
                    }
                    local state = {GetStateBool=function(_,key) assert(key=="DoneIntro");event("done");return true end}
                    local resources = {
                        NewResource=function() event("new");return 16 end,
                        ReleaseResource=function(_,id) assert(id==16);event("close") end,
                        WasVillagerTalkedTo=function(_,actor) assert(actor==17);event("talk");return false end,
                        ResetResource=function(_,id) assert(id==16);event("reset") end,
                        IsHitByHeroExceptAbility=function(_,actor,ability)
                            assert(actor==17 and ability==14);event("hit");return false
                        end,
                    }
                    main(quest,17,resources,state)
                    local expected = {
                        "frame,term",
                        "frame,term,new,term,close",
                        "frame,term,new,term,done,talk,returned,term,close",
                        "frame,term,new,term,done,talk,returned,term,reset,hit,frame,term,close",
                    }
                    assert(table.concat(events,",")==expected[cancel])
                ''')

    def test_phase_error_releases_outer_control_and_preserves_error(self):
        lua=LuaRuntime()
        lua.execute('introduceBarrelThug=function() error("phase failure",0) end')
        lua.globals().main=lua.execute(Path(__file__).with_name('barrel_thug_main_body.lua').read_text())
        lua.execute('''
            local closes=0
            local ok,err=pcall(main,
                {NewScriptFrame=function() end,IsActiveThreadTerminating=function() return false end},17,
                {NewResource=function() return 16 end,ReleaseResource=function(_,id)
                    assert(id==16);closes=closes+1;error("close failure",0)
                end}, {GetStateBool=function() return false end})
            assert(not ok and err=="phase failure" and closes==1)
        ''')
