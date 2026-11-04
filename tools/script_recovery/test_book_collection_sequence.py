"""Composed generated quest/Teacher progression with engine and persistence doubles.

Runs real generated Main, selection, donation, argument and persistence functions.
Macros record requested awards; this does not prove game item delivery or saves.
"""
import copy

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.test_book_reaction_lowering import generated


class Collection:
    def __init__(self, root_source, teacher_source, saved=None):
        self.sources = root_source, teacher_source
        self.state, self.flags, self.inventory = {}, {}, set()
        self.strings = {}
        self.events, self.live = [], {}
        self.answer, self.pending_answers, self.frames = 1, 0, 0
        self.hour = 1200
        self.stopped, self.paused, self.camera = False, False, False
        self.root, self.lua = LuaRuntime(), LuaRuntime()
        self.root.execute(root_source)
        self.lua.execute(teacher_source)
        self.me = self.lua.table_from({'Speak': self.speak,
                                      'IsPerformingScriptTask': lambda *_: False,
                                      'IsTalkedToByHero': lambda *_: True})
        self.resources = self.lua.table_from({
            'MemberResource': lambda *_: 'teacher', 'ScriptThing': lambda *_: self.me,
            'PrepareResource': lambda *_: None, 'TryAcquire': lambda *_: True,
            'NewResource': lambda *_: self.new('resource'),
            'NewStringMap': lambda *_: self.new('strings'),
            'NewActorMap': lambda *_: self.new('actors'),
            'StartMovie': lambda *_: self.new('movie'),
            'DestroyMovie': self.destroy_movie,
            'DestroyStringMap': lambda _, v: self.destroy('strings', v),
            'DestroyActorMap': lambda _, v: self.destroy('actors', v),
            'ReleaseResource': lambda _, v: self.destroy('resource', v),
            'SetString': self.set_string, 'ClearStringMap': self.clear,
            'SetActor': self.set_actor, 'RunMacroWithStrings': self.macro,
            'IsPerformingScriptTask': lambda *_: False})
        methods = {
            'RetailResources': lambda *_: self.resources, 'GetHero': lambda *_: 'hero',
            'ReadGlobalGameData': lambda _, off: {1248:16,1252:25,1256:35,1260:40,1264:9,1268:9}.get(off,0),
            'ReadGlobalGameDataFloat': lambda *_: 1,
            'SnapshotGlobalConversations': lambda *_: None,  # Worker threads are recorded, not executed here.
            'ReadGlobalGameDataStringAt': self.definition,
            'StateStringVectorResize': lambda _, k, n: self.strings.__setitem__(k, (self.strings.get(k, []) + [''] * n)[:n]),
            'SetStateStringVectorAt': lambda _, k, i, v: self.strings[k].__setitem__(i, v),
            'GetStateStringVectorAt': lambda _, k, i: self.strings[k][i],
            'SetStateInt': self.set_int, 'GetStateInt': lambda _, k: self.state[k],
            'SetStateBool': lambda _, k, v: self.state.__setitem__(k,bool(v)),
            'GetStateBool': lambda _, k: self.state.get(k,False),
            'StateFlagVectorResize': lambda _, k, n: self.flags.__setitem__(k,[False]*n),
            'SetStateFlagVectorAt': lambda _, k, i, v: self.flags[k].__setitem__(i,bool(v)),
            'GetStateFlagVectorAt': lambda _, k, i: self.flags[k][i],
            'IsObjectInThingsPossession': lambda _, key, hero: key in self.inventory,
            'TakeObjectFromHero': self.take,
            'GiveHeroYesNoQuestion': lambda _, key, *args: self.events.append(('question',key)),
            'MsgIsQuestionAnsweredYesOrNo': self.answer_question,
            'IsActiveThreadTerminating': lambda *_: self.stopped,
            'NewScriptFrame': self.frame,
            'GetHealth': lambda *_: 100, 'GetTimeOfDay': lambda *_: self.hour,
            'GetMasterGameState': lambda _, k: {'PostSavePosition':1800,'TeddySolution':'A'}.get(k,False),
            'GetHeroAttractiveness': lambda *_: 0.75, 'GetHeroScariness': lambda *_: 0.75,
            'GetThingWithScriptName': lambda *_: None,
            'GiveHeroMorality': lambda _, n: self.events.append(('morality',n)),
            'EntityPostOpinionDeedToRecipient': lambda _, hero, deed, actor: self.events.append(('opinion',hero,deed)),
            'FixMovieSequenceCamera': lambda _, v: setattr(self,'camera',v),
            'PauseAllNonScriptedEntities': lambda _, v: setattr(self,'paused',v),
            'CreateThread': lambda _, name, args: self.events.append(('thread',name,args['args'][1])),
            'GetActiveQuestName': lambda *_: 'V_BookCollecting',
            'SetQuestAsCompleted': lambda _, name, *flags: self.events.append(('complete',name,*flags)),
            'PersistTransferInt': self.transfer, 'PersistTransferBool': self.transfer,
            'PersistStateFlagVector': self.transfer_flags,
        }
        for name in ['FadeScreenIn','SetThingPersistent','SetThingHasInformation','ClearThingHasInformation',
                     'GiveHeroQuestCardDirectly','SetQuestCardObjective','AddRumourCategory',
                     'AddNewRumourToCategory','AddGossipVillage','AddGossipFactionToCategory','RemoveRumourCategory']:
            methods[name] = lambda *_: None
        self.q = self.lua.table_from(methods)
        self.rq = self.root.table_from(methods)
        # Keep Main alive across interactions, like its native long-lived loop.
        # Yield in Lua after movie destruction; Python callbacks cannot yield.
        self.resources['DestroyMovie'] = self.lua.eval('''function(callback)
            return function(self,value)
                callback(self,value)
                coroutine.yield('movie_closed')
            end
        end''')(self.destroy_movie)
        self.resume = self.lua.eval('''function(q,me)
            local thread=coroutine.create(function() Main(q,me) end)
            return function()
                local ok,event=coroutine.resume(thread)
                if not ok then error(event) end
                return event
            end
        end''')(self.q,self.me)
        self.root.globals().Init(self.rq)
        self.lua.globals().Init(self.q,self.me)
        if saved:
            self.inventory = set(saved['inventory'])
            self.storage = copy.deepcopy(saved['storage'])
            self.mode = 'load'
            self.root.globals().OnPersist(self.rq,'context')

    def new(self, kind):
        value = {}
        self.live[id(value)] = kind,value
        return value

    def require(self, value, kind):
        assert self.live[id(value)][0] == kind

    def destroy(self, kind, value):
        self.require(value,kind)
        del self.live[id(value)]

    def destroy_movie(self, _, value):
        assert not self.paused
        self.destroy('movie',value)

    def set_string(self, _, value, key, text):
        self.require(value,'strings');value[key]=text

    def set_actor(self, _, value, key, actor):
        self.require(value,'actors');value[key]=actor

    def clear(self, _, value):
        self.require(value,'strings');self.events.append(('clear',dict(value)));value.clear()

    def macro(self, _, name, actors, strings, *args):
        self.require(actors,'actors');self.require(strings,'strings')
        self.require(actors['Hero'],'resource');assert actors['Teacher']=='teacher'
        if 'TEACHER_GIVES' in name:
            assert set(strings)=={'$PRIZE'}
            self.events.append(('award_request',self.state['BooksDonated'],strings['$PRIZE']))
        self.events.append(('macro',name,dict(strings)))

    def speak(self, _, hero, key, *args):
        assert hero=='hero';self.events.append(('speech',key));return True

    def definition(self, _, off, index):
        assert off==0x4d4 and 0<=index<40
        return f'BOOK_{index}'

    def set_int(self, _, key, value):
        self.state[key]=(int(value)+2**31)%2**32-2**31

    def take(self, _, key):
        assert key in self.inventory
        self.inventory.remove(key);self.events.append(('take',key))

    def frame(self, *_):
        self.frames+=1;assert self.frames<200
        return not self.stopped

    def answer_question(self, *_):
        if self.pending_answers:
            self.pending_answers-=1;return -1
        return self.answer

    def transfer(self, _, context, key, value):
        assert context=='context'
        if self.mode=='save':self.storage[key]=value
        return self.storage[key]

    def transfer_flags(self, _, context, key):
        assert context=='context'
        if self.mode=='save':self.storage[key]=self.flags[key][:]
        else:self.flags[key]=self.storage[key][:]

    def checkpoint(self):
        assert not self.live and not self.state['ReadingBook']
        self.mode,self.storage='save',{}
        self.root.globals().OnPersist(self.rq,'context')
        return copy.deepcopy(dict(storage=self.storage,inventory=sorted(self.inventory)))

    def interact(self):
        self.stopped=False;self.frames=0
        assert self.resume()=='movie_closed'
        assert not self.live and not self.paused and not self.camera
        assert not self.state['ReadingBook']


GOOD_ORDER = list(range(8))+[15]+list(range(8,15))+list(range(16,21))+[22,23,24,21]
EVIL_ORDER = [16,17,18,19,20,22,23,24,21]+list(range(16))


@pytest.mark.parametrize('order,hat',[(GOOD_ORDER,'GOOD'),(EVIL_ORDER,'EVIL')])
@pytest.mark.parametrize('reloads',[(),(1,8,9,24)])
def test_full_donation_sequence_and_fresh_vm_persistence(generated,order,hat,reloads):
    _,roots,teachers=generated
    assert sorted(order)==list(range(25))
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact()
        assert h.state['DoneIntro'] and h.state['BooksDonated']==0
        awards=[];donated=set()
        for number,index in enumerate(order,1):
            h.events.clear();h.inventory.add(f'BOOK_{index}');h.pending_answers=2
            h.interact();donated.add(index)
            assert h.state['BooksDonated']==number
            assert h.state['GoodBooksDonated']==sum(i<16 for i in donated)
            assert not h.inventory and h.state['LastBookRequested']==-1
            assert {i for i,v in enumerate(h.flags['BookDonated']) if v}==donated
            assert {i for i,v in enumerate(h.flags['BookOwned']) if v}==donated
            assert [e for e in h.events if e[0]=='take']==[('take',f'BOOK_{index}')]
            assert ('thread','BookReaction',index) in h.events
            awards.extend(e for e in h.events if e[0]=='award_request')
            if number in (9,25):
                cleared=[e[1] for e in h.events if e[0]=='clear']
                assert len(cleared)==1 and '$ARG1' in cleared[0]
            if number in reloads:
                oldstate,oldflags=copy.deepcopy((h.state,h.flags))
                h=Collection(root,teacher,h.checkpoint())
                assert h.state==oldstate and h.flags==oldflags
        assert awards==[('award_request',9,f'OBJECT_HERO_HAT_WIZARD_{hat}'),
                        ('award_request',25,'OBJECT_SILVER_KEY')]
        assert h.state['HatRewarded'] and h.state['KeyRewarded']
        restored=Collection(root,teacher,h.checkpoint())
        assert restored.state==h.state and restored.flags==h.flags
        for completed in (h,restored):
            completed.events.clear();completed.interact()
            assert ('complete','V_BookCollecting',False,False,False) in completed.events
            assert ('speech','TEXT_QST_B16_COMPLETE') in completed.events
            assert not any(e[0] in ('take','macro','award_request') for e in completed.events)
            assert completed.state['BooksDonated']==25 and completed.state['GoodBooksDonated']==16
            completed.events.clear();completed.interact()
            assert ('speech','TEXT_QST_B16_COMPLETE') in completed.events
            assert not any(e[0] in ('complete','take','macro','award_request') for e in completed.events)


@pytest.mark.parametrize('index',[3,16])
def test_refusal_reload_and_retry_follow_real_main(generated,index):
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact();h.events.clear()
        h.inventory.add(f'BOOK_{index}');h.answer=0;h.interact()
        assert h.state['BooksDonated']==0 and not any(h.flags['BookDonated'])
        assert h.inventory=={f'BOOK_{index}'} and h.flags['BookOwned'][index]
        assert h.state['LastBookRequested']==(index if index<16 else -1)
        assert not any(e[0] in ('take','macro','award_request') for e in h.events)
        h=Collection(root,teacher,h.checkpoint());h.interact()
        assert h.state['BooksDonated']==1 and h.state['GoodBooksDonated']==(index<16)
        assert h.state['LastBookRequested']==-1 and not h.inventory


def test_comment_and_duplicate_keep_inventory_without_extra_donation(generated):
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact()
        h.inventory.add('BOOK_0');h.interact()
        h=Collection(root,teacher,h.checkpoint());h.inventory.add('BOOK_0');h.events.clear();h.interact()
        assert ('speech','TEXT_QST_B16_BOOK_NOT_FOUND') in h.events
        assert h.state['BooksDonated']==1 and h.inventory=={'BOOK_0'}
        h.inventory.add('BOOK_25');h.events.clear();h.interact()
        assert ('speech','TEXT_QST_B16_BOOK_COMMENT_0') in h.events
        h=Collection(root,teacher,h.checkpoint());h.inventory.add('BOOK_26');h.interact()
        assert ('speech','TEXT_QST_B16_BOOK_COMMENT_1') in h.events
        assert h.inventory=={'BOOK_0','BOOK_25','BOOK_26'}
        assert h.state['BooksDonated']==h.state['GoodBooksDonated']==1
        assert {i for i,v in enumerate(h.flags['BookDonated']) if v}=={0}
        assert {i for i,v in enumerate(h.flags['BookOwned']) if v}=={0,25,26}


@pytest.mark.parametrize('hour,wrong_time',[(844,True),(845,False),(846,False),
                                         (1614,False),(1615,False),(1616,True)])
def test_school_hours_boundaries_in_continuing_main(generated,hour,wrong_time):
    # Retail E554B0/JL and E554C6/JG: talk window includes845 and1615.
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact();h.events.clear();h.hour=hour
        h.interact()
        text='WRONG_TIME' if wrong_time else 'NONE_FOUND'
        assert ('speech','TEXT_QST_B16_BOOK_'+text) in h.events
        assert not any(e[0] in ('question','take','macro','award_request') for e in h.events)
        assert h.state['BooksDonated']==h.state['GoodBooksDonated']==0


@pytest.mark.parametrize('replacement',[None,4])
def test_saved_request_lost_inventory_does_not_consume_missing_book(generated,replacement):
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact();h.inventory.add('BOOK_3');h.answer=0;h.interact()
        assert h.state['LastBookRequested']==3
        saved=h.checkpoint();saved['inventory']=[]
        h=Collection(root,teacher,saved)
        if replacement is not None:h.inventory.add(f'BOOK_{replacement}')
        h.interact()
        assert ('speech','TEXT_QST_B16_BOOK_LOST') in h.events
        assert ('take','BOOK_3') not in h.events and not h.flags['BookDonated'][3]
        assert h.state['BooksDonated']==(replacement is not None)
        if replacement is not None:
            assert h.flags['BookDonated'][replacement] and not h.inventory
            assert ('speech',f'TEXT_QST_B16_BOOK_REQUEST_{replacement}') in h.events


def test_bulk_inventory_matches_retail_selection_order(generated):
    # Native E57530 scan reference, marathon14/bulk_native_comparison.json.
    # First unseen book wins; once seen, the last eligible book in that category
    # wins. Good books precede grudging books, then comments/unsupported books.
    expected=[0,*range(15,0,-1),16,*range(24,16,-1)]
    _,roots,teachers=generated
    for root,teacher in zip(roots,teachers):
        h=Collection(root,teacher);h.interact()
        h.inventory={f'BOOK_{i}' for i in range(40)}
        awards=[]
        for number,index in enumerate(expected,1):
            h.events.clear();h.interact()
            assert [e for e in h.events if e[0]=='take']==[('take',f'BOOK_{index}')]
            assert h.state['BooksDonated']==number
            assert {i for i,v in enumerate(h.flags['BookDonated']) if v}==set(expected[:number])
            assert {i for i,v in enumerate(h.flags['BookOwned']) if v}==set(range(16 if number<=16 else 25))
            awards.extend(e for e in h.events if e[0]=='award_request')
            if number in (8,9,16,24):h=Collection(root,teacher,h.checkpoint())
        assert h.inventory=={f'BOOK_{i}' for i in range(25,40)}
        assert awards==[('award_request',9,'OBJECT_HERO_HAT_WIZARD_GOOD'),
                        ('award_request',25,'OBJECT_SILVER_KEY')]
