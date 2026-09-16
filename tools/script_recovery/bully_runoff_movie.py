"""Pinned runoff maps/movie continuation; capability additions remain unapplied."""
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from collections import deque
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function BullyRunoffMovie(quest, resources, hero, victim, bully, retainedVictim)
    local actors = resources:NewActorMap()
    resources:SetActor(actors, "HERO", hero)
    resources:SetActor(actors, "BRAT", victim)
    resources:SetActor(actors, "BULLY", bully)
    local inputs = resources:NewStringMap()
    local attackedVictim = quest:GetStateBool("HeroAttackedVictim")
    if quest:IsActiveThreadTerminating() then
        resources:DestroyStringMap(inputs)
        resources:DestroyActorMap(actors)
        return false
    end
    resources:SetString(inputs, "$BRATLINE", attackedVictim
        and "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT" or "TEXT_QST_048_VICTIM_THANKS")
    local movie = resources:StartMovie("")
    resources:Pause(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_OAKVALEINTRO_BULLYRUN1", actors, inputs, false, true)
    local givenTeddy = quest:GetStateBool("GivenHeroTeddy")
    if quest:IsActiveThreadTerminating() then
        -- Native cancellation does not issue FixMovieSequenceCamera(false).
        resources:Pause(false)
        resources:DestroyMovie(movie)
        resources:DestroyStringMap(inputs)
        resources:DestroyActorMap(actors)
        return false
    end
    if not givenTeddy then
        resources:RunMacro("CS_OAKVALEINTRO_BULLYRUN2", actors, false, true)
        resources:ClearThingHasInformation(retainedVictim)
        quest:SetStateBool("GivenHeroTeddy", true)
    else
        resources:RunMacro("CS_OAKVALEINTRO_BULLYRUNDUMMY", actors, false, true)
    end
    quest:FixMovieSequenceCamera(false)
    resources:Pause(false)
    resources:DestroyMovie(movie)
    resources:DestroyStringMap(inputs)
    resources:DestroyActorMap(actors)
    return true
end
'''

def evidence(data=None):
    data=data or RData();_,w=recover(data) # full pinned native body, including EDI definitions
    c=Cs(CS_ARCH_X86,CS_MODE_32);c.detail=True
    ins=list(c.disasm(data.bytes_at(w['mainAddress'],w['mainSize']),w['mainAddress']))
    # Every instruction on the late runoff route is dominated by the bound-self
    # reload at BD9F; no subsequent EDI definition until the function epilogue.
    last=[i for i in ins if 0xdbbd9f<=i.address<=0xdbccd1 and
          any(i.reg_name(r) in ('edi','di') for r in i.regs_access()[1])]
    if [(i.address,i.mnemonic,i.op_str) for i in last]!=[(0xdbbd9f,'mov','edi, dword ptr [esp + 0x50]')]:
        raise ValueError('Bully runoff bound actor definition changed')
    positions={i.address:n for n,i in enumerate(ins)}
    queue=deque([(0,None)]);seen=set();origins=set()
    while queue:
        index,origin=queue.popleft()
        if (index,origin) in seen:continue
        seen.add((index,origin));i=ins[index]
        if i.address==0xdbccd1:origins.add(origin)
        if any(i.reg_name(r) in ('edi','di') for r in i.regs_access()[1]):origin=i.address
        if i.mnemonic.startswith('ret'):continue
        nexts=[index+1]
        if i.mnemonic.startswith('j'):
            target=positions[i.operands[0].imm]
            nexts=[target] if i.mnemonic=='jmp' else nexts+[target]
        queue.extend((n,origin) for n in nexts)
    if origins!={0xdbbd9f}:raise ValueError(f'Unproved RemoveThing actor reaching definitions: {origins}')
    literals={a:data.string_at(a) for a in (0x1255174,0x12d99e8,0x12678fc,0x12d99dc,
              0x12d99b4,0x12d9998,0x12d997c,0x12d9960,0x12d9940)}
    expected=['HERO','BRAT','BULLY','$BRATLINE','TEXT_QST_048_VICTIM_THANKS_AFTER_HIT',
              'TEXT_QST_048_VICTIM_THANKS','CS_OAKVALEINTRO_BULLYRUN1',
              'CS_OAKVALEINTRO_BULLYRUN2','CS_OAKVALEINTRO_BULLYRUNDUMMY']
    if list(literals.values())!=expected:raise ValueError('Bully runoff literals changed')
    return {'actorMap':100,'actorResources':{'HERO':60,'BRAT':116,'BULLY':16},
            'stringMap':88,'stringKey':'$BRATLINE','movie':240,
            'macroCalls':[0xdbcb8a,0xdbcbcd,0xdbcc61],
            'macroABI':'ECX CString, EDX actor map; stack null flags, input map or null, false, true',
            'clearInformationThing':44,'clearInformationCall':0xdbcbe5,
            'normalCleanup':[0xdbcc82,0xdbcc8f,0xdbcc98,0xdbcca1,0xdbccaa,0xdbccb3],
            'cancelCleanup':[0xdbcc06,0xdbcc13,0xdbcc1c,0xdbcc25,0xdbcc2e,0xdbcc37],
            'removeThingCall':0xdbccd1,'removeThing':'bound self; false,true','actorCFGStates':len(seen),
            'limits':'Map adapters and retained-Thing information adapter pending integration; callback error close uses existing host policy.'}

def lower(source):
    proof=evidence()
    start=source.index('                -- TODO(native): StdMap_Construct_API();')
    end=source.index('            end\n            ::LAB_00dbcc2a::',start)
    if source[start:end].count('RunCutsceneMacro_Func')!=3:raise ValueError('Runoff draft correspondence changed')
    replacement='''                if BullyRunoffMovie(quest, resources, bully_hero_control, bully_victim_control, bully_control, r1) then
                    resources:ReleaseResource(bully_victim_control)
                    bully_victim_control = nil
                    resources:ReleaseResource(bully_hero_control)
                    bully_hero_control = nil
                    quest:SetStateBool("BullyRanOff", true)
                    require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
                    quest:RemoveThing(me, false, true)
                    goto LAB_00dbcce2
                end
'''
    return SOURCE+'\n'+source[:start]+replacement+source[end:],proof
