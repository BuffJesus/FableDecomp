-- Generated native draft: FinalMaze. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar3, bVar4, cVar5, fVar17, fVar2, fVar20, fret_0, fret_00, iVar14, iVar18, iVar19, iVar21, iVar22, pCVar10, pCVar6, pCVar8, pCVar9, pcVar15, pppuVar16, r1, r2, r3, r4, r5, uVar12, uVar13, xStack_10, xStack_1c, xStack_38, xStack_5c, xStack_70, xStack_80
    local alive = true
    uVar12 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_70 = resources:NewResource()
    resources:PrepareResource(xStack_70)
    bVar3 = resources:TryAcquire(xStack_70, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d664b9 end
        bVar3 = resources:TryAcquire(xStack_70, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d664b9 end
    __native_entity_state:SetStateBool("NotFighting", true)
    __native_entity_state:SetStateBool("NotBeaten", true)
    __native_entity_state:SetStateInt("BeenHit", 0)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    xStack_38 = resources:NewResource()
    resources:PrepareResource(xStack_38)
    iVar19 = 4
    pppuVar16 = xStack_38
    pCVar6 = quest:GetHero()
    bVar3 = resources:TryAcquire(pppuVar16, pCVar6, iVar19)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_38)
            resources:ReleaseResource(xStack_70)
            return
        end
        iVar19 = 4
        pppuVar16 = xStack_38
        pCVar6 = quest:GetHero()
        bVar3 = resources:TryAcquire(pppuVar16, pCVar6, iVar19)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d649a2: (native jump target)
        resources:ReleaseResource(xStack_38)
        resources:ReleaseResource(xStack_70)
        return
    end
    xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_38)
    resources:SetActor(xStack_1c, "MAZE", xStack_70)
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", xStack_1c, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_38)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    xStack_5c = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    resources:PrepareResource(xStack_70)
    quest:EntitySetBossPhase(me, 0)
    bVar3 = false
    pCVar6 = quest:GetHero()
    quest:EntitySetAsDamageable(pCVar6, bVar3)
    quest:UpdateQuestInfoCounter(xStack_5c, __native_entity_state:GetStateInt("BeenHit"), -1)
    xStack_80 = quest:RegisterTimer()
    quest:SetTimer(xStack_80, 0)
    fVar20 = 20.0
    fVar17 = 5.0
    pCVar8 = me:GetPos()
    r1 = quest:EntityWillTeleportToArea(me, pCVar8.x, fVar17, fVar20)
    quest:CacheMusicSet(0x2f)
    iVar14 = 0
    cVar5 = __native_entity_state:GetStateBool("NotBeaten")
    while cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d664b0 end
        uVar13 = uVar12 | 3
        bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
        if bVar3 then
            goto LAB_00d64cba
        else
            uVar13 = uVar12 | 0xf
            bVar4 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
            bVar3 = false
            if bVar4 then goto LAB_00d64cba end
        end
        goto FLOW_past_lab_00d64cba
        ::LAB_00d64cba::
        bVar3 = true
        ::FLOW_past_lab_00d64cba::
        if (uVar13 & 8) ~= 0 then
            uVar13 = uVar13 & 0xfffffff7
        end
        if (uVar13 & 4) ~= 0 then
            uVar13 = uVar13 & 0xfffffffb
        end
        if (uVar13 & 2) ~= 0 then
            uVar13 = uVar13 & 0xfffffffd
        end
        if (uVar13 & 1) ~= 0 then
            uVar13 = uVar13 & 0xfffffffe
        end
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d664b0 end
            iVar19 = __native_entity_state:GetStateInt("BeenHit") + 1
            __native_entity_state:SetStateInt("BeenHit", iVar19)
            if iVar19 == 7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d664b0 end
                __native_entity_state:SetStateBool("NotBeaten", false)
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(xStack_5c, __native_entity_state:GetStateInt("BeenHit"), -1)
            iVar19 = quest:GetTimer(xStack_80)
            uVar12 = uVar13
            __native_condition_1 = iVar19 < 1
            if __native_condition_1 then
                bVar3 = quest:IsConversationActive(iVar14)
                __native_condition_1 = not bVar3
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d664b0 end
                uVar12 = math.random(0, 32767)
                uVar12 = uVar12 & 0x80000001
                bVar3 = uVar12 == 0
                if uVar12 < 0 then
                    bVar3 = (uVar12 - 1 | 0xfffffffe) == 0xffffffff
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d664b0 end
                    iVar14 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar14, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, pCVar6, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d664b0 end
                    iVar14 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar14, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, pCVar6, false)
                end
                quest:SetTimer(xStack_80, 5)
            end
        else
            uVar12 = uVar13 | 0x30
            bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
            if bVar3 then
                goto LAB_00d64f41
            else
                uVar12 = uVar13 | 0xf0
                bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                if bVar3 then goto LAB_00d64f41 end
                goto LAB_00d64f56
            end
            goto FLOW_past_lab_00d64f41
            ::LAB_00d64f41::
            bVar4 = quest:IsConversationActive(iVar14)
            bVar3 = true
            if bVar4 then goto LAB_00d64f56 end
            ::FLOW_past_lab_00d64f41::
            goto FLOW_past_lab_00d64f56
            ::LAB_00d64f56::
            bVar3 = false
            ::FLOW_past_lab_00d64f56::
            if uVar12 < 0 then
                uVar12 = uVar12 & 0xffffff7f
            end
            if (uVar12 & 0x40) ~= 0 then
                uVar12 = uVar12 & 0xffffffbf
            end
            if (uVar12 & 0x20) ~= 0 then
                uVar12 = uVar12 & 0xffffffdf
            end
            if (uVar12 & 0x10) ~= 0 then
                uVar12 = uVar12 & 0xffffffef
            end
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d664b0 end
                iVar14 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar14, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, pCVar6, false)
                quest:ModifyThingHealth(me, 1000.0, false)
            else
                bVar3 = me:MsgIsHitByHeroSpecialAbility(0xb)
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d664b0 end
                    bVar3 = quest:IsConversationActive(iVar14)
                    if not bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d664b0 end
                        iVar14 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar14, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, pCVar6, false)
                    end
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
            end
        end
        cVar5 = __native_entity_state:GetStateBool("NotBeaten")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:RemoveQuestInfoElement(xStack_5c)
        quest:DisplayQuestInfo(false)
        __native_entity_state:SetStateInt("BeenHit", 0)
        __native_entity_state:SetStateBool("NotBeaten", true)
        resources:PrepareResource(xStack_70)
        bVar3 = resources:TryAcquire(xStack_70, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d664b0 end
            bVar3 = resources:TryAcquire(xStack_70, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_38 = resources:StartMovie("")
            -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
            quest:FixMovieSequenceCamera(true)
            quest:Pause(0.5)
            bVar3 = false
            pCVar6 = quest:GetHero()
            quest:EntitySetAsDrawable(pCVar6, bVar3)
            iVar21 = -1
            iVar18 = 0
            fVar17 = -1.0
            pCVar9 = quest:GetThingWithScriptName("CAM_RC_MAZE")
            quest:CameraUseCameraPoint(pCVar9, me, fVar17, iVar18, iVar21)
            pCVar9 = resources:ScriptThing(xStack_70)
            pCVar6 = pCVar9
            fret_0 = quest:GetHealth(pCVar6)
            fVar2 = 0.0
            if fVar2 < fret_0 then
                iVar22 = 0
                iVar21 = 1
                iVar18 = 0
                iVar19 = 0
                pcVar15 = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST"
                pCVar6 = quest:GetHero()
                r2 = me:Speak(pCVar6, pcVar15, iVar19, (iVar18 ~= 0), (iVar21 ~= 0), (iVar22 ~= 0))
                iVar19 = me:IsPerformingScriptTask()
                cVar5 = iVar19
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d65b9f end
                    iVar19 = me:IsPerformingScriptTask()
                    cVar5 = iVar19
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    goto LAB_00d65b9f
                end
                goto FLOW_past_lab_00d65b9f
                ::LAB_00d65b9f::
                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                resources:DestroyMovie(xStack_38)
                goto LAB_00d664b0
                ::FLOW_past_lab_00d65b9f::
            end
            bVar3 = true
            pCVar6 = quest:GetHero()
            quest:EntitySetAsDrawable(pCVar6, bVar3)
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(xStack_38)
            resources:PrepareResource(xStack_70)
            quest:EntitySetBossPhase(me, 1)
            iVar19 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
            xStack_5c = iVar19
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(iVar19, __native_entity_state:GetStateInt("BeenHit"), -1)
            fVar20 = 20.0
            fVar17 = 5.0
            pCVar8 = me:GetPos()
            r3 = quest:EntityWillTeleportToArea(me, pCVar8.x, fVar17, fVar20)
            cVar5 = __native_entity_state:GetStateBool("NotBeaten")
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d664b0 end
                uVar13 = uVar12 | 0x300
                bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                if bVar3 then
                    goto LAB_00d654d1
                else
                    uVar13 = uVar12 | 0xf00
                    bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
                    if bVar3 then goto LAB_00d654d1 end
                    goto LAB_00d654e6
                end
                goto FLOW_past_lab_00d654d1
                ::LAB_00d654d1::
                bVar4 = quest:IsConversationActive(iVar14)
                bVar3 = true
                if bVar4 then goto LAB_00d654e6 end
                ::FLOW_past_lab_00d654d1::
                goto FLOW_past_lab_00d654e6
                ::LAB_00d654e6::
                bVar3 = false
                ::FLOW_past_lab_00d654e6::
                if (uVar13 & 0x800) ~= 0 then
                    uVar13 = uVar13 & 0xfffff7ff
                end
                if (uVar13 & 0x400) ~= 0 then
                    uVar13 = uVar13 & 0xfffffbff
                end
                if (uVar13 & 0x200) ~= 0 then
                    uVar13 = uVar13 & 0xfffffdff
                end
                if (uVar13 & 0x100) ~= 0 then
                    uVar13 = uVar13 & 0xfffffeff
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d664b0 end
                    iVar14 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar14, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, pCVar6, false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                else
                    uVar12 = uVar13 | 0x3000
                    bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
                    if bVar3 then
                        goto LAB_00d65670
                    else
                        uVar12 = uVar13 | 0xf000
                        bVar4 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                        bVar3 = false
                        if bVar4 then goto LAB_00d65670 end
                    end
                    goto FLOW_past_lab_00d65670
                    ::LAB_00d65670::
                    bVar3 = true
                    ::FLOW_past_lab_00d65670::
                    if (uVar12 >> 8) < 0 then
                        uVar12 = uVar12 & 0xffff7fff
                    end
                    if (uVar12 & 0x4000) ~= 0 then
                        uVar12 = uVar12 & 0xffffbfff
                    end
                    if (uVar12 & 0x2000) ~= 0 then
                        uVar12 = uVar12 & 0xffffdfff
                    end
                    if (uVar12 & 0x1000) ~= 0 then
                        uVar12 = uVar12 & 0xffffefff
                    end
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d664b0 end
                        iVar19 = __native_entity_state:GetStateInt("BeenHit") + 1
                        __native_entity_state:SetStateInt("BeenHit", iVar19)
                        if iVar19 == 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d664b0 end
                            __native_entity_state:SetStateBool("NotBeaten", false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        end
                        quest:UpdateQuestInfoCounter(xStack_5c, __native_entity_state:GetStateInt("BeenHit"), -1)
                        iVar19 = quest:GetTimer(xStack_80)
                        __native_condition_2 = iVar19 < 1
                        if __native_condition_2 then
                            bVar3 = quest:IsConversationActive(iVar14)
                            __native_condition_2 = not bVar3
                        end
                        if __native_condition_2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d664b0 end
                            uVar13 = math.random(0, 32767)
                            uVar13 = uVar13 & 0x80000001
                            bVar3 = uVar13 == 0
                            if uVar13 < 0 then
                                bVar3 = (uVar13 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d664b0 end
                                iVar14 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar14, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, pCVar6, false)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d664b0 end
                                iVar14 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar14, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, pCVar6, false)
                            end
                            quest:SetTimer(xStack_80, 5)
                        end
                    else
                        bVar3 = me:MsgIsHitByHeroSpecialAbility(0xb)
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d664b0 end
                            bVar3 = quest:IsConversationActive(iVar14)
                            if not bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d664b0 end
                                iVar14 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar14, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, pCVar6, false)
                            end
                            quest:ModifyThingHealth(me, 1000.0, false)
                        end
                    end
                end
                iVar19 = xStack_5c
                cVar5 = __native_entity_state:GetStateBool("NotBeaten")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:RemoveQuestInfoElement(iVar19)
                quest:DisplayQuestInfo(false)
                __native_entity_state:SetStateInt("BeenHit", 0)
                __native_entity_state:SetStateBool("NotBeaten", true)
                resources:PrepareResource(xStack_70)
                bVar3 = resources:TryAcquire(xStack_70, me, 4)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d664b0 end
                    bVar3 = resources:TryAcquire(xStack_70, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    xStack_38 = resources:StartMovie("")
                    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
                    quest:FixMovieSequenceCamera(true)
                    quest:Pause(0.5)
                    bVar3 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetAsDrawable(pCVar6, bVar3)
                    iVar21 = -1
                    iVar18 = 0
                    fVar17 = -1.0
                    pCVar9 = quest:GetThingWithScriptName("CAM_RC_MAZE")
                    quest:CameraUseCameraPoint(pCVar9, me, fVar17, iVar18, iVar21)
                    pCVar9 = resources:ScriptThing(xStack_70)
                    pCVar6 = pCVar9
                    fret_00 = quest:GetHealth(pCVar6)
                    fVar2 = 0.0
                    if fVar2 < fret_00 then
                        iVar22 = 0
                        iVar21 = 1
                        iVar18 = 0
                        iVar19 = 0
                        pcVar15 = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST"
                        pCVar6 = quest:GetHero()
                        r4 = me:Speak(pCVar6, pcVar15, iVar19, (iVar18 ~= 0), (iVar21 ~= 0), (iVar22 ~= 0))
                        iVar19 = me:IsPerformingScriptTask()
                        cVar5 = iVar19
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                                resources:DestroyMovie(xStack_38)
                                goto LAB_00d664b0
                            end
                            iVar19 = me:IsPerformingScriptTask()
                            cVar5 = iVar19
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            -- LAB_00d65b83: (native jump target)
                            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                            resources:DestroyMovie(xStack_38)
                            goto LAB_00d664b0
                        end
                    end
                    bVar3 = true
                    pCVar6 = quest:GetHero()
                    quest:EntitySetAsDrawable(pCVar6, bVar3)
                    quest:FixMovieSequenceCamera(false)
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(xStack_38)
                    resources:PrepareResource(xStack_70)
                    quest:EntitySetBossPhase(me, 2)
                    iVar19 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
                    xStack_5c = iVar19
                    quest:DisplayQuestInfo(true)
                    quest:UpdateQuestInfoCounter(iVar19, __native_entity_state:GetStateInt("BeenHit"), -1)
                    fVar20 = 20.0
                    fVar17 = 5.0
                    pCVar8 = me:GetPos()
                    r5 = quest:EntityWillTeleportToArea(me, pCVar8.x, fVar17, fVar20)
                    cVar5 = __native_entity_state:GetStateBool("NotBeaten")
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d664b0 end
                        uVar13 = uVar12 | 0x30000
                        bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                        if bVar3 then
                            goto LAB_00d65d51
                        else
                            uVar13 = uVar12 | 0xf0000
                            bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
                            if bVar3 then goto LAB_00d65d51 end
                            goto LAB_00d65d66
                        end
                        goto FLOW_past_lab_00d65d51
                        ::LAB_00d65d51::
                        bVar4 = quest:IsConversationActive(iVar14)
                        bVar3 = true
                        if bVar4 then goto LAB_00d65d66 end
                        ::FLOW_past_lab_00d65d51::
                        goto FLOW_past_lab_00d65d66
                        ::LAB_00d65d66::
                        bVar3 = false
                        ::FLOW_past_lab_00d65d66::
                        if (uVar13 & 0x80000) ~= 0 then
                            uVar13 = uVar13 & 0xfff7ffff
                        end
                        if (uVar13 & 0x40000) ~= 0 then
                            uVar13 = uVar13 & 0xfffbffff
                        end
                        if (uVar13 & 0x20000) ~= 0 then
                            uVar13 = uVar13 & 0xfffdffff
                        end
                        if (uVar13 & 0x10000) ~= 0 then
                            uVar13 = uVar13 & 0xfffeffff
                        end
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d664b0 end
                            iVar14 = quest:AddNewConversation(me, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar14, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, pCVar6, false)
                            goto LAB_00d65e4c
                        else
                            uVar12 = uVar13 | 0x300000
                            bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
                            if bVar3 then
                                goto LAB_00d65ef7
                            else
                                uVar12 = uVar13 | 0xf00000
                                bVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                                if bVar3 then goto LAB_00d65ef7 end
                                goto LAB_00d65f0c
                            end
                            goto FLOW_past_lab_00d65ef7
                            ::LAB_00d65ef7::
                            bVar4 = quest:IsConversationActive(iVar14)
                            bVar3 = true
                            if bVar4 then goto LAB_00d65f0c end
                            ::FLOW_past_lab_00d65ef7::
                            goto FLOW_past_lab_00d65f0c
                            ::LAB_00d65f0c::
                            bVar3 = false
                            ::FLOW_past_lab_00d65f0c::
                            if (uVar12 & 0x800000) ~= 0 then
                                uVar12 = uVar12 & 0xff7fffff
                            end
                            if (uVar12 & 0x400000) ~= 0 then
                                uVar12 = uVar12 & 0xffbfffff
                            end
                            if (uVar12 & 0x200000) ~= 0 then
                                uVar12 = uVar12 & 0xffdfffff
                            end
                            if (uVar12 & 0x100000) ~= 0 then
                                uVar12 = uVar12 & 0xffefffff
                            end
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    iVar14 = quest:AddNewConversation(me, false, false)
                                    pCVar6 = quest:GetHero()
                                    quest:AddPersonToConversation(iVar14, pCVar6)
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, pCVar6, false)
                                    goto LAB_00d65e4c
                                end
                                goto LAB_00d664b0
                            end
                            bVar3 = me:MsgIsHitByHeroSpecialAbility(0xb)
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d664b0 end
                                iVar19 = __native_entity_state:GetStateInt("BeenHit") + 1
                                __native_entity_state:SetStateInt("BeenHit", iVar19)
                                if iVar19 == 7 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d664b0 end
                                    __native_entity_state:SetStateBool("NotBeaten", false)
                                    quest:ModifyThingHealth(me, 1000.0, false)
                                end
                                quest:UpdateQuestInfoCounter(xStack_5c, __native_entity_state:GetStateInt("BeenHit"), -1)
                                iVar19 = quest:GetTimer(xStack_80)
                                __native_condition_3 = iVar19 < 1
                                if __native_condition_3 then
                                    bVar3 = quest:IsConversationActive(iVar14)
                                    __native_condition_3 = not bVar3
                                end
                                if __native_condition_3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d664b0 end
                                    uVar13 = math.random(0, 32767)
                                    uVar13 = uVar13 & 0x80000001
                                    bVar3 = uVar13 == 0
                                    if uVar13 < 0 then
                                        bVar3 = (uVar13 - 1 | 0xfffffffe) == 0xffffffff
                                    end
                                    if bVar3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d664b0 end
                                        iVar14 = quest:AddNewConversation(me, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar14, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, pCVar6, false)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d664b0 end
                                        iVar14 = quest:AddNewConversation(me, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar14, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar14, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, pCVar6, false)
                                    end
                                    quest:SetTimer(xStack_80, 5)
                                end
                            end
                        end
                        goto FLOW_past_lab_00d65e4c
                        ::LAB_00d65e4c::
                        quest:ModifyThingHealth(me, 1000.0, false)
                        ::FLOW_past_lab_00d65e4c::
                        iVar19 = xStack_5c
                        cVar5 = __native_entity_state:GetStateBool("NotBeaten")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:RemoveQuestInfoElement(iVar19)
                        quest:DisplayQuestInfo(false)
                        resources:PrepareResource(xStack_70)
                        bVar3 = resources:TryAcquire(xStack_70, me, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d664b0 end
                            bVar3 = resources:TryAcquire(xStack_70, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            me:ClearCommands()
                            quest:EntitySetInFaction(me, "FACTION_HERO")
                            xStack_38 = resources:NewResource()
                            resources:PrepareResource(xStack_38)
                            iVar19 = 4
                            pppuVar16 = xStack_38
                            pCVar6 = quest:GetHero()
                            bVar3 = resources:TryAcquire(pppuVar16, pCVar6, iVar19)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d6632b end
                                iVar19 = 4
                                pppuVar16 = xStack_38
                                pCVar6 = quest:GetHero()
                                bVar3 = resources:TryAcquire(pppuVar16, pCVar6, iVar19)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                goto LAB_00d6632b
                            else
                                pCVar9 = resources:NewActorMap()
                                resources:SetActor(pCVar9, "HERO", xStack_38)
                                resources:SetActor(pCVar9, "MAZE", xStack_70)
                                xStack_10 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_WIN", pCVar9, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                resources:DestroyActorMap(pCVar9)
                                resources:ReleaseResource(xStack_38)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:ModifyThingHealth(me, 1000.0, false)
                                quest:RemoveThing(me, false, true)
                                bVar3 = true
                                pCVar10 = quest:GetHero()
                                quest:EntitySetAsDamageable(pCVar10, bVar3)
                            end
                            goto FLOW_past_lab_00d6632b
                            ::LAB_00d6632b::
                            resources:ReleaseResource(xStack_38)
                            ::FLOW_past_lab_00d6632b::
                        end
                    end
                end
            end
        end
    end
    ::LAB_00d664b0::
    quest:DeregisterTimer(xStack_80)
    ::LAB_00d664b9::
    resources:ReleaseResource(xStack_70)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

