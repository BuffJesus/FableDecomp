"""Structured main dialogue, preserving every native gate and query boundary."""
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

SOURCE='''function BullyMainDialogue(quest, resources, control, state)
    local function speak(key)
        if not BullyControlledHealthAboveThreshold(resources, control) then return true end
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
    if not state:GetStateBool("DoneIntro") then
        if quest:IsActiveThreadTerminating() then return false end
        if not speak("TEXT_QST_048_BULLY_GET_LOST") then return false end
        state:SetStateBool("DoneIntro", true)
        return true
    end
    if quest:IsActiveThreadTerminating() then return false end
    if quest:GetStateBool("HeroAttackedVictim") then
        if quest:IsActiveThreadTerminating() then return false end
        if not state:GetStateBool("SaidPieceAboutAttackingVictim") then
            if quest:IsActiveThreadTerminating() then return false end
            if not speak("TEXT_QST_048_BULLY_IN_COMMON") then return false end
            state:SetStateBool("SaidPieceAboutAttackingVictim", true)
            return true
        end
        if quest:IsActiveThreadTerminating() then return false end
        local fewHits = state:GetStateInt("HitsTaken") < 3
        if quest:IsActiveThreadTerminating() then return false end
        return speak(fewHits and "TEXT_QST_048_BULLY_NASTY_STREAK" or "TEXT_QST_048_BULLY_DONT_HIT_ME")
    end
    if quest:IsActiveThreadTerminating() then return false end
    return speak("TEXT_QST_048_BULLY_BADGERING")
end
'''

def lower(source,data=None):
    data=data or RData();recover(data)
    c=Cs(CS_ARCH_X86,CS_MODE_32);c.detail=True
    instructions=list(c.disasm(data.bytes_at(0xdbbe8a,0x3ff),0xdbbe8a))
    calls=[];keys=[]
    for n,i in enumerate(instructions):
        if i.mnemonic=='call' and i.op_str=='0x7e7390':
            calls.append(i.address)
            literals=[j.operands[0].imm for j in instructions[max(0,n-12):n]
                      if j.mnemonic=='push' and j.op_str.startswith('0x12')]
            if len(literals)!=1:raise ValueError('Bully dialogue speech literal correspondence changed')
            keys.append(data.string_at(literals[0]))
    if keys!=['TEXT_QST_048_BULLY_GET_LOST','TEXT_QST_048_BULLY_IN_COMMON','TEXT_QST_048_BULLY_NASTY_STREAK','TEXT_QST_048_BULLY_DONT_HIT_ME','TEXT_QST_048_BULLY_BADGERING']:
        raise ValueError('Bully main dialogue literals changed')
    # This occurrence is the post-movie main talk dispatch, not the initial loop.
    start=source.index('            if __native_entity_state:GetStateBool("DoneIntro") then\n')
    finish='            ::LAB_00dbc289::\n            finish_bully_movie()'
    end=source.index(finish,start)+len(finish)
    old=source[start:end]
    if old.count('resources:Speak(')!=5 or old.count('BullyControlledHealthAboveThreshold')!=5:
        raise ValueError('Bully main dialogue correspondence changed')
    replacement='''            alive = BullyMainDialogue(quest, resources, bully_control, __native_entity_state)
            finish_bully_movie()
            if not alive then goto LAB_00dbcce2 end'''
    return SOURCE+'\n'+source[:start]+replacement+source[end:],{'region':[0xdbbe8a,0xdbc289],
        'cancelDestinations':[0xdbc1c3,0xdbc851],'speechSites':calls,
        'semantics':'Five health-controlled speech branches; state writes only after their native health/speech/cancellation gates.',
        'limits':'Caller still owns self control and movie. Structured dialogue does not replace acquisition or movie policy.'}
