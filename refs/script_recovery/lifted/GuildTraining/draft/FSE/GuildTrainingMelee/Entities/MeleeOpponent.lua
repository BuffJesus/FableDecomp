-- Generated native draft: MeleeOpponent. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local CVar3, bVar4, cVar5, fVar2, fret_0, fret_00, iVar10, iVar11, iVar8, iVar9, p0, p1, pCVar6, pThing, pThing_00, piVar1, r1, r2, r3, r4, r5, uVar7, xStack_bc, xStack_d8, xStack_dc
    local alive = true
    xStack_d8 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_d8, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_d8)
            return
        end
        bVar4 = resources:TryAcquire(xStack_d8, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:EntitySetAsKillable(me, false, true)
        cVar5 = quest:GetStateBool("TalkedToWhisper")
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d5685e end
            cVar5 = quest:GetStateBool("TalkedToWhisper")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            pCVar6 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
            iVar11 = 1
            iVar10 = 0
            iVar9 = 1
            iVar8 = 0x40400000
            p0 = pCVar6:GetPos()
            me:MoveToPosition(p0, iVar8, iVar9, (iVar10 ~= 0), (iVar11 ~= 0))
            iVar8 = me:IsPerformingScriptTask()
            cVar5 = iVar8
            while (cVar5 and (not quest:GetStateBool("WhisperStopWalking"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d5685e end
                iVar8 = me:IsPerformingScriptTask()
                cVar5 = iVar8
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                me:ClearCommands()
                bVar4 = false
                if bVar4 ~= 0 then
                end
                quest:SetStateBool("WhisperArrived", true)
                quest:EntitySetAsKillable(me, false, true)
                iVar8 = quest:GetStateInt("TutorialState")
                while iVar8 == 1 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d57f5d end
                    bVar4 = me:IsTalkedToByHero()
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d57f5d end
                        xStack_bc = resources:StartMovie("")
                        quest:StartMovieSequence()
                        pCVar6 = 0x1
                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                        bVar4 = false
                        if bVar4 ~= 0 then
                        end
                        bVar4 = resources:TryAcquire(xStack_d8, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_bc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            bVar4 = resources:TryAcquire(xStack_d8, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d56eb7: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_bc)
                            resources:ReleaseResource("")
                            return
                        end
                        pCVar6 = resources:ScriptThing(xStack_d8)
                        pThing_00 = pCVar6
                        fret_0 = quest:GetHealth(pThing_00)
                        fVar2 = 0.0
                        if fVar2 < fret_0 then
                            iVar11 = 0
                            iVar10 = 1
                            iVar9 = 0
                            iVar8 = 0
                            p1 = "TEXT_QST_028_WHISPER_MEET"
                            pCVar6 = quest:GetHero()
                            r1 = me:Speak(pCVar6, p1, iVar8, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                            iVar8 = me:IsPerformingScriptTask()
                            cVar5 = iVar8
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_bc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                iVar8 = me:IsPerformingScriptTask()
                                cVar5 = iVar8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_bc)
                                resources:ReleaseResource("")
                                return
                            end
                        end
                        bVar4 = false
                        if bVar4 ~= 0 then
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_bc)
                    else
                        bVar4 = me:MsgIsHitByHero()
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d57f5d end
                            quest:SetStateBool("EarlyHitWhisper", true)
                        end
                    end
                    iVar8 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    cVar5 = __native_entity_state:GetStateBool("RepeatMelee")
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d57f5d end
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 ~= 3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d57f5d end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d57f5d end
                        quest:SetStateBool("MeleeOpponentReset", false)
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK")
                        pCVar6 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar6)
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(me)
                        xStack_dc = quest:RegisterTimer()
                        quest:SetTimer(xStack_dc, 0xf)
                        iVar8 = quest:GetStateInt("TutorialState")
                        while (iVar8 == 3 and (quest:GetStateInt("GenericTutorialCounter") < 7)) do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            bVar4 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                            if bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                                iVar8 = quest:GetTimer(xStack_dc)
                                if iVar8 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    iVar9 = quest:AddNewConversation(me, false, false)
                                    pCVar6 = quest:GetHero()
                                    quest:AddPersonToConversation(iVar9, pCVar6)
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, pCVar6, false)
                                    quest:SetTimer(xStack_dc, 0xf)
                                end
                            else
                                bVar4 = me:MsgIsHitByHero()
                                if bVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    quest:ModifyThingHealth(me, 1000.0, false)
                                    CVar3 = 0x0
                                    if nil == nil then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        bVar4 = quest:IsXbox()
                                        if bVar4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_dc)
                                                resources:ReleaseResource(xStack_d8)
                                                return
                                            end
                                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP")
                                            bVar4 = quest:MsgIsGameInfoClickedPast()
                                            while not bVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if bVar4 then
                                                    quest:DeregisterTimer(xStack_dc)
                                                    resources:ReleaseResource(xStack_d8)
                                                    return
                                                end
                                                bVar4 = quest:MsgIsGameInfoClickedPast()
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_dc)
                                                resources:ReleaseResource(xStack_d8)
                                                return
                                            end
                                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC")
                                            bVar4 = quest:MsgIsGameInfoClickedPast()
                                            while not bVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if bVar4 then
                                                    quest:DeregisterTimer(xStack_dc)
                                                    resources:ReleaseResource(xStack_d8)
                                                    return
                                                end
                                                bVar4 = quest:MsgIsGameInfoClickedPast()
                                            end
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                    end
                                    -- TODO(native): xStack_c8 = (CCharString)(((int)CVar3 + 1) % 5);
                                end
                            end
                            iVar8 = quest:GetTimer(xStack_dc)
                            if iVar8 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                iVar9 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar9, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", me, pCVar6, false)
                                quest:SetTimer(xStack_dc, 0xf)
                            end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d57f7a: (native jump target)
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 ~= 4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE")
                        pCVar6 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar6)
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(me)
                        quest:SetTimer(xStack_dc, 0xf)
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 == 4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            bVar4 = quest:IsPlayerCreatureBlocking()
                            if bVar4 then
                                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 1);
                                pCVar6 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                bVar4 = pCVar6:MsgIsHitBy("MeleeOpponent")
                                if not bVar4 then
                                    bVar4 = false
                                    goto FLOW_after_lab_00d57276
                                end
                                bVar4 = true
                            else
                                -- LAB_00d57276: (native jump target)
                                bVar4 = false
                            end
                            ::FLOW_after_lab_00d57276::
                            if (xStack_bc & 1) ~= 0 then
                                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffe);
                            end
                            if bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                piVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x48)
                                -- TODO(native): *piVar1 = *piVar1 + 1;
                                iVar8 = quest:GetTimer(xStack_dc)
                                if iVar8 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    iVar9 = quest:AddNewConversation(me, false, false)
                                    pCVar6 = quest:GetHero()
                                    quest:AddPersonToConversation(iVar9, pCVar6)
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, pCVar6, false)
                                    -- LAB_00d57742: (native jump target)
                                    quest:SetTimer(xStack_dc, 0xf)
                                end
                            else
                                pCVar6 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                bVar4 = pCVar6:MsgIsHitBy("MeleeOpponent")
                                if bVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    if nil == nil then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if not bVar4 then
                                            r2 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                            iVar9 = quest:AddNewConversation(r2, false, false)
                                            pCVar6 = quest:GetHero()
                                            quest:AddPersonToConversation(iVar9, pCVar6)
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar9, "TEXT_QST_028_MAZE_BLOCK", r2, pCVar6, false)
                                            bVar4 = quest:IsXbox()
                                            if bVar4 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if not bVar4 then
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP")
                                                    bVar4 = quest:MsgIsGameInfoClickedPast()
                                                    while not bVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar4 = not alive
                                                        if bVar4 then goto LAB_00d57f71 end
                                                        bVar4 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    -- LAB_00d5754c: (native jump target)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar4 = not alive
                                                    if not bVar4 then
                                                        quest:SetTimer(xStack_dc, 0xf)
                                                        -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                                        goto LAB_00d5775c
                                                    end
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if not bVar4 then
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")
                                                    bVar4 = quest:MsgIsGameInfoClickedPast()
                                                    while not bVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar4 = not alive
                                                        if bVar4 then goto LAB_00d57f71 end
                                                        bVar4 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar4 = not alive
                                                    if not bVar4 then
                                                        quest:SetTimer(xStack_dc, 0xf)
                                                        -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                                        goto LAB_00d5775c
                                                    end
                                                    goto FLOW_after_lab_00d5754c
                                                end
                                            end
                                            ::FLOW_after_lab_00d5754c::
                                            ::LAB_00d57f71::
                                        end
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    uVar7 = uVar7 & 0x80000001
                                    if uVar7 < 0 then
                                        uVar7 = (uVar7 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar7 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        iVar8 = quest:GetTimer(xStack_dc)
                                        if iVar8 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_dc)
                                                resources:ReleaseResource(xStack_d8)
                                                return
                                            end
                                            iVar9 = quest:AddNewConversation(me, false, false)
                                            pCVar6 = quest:GetHero()
                                            quest:AddPersonToConversation(iVar9, pCVar6)
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, pCVar6, false)
                                            quest:SetTimer(xStack_dc, 0xf)
                                        end
                                    end
                                    -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                else
                                    bVar4 = me:MsgIsHitByHero()
                                    if bVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        iVar8 = quest:GetTimer(xStack_dc)
                                        if iVar8 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if not bVar4 then
                                                iVar9 = quest:AddNewConversation(me, false, false)
                                                pCVar6 = quest:GetHero()
                                                quest:AddPersonToConversation(iVar9, pCVar6)
                                                pCVar6 = quest:GetHero()
                                                quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", me, pCVar6, false)
                                                quest:SetTimer(xStack_dc, 0xf)
                                                goto FLOW_after_lab_00d57742
                                            end
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                    end
                                end
                            end
                            ::FLOW_after_lab_00d57742::
                            ::LAB_00d5775c::
                            iVar8 = quest:GetTimer(xStack_dc)
                            if iVar8 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                iVar9 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar9, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", me, pCVar6, false)
                                quest:SetTimer(xStack_dc, 0xf)
                            end
                            pCVar6 = quest:GetHero()
                            fret_00 = quest:GetHealth(pCVar6)
                            if fret_00 < quest:ReadGlobalGameDataFloat(0xed8) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                quest:ClearThingBestEnemyTarget(me)
                            end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 ~= 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                        CVar3 = xStack_dc
                        quest:SetTimer(xStack_dc, 0xf)
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 == 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            bVar4 = me:MsgIsHitByHero()
                            if bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_dc)
                                    resources:ReleaseResource(xStack_d8)
                                    return
                                end
                                iVar8 = quest:GetTimer(CVar3)
                                if iVar8 < 9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    uVar7 = uVar7 & 0x80000001
                                    if uVar7 < 0 then
                                        uVar7 = (uVar7 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar7 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        iVar9 = quest:AddNewConversation(me, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar9, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, pCVar6, false)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        r3 = quest:GetThingWithScriptName("MeleeThunder")
                                        iVar9 = quest:AddNewConversation(r3, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar9, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar9, "TEXT_QST_028_THUNDER_MELEE_DEFEND", r3, pCVar6, false)
                                        -- LAB_00d57e71: (native jump target)
                                    end
                                    -- LAB_00d57e76: (native jump target)
                                    quest:SetTimer(xStack_dc, 0xf)
                                end
                            else
                                bVar4 = quest:IsPlayerCreatureBlocking()
                                if bVar4 then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 2);
                                    pCVar6 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    bVar4 = pCVar6:MsgIsHitBy("MeleeOpponent")
                                    if not bVar4 then
                                        bVar4 = false
                                        goto FLOW_after_lab_00d57af8
                                    end
                                    bVar4 = true
                                else
                                    -- LAB_00d57af8: (native jump target)
                                    bVar4 = false
                                end
                                ::FLOW_after_lab_00d57af8::
                                if (xStack_bc & 2) ~= 0 then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffd);
                                end
                                if bVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                    iVar8 = quest:GetTimer(CVar3)
                                    if iVar8 < 9 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if not bVar4 then
                                            uVar7 = uVar7 & 0x80000001
                                            if uVar7 < 0 then
                                                uVar7 = (uVar7 - 1 | 0xfffffffe) + 1
                                            end
                                            if uVar7 == 1 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if not bVar4 then
                                                    iVar9 = quest:AddNewConversation(me, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar9, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, pCVar6, false)
                                                    quest:SetTimer(xStack_dc, 0xf)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar4 = not alive
                                                if not bVar4 then
                                                    r4 = quest:GetThingWithScriptName("MeleeThunder")
                                                    iVar9 = quest:AddNewConversation(r4, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar9, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar9, "TEXT_QST_028_THUNDER_MELEE_ATTACK", r4, pCVar6, false)
                                                    -- LAB_00d57e76_c40: (native jump target)
                                                    quest:SetTimer(xStack_dc, 0xf)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            end
                                        end
                                        quest:DeregisterTimer(xStack_dc)
                                        resources:ReleaseResource(xStack_d8)
                                        return
                                    end
                                else
                                    pCVar6 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    bVar4 = pCVar6:MsgIsHitBy("MeleeOpponent")
                                    if bVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                        iVar8 = quest:GetTimer(CVar3)
                                        if iVar8 < 9 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if not bVar4 then
                                                uVar7 = uVar7 & 0x80000001
                                                if uVar7 < 0 then
                                                    uVar7 = (uVar7 - 1 | 0xfffffffe) + 1
                                                end
                                                if uVar7 == 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar4 = not alive
                                                    if not bVar4 then
                                                        iVar9 = quest:AddNewConversation(me, false, false)
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddPersonToConversation(iVar9, pCVar6)
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar9, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, pCVar6, false)
                                                        quest:SetTimer(xStack_dc, 0xf)
                                                        goto FLOW_after_lab_00d57e76
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar4 = not alive
                                                    if not bVar4 then
                                                        r5 = quest:GetThingWithScriptName("MeleeThunder")
                                                        iVar9 = quest:AddNewConversation(r5, false, false)
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddPersonToConversation(iVar9, pCVar6)
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar9, "TEXT_QST_028_THUNDER_MELEE_FINISH", r5, pCVar6, false)
                                                        -- LAB_00d57e76_c44: (native jump target)
                                                        quest:SetTimer(xStack_dc, 0xf)
                                                        goto FLOW_after_lab_00d57e76
                                                    end
                                                end
                                            end
                                            quest:DeregisterTimer(xStack_dc)
                                            resources:ReleaseResource(xStack_d8)
                                            return
                                        end
                                    end
                                end
                            end
                            ::FLOW_after_lab_00d57e76::
                            CVar3 = xStack_dc
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        iVar8 = quest:GetStateInt("TutorialState")
                        while iVar8 ~= 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            iVar8 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        cVar5 = quest:GetStateBool("MeleeRepeatKnown")
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            cVar5 = quest:GetStateBool("MeleeRepeatKnown")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_dc)
                            resources:ReleaseResource(xStack_d8)
                            return
                        end
                        if not quest:GetStateBool("MeleeRepeating") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                            __native_entity_state:SetStateBool("RepeatMelee", false)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_dc)
                                resources:ReleaseResource(xStack_d8)
                                return
                            end
                        end
                        quest:SetStateBool("MeleeOpponentReset", true)
                        quest:DeregisterTimer(xStack_dc)
                        cVar5 = __native_entity_state:GetStateBool("RepeatMelee")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                end
                ::LAB_00d57f5d::
                resources:ReleaseResource(xStack_d8)
                return
            end
        end
    end
    ::LAB_00d5685e::
    resources:ReleaseResource(xStack_d8)
end

function Init(quest, me)
    quest:SetStateInt("TutorialState", 1)
    __native_entity_state:SetStateInt("BadHit", 0)
    __native_entity_state:SetStateBool("RepeatMelee", true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

