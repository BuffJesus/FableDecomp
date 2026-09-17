-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- FinalMaze.Main (retail 0x00d647f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult3, scratchValue5, predicateResult4, predicateResult9, predicateResult10
    local scratchValue12, predicateResult15, predicateResult16, scratchValue19, notBeaten2
    local notBeaten3, scratchValue24, getStateInt, scratchValue25, getStateInt2, scratchValue26
    local getStateInt3, scratchValue30, scratchValue34, scratchValue35, scratchValue36
    local scratchValue37, u_stk_78_1, scratchValue38, scratchValue39, scratchValue40, scratchValue41
    local scratchValue42, scratchValue43, timerId
    scratchValue34 = 0
    u_stk_78_1 = 0
    if not quest:NewScriptFrame(me) then return end
    scratchValue43 = resources:NewResource()
    while not resources:TryAcquire(scratchValue43, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b9 end
    end
    state:SetBool("NotFighting", true)
    state:SetBool("NotBeaten", true)
    state:SetInt("BeenHit", 0)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    scratchValue41 = resources:NewResource()
    while not resources:TryAcquire(scratchValue41, quest:GetHero(), 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(scratchValue41)
            resources:ReleaseResource(scratchValue43)
            return
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue41)
        resources:ReleaseResource(scratchValue43)
        return
    end
    scratchValue40 = resources:NewActorMap()
    resources:SetActor(scratchValue40, "HERO", scratchValue41)
    resources:SetActor(scratchValue40, "MAZE", scratchValue43)
    scratchValue38 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", scratchValue40, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue38)
    resources:DestroyActorMap(scratchValue40)
    resources:ReleaseResource(scratchValue41)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    scratchValue42 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:EntitySetBossPhase(me, 0)
    quest:EntitySetAsDamageable(quest:GetHero(), false)
    quest:UpdateQuestInfoCounter(scratchValue42, state:GetInt("BeenHit"), -1)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    scratchValue24 = 0
    while state:GetBool("NotBeaten") do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue37 = scratchValue34 | 3
        u_stk_78_1 = scratchValue37
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult3 = true
        else
            scratchValue37 = scratchValue34 | 15
            u_stk_78_1 = scratchValue37
            predicateResult3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
        end
        if scratchValue37 & true then
            scratchValue37 = scratchValue37 & 0xfffffff7
            u_stk_78_1 = scratchValue37
        end
        if scratchValue37 & true then
            scratchValue37 = scratchValue37 & 0xfffffffb
            u_stk_78_1 = scratchValue37
        end
        if scratchValue37 & true then
            scratchValue37 = scratchValue37 & 0xfffffffd
            u_stk_78_1 = scratchValue37
        end
        if scratchValue37 & true then
            scratchValue37 = scratchValue37 & 0xfffffffe
            u_stk_78_1 = scratchValue37
        end
        if predicateResult3 then
            getStateInt = state:GetInt("BeenHit") + 1
            state:SetInt("BeenHit", getStateInt)
            if getStateInt == 7 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                state:SetBool("NotBeaten", false)
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(scratchValue42, state:GetInt("BeenHit"), -1)
            scratchValue34 = scratchValue37
            if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                scratchValue34 = scratchValue34 & 0x80000001
                scratchValue5 = scratchValue34 == 0
                if scratchValue34 < 0 then
                    scratchValue5 = (scratchValue34 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue5 then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, quest:GetHero(), false)
                else
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, quest:GetHero(), false)
                end
                quest:SetTimer(timerId, 5)
            end
        else
            scratchValue34 = scratchValue37 | 48
            u_stk_78_1 = scratchValue34
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult4 = true
                if quest:IsConversationActive(scratchValue24) then
                    predicateResult4 = false
                    goto FLOW_after_lab_00d64f56
                end
            else
                scratchValue34 = scratchValue37 | 240
                u_stk_78_1 = scratchValue34
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                    predicateResult4 = true
                    if not quest:IsConversationActive(scratchValue24) then
                        goto FLOW_after_lab_00d64f56
                    end
                end
                predicateResult4 = false
            end
            ::FLOW_after_lab_00d64f56::
            if scratchValue34 < 0 then
                scratchValue34 = scratchValue34 & 0xffffff7f
                u_stk_78_1 = scratchValue34
            end
            if scratchValue34 & true then
                scratchValue34 = scratchValue34 & 0xffffffbf
                u_stk_78_1 = scratchValue34
            end
            if scratchValue34 & true then
                scratchValue34 = scratchValue34 & 0xffffffdf
                u_stk_78_1 = scratchValue34
            end
            if scratchValue34 & true then
                scratchValue34 = scratchValue34 & 0xffffffef
                u_stk_78_1 = scratchValue34
            end
            if predicateResult4 then
                scratchValue24 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, quest:GetHero(), false)
                quest:ModifyThingHealth(me, 1000.0, false)
            elseif me:MsgIsHitByHeroSpecialAbility(me) then
                if not quest:IsConversationActive(scratchValue24) then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, quest:GetHero(), false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveQuestInfoElement(scratchValue42)
        quest:DisplayQuestInfo(false)
        state:SetInt("BeenHit", 0)
        state:SetBool("NotBeaten", true)
        while not resources:TryAcquire(scratchValue43, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue41 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
            quest:FixMovieSequenceCamera(true)
            quest:Pause(0.5)
            quest:EntitySetAsDrawable(quest:GetHero(), false)
            quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
            quest:GetHealth(resources:ScriptThing(scratchValue43))
            if 0.0 < fret_0 then
                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                        resources:DestroyMovie(scratchValue41)
                        goto LAB_00d664b0
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue41)
                    goto LAB_00d664b0
                end
            end
            quest:EntitySetAsDrawable(quest:GetHero(), true)
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(scratchValue41)
            quest:EntitySetBossPhase(me, 1)
            scratchValue25 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(scratchValue25, state:GetInt("BeenHit"), -1)
            quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
            notBeaten2 = state:GetBool("NotBeaten")
            scratchValue35 = u_stk_78_1
            while notBeaten2 do
                if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                scratchValue37 = scratchValue35 | 768
                u_stk_78_1 = scratchValue37
                if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                    predicateResult9 = true
                    if quest:IsConversationActive(scratchValue24) then
                        predicateResult9 = false
                        goto FLOW_after_lab_00d654e6
                    end
                else
                    scratchValue37 = scratchValue35 | 3840
                    u_stk_78_1 = scratchValue37
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                        predicateResult9 = true
                        if not quest:IsConversationActive(scratchValue24) then
                            goto FLOW_after_lab_00d654e6
                        end
                    end
                    predicateResult9 = false
                end
                ::FLOW_after_lab_00d654e6::
                if scratchValue37 & true then
                    scratchValue37 = scratchValue37 & 0xfffff7ff
                    u_stk_78_1 = scratchValue37
                end
                if scratchValue37 & true then
                    scratchValue37 = scratchValue37 & 0xfffffbff
                    u_stk_78_1 = scratchValue37
                end
                if scratchValue37 & true then
                    scratchValue37 = scratchValue37 & 0xfffffdff
                    u_stk_78_1 = scratchValue37
                end
                if scratchValue37 & true then
                    scratchValue37 = scratchValue37 & 0xfffffeff
                    u_stk_78_1 = scratchValue37
                end
                if predicateResult9 then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, quest:GetHero(), false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                else
                    scratchValue35 = scratchValue37 | 0x3000
                    u_stk_78_1 = scratchValue35
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                        predicateResult10 = true
                    else
                        scratchValue35 = scratchValue37 | 0xf000
                        u_stk_78_1 = scratchValue35
                        predicateResult10 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                    end
                    if scratchValue35 >> 8 < 0 then
                        scratchValue35 = scratchValue35 & 0xffff7fff
                        u_stk_78_1 = scratchValue35
                    end
                    if scratchValue35 & 0x4000 ~= 0 then
                        scratchValue35 = scratchValue35 & 0xffffbfff
                        u_stk_78_1 = scratchValue35
                    end
                    if scratchValue35 & 0x2000 ~= 0 then
                        scratchValue35 = scratchValue35 & 0xffffdfff
                        u_stk_78_1 = scratchValue35
                    end
                    if scratchValue35 & true then
                        scratchValue35 = scratchValue35 & 0xffffefff
                        u_stk_78_1 = scratchValue35
                    end
                    if predicateResult10 then
                        getStateInt2 = state:GetInt("BeenHit") + 1
                        state:SetInt("BeenHit", getStateInt2)
                        if getStateInt2 == 7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            state:SetBool("NotBeaten", false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        end
                        quest:UpdateQuestInfoCounter(state:GetInt("BeenHit"), -1, 0.0)
                        if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            scratchValue37 = scratchValue37 & 0x80000001
                            scratchValue12 = scratchValue37 == 0
                            if scratchValue37 < 0 then
                                scratchValue12 = (scratchValue37 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if scratchValue12 then
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                -- TODO(native): (**(code **)((int)xStack_7c + 0x5b4))(*(void **)(this + 4),iVar14,pCVar6);
                                -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                -- TODO(native): (**(code **)((int)xStack_7c + 0x5b8))(*(void **)(this + 4),iVar14,&xStack_44,false,pCVar10,pCVar6);
                            else
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                -- TODO(native): (**(code **)((int)xStack_7c + 0x5b4))(*(void **)(this + 4),iVar14,pCVar6);
                                -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                -- TODO(native): (**(code **)((int)xStack_7c + 0x5b8))(*(void **)(this + 4),iVar14,&xStack_4c,false,pCVar10,pCVar6);
                            end
                            quest:SetTimer(timerId, 5)
                        end
                    elseif me:MsgIsHitByHeroSpecialAbility(me) then
                        if not quest:IsConversationActive(scratchValue24) then
                            scratchValue24 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, quest:GetHero(), false)
                        end
                        quest:ModifyThingHealth(me, 1000.0, false)
                    end
                end
                scratchValue25 = scratchValue42
                notBeaten2 = state:GetBool("NotBeaten")
            end
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(scratchValue25)
                quest:DisplayQuestInfo(false)
                state:SetInt("BeenHit", 0)
                state:SetBool("NotBeaten", true)
                while not resources:TryAcquire(scratchValue43, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue41 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
                    quest:FixMovieSequenceCamera(true)
                    quest:Pause(0.5)
                    quest:EntitySetAsDrawable(quest:GetHero(), false)
                    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
                    quest:GetHealth(resources:ScriptThing(scratchValue43))
                    if 0.0 < fret_00 then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST", 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                                resources:DestroyMovie(scratchValue41)
                                goto LAB_00d664b0
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                            resources:DestroyMovie(scratchValue41)
                            goto LAB_00d664b0
                        end
                    end
                    quest:EntitySetAsDrawable(quest:GetHero(), true)
                    quest:FixMovieSequenceCamera(false)
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue41)
                    quest:EntitySetBossPhase(me, 2)
                    scratchValue26 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
                    quest:DisplayQuestInfo(true)
                    quest:UpdateQuestInfoCounter(scratchValue26, state:GetInt("BeenHit"), -1)
                    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
                    notBeaten3 = state:GetBool("NotBeaten")
                    scratchValue36 = u_stk_78_1
                    while notBeaten3 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        scratchValue37 = scratchValue36 | 0x30000
                        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                            predicateResult15 = true
                            if quest:IsConversationActive(scratchValue24) then
                                predicateResult15 = false
                                goto FLOW_after_lab_00d65d66
                            end
                        else
                            scratchValue37 = scratchValue36 | 0xf0000
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                                predicateResult15 = true
                                if not quest:IsConversationActive(scratchValue24) then
                                    goto FLOW_after_lab_00d65d66
                                end
                            end
                            predicateResult15 = false
                        end
                        ::FLOW_after_lab_00d65d66::
                        if scratchValue37 & 0x80000 ~= 0 then
                            scratchValue37 = scratchValue37 & 0xfff7ffff
                        end
                        if scratchValue37 & 0x40000 ~= 0 then
                            scratchValue37 = scratchValue37 & 0xfffbffff
                        end
                        if scratchValue37 & 0x20000 ~= 0 then
                            scratchValue37 = scratchValue37 & 0xfffdffff
                        end
                        if scratchValue37 & 0x10000 ~= 0 then
                            scratchValue37 = scratchValue37 & 0xfffeffff
                        end
                        if predicateResult15 then
                            scratchValue24 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, quest:GetHero(), false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        else
                            scratchValue36 = scratchValue37 | 0x300000
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                                predicateResult16 = true
                                if quest:IsConversationActive(scratchValue24) then
                                    predicateResult16 = false
                                    goto FLOW_after_lab_00d65f0c
                                end
                            else
                                scratchValue36 = scratchValue37 | 0xf00000
                                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                                    predicateResult16 = true
                                    if not quest:IsConversationActive(scratchValue24) then
                                        goto FLOW_after_lab_00d65f0c
                                    end
                                end
                                predicateResult16 = false
                            end
                            ::FLOW_after_lab_00d65f0c::
                            if scratchValue36 & 0x800000 ~= 0 then
                                scratchValue36 = scratchValue36 & 0xff7fffff
                            end
                            if scratchValue36 & 0x400000 ~= 0 then
                                scratchValue36 = scratchValue36 & 0xffbfffff
                            end
                            if scratchValue36 & 0x200000 ~= 0 then
                                scratchValue36 = scratchValue36 & 0xffdfffff
                            end
                            if scratchValue36 & 0x100000 ~= 0 then
                                scratchValue36 = scratchValue36 & 0xffefffff
                            end
                            if predicateResult16 then
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, quest:GetHero(), false)
                                quest:ModifyThingHealth(me, 1000.0, false)
                            else
                                if me:MsgIsHitByHeroSpecialAbility(me) then
                                    getStateInt3 = state:GetInt("BeenHit") + 1
                                    state:SetInt("BeenHit", getStateInt3)
                                    if getStateInt3 == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                                        state:SetBool("NotBeaten", false)
                                        quest:ModifyThingHealth(me, 1000.0, false)
                                    end
                                    quest:UpdateQuestInfoCounter(state:GetInt("BeenHit"), -1, 0.0)
                                    if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                                        scratchValue37 = scratchValue37 & 0x80000001
                                        scratchValue19 = scratchValue37 == 0
                                        if scratchValue37 < 0 then
                                            scratchValue19 = (scratchValue37 - 1 | 0xfffffffe) == 0xffffffff
                                        end
                                        if scratchValue19 then
                                            scratchValue24 = quest:AddNewConversation(me, false, false)
                                            -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                            -- TODO(native): (**(code **)((int)xStack_7c + 0x5b4))(*(void **)(this + 4),iVar14,pCVar6);
                                            -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                            -- TODO(native): (**(code **)((int)xStack_7c + 0x5b8))(*(void **)(this + 4),iVar14,&xStack_4c,false,pCVar10,pCVar6);
                                        else
                                            scratchValue24 = quest:AddNewConversation(me, false, false)
                                            -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                            -- TODO(native): (**(code **)((int)xStack_7c + 0x5b4))(*(void **)(this + 4),iVar14,pCVar6);
                                            -- TODO(native): xStack_7c = **(CCharString **)(this + 4);
                                            -- TODO(native): (**(code **)((int)xStack_7c + 0x5b8))(*(void **)(this + 4),iVar14,&xStack_28,false,pCVar10,pCVar6);
                                        end
                                        quest:SetTimer(timerId, 5)
                                    end
                            end
                            end
                        end
                        scratchValue26 = scratchValue42
                        notBeaten3 = state:GetBool("NotBeaten")
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(scratchValue26)
                        quest:DisplayQuestInfo(false)
                        while not resources:TryAcquire(scratchValue43, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            quest:EntitySetInFaction(me, "FACTION_HERO")
                            scratchValue41 = resources:NewResource()
                            while not resources:TryAcquire(scratchValue41, quest:GetHero(), 4) do
                                if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue41); goto FLOW_after_lab_00d6632b end
                            end
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue41)
                            else
                                scratchValue30 = resources:NewActorMap()
                                resources:SetActor(scratchValue30, "HERO", scratchValue41)
                                resources:SetActor(scratchValue30, "MAZE", scratchValue43)
                                scratchValue39 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_WIN", scratchValue30, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue39)
                                resources:DestroyActorMap(scratchValue30)
                                resources:ReleaseResource(scratchValue41)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:ModifyThingHealth(me, 1000.0, false)
                                quest:RemoveThing(me, false, true)
                                quest:EntitySetAsDamageable(quest:GetHero(), true)
                            end
                            ::FLOW_after_lab_00d6632b::
                        end
                    end
                end
            end
        end
    end
    ::LAB_00d664b0::
    quest:DeregisterTimer(timerId)
    ::LAB_00d664b9::
    resources:ReleaseResource(scratchValue43)
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

