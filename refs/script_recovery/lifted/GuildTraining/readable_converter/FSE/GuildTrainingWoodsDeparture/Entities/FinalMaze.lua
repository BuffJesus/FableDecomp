-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local notFighting, notBeaten, beenHit

-- FinalMaze.Main (retail 0x00d647f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, scratchValue5, predicateResult4, predicateResult9, predicateResult
    local scratchValue, predicateResult15, predicateResult16, scratchValue19, addNewConversation
    local infoCounter, infoCounter2, scratchValue31, scratchValue32, resource, infoCounter3, timerId
    scratchValue31 = 0
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(4) then goto LAB_00d664b9 end
    notFighting = true
    notBeaten = true
    beenHit = 0
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, hero, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            me:ReleaseControl()
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        me:ReleaseControl()
        return
    end
    quest:StartCutscene({HERO = hero, MAZE = me}, {}, true)
    quest:RunCutscene("CS_GUILD_DEPARTURE_MAZE_START", true, false)
    quest:EndCutscene()
    resources:ReleaseResource(resource)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    infoCounter3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:EntitySetBossPhase(me, 0)
    quest:EntitySetAsDamageable(hero, false)
    quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    addNewConversation = 0
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue32 = scratchValue31 | 3
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult3 = true
        else
            scratchValue32 = scratchValue31 | 15
            predicateResult3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
        end
        if scratchValue32 & 8 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffff7
        end
        if scratchValue32 & 4 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffffb
        end
        if scratchValue32 & 2 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffffd
        end
        if scratchValue32 & 1 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffffe
        end
        if predicateResult3 then
            local getStateInt = beenHit + 1
            beenHit = getStateInt
            if getStateInt == 7 then
                notBeaten = false
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
            if not (quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)) then goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
            scratchValue31 = math.random(0, 32767) & 0x80000001
            scratchValue5 = scratchValue31 == 0
            if scratchValue31 < 0 then
                scratchValue5 = (scratchValue31 - 1 | 0xfffffffe) == 0xffffffff
            end
            if scratchValue5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
            end
            quest:SetTimer(timerId, 5)
        else
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult4 = true
                if quest:IsConversationActive(addNewConversation) then
                    predicateResult4 = false
                    goto FLOW_after_lab_00d64f56
                end
            else
                scratchValue31 = scratchValue32 | 240
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                    predicateResult4 = true
                    if not quest:IsConversationActive(addNewConversation) then
                        goto FLOW_after_lab_00d64f56
                    end
                end
                predicateResult4 = false
            end
            ::FLOW_after_lab_00d64f56::
            if scratchValue31 < 0 then
                scratchValue31 = scratchValue31 & 0xffffff7f
            end
            if scratchValue31 & 64 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffffbf
            end
            if scratchValue31 & 32 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffffdf
            end
            if scratchValue31 & 16 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffffef
            end
            if predicateResult4 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, hero, false)
                quest:ModifyThingHealth(me, 1000.0, false)
            elseif me:MsgIsHitByHeroSpecialAbility(me) then
                if not quest:IsConversationActive(addNewConversation) then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, hero, false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter3)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    if not me:AcquireControl(4) then goto LAB_00d664b0 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    resource = resources:StartMovie("")
    quest:StartMovieSequence()
    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    quest:EntitySetAsDrawable(hero, false)
    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
    if 0.0 < quest:GetHealth(me) then
        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST", GROUP_SELECT_FIRST, false, true, false)
        while me:IsPerformingScriptTask() do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                resources:DestroyMovie(resource)
                goto LAB_00d664b0
            end
        end
        if quest:IsActiveThreadTerminating() then
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(resource)
            goto LAB_00d664b0
        end
    end
    quest:EntitySetAsDrawable(hero, true)
    quest:FixMovieSequenceCamera(false)
    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
    resources:DestroyMovie(resource)
    quest:EntitySetBossPhase(me, 1)
    infoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter, beenHit, -1)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue32 = scratchValue31 | 768
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult9 = true
            if quest:IsConversationActive(addNewConversation) then
                predicateResult9 = false
                goto FLOW_after_lab_00d654e6
            end
        else
            scratchValue32 = scratchValue31 | 3840
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                predicateResult9 = true
                if not quest:IsConversationActive(addNewConversation) then
                    goto FLOW_after_lab_00d654e6
                end
            end
            predicateResult9 = false
        end
        ::FLOW_after_lab_00d654e6::
        if scratchValue32 & 2048 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffff7ff
        end
        if scratchValue32 & 1024 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffbff
        end
        if scratchValue32 & 512 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffdff
        end
        if scratchValue32 & 256 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffffeff
        end
        if predicateResult9 then
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, hero, false)
            quest:ModifyThingHealth(me, 1000.0, false)
        else
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult = true
            else
                scratchValue31 = scratchValue32 | 0xf000
                predicateResult = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
            end
            if scratchValue31 >> 8 < 0 then
                scratchValue31 = scratchValue31 & 0xffff7fff
            end
            if scratchValue31 & 0x4000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffbfff
            end
            if scratchValue31 & 0x2000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffdfff
            end
            if scratchValue31 & 4096 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffffefff
            end
            if predicateResult then
                local getStateInt2 = beenHit + 1
                beenHit = getStateInt2
                if getStateInt2 == 7 then
                    notBeaten = false
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
                quest:UpdateQuestInfoCounter(infoCounter, beenHit, -1)
                if not (quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)) then goto continue_3 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                scratchValue32 = math.random(0, 32767) & 0x80000001
                scratchValue = scratchValue32 == 0
                if scratchValue32 < 0 then
                    scratchValue = (scratchValue32 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                end
                quest:SetTimer(timerId, 5)
            elseif me:MsgIsHitByHeroSpecialAbility(me) then
                if not quest:IsConversationActive(addNewConversation) then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, hero, false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    if not me:AcquireControl(4) then goto LAB_00d664b0 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    resource = resources:StartMovie("")
    quest:StartMovieSequence()
    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    quest:EntitySetAsDrawable(hero, false)
    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
    if 0.0 < quest:GetHealth(me) then
        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST", GROUP_SELECT_FIRST, false, true, false)
        while me:IsPerformingScriptTask() do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                resources:DestroyMovie(resource)
                goto LAB_00d664b0
            end
        end
        if quest:IsActiveThreadTerminating() then
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(resource)
            goto LAB_00d664b0
        end
    end
    quest:EntitySetAsDrawable(hero, true)
    quest:FixMovieSequenceCamera(false)
    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
    resources:DestroyMovie(resource)
    quest:EntitySetBossPhase(me, 2)
    infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter2, beenHit, -1)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue32 = scratchValue31 | 0x30000
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult15 = true
            if quest:IsConversationActive(addNewConversation) then
                predicateResult15 = false
                goto FLOW_after_lab_00d65d66
            end
        else
            scratchValue32 = scratchValue31 | 0xf0000
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                predicateResult15 = true
                if not quest:IsConversationActive(addNewConversation) then
                    goto FLOW_after_lab_00d65d66
                end
            end
            predicateResult15 = false
        end
        ::FLOW_after_lab_00d65d66::
        if scratchValue32 & 0x80000 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfff7ffff
        end
        if scratchValue32 & 0x40000 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffbffff
        end
        if scratchValue32 & 0x20000 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffdffff
        end
        if scratchValue32 & 0x10000 ~= 0 then
            scratchValue32 = scratchValue32 & 0xfffeffff
        end
        if predicateResult15 then
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, hero, false)
            quest:ModifyThingHealth(me, 1000.0, false)
        else
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult16 = true
                if quest:IsConversationActive(addNewConversation) then
                    predicateResult16 = false
                    goto FLOW_after_lab_00d65f0c
                end
            else
                scratchValue31 = scratchValue32 | 0xf00000
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                    predicateResult16 = true
                    if not quest:IsConversationActive(addNewConversation) then
                        goto FLOW_after_lab_00d65f0c
                    end
                end
                predicateResult16 = false
            end
            ::FLOW_after_lab_00d65f0c::
            if scratchValue31 & 0x800000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xff7fffff
            end
            if scratchValue31 & 0x400000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffbfffff
            end
            if scratchValue31 & 0x200000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffdfffff
            end
            if scratchValue31 & 0x100000 ~= 0 then
                scratchValue31 = scratchValue31 & 0xffefffff
            end
            if predicateResult16 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, hero, false)
                quest:ModifyThingHealth(me, 1000.0, false)
            else
                if me:MsgIsHitByHeroSpecialAbility(me) then
                    local getStateInt3 = beenHit + 1
                    beenHit = getStateInt3
                    if getStateInt3 == 7 then
                        notBeaten = false
                        quest:ModifyThingHealth(me, 1000.0, false)
                    end
                    quest:UpdateQuestInfoCounter(infoCounter2, beenHit, -1)
                    if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                        scratchValue32 = math.random(0, 32767) & 0x80000001
                        scratchValue19 = scratchValue32 == 0
                        if scratchValue32 < 0 then
                            scratchValue19 = (scratchValue32 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if scratchValue19 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                        end
                        quest:SetTimer(timerId, 5)
                    end
            end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter2)
    quest:DisplayQuestInfo(false)
    if not me:AcquireControl(4) then goto LAB_00d664b0 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    me:ClearCommands()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); goto FLOW_after_lab_00d6632b end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
    else
        quest:StartCutscene({HERO = hero, MAZE = me}, {}, true)
        quest:RunCutscene("CS_GUILD_DEPARTURE_MAZE_WIN", true, false)
        quest:EndCutscene()
        resources:ReleaseResource(resource)
        quest:SetStateBool("MissionSucceeded", true)
        quest:ModifyThingHealth(me, 1000.0, false)
        quest:RemoveThing(me, false, true)
        quest:EntitySetAsDamageable(hero, true)
    end
    ::FLOW_after_lab_00d6632b::
    ::LAB_00d664b0::
    quest:DeregisterTimer(timerId)
    ::LAB_00d664b9::
    me:ReleaseControl()
end

-- FinalMaze.Init (retail 0x00d62320)
function Init(quest, me)
end

-- FinalMaze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- FinalMaze.OnPredicateFail (retail 0x00d62330)
function OnPredicateFail(quest, me)
end

