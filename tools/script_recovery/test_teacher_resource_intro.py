"""Teacher introduction must speak and poll its acquired resource before cleanup."""
import os
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

ROOT=Path(__file__).resolve().parents[2]


@pytest.mark.parametrize('stage',['draft','readable'])
@pytest.mark.parametrize('exposed',[False,True])
@pytest.mark.parametrize('health',[10,0,-1])
@pytest.mark.parametrize('abort',[False,True])
def test_teacher_intro_receiver_speech_wait_and_movie(stage,exposed,health,abort):
    root=Path(os.environ.get('TEACHER_INTRO_ARTIFACTS',ROOT/'refs/script_recovery/lifted/BookCollecting'))
    lua=LuaRuntime()
    lua.execute("package.preload['V_BookCollecting.native_quest_helpers']=function() return {AddGossip=function() end,helper_E55C60=function() end} end")
    lua.execute((root/stage/'FSE/V_BookCollecting/Entities/BS_Teacher.lua').read_text())
    lua.globals().exposed=exposed;lua.globals().health=health;lua.globals().abort=abort
    lua.execute('''
        me,hero,quest,resources,member,movie={},{},{},{},{},{}
        local state={BooksInGame=0,BooksDonated=0,GossipState=0,DoneIntro=false,HasInformation=false}
        local spoken,polls,frames,attempts,created,completed,destroyed=0,0,0,0,0,0,0
        local stop={}
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:ReadGlobalGameData() return 20 end
        function quest:ReadGlobalGameDataFloat() return 0 end
        function quest:GetStateInt(k) assert(state[k]~=nil,k); return state[k] end
        function quest:SetStateInt(k,v) state[k]=v end
        function quest:GetStateBool(k) return state[k] or false end
        function quest:SetStateBool(k,v)
            if k=='DoneIntro' then
                assert(v and (health<=0 or (spoken==1 and polls==3 and frames==2)))
                completed=completed+1
            end
            state[k]=v
        end
        function quest:SetThingPersistent(v,b) assert(v==me and b) end
        function quest:GetMasterGameState(k) return k=='HeroExposedLadyGrey' and exposed end
        function quest:GetTimeOfDay() return 1200 end
        function quest:IsActiveThreadTerminating() return abort and frames>0 end
        function quest:NewScriptFrame(v)
            assert(v==me); if spoken>0 then frames=frames+1 end; assert(frames<4)
            return not self:IsActiveThreadTerminating()
        end
        function quest:SetThingHasInformation(v) assert(v==me) end
        function me:IsTalkedToByHero() return true end
        function resources:StartMovie(key) assert(key==''); created=created+1; return movie end
        function quest:PauseAllNonScriptedEntities(v) paused=v end
        function resources:MemberResource(key,owner) assert(key=='seh_me' and owner==me); return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,actor,priority)
            assert(v==member and actor==me and priority==4);attempts=attempts+1;return attempts>1
        end
        function resources:ScriptThing(v) assert(v==member); return me end
        function quest:GetHealth(v) assert(v==me);return health end
        function me:Speak(actor,key,method,a,b,c)
            assert(actor==hero and key==(exposed and 'TEXT_QST_B16_INTRO_B' or 'TEXT_QST_B16_INTRO'))
            assert(method==0 and a==false and b==true and c==false);spoken=spoken+1;return true
        end
        function resources:IsPerformingScriptTask(v) assert(v==member);polls=polls+1;return polls<3 end
        function quest:GetActiveQuestName() return 'V_BookCollecting' end
        function quest:GiveHeroQuestCardDirectly(card,q,b)
            assert(card=='OBJECT_QUEST_CARD_BOOK_COLLECTION' and q=='V_BookCollecting' and b==false)
        end
        function quest:SetQuestCardObjective(q,key,a,b)
            assert(q=='V_BookCollecting' and key=='TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01' and a=='' and b=='')
        end
        function resources:DestroyMovie(v)
            assert(v==movie and paused==false and completed==((abort and health>0) and 0 or 1))
            destroyed=destroyed+1;error(stop)
        end
        Init(quest,me)
        local ok,err=pcall(Main,quest,me)
        assert(not ok and err==stop,tostring(err))
        assert(created==1 and destroyed==1 and attempts==2)
        assert(completed==((abort and health>0) and 0 or 1))
        if abort and health>0 then assert(frames==1 and polls==1) end
        assert(spoken==(health>0 and 1 or 0))
    ''')
