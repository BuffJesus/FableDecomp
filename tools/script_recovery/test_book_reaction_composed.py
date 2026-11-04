import hashlib
import json
from pathlib import Path
import sys
import unittest
import subprocess
import tempfile
from functools import lru_cache
from lupa.lua54 import LuaRuntime

W=Path(__file__).resolve().parent
R=W.parents[1]
sys.path.insert(0,str(R))


@lru_cache(maxsize=1)
def generated_sources():
    from tools.script_recovery.build_readable_unit import build
    with tempfile.TemporaryDirectory(prefix='book_conversation_', dir=R/'work') as temporary:
        out=Path(temporary)
        subprocess.run([sys.executable,'-m','tools.script_recovery.convert_quest_unit','--unit','book_collecting','--out',str(out/'draft')],cwd=R,check=True,capture_output=True,text=True)
        build('book_collecting',draft=out/'draft',out=out/'readable')
        return {stage:(out/stage/'FSE/V_BookCollecting/V_BookCollecting.lua').read_text() for stage in ['draft','readable']}


def lua_execute(style,speaker='boy0',dialogue='TEXT_REACTION',animation='START',loop='NULL',
                book=2,line=1,conversation_frames=2,task_frames=3,reading_at=999,terminate_at=999,initial_term=False,
                outer=False,lines=3,acquire=True):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute(generated_sources()[style])
    settings=locals().copy()
    for key in ('speaker','dialogue','animation','loop','book','line','conversation_frames','task_frames','reading_at','terminate_at','initial_term','outer','lines','acquire'):
        lua.globals()[key.upper()]=settings[key]
    quest,events=lua.execute('''
local frame,events=0,{}
local conversationStart,conversationId=0,6
local deadlines={boy=TASK_FRAMES,girl=TASK_FRAMES}
local function record(...) events[#events+1]={...} end
local hero={name='hero'}
local actor={name=SPEAKER}
local actors={boy0={name='boy0'},girl0={name='girl0'},marker_boy={name='marker_boy'},marker_girl={name='marker_girl'}}
local resources={}
local members={seh_Boy={name='boy'},seh_Girl={name='girl'}}
function resources:MemberResource(name) assert(members[name]);return members[name] end
function resources:PlayAnimation(member,name,...)
    assert(member==members.seh_Boy or member==members.seh_Girl)
    deadlines[member.name]=frame+TASK_FRAMES
    record('animation',member.name,name,...)
end
function resources:PlayLoopingAnimation(member,name,...)
    assert(member==members.seh_Boy or member==members.seh_Girl)
    record('loop',member.name,name,...)
end
function resources:IsPerformingScriptTask(member)
    assert(member==members.seh_Boy or member==members.seh_Girl)
    local active=frame<deadlines[member.name];record('task',member.name,active);return active
end
function resources:TryAcquire(member,who,priority)
    assert(member==members.seh_Boy or member==members.seh_Girl)
    record('acquire',member.name,who.name,priority,ACQUIRE);return ACQUIRE
end
function resources:ClearAllActionsIncludingLoopingAnimations(member)
    deadlines[member.name]=frame;record('clear.actions',member.name)
end
function resources:ClearCommands(member) record('clear.commands',member.name) end
function resources:PrepareResource(member) record('prepare',member.name) end
local conversation={Speaker={},Dialogue={},Animation={},AnimLoop={}}
conversation.Speaker[LINE+1]=SPEAKER
conversation.Dialogue[LINE+1]=DIALOGUE
conversation.Animation[LINE+1]=ANIMATION
conversation.AnimLoop[LINE+1]=LOOP
local definitions={[BOOK+1]=conversation}
conversation.Lines=LINES
if OUTER then
    for index=0,LINES-1 do
        conversation.Speaker[index+1]=index%2==0 and 'boy0' or 'girl0'
        conversation.Dialogue[index+1]=DIALOGUE=='NULL' and DIALOGUE or DIALOGUE..'_'..index
        conversation.Animation[index+1]=ANIMATION=='NULL' and ANIMATION or ANIMATION..'_'..index
        conversation.AnimLoop[index+1]=LOOP
    end
end
local quest={}
function quest:RetailResources() return resources end
function quest:GetHero() return hero end
function quest:GetStateConversations(name) assert(name=='BookReactions');return definitions end
function quest:GetThingWithScriptName(name)
    if OUTER then assert(actors[name]);return actors[name] end
    assert(name==SPEAKER);return actor
end
function quest:ReadGlobalGameDataStringAt(offset,index)
    assert(index==BOOK and (offset==0x4b0 or offset==0x4bc));return offset==0x4b0 and 'marker_boy' or 'marker_girl'
end
function quest:EntityTeleportToThing(who,target,flag) record('teleport',who.name,target.name,flag) end
function quest:SetIsPushableByHero(who,value) record('pushable',who.name,value) end
function quest:AddNewConversation(who,a,b)
    assert((who==actor or who==actors.boy0 or who==actors.girl0) and a==false and b==false)
    conversationId=conversationId+1;conversationStart=frame;record('new',who.name);return conversationId
end
function quest:AddPersonToConversation(id,who) assert(who==hero);record('person',id,who.name) end
function quest:AddLineToConversation(id,text,who,other,flag)
    assert((who==actor or who==actors.boy0 or who==actors.girl0) and other==hero);record('line',id,text,who.name,other.name,flag)
end
function quest:IsActiveThreadTerminating() return INITIAL_TERM or frame>=TERMINATE_AT end
function quest:NewScriptFrame() frame=frame+1;record('frame',frame);return not self:IsActiveThreadTerminating() end
function quest:IsConversationActive(id)
    local active=id~=-1 and frame-conversationStart<CONVERSATION_FRAMES;record('active',id,active);return active
end
function quest:GetStateBool(name) assert(name=='ReadingBook');return frame>=READING_AT end
function quest:RemoveConversation(id,flag) record('remove',id,flag) end
return quest,events
''')
    if outer:lua.globals().BookReaction(quest,book)
    else:
        function=lua.globals().helper_E569D0 if style=='draft' else lua.globals().DoConversation
        function(quest,book,line)
    return [tuple(v[j] for j in range(1,len(v)+1)) for v in events.values()]

CASES=[]
for speaker in ('boy0','girl0'):
    for dialogue in ('TEXT_REACTION','NULL'):
        for loop in ('NULL','IDLE',''):
            for conversation_frames,task_frames in ((0,0),(2,0),(0,3),(2,3)):
                for reading_at,terminate_at,initial_term in ((999,999,False),(1,999,False),(999,2,False),(999,999,True)):
                    CASES.append(dict(speaker=speaker,dialogue=dialogue,loop=loop,
                        conversation_frames=conversation_frames,task_frames=task_frames,
                        reading_at=reading_at,terminate_at=terminate_at,initial_term=initial_term))

OUTER_CASES=[]
for lines in (0,1,3):
    for dialogue in ('TEXT_REACTION','NULL'):
        for loop in ('NULL','IDLE'):
            for acquire in (True,False):
                for reading_at,terminate_at,initial_term in (
                    (999,999,False),(0,999,False),(1,999,False),(3,999,False),
                    (999,1,False),(999,2,False),(999,5,False),(999,999,True)):
                    OUTER_CASES.append(dict(outer=True,lines=lines,dialogue=dialogue,loop=loop,
                        acquire=acquire,reading_at=reading_at,terminate_at=terminate_at,initial_term=initial_term))

@lru_cache(maxsize=1)
def native_results():
    # Unicorn/ctypes runs in its own process on Windows; pytest hosts only Lua.
    with tempfile.TemporaryDirectory(prefix='book_reaction_native_',dir=R/'work') as owned:
        scratch=Path(owned).resolve()
        assert scratch.is_relative_to((R/'work').resolve())
        cases=scratch/'cases.json';output=scratch/'results.json'
        cases.write_text(json.dumps(CASES+OUTER_CASES),encoding='utf-8')
        result=subprocess.run([sys.executable,str(W/'book_reaction_native.py'),
            '--cases',str(cases),'--out',str(output)],cwd=R,capture_output=True,text=True)
        assert result.returncode==0,result.stdout+result.stderr
        return [[tuple(event) for event in events] for events in json.loads(output.read_text())]

class TestConversation(unittest.TestCase):
    def test_original_worker_calls_and_lifetimes(self):
        for index,case in enumerate(CASES):
            expected=native_results()[index]
            for style in ('draft','readable'):
                with self.subTest(case=case,style=style):
                    self.assertEqual(lua_execute(style,**case),expected)

    def test_composed_book_reaction_owns_child_resources(self):
        for index,case in enumerate(OUTER_CASES):
            expected=native_results()[len(CASES)+index]
            for style in ('draft','readable'):
                with self.subTest(case=case,style=style):
                    self.assertEqual(lua_execute(style,**case),expected)

if __name__=='__main__':unittest.main()
