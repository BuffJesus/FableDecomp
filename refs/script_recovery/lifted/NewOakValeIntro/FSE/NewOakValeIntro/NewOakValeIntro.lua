-- Generated native draft: Q_NewOakValeIntro. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_vectors = {
    [0xd8] = {"TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_50", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_60"},
    [0xa8] = {"TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_50", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_60"},
    [0xe4] = {"TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_40"},
    [0xb4] = {"TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_40"},
    [0xcc] = {"TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_50", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_60"},
    [0x9c] = {"TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_50", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_60"},
    [0xf0] = {"TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_50"},
    [0xc0] = {"TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_50"},
}

function RegisterMain(quest)
    quest:CreateThread("Main")  -- native thread body 0x00CDD440: lift it as function Main(quest)
    if not bVar1 then
    end
end

function Main(quest)
    local bVar5, pCVar2
    local alive = true
    quest:AddEntityBinding("NOVI_LiveFather", "NewOakValeIntro/Entities/NOVI_LiveFather")
    local bVar4 = not bVar5 and bVar5
    quest:AddEntityBinding("NOVI_Theresa", "NewOakValeIntro/Entities/NOVI_Theresa")
    quest:AddEntityBinding("NOVI_Guard", "NewOakValeIntro/Entities/NOVI_Guard")
    quest:AddEntityBinding("NOVI_Villager", "NewOakValeIntro/Entities/NOVI_Villager")
    quest:AddEntityBinding("NOVI_Bully", "NewOakValeIntro/Entities/NOVI_Bully")
    quest:AddEntityBinding("NOVI_Victim", "NewOakValeIntro/Entities/NOVI_Victim")
    quest:AddEntityBinding("NOVI_TeddyGirl", "NewOakValeIntro/Entities/NOVI_TeddyGirl")
    quest:AddEntityBinding("NOVI_AffairMan", "NewOakValeIntro/Entities/NOVI_AffairMan")
    quest:AddEntityBinding("NOVI_AffairWoman", "NewOakValeIntro/Entities/NOVI_AffairWoman")
    quest:AddEntityBinding("NOVI_AffairWife", "NewOakValeIntro/Entities/NOVI_AffairWife")
    quest:AddEntityBinding("NOVI_BookTrader", "NewOakValeIntro/Entities/NOVI_BookTrader")
    quest:AddEntityBinding("NOVI_BarrelMan", "NewOakValeIntro/Entities/NOVI_BarrelMan")
    quest:AddEntityBinding("NOVI_BarrelThug", "NewOakValeIntro/Entities/NOVI_BarrelThug")
    quest:AddEntityBinding("NOVI_Barrel", "NewOakValeIntro/Entities/NOVI_Barrel")
    quest:AddEntityBinding("NOVI_CreatedBeetle", "NewOakValeIntro/Entities/NOVI_CreatedBeetle")
    if pCVar2 == nil then
        pCVar2 = 0x0
    else
        -- TODO(native): CCharString::CCharString((CCharString *)(pCVar2 + 4),local_10);
        -- TODO(native): *(CPersonalScript_GlobalThingsScript **)(pCVar2 + 8) = this;
        -- TODO(native): pCVar2[0x14] = (CEntityScriptBindingBase)0x1;
        -- TODO(native): *(undefined4 *)(pCVar2 + 0x18) = 0;
    end
    -- TODO(native): CScriptBase::AddEntityScriptBinding((CScriptBase *)this,pCVar2);
    quest:FinalizeEntityBindings()
    if quest:GetStateBool("AttackOver") then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        quest:DeactivateQuest("Q__OakValeIntro_PostAttack", pCVar2)
    end
    -- TODO(native): p_Var6 = (_func_void *)&local_4;
    local ppVar3 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(ppVar3, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "")
    quest:CreateThread("StartBarrelTimer")  -- native thread body Script_Guard_Duty: lift it as function StartBarrelTimer(quest)
    if not bVar5 then
    end
    DoMission(quest)
end

function Init(quest)
    quest:SetStateBool("VictimShake", false)
    quest:SetStateBool("DadFound", false)
    quest:SetStateBool("AttackOver", false)
    quest:SetStateBool("DadFinishedIntro", false)
    quest:SetStateBool("BullySubdued", false)
    quest:SetStateBool("BullyRanOff", false)
    quest:SetStateBool("GivenHeroTeddy", false)
    quest:SetStateBool("HeroAttackedVictim", false)
    quest:SetStateInt("GoodDeedsPerformed", 0)
    quest:SetStateInt("BadDeedsPerformed", 0)
    quest:SetStateInt("GuardsDealtWithBadDeeds", 0)
    quest:SetStateBool("HeroDiscoveredInfidelity", false)
    quest:SetStateBool("BarrelManLeftHeroInCharge", false)
    quest:SetStateBool("BarrelManSpokenToHeroOnReturn", false)
    quest:SetStateBool("InstructionGiven_Barrels", false)
    quest:SetStateBool("ReceiveKiss", false)
    quest:SetStateBool("ReceiveHug", false)
    quest:SetStateBool("BarrelBrokenPersistent", false)
    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)
    quest:SetStateInt("GUIBullyHealthCounter", -999)
    quest:SetStateBool("TeddyRuined", false)
    quest:SetStateBool("GuardsSpokenOnce", false)
    quest:SetStateBool("TalkingToWoman", false)
    quest:SetStateBool("GivenSweets", false)
    quest:SetStateBool("GivenTheresaChocs", false)
    quest:SetStateBool("VictimComplainsAboutLosingTeddy", false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (0), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (1), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (2), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (3), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (4), false)
end

function OnPersist(quest, context)
    local attackOver = quest:GetStateBool("AttackOver") or false
    attackOver = quest:PersistTransferBool(context, "AttackOver", attackOver)
    quest:SetStateBool("AttackOver", attackOver)
end

function DoMission(quest)
    local CVar1, bVar4, p_Var5, ppVar7, r1, r2, r3, r4, uVar8
    local alive = true
    local pCVar9 = "StartOakVale"
    local cVar3 = quest:IsRegionLoaded("StartOakVale")
    while not cVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar3 = quest:IsRegionLoaded("StartOakVale")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    if not quest:GetStateBool("AttackOver") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        uVar8 = 0
        quest:FadeScreenOutUntilNextCallToFadeScreenIn(0x40000000, uVar8)
        p_Var5 = quest:GetHero()
        r1 = quest:TurnCreatureInto(p_Var5, "CREATURE_HERO_CHILD")
        quest:CreateThread("WatchBarrels")  -- native thread body 0x00DBE890: lift it as function WatchBarrels(quest)
        if (uVar8 & 1) ~= 0 then
            uVar8 = uVar8 & 0xfffffffe
        end
        quest:CreateThread("WatchForGotGold")  -- native thread body CQ_FireHeartScript::CheckGameEnding: lift it as function WatchForGotGold(quest)
        if (uVar8 & 2) ~= 0 then
            uVar8 = uVar8 & 0xfffffffd
        end
        quest:CreateThread("ManageQuestCoreMarkers")  -- native thread body ManageQuestCoreMarkers: lift it as function ManageQuestCoreMarkers(quest)
        if (uVar8 & 4) ~= 0 then
        end
        quest:CacheMusicSet(0)
        quest:ActivateQuest("Q_NewOakValeIntro_PreAttack")
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        r2 = quest:GetHero()
        quest:EntitySetAsKillable(r2, false)
        quest:SetTimeAsStopped(true)
        quest:SetTimeOfDay(0x41400000)
        quest:SetHeroSleepingAsEnabled(nil --[[missing]])
        quest:DisplayMoneyBag(true)
        r3 = quest:GetThingWithScriptName("HerosOldHouse")
        -- TODO(native): OverrideAutomaticHouseLocking is not a ForgeFSE binding
        quest:OverrideAutomaticHouseLocking()
        -- TODO(native): OpenHouseDoors is not a ForgeFSE binding
        quest:OpenHouseDoors()
        ppVar7 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(ppVar7, true, nil --[[missing]])
        quest:OverrideMusic(0x13, false, true)
        CVar1 = quest:GetStateBool("AttackOver")
        while not CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            CVar1 = quest:GetStateBool("AttackOver")
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        AttackStuff(quest)
        PostAttackStuff(quest)
        quest:FadeScreenOutUntilNextCallToFadeScreenIn(0x3f000000, nil --[[missing]])
        r4 = quest:GetHero()
        quest:EntitySetAsKillable(r4, nil --[[missing]])
        quest:SetHeroSleepingAsEnabled(nil --[[missing]])
        ppVar7 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(ppVar7, false, nil --[[missing]], nil --[[missing]])
        ppVar7 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(ppVar7, 0)
    end
end

function AttackStuff(quest)
    local pCVar3 = "Q__OakValeIntro_PostAttack"
    quest:ActivateQuest("Q__OakValeIntro_PostAttack")
    quest:DeactivateQuest("Q_NewOakValeIntro_PreAttack", nil --[[missing]])
    quest:SetTimeOfDay(0x41b80000)
    quest:TransitionToTheme("ENVIRONMENT_OV_POSTATTACK", nil --[[missing]])
    local ppVar2 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(ppVar2, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06", "", "")
end

function PostAttackStuff(quest)
    local CVar4, bVar3, cVar2, fVar11, pCVar12, pCVar8, piVar5, hero, ppVar7, ppVar9, r1, uVar15
    local alive = true
    while true do
        piVar5 = quest:GetThingWithScriptName("M_PostAttackStart")
        cVar2 = (piVar5 ~= nil and piVar5:IsAlive())
        if cVar2 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:CacheMusicSet(45)
        CVar4 = quest:GetThingWithScriptName("M_PostAttackStart")
        hero = quest:GetHero()
        quest:EntityTeleportToThing(hero, CVar4, false)
        uVar15 = 1
        ppVar7 = quest:GetThingWithScriptName("V_OakVale")
        quest:SetVillageLimbo(ppVar7, true)
        pCVar12 = 0x0
        quest:DisplayMoneyBag((pCVar12 ~= 0))
        quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
        quest:AddLogbookStoryEntry(20)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:CameraResetToViewBehindHero(0.0)
            quest:CameraDefault()
            quest:CacheMusicSet(57)
            quest:FadeScreenIn()
            r1 = quest:GetThingWithScriptName("MK_OVI_DADTRIGGER")
            fVar11 = 5.0
            pCVar8 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar8, r1, 5.0)
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00dbef57 end
                fVar11 = 5.0
                pCVar8 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar8, r1, 5.0)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:SetStateBool("DadFound", true)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&stack0xffffff98);
                ppVar9 = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): this_00 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff74,(CCharString *)&stack0xffffff70);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)this_00,pCVar10);
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                ppVar9 = 0x0
                -- TODO(native): RunCutsceneMacro_Func();
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                -- TODO(native): StdMap_Destroy_API();
                ppVar9 = quest:GetThingWithScriptName("V_OakVale")
                quest:SetVillageLimbo(ppVar9, false)
                -- TODO(native): pCStack_d8 = this + 0x4c;
                quest:SetTimeAsStopped(false)
                quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)
                quest:ResetToDefaultTheme(0)
                quest:StopOverrideMusic(false)
            end
            ::LAB_00dbef57::
        end
    end
end

function ManageQuestCoreMarkers(quest)
    local CVar1, bVar2, cVar3, iVar4, ppVar6, r1, r2, r3
    local alive = true
    r1 = quest:GetThingWithScriptName("NOVI_LiveFather")
    r2 = quest:GetThingWithScriptName("NOVI_BookTrader")
    r3 = quest:GetThingWithScriptName("NOVI_Theresa")
    quest:MiniMapAddMarker(r1, "HUD_ORB_QUEST_CORE")
    iVar4 = quest:GetHeroGold()
    while iVar4 < 3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar4 = quest:GetHeroGold()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        cVar3 = quest:IsHeroControlledByPlayer()
        while not cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00dbe7b4 end
            cVar3 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:MiniMapRemoveMarker(r1)
            quest:MiniMapAddMarker(r2, "HUD_ORB_QUEST_CORE")
            cVar3 = quest:DisplayTutorial(0x13)
            if cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00dbe7b4 end
                cVar3 = quest:MsgIsTutorialClickedPast()
                while not cVar3 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00dbe7b4 end
                    cVar3 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00dbe7b4 end
            end
            CVar1 = quest:GetStateBool("GivenSweets")
            while not CVar1 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00dbe7b4 end
                CVar1 = quest:GetStateBool("GivenSweets")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:MiniMapRemoveMarker(r2)
                quest:MiniMapAddMarker(r3, "HUD_ORB_QUEST_CORE")
                CVar1 = quest:GetStateBool("GivenTheresaChocs")
                while not CVar1 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00dbe7b4 end
                    CVar1 = quest:GetStateBool("GivenTheresaChocs")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:MiniMapRemoveMarker(r3)
                    quest:MiniMapAddMarker(r1, "HUD_ORB_QUEST_CORE")
                end
            end
        end
    end
    ::LAB_00dbe7b4::
end

function StartBarrelTimer(quest)
    local CVar1, bVar2, fVar7, iVar3, native_bar_timer_value, pCVar4, r1, uVar6
    local alive = true
    iVar3 = quest:GetTimer(quest:GetStateInt("WatchTimer"))
    while iVar3 < 1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar3 = quest:GetTimer(quest:GetStateInt("WatchTimer"))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        uVar6 = quest:AddQuestInfoBar(45.0, 0.0, {r=0, g=255, b=0, a=255}, {r=0, g=255, b=0, a=255}, "HUD_CLOCK_ICON", "", 1.0)
        quest:SetStateInt("GUIBarrelCounter", uVar6)
        r1 = quest:GetThingWithScriptName("M_WHouse_GuardPoint")
        CVar1 = quest:GetStateBool("BarrelManSpokenToHeroOnReturn")
        while not CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00db5192 end
            fVar7 = 2.0
            pCVar4 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar4, r1, fVar7)
            if bVar2 then
                quest:ChangeQuestInfoBarColour(quest:GetStateInt("GUIBarrelCounter"), {r=0, g=255, b=0, a=255}, {r=0, g=255, b=0, a=255})
            else
                quest:ChangeQuestInfoBarColour(quest:GetStateInt("GUIBarrelCounter"), {r=255, g=0, b=0, a=255}, {r=255, g=0, b=0, a=255})
            end
            native_bar_timer_value = quest:GetTimer(quest:GetStateInt("WatchTimer"))
            quest:UpdateQuestInfoBar(quest:GetStateInt("GUIBarrelCounter"), native_bar_timer_value, -1.0, -1.0)
            CVar1 = quest:GetStateBool("BarrelManSpokenToHeroOnReturn")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBarrelCounter"))
        end
        ::LAB_00db5192::
    end
end

function WatchBarrels(quest)
    local bVar2, iVar3, iVar4, pCVar7, ppVar6, puVar1, r1, r2, uVar5
    local alive = true
    pCVar7 = "NOVI_Barrel"
    uVar5 = quest:GetAllThingsWithScriptName("NOVI_Barrel")
    iVar3 = uVar5
    while iVar3 < 1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            while puVar1 ~= puStack_28 do
                -- TODO(native): (**(code **)*puVar1)(0);
                puVar1 = puVar1 + 3
            end
            if puStack_2c == nil then
                return
            end
            -- TODO(native): free(puStack_2c);
            return
        end
        uVar5 = quest:GetAllThingsWithScriptName("NOVI_Barrel")
        iVar3 = uVar5
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00dbeb07: (native jump target)
    else
        iVar3 = (puStack_28 - puStack_2c) / 0xc
        quest:SetStateBool("BarrelBrokenInstantaneous", false)
        iVar4 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while true do
            if not (not bVar2) then break end
            if quest:GetStateBool("AttackOver") then return end  -- TODO(native): goto LAB_00dbeb07
            if quest:GetStateBool("BarrelBrokenInstantaneous") then
                iVar4 = iVar4 + 1
                quest:SetStateBool("BarrelBrokenInstantaneous", false)
                if iVar4 == 1 then
                    AddBadDeed(quest, 0)
                else
                    if iVar4 == iVar3 + -1 then
                        r1 = quest:GetThingWithScriptName("NOVI_Barrel")
                        quest:AddItemToContainer(r1, "OBJECT_GOLD_1")
                        -- TODO(native): this = (C3DClothPrimitive *)&uStack_20;
                    else
                        if iVar4 <= iVar3 + -4 then goto LAB_00dbeabd end
                        r2 = quest:CreateCreature("CREATURE_OAKVALE_STAG_BEETLE", param_1 + 0x76, "NOVI_CreatedBeetle")
                        quest:EntitySetMaxHealth(r2, 0x40000000, true)
                        -- TODO(native): this = (C3DClothPrimitive *)apStack_14;
                    end
                end
            end
            ::LAB_00dbeabd::
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
        while puVar1 ~= puStack_28 do
            -- TODO(native): (**(code **)*puVar1)(0);
            puVar1 = puVar1 + 3
        end
        if puStack_2c ~= nil then
            -- TODO(native): free(puStack_2c);
            return
        end
    end
end

function WatchForGotGold(quest)
    local bVar1, ppVar3
    local alive = true
    local iVar2 = quest:GetHeroGold()
    while true do
        if 2 < iVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                ppVar3 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(ppVar3, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03", "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03", "")
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        iVar2 = quest:GetHeroGold()
    end
end

function AddGoodDeed(quest)
    local bVar1, cVar2, uVar4
    local alive = true
    quest:SetStateInt("GoodDeedsPerformed", quest:GetStateInt("GoodDeedsPerformed") + 1)
    quest:GiveHeroMorality(0.0010000000474974513)
    if (quest:GetStateInt("GoodDeedsPerformed") == 1) and (quest:GetStateInt("BadDeedsPerformed") == 0) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED")
        cVar2 = quest:MsgIsGameInfoClickedPast()
        while not cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            cVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_GOOD_DEED")
        cVar2 = quest:MsgIsGameInfoClickedPast()
        while not cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            cVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    end
    local iVar3 = quest:GetHeroGold()
    if (iVar3 < 3) and (not quest:GetStateBool("GivenSweets")) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        uVar4 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(uVar4, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02", "", "")
    end
    quest:UpdateQuestInfoCounter(quest:GetStateInt("GUIGoodDeedCounter"), quest:GetStateInt("GoodDeedsPerformed"), -1)
end

function AddBadDeed(quest, native_arg_param_1)
    local bVar1, cVar2
    local alive = true
    quest:SetStateInt("BadDeedsPerformed", quest:GetStateInt("BadDeedsPerformed") + 1)
    quest:GiveHeroMorality(-0.0010000000474974513)
    if (quest:GetStateInt("BadDeedsPerformed") == 1) and (quest:GetStateInt("GoodDeedsPerformed") == 0) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED")
            cVar2 = quest:MsgIsGameInfoClickedPast()
            while not cVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                cVar2 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
                quest:SetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1), true)
                return
            end
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            if not quest:GetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1)) then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                quest:DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_BAD_DEED")
                cVar2 = quest:MsgIsGameInfoClickedPast()
                while not cVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
            end
            quest:SetStateBool("WhichBadDeedsPerformed_" .. (native_arg_param_1), true)
        end
    end
end

function destructor(quest, native_arg_param_1)
    -- TODO(native): CParticleEmitter_Dtor_7();
    if (native_arg_param_1 & 1) ~= 0 then
    end
    return this
end

