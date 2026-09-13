"""Generate actual Guardian adapter integration cases from native replay evidence."""
import json

FACADE='''
local current, queries, poll, row, piece
local quest = {}
function quest:WithRetailResources(f) with_resources(f) end
function quest:RegisterBoundAliveCondition() nativeQuest:RegisterBoundAliveCondition() end
function quest:GetHero() return hero end
function quest:IsActiveThreadTerminating()
    queries=queries+1
    local stop=native_cancelled() or (current.stop_query>0 and queries>=current.stop_query)
    record('query',stop);return stop
end
function quest:SetStateBool(key,v) assert(key=='PieceOver');piece=v;record('piece_over',v) end
function quest:GetStateBool(key) assert(key=='PieceOver');return piece end
function quest:GetActiveQuestName() record('active');return 'QS_GuardianTrophyDealerInfo' end
function quest:SetThingHasInformation(actor,...) assert(actor==me);record('information',...) end
function quest:EntitySetAsKillable(actor,...) assert(actor==me);record('killable',...) end
function quest:EntitySetAsDamageable(actor,...) assert(actor==me);record('damageable',...) end
function quest:SetIsPushableByHero(actor,...) assert(actor==me);record('pushable',...) end
function quest:KickOffQuestStartScreen(...) record('start_screen',...) end
function quest:MiniMapAddMarker(actor,...) assert(actor==me);record('add_marker',...) end
function quest:MiniMapRemoveMarker(actor) assert(actor==me);record('remove_marker') end
function quest:ClearThingHasInformation(actor) assert(actor==me);record('clear_information') end
function quest:EntitySetThingAsAllyOfThing(a,b)
    assert((a==me and b==hero) or (a==hero and b==me))
    record('ally',a==me and 'maze' or 'hero',b==me and 'maze' or 'hero')
end
function quest:FixMovieSequenceCamera(...) record('fix_camera',...) end
function quest:GiveQuestCardDirectly(...) record('quest_card',...) end
function quest:EntitySetFacingAngle(actor,...) assert(actor==me);record('facing',...) end
function quest:EntitySetFacingAngleTowardsThing(actor,target,flag)
    assert(actor==me and target==hero);record('face_hero',flag)
end
function quest:AddLogbookStoryEntry(...) record('logbook',...) end
function quest:SetQuestAsCompleted(...) record('complete',...) end
bind_actor_methods({
    GetAngleXY=function(actor) assert(actor==me);record('angle',current.angle);return current.angle end,
    MsgIsHitByHero=function(actor)
        assert(actor==me);poll=poll+1;row=current.polls[math.min(poll,#current.polls)] or {}
        local v=row.hit or false;record('hit',v);return v
    end,
    MsgIsHitByAnySpecialAbilityFromHero=function(actor)
        assert(actor==me);local v=row.special or false;record('special',v);return v
    end,
    MsgIsHitByHealLifeFromHero=function(actor)
        assert(actor==me);local v=row.heal or false;record('heal',v);return v
    end,
    IsTalkedToByHero=function(actor)
        assert(actor==me);local v=row.use or false;record('use',v);return v
    end
})
'''


def literal(v):
    if isinstance(v,bool):return str(v).lower()
    if isinstance(v,(float,int)):return str(v)
    if isinstance(v,str):return json.dumps(v)
    if isinstance(v,list):return '{'+','.join(map(literal,v))+'}'
    if isinstance(v,dict):return '{'+','.join('['+literal(k)+']='+literal(x) for k,x in v.items())+'}'
    raise TypeError(type(v))


def event_string(event):
    def field(v):
        if isinstance(v,bool):return str(v).lower()
        if isinstance(v,(int,float)):return format(v,'.9g')
        return v
    return '|'.join(map(field,event))


def replay_source(cases,reference,package=None):
    lines=['local Q=dofile('+literal((reference/'quest.lua').as_posix())+')',
           'local A=dofile('+literal((reference/'runtime_adapter.lua').as_posix())+')',FACADE]
    if package:
        lines += ['package.path='+literal(package.as_posix()+'/?.lua;')+'..package.path']
    for case in cases:
        inputs={k:v for k,v in case.items() if k not in ('events','piece_over')}
        lines += ['current='+literal(inputs),'queries=0;poll=0;row={};piece=false;reset_native(current)',
                  *(['dofile('+literal((package/'GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua').as_posix())+')',
                     'Init(quest,me)','Main(quest,me)'] if package else
                    ['local angle=Q.entity_init(A.new(quest,nil,me))','A.run(Q,"entity_main",quest,me,angle)']),
                  'assert(piece=='+literal(case['piece_over'])+')',
                  'verify_native('+literal(case['name'])+','+literal([event_string(e) for e in case['events']])+')']
    return '\n'.join(lines)+'\n'


def watcher_source(cases,package):
    lines=['package.path='+literal(package.as_posix()+'/?.lua;')+'..package.path',
           'dofile('+literal((package/'GuardianTrophyDealerInfo/GuardianTrophyDealerInfo.lua').as_posix())+')',
           '''local current,piece,frames,queries
local quest={}
function quest:WithRetailResources(f) with_resources(f) end
function quest:GetStateBool(key) assert(key=='PieceOver');return piece end
function quest:SetStateBool(key,v) assert(key=='PieceOver');piece=v;record('piece_over',v) end
function quest:IsActiveThreadTerminating()
    record('query');queries=queries+1
    return current.stop_query>0 and queries>=current.stop_query
end
function quest:GetActiveQuestName() record('active');return 'QS_GuardianTrophyDealerInfo' end
function quest:DeactivateQuestLater(name,delay)
    assert(name=='QS_GuardianTrophyDealerInfo' and delay==0);record('deactivate')
end
set_frame_callback(function()
    frames=frames+1
    if current.set_frame>0 and frames>=current.set_frame then piece=true end
end)
''']
    for case in cases:
        inputs={**case,'watcher':True,'acquisitions':[],'health':[],'busy':[],'cancel_macro':0}
        inputs.pop('events')
        lines+=['current='+literal(inputs),'piece=current.piece;frames=0;queries=0;reset_native(current)',
                'WaitForPieceOver(quest)','verify_native('+literal(case['name'])+','+literal(case['events'])+')']
    # Root operations compared to the archived native Main disassembly contract.
    lines+=['''current={acquisitions={},health={},busy={},cancel_macro=0}
reset_native(current)
piece=true
function quest:AddEntityBinding(name,path,flags)
    assert(path=='GuardianTrophyDealerInfo/Entities/GTDI_Maze');record('bind',name,flags)
end
function quest:FinalizeEntityBindings() record('finalize') end
function quest:CreateThread(name,options) assert(options.region=='');record('spawn',name,options.region) end
function quest:SetQuestCardObjective(...) record('objective',...) end
Init(quest);assert(piece==false);Main(quest)
verify_native('root_init_main',{'piece_over|false','bind|GTDI_Maze|0','finalize',
 'spawn|WaitForPieceOver|','active',
 'objective|QS_GuardianTrophyDealerInfo|TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01|HeroGuildComplexInside|HeroGuildComplexInside'})
''']
    return '\n'.join(lines)+'\n'
