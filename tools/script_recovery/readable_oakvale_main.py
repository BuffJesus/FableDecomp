"""Remove Main's native allocation flags after binding/objective recovery."""
import hashlib
import re
from tools.script_recovery.prepare_oakvale_main_registration import prove as prove_registration

RAW_SHA='6a89d6b21ec7a9994913d4872a7bf171e600b984f6713369ebb76a7249c15aae'   # 2026-09-17: the sixteenth binding lifts in the draft itself


def lower(source):
    registration_start=source.index('\nfunction RegisterMain(')+1
    registration_end=source.index('\nfunction Main(',registration_start)+1
    if hashlib.sha256(source[registration_start:registration_end].encode()).hexdigest()!='0b0e0c3e9ae01e07e98f2a7483bb80546f1073acc4cb42c98dbfde3dae6bf010':
        raise ValueError('Oakvale Main registration draft changed')
    registration=prove_registration()
    source=source[:registration_start]+'''-- LuaQuestHost::RegisterMain owns native Main registration (empty section).
-- Main is invoked through the native virtual thunk; no second Lua thread is created.

'''+source[registration_end:]
    start=source.index('\nfunction Main(')+1;end=source.index('\nfunction Init(',start)+1
    raw=source[start:end]
    if hashlib.sha256(raw.encode()).hexdigest()!=RAW_SHA:raise ValueError('Recovered Oakvale Main draft changed')
    bindings=re.findall(r'^    quest:AddEntityBinding\([^\n]+',raw,re.M)
    if len(bindings)!=16:raise ValueError('Expected sixteen recovered Main bindings')
    body='function Main(quest)\n'+'\n'.join(bindings)+'''
    quest:FinalizeEntityBindings()
    if quest:GetStateBool("AttackOver") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)
    end
    quest:WithRetailResources(function(resources)
        resources:SetInitialOakvaleObjective()
    end)
    quest:CreateThread("StartBarrelTimer")
    DoMission(quest)
end

'''
    return source[:start]+body+source[end:],dict(status='structured Main after native binding and objective recovery',
        rawSha256=RAW_SHA,bindingCount=16,registration=dict(native=registration,implementation='LuaQuestHost::RegisterMain'),
        limits='Native tail control flow checked. Binding and thread allocation failure/engine scheduling remain separate integration gates.')
