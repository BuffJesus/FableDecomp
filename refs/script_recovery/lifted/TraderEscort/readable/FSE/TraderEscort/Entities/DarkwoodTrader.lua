-- Readable native conversion: DarkwoodTrader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderToBalverineDelay = 3572,  -- 140
    TE_TraderLowInfectionTime = 3576,  -- 80
}

local helpers = require("TraderEscort.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local regulateBanterComment, incubationTime, regulateFollowStateComment, brainState_, leadToCamp
local inSafeZone, greetedBuddy, self0X14, infected, currentAIState, previousAIState, traderHealthID

-- DarkwoodTrader.Main (retail 0x00e07640)
function Main(quest, me)
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, CVar24_b3, b, scratchValue4, controlAcquired, scratchValue7, scratchValue8
    local c_stk_171_2, c_stk_171_4, c_stk_171_5, c_stk_171_6, c_stk_171_8, c_stk_171_9, c_stk_171_10
    local c_stk_171_12, c_stk_171_13, c_stk_171_14, scratchValue10, scratchValue12, scratchValue13
    local questionAnswer, sequence, sequence3, p0, hero2, getHero, scratchValue30, scratchValue31
    local getDataString, scratchValue32, traderEndPos, runOffPos, campTraderA, this_00
    local scratchValue34, addNewConversation, scratchValue36, scratchValue37, scratchValue38
    local scratchValue39, actorMap, scratchValue40, scratchValue41, scratchValue42
    if not quest:NewScriptFrame(me) then return end
    local resource2 = resources:NewResource()
    while not quest:GetStateBool("IntroFinished") do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
        -- TODO(native): c_stk_171 = (**(*me + 0x6c))("SCRIPT_NAME_HERO")
        local c_stk_171_1 = nil --[[unresolved native value]]
        if c_stk_171_1 ~= 0 and quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    if brainState_ ~= 1 then
        goto LAB_00e083cd
    end
    goto FLOW_past_lab_00e083cd
    ::LAB_00e083cd::
    -- TODO(native): CCreatureAction_TrollWhackGroundBase__HandleTrader(this,2);
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + 1)
    quest:SetTimer(regulateBanterComment, 30)
    if not quest:IsActiveThreadTerminating() then
        repeat
            if leadToCamp then goto LAB_00e0843a end
            -- TODO(native): xStack_168 = xStack_168 | 2;
            c_stk_171_2 = 1
            if not quest:IsRegionLoaded("BarrowFields") then goto LAB_00e0843a end
            goto FLOW_past_lab_00e0843a
            ::LAB_00e0843a::
            c_stk_171_2 = 0
            ::FLOW_past_lab_00e0843a::
            if resource2 & 2 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffffffd;
            end
            if c_stk_171_2 ~= 0 then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                quest:SetTimer(commentTimer, 0)
                helpers.MakeTraderComment(quest, me, "IN_BARROW_FIELD", me, 0)
                resources:PrepareResource(resource2)
                while not me:AcquireControl(4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                if quest:IsActiveThreadTerminating() then break end
                local tradersStopHere = quest:GetThingWithScriptName("M_TradersStopHere")
                -- TODO(native): unaff_EBP_b3 = 0;
                scratchValue10 = 0
                me:ClearCommands()
                if scratchValue37 == nil then
                    scratchValue32 = {x = 0, y = 0, z = 0}
                else
                    -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                    scratchValue32 = nil --[[unresolved native value]]
                end
                me:MoveToPosition(scratchValue32, 1.0, ENTITY_MOVE_RUN, false, true)
                scratchValue4 = quest:IsDistanceBetweenThingsUnder(me, tradersStopHere, 2.0)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    -- TODO(native): xStack_20[0] = (quest:GetDistanceBetweenThings(p0, xStack_12c) ^ 2);
                    local c_stk_171_3 = scratchValue41[0] < (quest:GetDistanceBetweenThings(hero, tradersStopHere) ^ 2)
                    hero2 = hero
                    if quest:IsDistanceBetweenThingsUnder(me, hero, 1091567616) or not c_stk_171_3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                        hero2 = hero
                        if quest:IsDistanceBetweenThingsUnder(me, hero, 1084227584) or not c_stk_171_3 then
                            if scratchValue10 ~= 0 then
                                me:ClearCommands()
                                if scratchValue37 == nil then
                                    scratchValue32 = {x = 0, y = 0, z = 0}
                                else
                                    -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                                    scratchValue32 = nil --[[unresolved native value]]
                                end
                                me:MoveToPosition(scratchValue32, 1.0, ENTITY_MOVE_RUN, false, true)
                                scratchValue10 = 0
                            end
                        elseif scratchValue10 ~= 1 then
                            me:ClearCommands()
                            if scratchValue37 == nil then
                                scratchValue32 = {x = 0, y = 0, z = 0}
                            else
                                -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                                scratchValue32 = nil --[[unresolved native value]]
                            end
                            me:MoveToPosition(scratchValue32, 1.0, ENTITY_MOVE_WALK, false, true)
                            scratchValue10 = 1
                        end
                        -- TODO(native): unaff_EBP_b3 = 0;
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                        me:ClearCommands()
                        me:ClearAllActions()
                        if unaff_EBP_b3 then scratchValue4 = quest:IsDistanceBetweenThingsUnder(me, scratchValue38, 2.0); goto continue_1 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_14)
    --[[unresolved native value]]
                        quest:AddLineToConversation(addNewConversation, ("TEXT_QST_067_" .. nil) .. "_FOLLOW_ME", me, hero, false)
                        quest:Pause(1.0)
                        -- TODO(native): unaff_EBP = (int *)CONCAT13(1,(int3)unaff_EBP);
                        scratchValue10 = 2
                    end
                    scratchValue4 = quest:IsDistanceBetweenThingsUnder(me, scratchValue38, 2.0)
                    ::continue_1::
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                helpers.MakeTraderComment(quest, me, "THANKS_10", me, 0)
                leadToCamp = true
            end
            if quest:GetStateBool("EndStarted") then
                if quest:IsActiveThreadTerminating() then break end
                -- TODO(native): quest:RemoveQuestInfoElement(*(*(this + 4) + 0x30))
                quest:MiniMapRemoveMarker(me)
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, false)
                quest:ModifyThingHealth(me, __unknown_push, false)
                resources:PrepareResource(resource2)
                controlAcquired = resources:TryAcquire(resource2, me, 4)
                goto LAB_00e09fd9
            end
            if quest:GetStateBool("TradersShouldBeScared") then
                if quest:IsActiveThreadTerminating() then break end
                runOffPos = quest:GetThingWithScriptName("DTE_RunOffPos")
                quest:EntityStopFollowing(me)
                quest:SetEntityAsRegionFollowing(hero, me, false)
                quest:EntitySetAsScared(me, true)
                resources:PrepareResource(resource2)
                while not me:AcquireControl(4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
                if scratchValue40 ~= nil then
                    -- TODO(native): puVar5 = (**(*xStack_158 + 0x18))()
                    scratchValue32 = nil --[[unresolved native value]]
                end
                -- TODO(native): xStack_e8 = (int *)puVar5.x;
                -- TODO(native): CStack_110 = *(CCharString *)(puVar5 + 1);
                -- TODO(native): xStack_cc = puVar5.z;
                resources:ScriptThing(resource2)
                -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_e8,iVar23);
    --[[unresolved native result]]
                while nil ~= 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    me:MoveToPosition(scratchValue42, 1.0, ENTITY_MOVE_RUN, false, true)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                    resources:ScriptThing(resource2)
                    -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_e8,iVar23);
    --[[unresolved native result]]
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                while quest:GetStateBool("TradersShouldBeScared") do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                end
                resources:PrepareResource(resource2)
                quest:EntityFollowThing(me, hero, nil --[[missing]], nil --[[missing]])
                quest:SetEntityAsRegionFollowing(hero, me, true)
                getHero = hero
                scratchValue4 = quest:IsDistanceBetweenThingsUnder(me, hero, 1.4013e-45)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    getHero = hero
                    scratchValue4 = quest:IsDistanceBetweenThingsUnder(me, hero, 20.0)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                -- TODO(native): pCVar11 = (**(*me + 0xc))(pCVar12,"_KILLED_EARTH_TROLL",0,me,uVar13)
    --[[unresolved native value]]
                quest:AddLineToConversation(addNewConversation, ("TEXT_QST_067_" .. nil) .. "_KILLED_EARTH_TROLL", me, hero, false)
            end
            if inSafeZone then goto LAB_00e08d39 end
            -- TODO(native): xStack_168 = xStack_168 | 4;
            c_stk_171_4 = 1
            if not quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08d39 end
            goto FLOW_past_lab_00e08d39
            ::LAB_00e08d39::
            c_stk_171_4 = 0
            ::FLOW_past_lab_00e08d39::
            if c_stk_171_4 == 0 then
                if not inSafeZone then
                    goto LAB_00e08dc8
                else
                    -- TODO(native): xStack_168 = xStack_168 | 8;
                    c_stk_171_5 = 1
                    if quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08dc8 end
                end
                goto FLOW_past_lab_00e08dc8
                ::LAB_00e08dc8::
                c_stk_171_5 = 0
                ::FLOW_past_lab_00e08dc8::
                if c_stk_171_5 ~= 0 then
                    if quest:IsActiveThreadTerminating() then break end
                    quest:EntitySetAsScared(me, true)
                    inSafeZone = false
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                quest:EntitySetAsScared(me, false)
                inSafeZone = true
            end
            if greetedBuddy then goto LAB_00e08e52 end
            -- TODO(native): xStack_168 = xStack_168 | 0x10;
            c_stk_171_6 = 1
            if not quest:IsRegionLoaded("Darkwood4") then goto LAB_00e08e52 end
            goto FLOW_past_lab_00e08e52
            ::LAB_00e08e52::
            c_stk_171_6 = 0
            ::FLOW_past_lab_00e08e52::
            if resource2 & 16 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffef;
            end
            if c_stk_171_6 ~= 0 then
                if quest:IsActiveThreadTerminating() then break end
                local darkwoodTrader2 = quest:GetNearestWithScriptName(hero, "DarkwoodTrader")
                quest:SetStateString("TraderToTalk", actorMap:GetDataString())
                campTraderA = quest:GetThingWithScriptName("TE_CampTrader_A")
                if campTraderA ~= nil and campTraderA:IsAlive() then
                    quest:EntityStopFollowing(me)
                    quest:SetEntityAsRegionFollowing(hero, me, false)
                    resources:PrepareResource(resource2)
                    while not me:AcquireControl(4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                    if actorMap ~= nil then
                        -- TODO(native): puVar5 = (**(*xStack_14c + 0x18))()
                        scratchValue32 = nil --[[unresolved native value]]
                    end
                    -- TODO(native): xStack_118 = puVar5.x;
                    -- TODO(native): xStack_e8 = (int *)puVar5.y;
                    -- TODO(native): CStack_110 = *(CCharString *)(puVar5 + 2);
                    resources:ScriptThing(resource2)
                    -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_118,iVar23);
                    scratchValue12 = nil --[[unresolved native result]]
                    while scratchValue12 ~= 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                        me:MoveToPosition(scratchValue36, 3.0, ENTITY_MOVE_WALK, false, true)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                        resources:ScriptThing(resource2)
                        -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_118,iVar23);
                        scratchValue12 = nil --[[unresolved native result]]
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
                    -- TODO(native): (**(code **)(*(int *)p0 + 0x18))();
                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                    quest:CreateEffect()
                    quest:ModifyThingHealth(me, 1000.0, false)
                    -- TODO(native): piVar14 = (**(*me + 0xc))(&xStack_34)
                    scratchValue31 = nil --[[unresolved native value]]
                    -- TODO(native): puVar5 = *piVar14
                    scratchValue32 = nil --[[unresolved native value]]
                    -- TODO(native): puVar1 = *(self0X14 + 0x78)
    --[[unresolved native value]]
                    if nil == scratchValue32 then
                        c_stk_171_8 = 1
                    elseif nil == nil or scratchValue32 == nil then
                        c_stk_171_8 = 0
                    elseif (nil)[1] == scratchValue32.y then
                        -- TODO(native): iVar23 = CBasicString<char>::Compare((void *)*puVar1,(void *)puVar5.x);
                        c_stk_171_8 = scratchValue12 == 0 and 1 or 0
                    else
                        c_stk_171_8 = 0
                    end
                    if c_stk_171_8 == 0 then
                        if not quest:IsActiveThreadTerminating() then
                            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e0a380 end
                            end
                            if not quest:IsActiveThreadTerminating() then quest:Pause(2.0); goto LAB_00e09493 end
                        end
                    else
                        if not quest:IsActiveThreadTerminating() then
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, campTraderA)
                            -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_1c)
    --[[unresolved native value]]
                            quest:AddLineToConversation(addNewConversation, ("TEXT_QST_067_" .. nil) .. "_GREET_ALLY_10", me, darkwoodTrader2, false)
                            -- TODO(native): pCVar12 = (**(*me + 0xc))(piVar14,"_GREET_ALLY_RESPONSE")
    --[[unresolved native value]]
                            quest:AddLineToConversation(addNewConversation, ("TEXT_QST_067_" .. nil) .. "_GREET_ALLY_RESPONSE", me, getHero, false)
                            -- TODO(native): pCVar12 = (**(*me + 0xc))(b,"_GREET_ALLY_20",0,me,xStack_168)
    --[[unresolved native value]]
                            quest:AddLineToConversation(addNewConversation, ("TEXT_QST_067_" .. nil) .. b, me, runOffPos, false)
                            quest:Pause(2.0)
                            quest:RemoveThing(traderEndPos, false, true)
                            while quest:IsConversationActive(addNewConversation) do
                                if not quest:NewScriptFrame(me) then goto LAB_00e0a380 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e09493 end
                        end
                    end
                    goto FLOW_past_lab_00e09493
                    ::LAB_00e09493::
                    greetedBuddy = true
                    if xStack_108:IsAlive() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e0a380 end
                        quest:RemoveThing(hero2, false, true)
                    end
                    resources:PrepareResource(resource2)
                    quest:EntityFollowThing(me, hero, 3.0, true)
                    quest:SetEntityAsRegionFollowing(hero, me, true)
                    goto LAB_00e09511
                    ::FLOW_past_lab_00e09493::
                    ::LAB_00e0a380::
                    resources:ReleaseResource(resource2)
                    return
                end
                ::LAB_00e09511::
            end
            if 0 < quest:GetStateInt("BalverinesToSurpriseHeroNeeded") and not inSafeZone then
                if quest:IsActiveThreadTerminating() then break end
                quest:EntitySetAsScared(me, true)
            end
            if infected == 0 or not quest:GetStateBool("SavedInMiddle") then
                goto LAB_00e095cf
            else
                if quest:IsRegionLoaded("Darkwood4") then goto LAB_00e095cf end
                -- TODO(native): xStack_168 = xStack_168 | 0x40;
                c_stk_171_9 = 0
                if quest:IsRegionLoaded("BarrowFields") then goto LAB_00e095cf end
            end
            goto FLOW_past_lab_00e095cf
            ::LAB_00e095cf::
            c_stk_171_9 = 1
            ::FLOW_past_lab_00e095cf::
            if resource2 & 64 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffbf;
            end
            if resource2 & 32 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffdf;
            end
            if c_stk_171_9 ~= 0 then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderToBalverineDelay))
            end
            if quest:GetTimer(incubationTime) == 0 then
                if not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") - 1)
                    resources:PrepareResource(resource2)
                    if this_00 ~= nil then
                        -- TODO(native): xStack_168 = xStack_168 | 0x380;
                        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_00,pCVar12,0);
                        -- TODO(native): *(code **)(this_00 + 0x34) = Script_Darkwood_Balverine_Trader;
                        -- TODO(native): *(undefined4 *)(this_00 + 0x38) = uVar7;
                    end
                    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
                    if resource2 & 512 ~= 0 then
                        scratchValue34 = resource2 & 0xfffffdff
                    end
                    if scratchValue34 & 256 ~= 0 then
                        scratchValue34 = scratchValue34 & 0xfffffeff
                    end
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                end
                break
            end
            sequence = quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeHigh")
            if sequence then
                addNewConversation = 4
                sequence = infected < 4
            end
            if sequence then
                if quest:IsActiveThreadTerminating() then break end
                c_stk_171_10 = helpers.MakeTraderComment(quest, me, "INFECTED_HIGH", me, 0)
                goto LAB_00e096c7
            else
                sequence3 = quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeMedium")
                if sequence3 then
                    addNewConversation = 3
                    sequence3 = infected < 3
                end
                if sequence3 then
                    if not quest:IsActiveThreadTerminating() then c_stk_171_10 = helpers.MakeTraderComment(quest, me, "INFECTED_MEDIUM", me, 0); goto LAB_00e096c7 end
                    break
                end
                if quest:GetTimer(incubationTime) < quest:GetStateInt("IncubationTimeLow") and infected < 2 then
                    if quest:IsActiveThreadTerminating() then break end
                    if helpers.MakeTraderComment(quest, me, "INFECTED_LOW", me, 0) then
                        if quest:IsActiveThreadTerminating() then break end
                        infected = 2
                    end
                end
            end
            goto FLOW_past_lab_00e096c7
            ::LAB_00e096c7::
            if c_stk_171_10 then
                if quest:IsActiveThreadTerminating() then break end
                infected = addNewConversation
            end
            ::FLOW_past_lab_00e096c7::
            -- TODO(native): cVar3 = (**(*me + 0x54))("")
    --[[unresolved native value]]
            if nil == 0 then
                -- TODO(native): cVar3 = (**(*me + 0xa8))("")
    --[[unresolved native value]]
                if nil ~= 0 then goto LAB_00e09848 end
                goto LAB_00e09886
            else
                goto LAB_00e09848
            end
            goto FLOW_past_lab_00e09886
            ::LAB_00e09886::
            c_stk_171_12 = 0
            ::FLOW_past_lab_00e09886::
            goto FLOW_past_lab_00e09848
            ::LAB_00e09848::
            -- TODO(native): xStack_168 = xStack_168 | 0x1000;
            -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
            c_stk_171_12 = 1
            if nil ~= 0 then goto LAB_00e09886 end
            ::FLOW_past_lab_00e09848::
            if resource2 & 4096 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffefff;
            end
            if resource2 & 2048 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffff7ff;
            end
            if resource2 & 1024 ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffffbff;
            end
            if c_stk_171_12 ~= 0 then
                if quest:IsActiveThreadTerminating() then break end
                quest:SetTimer(regulateFollowStateComment, 5)
                if quest:GetHealth(me) <= 5.0 then
                    goto LAB_00e099dc
                else
                    scratchValue34 = resource2
                    -- TODO(native): cVar3 = (**(*me + 0x54))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                    if nil ~= 0 then goto LAB_00e099dc end
                    -- TODO(native): xStack_168 = uVar16 | 0x6000;
                    -- TODO(native): cVar3 = (**(*me + 0xa8))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                    if nil ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 | 0x8000;
                        -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                        if nil == 0 then goto LAB_00e099dc end
                    end
                    c_stk_171_13 = 1
                end
                goto FLOW_past_lab_00e099dc
                ::LAB_00e099dc::
                c_stk_171_13 = 0
                ::FLOW_past_lab_00e099dc::
                if resource2 >> 8 < 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffff7fff;
                end
                if resource2 & 0x4000 ~= 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffffbfff;
                end
                if resource2 & 0x2000 ~= 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffffdfff;
                end
                if c_stk_171_13 == 0 then
                    if quest:GetHealth(me) <= 5.0 then
                        goto LAB_00e09b4f
                    else
                        scratchValue34 = resource2
                        -- TODO(native): cVar3 = (**(*me + 0x54))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                        if nil == 0 then
                            -- TODO(native): xStack_168 = uVar16 | 0x30000;
                            -- TODO(native): cVar3 = (**(*me + 0xa8))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                            if nil ~= 0 then
                                -- TODO(native): xStack_168 = xStack_168 | 0x40000;
                                -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                                if nil == 0 then goto LAB_00e09b48 end
                            end
                            goto LAB_00e09b4f
                        end
                        ::LAB_00e09b48::
                        c_stk_171_14 = 1
                    end
                    goto FLOW_past_lab_00e09b4f
                    ::LAB_00e09b4f::
                    c_stk_171_14 = 0
                    ::FLOW_past_lab_00e09b4f::
                    if resource2 & 0x40000 ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffbffff;
                    end
                    if resource2 & 0x20000 ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffdffff;
                    end
                    if resource2 & 0x10000 ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffeffff;
                    end
                    if c_stk_171_14 == 0 then goto LAB_00e09c13 end
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "HERO_HIT_ME", me, 0)
                    scratchValue13 = 2
                else
                    if quest:IsActiveThreadTerminating() then break end
                    helpers.MakeTraderComment(quest, me, "UNDER_ATTACK", me, 0)
                    scratchValue13 = 3
                end
                quest:SetTimer(commentTimer, scratchValue13)
            end
            ::LAB_00e09c13::
            if currentAIState == previousAIState then
                if quest:GetTimer(regulateBanterComment) == 0 and not leadToCamp then
                    if quest:IsActiveThreadTerminating() then break end
                    if quest:GetStateInt("TradersStillAliveCounter") == 1 then
                        scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER_SOLO", me, 0)
                    else
                        local switch1 = quest:GetStateInt("TimesBantered")
                        repeat
                            if switch1 == 0 then
                                scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER_FIRST", me, 1)
                                break
                            elseif switch1 == 1 then
                                scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER_SECOND", me, 1)
                                break
                            elseif switch1 == 2 then
                                scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER_THIRD", me, 1)
                                break
                            elseif switch1 == 3 then
                                scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER_FOURTH", me, 1)
                                break
                            else
                                scratchValue8 = helpers.MakeTraderComment(quest, me, "BANTER", me, 0)
                            end
                        until true
                    end
                    if not scratchValue8 then
                        if quest:IsActiveThreadTerminating() then break end
                        local scratchValue14 = math.random(0, 32767)
                        quest:SetTimer(regulateBanterComment, scratchValue14 % 15 + 25)
                    else
                        if quest:IsActiveThreadTerminating() then break end
                        local scratchValue15 = math.random(0, 32767)
                        quest:SetTimer(regulateBanterComment, scratchValue15 % 20 + 30)
                        quest:SetStateInt("TimesBantered", quest:GetStateInt("TimesBantered") + 1)
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                if quest:GetTimer(regulateFollowStateComment) == 0 and currentAIState == 2 then
                    helpers.MakeTraderComment(quest, me, "FOLLOWING", me, 0)
                    quest:SetTimer(regulateFollowStateComment, 10)
                end
            end
            previousAIState = currentAIState
            -- TODO(native): uVar7 = (**(*me + 0x34))()
            addNewConversation = nil --[[unresolved native value]]
            currentAIState = addNewConversation
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource2)
                do return end
            end
        until false
    end
    resources:ReleaseResource(resource2)
    do return end
    ::FLOW_past_lab_00e083cd::
    resources:PrepareResource(resource2)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    quest:SetThingAsConscious(me, false, "")
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    while not quest:GetStateBool("InfectedTraderCanGetUp") do
        if not quest:NewScriptFrame(me) then goto LAB_00e08601 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e08601 end
    quest:EntitySetAsDamageable(me, true)
    quest:EntitySetTargetable(me, true)
    quest:SetThingAsConscious(me, true, "")
    if quest:IsDistanceBetweenThingsOver(actorMap, hero, 1.4013e-45) then
        repeat
            if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
            if not quest:GetStateBool("MissionFailed") then goto continue_3 end
            if scratchValue ~= nil then
                -- TODO(native): puVar5 = (**(*CStack_110 + 0x18))()
                scratchValue32 = nil --[[unresolved native value]]
            end
            -- TODO(native): xStack_14c = puVar5.x;
            -- TODO(native): xStack_158 = *(CCharString *)(puVar5 + 1);
            -- TODO(native): xStack_114 = *(CCharString *)(puVar5 + 2);
            resources:ScriptThing(resource2)
            -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,xStack_14c,iVar23);
--[[unresolved native result]]
            -- TODO(native): unaff_EBX = CONCAT13((char)iVar23,(int3)unaff_EBX);
            while unaff_EBX >> 24 ~= 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                me:MoveToPosition(actorMap, 0, ENTITY_MOVE_RUN, false, true)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e085f8 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
                resources:ScriptThing(resource2)
                -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,xStack_14c,iVar23);
--[[unresolved native result]]
                -- TODO(native): unaff_EBX = CONCAT13((char)iVar23,(int3)unaff_EBX);
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e085f8 end
            quest:FadeOutAndKillEntity(me, true, 2.0, true)
            ::continue_3::
        until not quest:IsDistanceBetweenThingsOver(scratchValue39, hero, 5.0)
    end
    if not quest:IsActiveThreadTerminating() then
        -- TODO(native): unaff_EBX = CONCAT13(bVar2,(int3)unaff_EBX);
        -- TODO(native): cVar3 = (**(*me + 0x130))()
    --[[unresolved native value]]
        if nil == 0 and 0.0 < quest:GetHealth(me) then
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local darkwoodTrader3 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
            local darkwoodTrader = quest:GetFurthestWithScriptName(me, "DarkwoodTrader")
            local resource3 = resources:NewResource()
            local resource = resources:NewResource()
            local resource4 = resources:NewResource()
            resources:PrepareResource(resource3)
            while not me:AcquireControl(4) do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    addNewConversation = hero
                else
                    resources:ReleaseResource(resource4)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource3)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e085f8
                    addNewConversation = hero
                end
            end
            if not quest:IsActiveThreadTerminating() then
                -- TODO(native): cVar3 = (**(*unaff_EBP + 0x138))(xStack_124)
    --[[unresolved native value]]
                if nil ~= 0 then
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource4)
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(addNewConversation)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                    end
                end
                if darkwoodTrader3:GetDataString() == "SCARED" then
                    goto LAB_00e07cac
                else
                    scratchValue7 = 1
                    if not (darkwoodTrader ~= nil and darkwoodTrader:IsAlive()) then goto LAB_00e07cac end
                end
                goto FLOW_past_lab_00e07cac
                ::LAB_00e07cac::
                scratchValue7 = 0
                ::FLOW_past_lab_00e07cac::
                if scratchValue7 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e07db9 end
                    -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_140,(int)&xStack_128);
                    -- TODO(native): xStack_128 = xStack_bc_2;
                end
                quest:FixMovieSequenceCamera(true)
                actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", addNewConversation)
                resources:SetActor(actorMap, "TRADERI", campTraderA)
                if not (darkwoodTrader3 ~= nil and darkwoodTrader3:IsAlive()) then
                    goto LAB_00e081e9
                elseif not quest:IsActiveThreadTerminating() then
                    if darkwoodTrader ~= nil and darkwoodTrader:IsAlive() then
                        if not quest:IsActiveThreadTerminating() then
                            resources:PrepareResource(resource)
                            while resources:TryAcquire(resource, darkwoodTrader3, 4) == 0 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e07e30 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e07db0 end
                            resources:PrepareResource(resource4)
                            while resources:TryAcquire(resource4, darkwoodTrader, 4) == 0 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e07db0 end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                resources:SetActor(actorMap, "TRADERS", resource)
                                resources:SetActor(actorMap, "TRADERN", resource4)
                                scratchValue30 = "CS_DARKWOOD_TRADER_INFECTED_BOTH"
                                goto LAB_00e081c2
                            end
                        end
                        ::LAB_00e07e30::
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource4)
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(addNewConversation)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                    end
                    getDataString = darkwoodTrader3:GetDataString()
                    if not (getDataString ~= nil and getDataString == "SCARED") then
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource4)
                            resources:ReleaseResource(resource)
                            resources:ReleaseResource(addNewConversation)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e085f8
                        end
                        resources:PrepareResource(resource4)
                        while resources:TryAcquire(resource4, darkwoodTrader3, 4) == 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:DestroyActorMap(actorMap)
                                resources:ReleaseResource(resource4)
                                resources:ReleaseResource(resource)
                                resources:ReleaseResource(addNewConversation)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00e085f8
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap, "TRADERN", resource4)
                            scratchValue30 = "CS_DARKWOOD_TRADER_INFECTED_NORMAL"
                            goto LAB_00e081c2
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource)
                        while resources:TryAcquire(resource, darkwoodTrader3, 4) == 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e07db0 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap, "TRADERS", resource)
                            scratchValue30 = "CS_DARKWOOD_TRADER_INFECTED_SCARED"
                            goto LAB_00e081c2
                        end
                        goto FLOW_hoist_lab_00e081c2_1
                    end
                    goto FLOW_past_lab_00e081c2
                    ::LAB_00e081c2::
                    resources:RunMacro(scratchValue30, actorMap, false, true)
                    goto LAB_00e081e9
                    ::FLOW_hoist_lab_00e081c2_1::
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource4)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(addNewConversation)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e085f8
                    ::FLOW_past_lab_00e081c2::
                end
                goto FLOW_past_lab_00e081e9
                ::LAB_00e081e9::
                quest:GiveHeroYesNoQuestion("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource4)
                        resources:ReleaseResource(resource)
                        resources:ReleaseResource(addNewConversation)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e085f8
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if scratchValue4 then
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource4)
                            resources:ReleaseResource(resource)
                            resources:ReleaseResource(addNewConversation)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e085f8
                        end
                        scratchValue30 = "CS_DARKWOOD_TRADER_INFECTED_JOINS"
                    else
                        if scratchValue4 then goto LAB_00e07db0 end
                        scratchValue30 = "CS_DARKWOOD_TRADER_INFECTED_LEAVES"
                    end
                    resources:RunMacro(scratchValue30, actorMap, false, true)
                    getHero = 0
                    quest:FixMovieSequenceCamera(false)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource4)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(addNewConversation)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    if not unaff_EBX_b3 then
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroMorality(__unknown_push)
                            quest:RemoveThing(me, false, true)
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource2)
                        quest:ClearThingHasInformation(me)
                        goto LAB_00e083cd
                    end
                    goto LAB_00e085f8
                end
                ::FLOW_past_lab_00e081e9::
                ::LAB_00e07db0::
                resources:DestroyActorMap(actorMap)
            end
            ::LAB_00e07db9::
            resources:ReleaseResource(resource4)
            resources:ReleaseResource(resource)
            resources:ReleaseResource(addNewConversation)
            -- TODO(native): (**(code **)(*piVar14 + 0x5ec))(0);
            resources:DestroyMovie(movie)
        else
            while true do
                if quest:IsActiveThreadTerminating() then break end
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00e085f8::
    ::LAB_00e08601::
    resources:ReleaseResource(resource2)
    do return end
    ::LAB_00e09fd9::
    if controlAcquired == 0 then
        if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
        controlAcquired = resources:TryAcquire(resource2, me, 4)
        goto LAB_00e09fd9
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    while quest:IsDistanceBetweenThingsOver(me, campTraderA, 7.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
        if not me:IsPerformingScriptTask() then
            me:MoveToThing(addNewConversation, 4.0, ENTITY_MOVE_WALK)
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    resources:PrepareResource(resource2)
    if quest:IsActiveThreadTerminating() then goto LAB_00e0a346 end
    repeat
        -- TODO(native): cVar3 = (**(*me + 0x6c))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
        if nil ~= 0 then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource2)
            while resources:TryAcquire(resource2, me, 4) == 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e0a346 end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            local health = quest:GetHealth(resources:ScriptThing(stack0xfffffe48))
            -- TODO(native): CVar19._0_3_ = CVar24._0_3_;
            -- TODO(native): CVar19._3_1_ = 1;
            if CVar24_b3 ~= 0 then
                scratchValue30 = "_THANKS"
                -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_c8)
    --[[unresolved native value]]
                me:Speak(hero, scratchValue30, 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e0a33a end
                goto FLOW_past_lab_00e0a33a
                ::LAB_00e0a33a::
                resources:ReleaseResource(addNewConversation)
                break
                ::FLOW_past_lab_00e0a33a::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(addNewConversation)
            resources:PrepareResource(campTraderA)
        end
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(campTraderA); return end
    until false
    ::LAB_00e0a346::
    resources:ReleaseResource(resource2)
end

-- DarkwoodTrader.Init (retail 0x00e04bd0)
function Init(quest, me)
    incubationTime = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateFollowStateComment = quest:RegisterTimer()  -- native constructor: CTimer member
    regulateBanterComment = quest:RegisterTimer()  -- native constructor: CTimer member
    local scratchValue5, scratchValue, this_01
    brainState_ = 0
    currentAIState = 0
    previousAIState = 0
    if me:GetDataString() == "INFECTED" then
        brainState_ = 1
    end
    infected = 0
    quest:SetTimer(incubationTime, 0)
    greetedBuddy = false
    leadToCamp = false
    inSafeZone = false
    quest:SetTimer(regulateFollowStateComment, 0)
    quest:SetTimer(regulateBanterComment, 60)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingPersistent(me, true)
    quest:SetEntityAsFollowingHeroThroughTeleporters(me, false)
    if me:GetDataString() ~= "INFECTED" then
        if me:GetDataString() ~= "SCARED" then
            if me:GetDataString() ~= "FRIENDLY" then goto LAB_00e04e13 end
            scratchValue = "FRIENDLY_TRADER"
        else
            scratchValue = "SCARED_TRADER"
        end
    else
        scratchValue = "INFECTED_TRADER"
    end
    quest:EntitySetWillBeUsingNarrator(me, scratchValue)
    ::LAB_00e04e13::
    if this_01 ~= nil then
        -- TODO(native): xStack_10 = *(CCharString (*) [4])(this + 0xc);
        -- TODO(native): local_20 = *(int **)(this + 0x10);
        if scratchValue5 ~= nil then
            -- TODO(native): *local_20 = *local_20 + 1;
        end
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_01,pCVar6,0);
        -- TODO(native): *(code **)(this_01 + 0x34) = NScript::CQ_TraderEscortScript::WatchForPickpocketing;
        -- TODO(native): *(undefined4 *)(this_01 + 0x38) = uVar1;
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_01,sectionName);
end

-- DarkwoodTrader.OnPersist (retail 0x00e05330)
function OnPersist(quest, me, context)
    quest:SetStateInt("BrainState", quest:PersistTransferInt(context, "BrainState", quest:GetStateInt("BrainState") or 0))
end

-- DarkwoodTrader.OnPredicateFail (retail 0x00e01760)
function OnPredicateFail(quest, me)
    local scratchValue
    if quest:GetTimer(incubationTime) < 1 then
        return
    end
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") - 1)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
    if not me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        if quest:GetStateInt("TradersStillAliveCounter") == 2 then
            scratchValue = 1
        else
            if quest:GetStateInt("TradersStillAliveCounter") ~= 1 then goto LAB_00e018e4 end
            scratchValue = 0
        end
        helpers.MakeTraderComment(quest, me, "LAST_TRADER", me, scratchValue)
    else
        helpers.MakeTraderComment(quest, me, "HERO_KILLED_TRADER", quest:GetNearestWithScriptName(me, "DarkwoodTrader"), 0)
    end
    ::LAB_00e018e4::
    quest:SetMasterGameState("DarkwoodAllTradersAlive", false)
end

-- DarkwoodTrader.SetBrainState (retail 0x00e0a510)
function SetBrainState(quest, me, brainState)
    local scratchValue, scratchValue9
    local hero = quest:GetHero()
    brainState_ = brainState
    if brainState == 2 then
        local predicateResult = quest:IsActiveThreadTerminating()
        brainState = predicateResult
        if not predicateResult then
            quest:EntityFollowThing(me, hero, 3.0, true)
            quest:SetEntityAsRegionFollowing(hero, me, true)
            quest:DisplayQuestInfo(true)
            -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
    --[[unresolved native value]]
            if nil ~= nil then
                -- TODO(native): uStack_1c_b0 = !(iVar4 != 0);
            end
            if 3.0 == 0 then
                -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
    --[[unresolved native value]]
                if nil == nil then
                    scratchValue = 7
                    repeat
                        if scratchValue == 0 then break end
                        scratchValue = scratchValue - 1
                        -- TODO(native): uStack_1c_b0 = *pcVar5 == *pcVar6;
                    until not 3.0
                else
                    -- TODO(native): uStack_1c_b0 = !(iVar4 != 0);
                end
                if 3.0 == 0 then
                    -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
    --[[unresolved native value]]
                    if nil ~= nil then
                        -- TODO(native): uStack_1c = CONCAT31(uStack_1c._1_3_,!(iVar4 != 0));
                    end
                    brainState = CONCAT31(int3(extraout_EAX >> 8),scratchValue9)
                    if scratchValue9 ~= 0 then
                        local predicateResult2 = quest:IsActiveThreadTerminating()
                        brainState = predicateResult2
                        if not predicateResult2 then
                            traderHealthID = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
                        end
                    end
                else
                    local predicateResult3 = quest:IsActiveThreadTerminating()
                    brainState = predicateResult3
                    if not predicateResult3 then
                        traderHealthID = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER_HAT_01", 1.0)
                        return quest:EntitySetAsScared(me, true)
                    end
                end
            else
                local predicateResult4 = quest:IsActiveThreadTerminating()
                brainState = predicateResult4
                if not predicateResult4 then
                    traderHealthID = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER", 1.0)
                    infected = 1
                    if not quest:GetStateBool("SavedNearEnd") then
                        local predicateResult5 = quest:IsActiveThreadTerminating()
                        brainState = predicateResult5
                        if not predicateResult5 then
                            return quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderToBalverineDelay))
                        end
                    else
                        local predicateResult6 = quest:IsActiveThreadTerminating()
                        brainState = predicateResult6
                        if not predicateResult6 then
                            return quest:SetTimer(incubationTime, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderLowInfectionTime))
                        end
                    end
                end
            end
        end
    end
    return brainState
end

