"""Recover the omitted Nasty Streak post-speech cancellation join."""
from tools.script_recovery.bully_initial_phases import recover

def lower(source,data=None):
    recover(data)
    old='                                    -- TODO(native): goto LAB_00dbc27a'
    if source.count(old)!=1:raise ValueError('Bully Nasty Streak join correspondence changed')
    source=source.replace(old,'''                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        finish_bully_movie()
                                        goto LAB_00dbcce2
                                    end''')
    old='                                aVar5 = SUB41(uVar7,0)\n'
    if source.count(old)!=1:raise ValueError('Bully complaint-loop dead staging changed')
    # B8DE/B902 load AL directly for the complaint branch. The decompiler's
    # extra low-byte assignment is never consumed; later speech sets aVar5=0.
    source=source.replace(old,'')
    old='quest:GiveHeroYesNoQuestion(pCVar22, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")'
    if source.count(old)!=1:raise ValueError('Bully question correspondence changed')
    source=source.replace(old,'quest:GiveHeroYesNoQuestion(pCVar22, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)')
    for old,new in (
        ('if not alive then return end  -- TODO(native): goto LAB_00dbc848',
         'if not alive then goto LAB_00dbccdd end'),
        ('if not alive then return end  -- TODO(native): goto LAB_00dbc7ac',
         'if not alive then finish_bully_movie(); goto LAB_00dbccdd end')):
        if source.count(old)!=1:raise ValueError('Bully question cleanup join correspondence changed')
        source=source.replace(old,new)
    return source,{'nativeJoin':0xdbc27a,'branch':'Nasty Streak successful speech/wait joins post-speech termination query before ending movie and continuing to hit processing.',
                   'complaintLoop':[0xdbb8de,0xdbb902],'questionFlag':True,
                   'questionCleanup':[0xdbc848,0xdbc7ac,0xdbccdd],
                   'limits':'Question/movie phase ownership retained; whole actor integration remains separate from these joins.'}
