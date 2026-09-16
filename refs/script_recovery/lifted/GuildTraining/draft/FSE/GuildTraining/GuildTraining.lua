-- Generated native draft: Q_GuildTraining. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar3, bVar9, cVar4, iVar2, ppVar5, r1, uStack_44, uVar7
    local alive = true
    if quest:GetStateInt("GameState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:RemoveAllHeroWeapons()
        iVar2 = *piVar1
        -- TODO(native): puStack_40 = auStack_18;
        uStack_44 = quest:GetThingWithScriptName("GuildArrivalHSP")
        ppVar5 = quest:GetHero()
        quest:EntityTeleportToThing(ppVar5, uStack_44)
        cVar4 = quest:IsLevelLoaded("LookoutPoint")
        cVar4 = '\x01' - (cVar4)
        while cVar4 ~= 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            cVar4 = quest:IsLevelLoaded("LookoutPoint")
            cVar4 = '\x01' - (cVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_MELEE", "OBJECT_QUEST_CARD_TRAINING_MELEE", false)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        ppVar5 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(ppVar5, false, nil --[[missing]])
        RunArrivalCutscene(quest)
        quest:FadeScreenOut(0x3f000000, 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
    end
    iVar2 = *piVar1
    -- TODO(native): puStack_40 = auStack_18;
    uStack_44 = quest:GetThingWithScriptName("GuildTrainingHSP")
    ppVar5 = quest:GetHero()
    quest:EntityTeleportToThing(ppVar5, uStack_44)
    cVar4 = quest:IsLevelLoaded("HeroGuildComplex")
    cVar4 = '\x01' - (cVar4)
    while true do
        if cVar4 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:SetAllowScreenFadingOnNextRegionChange(nil --[[missing]])
                ppVar5 = 0x1
                quest:SetRegionTextDisplayAsActive((ppVar5 ~= 0))
                quest:AddEntityBinding("AppleGirl", "GuildTraining/Entities/AppleGirl")
                bVar9 = not bVar3 and bVar3
                quest:AddEntityBinding("ApprenticeSpeedTest", "GuildTraining/Entities/ApprenticeSpeedTest")
                quest:AddEntityBinding("SpeedFriend", "GuildTraining/Entities/SpeedFriend")
                quest:AddEntityBinding("RaceMarker", "GuildTraining/Entities/RaceMarker")
                quest:AddEntityBinding("MeleeApprentice", "GuildTraining/Entities/MeleeApprentice")
                quest:AddEntityBinding("CombatApprentice", "GuildTraining/Entities/CombatApprentice")
                quest:AddEntityBinding("SkillApprentice", "GuildTraining/Entities/SkillApprentice")
                quest:AddEntityBinding("WillApprentice", "GuildTraining/Entities/WillApprentice")
                quest:AddEntityBinding("HeroBed", "GuildTraining/Entities/HeroBed")
                quest:AddEntityBinding("SkillTarget", "GuildTraining/Entities/SkillTarget")
                quest:AddEntityBinding("WillDummy", "GuildTraining/Entities/WillDummy")
                quest:AddEntityBinding("BirdKiller", "GuildTraining/Entities/BirdKiller")
                quest:AddEntityBinding("KillBird", "GuildTraining/Entities/KillBird")
                quest:AddEntityBinding("PreMeleeMaze", "GuildTraining/Entities/PreMeleeMaze")
                quest:FinalizeEntityBindings()
                -- TODO(native): SetHeroGuideToShowQuestCardsWhenSpokenTo is not a ForgeFSE binding
                quest:SetHeroGuideToShowQuestCardsWhenSpokenTo()
                quest:SetWeaponOutCrimeEnabled(nil --[[missing]])
                quest:SetGuardsIgnoreCrimes(true)
                uVar7 = quest:GetHero()
                r1 = quest:GetNearestWithDefName(uVar7, ppVar5)
                -- TODO(native): p_Var11 = (_func_void *)0x0;
                quest:EnableGuards(r1, nil --[[missing]])
                quest:CreateThread("RunTutorials")  -- native thread body 0x00D45DD0: lift it as function RunTutorials(quest)
                if not bVar3 then
                end
                quest:CreateThread("CheckFriendlyAttacks")  -- native thread body 0x00D45060: lift it as function CheckFriendlyAttacks(quest)
                quest:CreateThread("KeepTabsOnWhisper")  -- native thread body KeepTabsOnWhisper: lift it as function KeepTabsOnWhisper(quest)
                if not bVar3 then
                end
                quest:CreateThread("WatchForSparrowKilled")  -- native thread body 0x00D3C840: lift it as function WatchForSparrowKilled(quest)
                if not bVar3 then
                end
                quest:CreateThread("KeepBookcaseExitRemoved")  -- native thread body KeepBookcaseExitRemoved: lift it as function KeepBookcaseExitRemoved(quest)
                if not bVar3 then
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        cVar4 = quest:IsLevelLoaded("HeroGuildComplex")
        cVar4 = '\x01' - (cVar4)
    end
end

function Init(quest)
    quest:SetStateInt("GameState", 0)
    quest:SetStateInt("HeroWarnings", 0)
    quest:SetStateBool("DomeCutsceneStart", false)
    quest:SetStateInt("StartedTesting", 0)
    quest:SetStateBool("StartedMeleeTesting", false)
    quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 0)
    -- TODO(native): this[100] = (CGameflowScript)0x0;
    quest:SetStateBool("DisplayBirdKilledMessage", false)
    GossipSetup(quest)
end

function OnPersist(quest, context)
    local gameState = quest:GetStateBool("GameState") or false
    gameState = quest:PersistTransferBool(context, "GameState", gameState)
    quest:SetStateBool("GameState", gameState)
    local currentBirdsKilled = quest:GetStateBool("CurrentBirdsKilled") or false
    currentBirdsKilled = quest:PersistTransferBool(context, "CurrentBirdsKilled", currentBirdsKilled)
    quest:SetStateBool("CurrentBirdsKilled", currentBirdsKilled)
end

function RunTutorials(quest)
    local aVar21, aVar23, bVar3, cVar2, iVar12, pCVar13, pCVar14, piVar6, piVar7, ppVar10, ppVar19, ppVar4, ppVar5, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r41, r42, r43, r44, r45, r46, r47, r48, r49, r5, r50, r51, r52, r53, r54, r55, r56, r57, r58, r59, r6, r7, r8, r9, uVar11, uVar22, uVar24, uVar9
    local alive = true
    r1 = quest:GetThingWithScriptName("SecretBookcase")
    r2 = quest:GetNearestWithDefName(r1, "REGION_EXIT_POINT")
    quest:SetRegionExitAsActive(r2, false)
    uVar24 = 0
    quest:SetExperienceSpendingAsEnabled((uVar24 ~= 0))
    uVar22 = 0
    quest:SetHeroSleepingAsEnabled((uVar22 ~= 0))
    r3 = quest:GetThingWithScriptName("GuildDoors")
    ppVar4 = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:MiniMapAddMarker(ppVar4, "HUD_ORB_QUEST_CORE")
    cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0x0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", nil --[[missing]])
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingDeparture", nil --[[missing]])
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingMelee", nil --[[missing]])
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingSkill", nil --[[missing]])
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
    end
    if quest:GetStateInt("GameState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:SetThingAsUsable(r3, nil --[[missing]])
        quest:SetThingPersistent(nil --[[missing]], nil --[[missing]])
        r4 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", r4)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_GUILD_SEAL_1")
        end
        quest:SetStateInt("GameState", 1)
    end
    ppVar5 = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    quest:SetTeleporterAsActive(ppVar5, nil --[[missing]])
    piVar6 = quest:GetThingWithScriptName("MeleeOpponent")
    cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        r5 = quest:GetThingWithScriptName("MeleeOpponent")
        quest:RemoveThing(r5)
    end
    if quest:GetStateInt("GameState") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:SetMasterGameState("ScorpionsDestroyed", false)
        quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", false)
        quest:SetMasterGameState("SkillTrainingStarted", false)
        piVar6 = quest:GetThingWithScriptName("PreMeleeDummy")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            piVar6 = quest:GetThingWithScriptName("OBJECT_STRAW_DUMMY_01")
            piVar6:GetPos()
            r6 = quest:CreateObject("PreMeleeDummyMarker", nil --[[missing]], "PreMeleeDummy")
            piVar7 = quest:GetThingWithScriptName("PreMeleeDummy")
            iVar12 = *piVar6
            piVar7:GetAngleXY()
            r7 = quest:GetThingWithScriptName("PreMeleeDummyMarker")
            quest:EntitySetFacingAngle(r7, nil --[[missing]])
        end
        piVar6 = quest:GetThingWithScriptName("MeleeApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r8 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(r8)
        end
        piVar6 = quest:GetThingWithScriptName("CombatApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r9 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(r9)
        end
        piVar6 = quest:GetThingWithScriptName("SkillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r10 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(r10)
        end
        piVar6 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r11 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(r11)
        end
        piVar6 = quest:GetThingWithScriptName("BirdKiller")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r12 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(r12)
        end
        r13 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_HERO_STICK", r13)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_HERO_STICK")
        end
        quest:SetCategoryActivity("Sweet Kid", nil --[[missing]])
        cVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingPreMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingPreMelee", nil --[[missing]])
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r14 = quest:GetAllThingsWithScriptName("AppleMarker")
            iVar12 = 0x0 - 0 >> 0x1f
            if (0x0 - 0) / 0xc + iVar12 ~= iVar12 then
                iVar12 = 0
                uVar11 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then return end  -- TODO(native): goto LAB_00d46c49
                    -- TODO(native): (**(code **)(*(int *)(iStack_94 + iVar12) + 0x18))();
                    r15 = quest:CreateObject("OBJECT_APPLE_RED_01", uVar11, "")
                    quest:SetThingPersistent(r15, (iVar12 ~= 0))
                    quest:SetThingPersistent(piVar6, false)
                    quest:RemoveThing(piVar7)
                    uVar11 = uVar11 + 1
                    iVar12 = iVar12 + 0xc
                until not (uVar11 < ((0x0 - 0) / 0xc))
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d46c49: (native jump target)
                goto LAB_00d496bc
            end
        end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
        while cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            cVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0x0)
        end
        quest:SetCategoryActivity("Sweet Kid", false)
        quest:SetStateInt("GameState", 3)
        quest:GiveHeroGold(nil --[[missing]])
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        piVar6 = quest:GetThingWithScriptName("PreMeleeDummy")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r16 = quest:GetThingWithScriptName("PreMeleeDummy")
            quest:RemoveThing(r16)
        end
        piVar6 = quest:GetThingWithScriptName("PreMeleeWhisper")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r17 = quest:GetThingWithScriptName("PreMeleeWhisper")
            quest:RemoveThing(r17)
        end
    end
    if quest:GetStateInt("GameState") == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        r18 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", r18)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
        end
        r19 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_IRON_KATANA", r19)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_KATANA")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        piVar6 = quest:GetThingWithScriptName("MeleeApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r20 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(r20)
        end
        piVar6 = quest:GetThingWithScriptName("CombatApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r21 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(r21)
        end
        piVar6 = quest:GetThingWithScriptName("SkillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r22 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(r22)
        end
        piVar6 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r23 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(r23)
        end
        piVar6 = quest:GetThingWithScriptName("BirdKiller")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r24 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(r24)
        end
        quest:SetCategoryActivity("Wannabe Hero", nil --[[missing]])
        quest:SetCategoryActivity("Young Apprentice", nil --[[missing]])
        quest:SetCategoryActivity("Young Annoyance", nil --[[missing]])
        r25 = quest:GetHero()
        r26 = quest:TurnCreatureInto(r25, "Young Apprentice")
        quest:GiveHeroExpression("EXPRESSION_FART", nil --[[missing]])
        quest:GiveHeroExpression("EXPRESSION_BELCH", nil --[[missing]])
        quest:GiveHeroExpression("EXPRESSION_GIGGLE", nil --[[missing]])
        quest:GiveHeroExpression("EXPRESSION_FLIRT", nil --[[missing]])
        quest:GiveHeroExpression("EXPRESSION_COSSACK", -1, true)
        cVar2 = quest:IsXbox()
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", nil --[[missing]])
        end
        ppVar5 = quest:GetHero()
        -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
        quest:EntitySetAsOpinionSource()
        quest:SetHeroAsTeenager(nil --[[missing]])
        quest:SetHeroAsApprentice(nil --[[missing]])
        quest:GiveHeroAbility(nil --[[missing]], nil --[[missing]])
        cVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingMelee", nil --[[missing]])
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
        while cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            cVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:RemoveQuestCardFromGuild("Q_GuildTraining")
        quest:SetCategoryActivity("Complete Melee", nil --[[missing]])
        quest:SetStateInt("GameState", 5)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        piVar6 = quest:GetThingWithScriptName("CombatApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar6:GetPos()
            r27 = quest:CreateCreature("CombatApprenticeMarker", nil --[[missing]], "CombatApprenticeMarker")
            piVar6 = quest:GetThingWithScriptName("CombatApprentice")
            piVar6:SetToKillOnLevelUnload(nil --[[missing]])
        end
    end
    if quest:GetStateInt("GameState") == 5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        r28 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", r28)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
        end
        r29 = quest:GetHero()
        cVar2 = quest:IsObjectInThingsPossession("OBJECT_YEW_CROSSBOW", r29)
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_CROSSBOW")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        quest:SetMasterGameState("MovingDummiesNeeded", false)
        piVar6 = quest:GetThingWithScriptName("MeleeApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r30 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(r30)
        end
        piVar6 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE")
        piVar6:GetPos()
        r31 = quest:CreateCreature("M_MeleeOpponentStand", nil --[[missing]], "M_MeleeOpponentStand")
        piVar6 = quest:GetThingWithScriptName("SkillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r32 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(r32)
        end
        piVar6 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r33 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(r33)
        end
        piVar6 = quest:GetThingWithScriptName("BirdKiller")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r34 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(r34)
        end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingSkill")
            quest:SetQuestAsPersistent("Q_GuildTrainingSkill", nil --[[missing]])
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
        while cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            cVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:SetCategoryActivity("Complete Skill", nil --[[missing]])
        quest:SetStateInt("GameState", 7)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("MovingDummiesNeeded", true)
        piVar6 = quest:GetThingWithScriptName("SkillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar6:GetPos()
            r35 = quest:CreateCreature("SkillApprenticeMarker", nil --[[missing]], "SkillApprenticeMarker")
            piVar6 = quest:GetThingWithScriptName("SkillApprentice")
            piVar6:SetToKillOnLevelUnload(nil --[[missing]])
        end
        piVar6 = quest:GetThingWithScriptName("BirdKiller")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar6:GetPos()
            r36 = quest:CreateCreature("BirdKillerMarker", nil --[[missing]], "BirdKiller")
        end
    end
    if quest:GetStateInt("GameState") == 7 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:GiveHeroAbility(nil --[[missing]], nil --[[missing]])
        piVar6 = quest:GetThingWithScriptName("MeleeApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            r37 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(r37)
        end
        piVar6 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE")
        piVar6:GetPos()
        r38 = quest:CreateCreature("M_MeleeOpponentStand", nil --[[missing]], "M_MeleeOpponentStand")
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingWill")
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
        while cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            cVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:SetCategoryActivity("Complete Will", nil --[[missing]])
        quest:SetStateInt("GameState", 9)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        piVar6 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar6:GetPos()
            r39 = quest:CreateCreature("WillApprenticeMarker", nil --[[missing]], "WillApprenticeMarker")
            piVar6 = quest:GetThingWithScriptName("WillApprentice")
            piVar6:SetToKillOnLevelUnload(nil --[[missing]])
        end
    end
    piVar6 = quest:GetThingWithScriptName("MeleeApprentice")
    cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
    if cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        r40 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(r40)
    end
    piVar6 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
    piVar6:GetPos()
    r41 = quest:CreateCreature("MeleeApprenticeMarker", nil --[[missing]], "MeleeApprenticeMarker")
    cVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    if not cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        quest:ActivateQuest("Q_GuildTrainingDeparture")
        quest:SetQuestAsPersistent("Q_GuildTrainingDeparture", nil --[[missing]])
    end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d496bc end
    cVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    while cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d496bc end
    quest:SetQuestCardObjective("Q_GuildTraining", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_13", "HeroGuildComplexInside")
    cVar2 = quest:IsLevelLoaded("HeroGuildComplex")
    while not cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d496bc end
        cVar2 = quest:IsLevelLoaded("HeroGuildComplex")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d496bc end
    r42 = quest:GetThingWithScriptName("TheRealGuildmaster")
    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_78);
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_78);
    if bVar3 then
    end
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar2 = nil --[[unresolved native result]]
    while cVar2 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d48800 end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar2 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_60);
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_60);
        if bVar3 then
        end
        r43 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar2 = nil --[[unresolved native result]]
        while cVar2 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d487f4 end
            r44 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar2 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff3c,(CCharString *)&stack0xffffff30);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar13);
            -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff3c,(CCharString *)&stack0xffffff30);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar14);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_50);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            ppVar5 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera((ppVar5 ~= 0))
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            iVar12 = *piVar6
            r45 = quest:GetThingWithScriptName("FrescoDomeHSP")
            ppVar5 = quest:GetHero()
            quest:EntityTeleportToThing(ppVar5, r45)
            quest:SetStateBool("DomeCutsceneStart", true)
            RunCeremonyCutscene(quest)
            quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", nil --[[missing]])
            quest:SetStateBool("DomeCutsceneStart", false)
            iVar12 = *piVar6
            uVar9 = quest:GetThingWithScriptName("HeroGuildComplexInsideHSP")
            ppVar5 = quest:GetHero()
            quest:EntityTeleportToThing(ppVar5, uVar9)
            cVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            while not cVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d496bc end
                cVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
            quest:OpenDoor(r44)
            quest:SetThingPersistent(r43, nil --[[missing]])
            quest:SetRegionExitAsActive(r42, nil --[[missing]])
            -- TODO(native): __ftol2();
            quest:GiveHeroExperience(nil --[[missing]])
            quest:GiveHeroObject("OBJECT_HERO_BOOTS", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_HERO_TROUSERS", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_HERO_SHIRT", nil --[[missing]])
            quest:GiveHeroObject("OBJECT_HERO_GLOVES", nil --[[missing]])
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d496bc end
            quest:SetHeroAsWearing("OBJECT_HERO_BOOTS")
            quest:SetHeroAsWearing("OBJECT_HERO_TROUSERS")
            quest:SetHeroAsWearing("OBJECT_HERO_SHIRT")
            quest:SetHeroAsWearing("OBJECT_HERO_GLOVES")
            piVar6 = quest:GetThingWithScriptName("SkillApprentice")
            cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
            if cVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d496bc end
                r46 = quest:GetThingWithScriptName("SkillApprentice")
                quest:RemoveThing(r46)
            end
            piVar6 = quest:GetThingWithScriptName("WillApprentice")
            cVar2 = (piVar6 ~= nil and piVar6:IsAlive())
            if cVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d496bc end
                r47 = quest:GetThingWithScriptName("WillApprentice")
                quest:RemoveThing(r47)
            end
            ppVar5 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(ppVar5)
            ppVar5 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(ppVar5)
            ppVar5 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(ppVar5)
            RunSaveXPCutscene2(quest)
            uVar9 = 0xd48ec4
            r48 = quest:GetThingWithScriptName("TheRealGuildmaster")
            quest:SetIsPushableByHero(r48, nil --[[missing]])
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_14", "HeroGuildComplexInside", "")
            -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffed4);
            quest:SetTimer(uVar9, 10)
            cVar2 = quest:MsgOnLeavingExperienceSpendingScreen()
            while not cVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d496b3 end
                iVar12 = quest:GetTimer(nil --[[missing]])
                if iVar12 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d496b3 end
                    ppVar5 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    ppVar10 = quest:AddNewConversation(ppVar5, nil --[[missing]], nil --[[missing]])
                    r49 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r49)
                    iVar12 = *piVar6
                    r50 = quest:GetHero()
                    r51 = quest:GetThingWithScriptName("TEXT_CS_028_LEAVING_TOUR_45")
                    quest:AddLineToConversation(nil --[[missing]], "TheRealGuildmaster", r51, r50)
                    quest:SetTimer(nil --[[missing]], nil --[[missing]])
                end
                cVar2 = quest:MsgOnLeavingExperienceSpendingScreen()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                RunSaveXPCutscene2(quest)
                quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                iVar12 = *piVar6
                r52 = quest:GetThingWithScriptName("TheRealGuildmaster")
                ppVar5 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                quest:EntityTeleportToThing(ppVar5, r52)
                cVar2 = quest:IsHeroControlledByPlayer()
                while not cVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d496b3 end
                    cVar2 = quest:IsHeroControlledByPlayer()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    r53 = quest:GetHero()
                    quest:EntityUnsetAsOpinionSource(r53)
                    quest:SetHeroAsApprentice(nil --[[missing]])
                    quest:GiveHeroExpression("EXPRESSION_FOLLOW", nil --[[missing]])
                    quest:GiveHeroExpression("EXPRESSION_WAIT", nil --[[missing]])
                    quest:SetThingAsUsable(r41, nil --[[missing]])
                    -- TODO(native): SetHeroGuideToShowQuestCardsWhenSpokenTo is not a ForgeFSE binding
                    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo()
                    ppVar5 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    quest:RemoveThing(ppVar5)
                    ppVar5 = quest:GetThingWithScriptName("PreMeleeMaze")
                    quest:RemoveThing(ppVar5)
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:FadeScreenIn()
                                        quest:SetTimeOfDay(nil --[[missing]])
                                        quest:SetWeaponOutCrimeEnabled(nil --[[missing]])
                                        quest:SetGuardsIgnoreCrimes(nil --[[missing]])
                                        r54 = quest:GetHero()
                                        r55 = quest:GetNearestWithDefName(r54, "VILLAGE_GUILD_COMPLEX_INSIDE")
                                        quest:EnableGuards(r55, nil --[[missing]])
                                        quest:SetHeroSleepingAsEnabled(nil --[[missing]])
                                        r56 = quest:GetHero()
                                        cVar2 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", r56)
                                        while cVar2 do
                                            alive = quest:NewScriptFrame()
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d496aa end
                                            quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                                            r57 = quest:GetHero()
                                            cVar2 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", r57)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            r58 = quest:GetAllThingsWithDefName("OBJECT_APPLE_RED_01")
                                            aVar23 = SUB41(uVar24,0)
                                            aVar21 = SUB41(uVar22,0)
                                            iVar12 = 0 - 0x0 >> 0x1f
                                            uVar11 = 0
                                            if (0 - 0x0) / 0xc + iVar12 ~= iVar12 then
                                                repeat
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d496a1 end
                                                    quest:RemoveThing(r39)
                                                    aVar23 = SUB41(uVar24,0)
                                                    aVar21 = SUB41(uVar22,0)
                                                    uVar11 = uVar11 + 1
                                                until not (uVar11 < ((0 - 0x0) / 0xc))
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                ppVar5 = quest:GetActiveQuestName()
                                                quest:SetQuestAsCompleted(ppVar5, false, false, false)
                                                alive = quest:NewScriptFrame()
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if not bVar3 then
                                                    alive = quest:NewScriptFrame()
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        alive = quest:NewScriptFrame()
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            alive = quest:NewScriptFrame()
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                alive = quest:NewScriptFrame()
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if not bVar3 then
                                                                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_THEGUILD")
                                                                    cVar2 = quest:DisplayTutorial(nil --[[missing]])
                                                                    if not cVar2 then
                                                                        -- LAB_00d4967d: (native jump target)
                                                                        r59 = quest:GetActiveQuestName()
                                                                        quest:DeactivateQuestLater(r59, nil --[[missing]])
                                                                    else
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if not bVar3 then
                                                                            cVar2 = quest:MsgIsTutorialClickedPast()
                                                                            while not cVar2 do
                                                                                alive = quest:NewScriptFrame()
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar3 = not alive
                                                                                if bVar3 then goto LAB_00d496a1 end
                                                                                cVar2 = quest:MsgIsTutorialClickedPast()
                                                                            end
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar3 = not alive
                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d4967d
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                            ::LAB_00d496a1::
                                        end
                                        ::LAB_00d496aa::
                                    end
                                end
                            end
                        end
                    end
                end
            end
            ::LAB_00d496b3::
            goto LAB_00d496bc
        end
        ::LAB_00d487f4::
    end
    ::LAB_00d48800::
    ::LAB_00d496bc::
end

function CheckFriendlyAttacks(quest)
    local CVar10, bVar4, cVar5, iVar12, iVar13, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, pCVar14, piVar6, ppStack_4c, ppVar15, puVar2, puVar3, r1, r2, r3, r4, r5, r6, r7, r8, r9, uVar11, uVar7
    local alive = true
    r1 = quest:GetThingWithScriptName("PreMeleeMaze")
    r2 = quest:GetAllCreaturesExcludingHero()
    iVar12 = 0x0 - puStack_a0 >> 0x1f
    uVar11 = 0
    if (0x0 - puStack_a0) / 0xc + iVar12 ~= iVar12 then
        iVar12 = 0
        -- TODO(native): unaff_BP = (C3DClothPrimitive)0x0;
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            puVar3 = 0x0
            -- TODO(native): if (bVar4) goto joined_r0x00d452d1;
            CVar10 = (unaff_BP | 1)
            -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar12) + 8))();
            cVar5 = CCharString__NotEqual()
            if cVar5 == 0 then
                -- LAB_00d45184: (native jump target)
                -- TODO(native): unaff_ESI = unaff_ESI & 0xffffff;
            else
                CVar10 = (unaff_BP | 3)
                -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar12) + 8))();
                cVar5 = CCharString__NotEqual()
                if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d45184
                CVar10 = (unaff_BP | 7)
                -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar12) + 8))();
                cVar5 = CCharString__NotEqual()
                if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d45184
                CVar10 = 0xf
                -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar12) + 8))();
                cVar5 = CCharString__NotEqual()
                -- TODO(native): unaff_ESI = CONCAT13(1,(int3)unaff_ESI);
                if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d45184
            end
            if (CVar10 & 8) ~= 0 then
                CVar10 = (CVar10 & 0xf7)
            end
            if (CVar10 & 4) ~= 0 then
                CVar10 = (CVar10 & 0xfb)
            end
            -- TODO(native): unaff_BP = CVar10;
            if (CVar10 & 2) ~= 0 then
                -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)CVar10 & 0xfd);
            end
            if (unaff_BP & 1) ~= 0 then
                -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP & 0xfe);
            end
            if (unaff_ESI >> 0x18) ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d45346
                -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar12) + 0x10c))();
                quest:EntitySetAsKillable(r1, (iVar12 ~= 0))
            end
            uVar11 = uVar11 + 1
            iVar12 = iVar12 + 0xc
        until not (uVar11 < ((0x0 - puStack_a0) / 0xc))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    puVar3 = 0x0
    if not bVar4 then
        while puVar2 ~= puVar3 do
            -- TODO(native): (**(code **)*puVar2)();
            puVar2 = puVar2 + 3
        end
        if puStack_a0 ~= nil then
            -- TODO(native): free(puStack_a0);
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        while not bVar4 do
            cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
            if not cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then break end
                cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
                while not cVar5 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d45d42 end
                    cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
                iVar13 = 0
                r3 = quest:GetAllCreaturesExcludingHero()
                iVar12 = 0x0 - 0x0 >> 0x1f
                uVar11 = 0
                if (0x0 - 0x0) / 0xc + iVar12 ~= iVar12 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00d45346
                        -- TODO(native): (**(code **)(*(int *)(iVar13 + (int)puStack_a0) + 8))();
                        cVar5 = CCharString__NotEqual()
                        CVar10 = (unaff_BP | 0x10)
                        if cVar5 == 0 then
                            -- LAB_00d4557d: (native jump target)
                            -- TODO(native): unaff_BP = CVar10;
                            -- TODO(native): unaff_ESI = unaff_ESI & 0xffffff;
                        else
                            -- TODO(native): (**(code **)(*(int *)(iVar13 + (int)puStack_a0) + 8))();
                            cVar5 = CCharString__NotEqual()
                            CVar10 = (unaff_BP | 0x30)
                            if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d4557d
                            -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar13) + 8))();
                            cVar5 = CCharString__NotEqual()
                            CVar10 = (unaff_BP | 0x70)
                            if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d4557d
                            -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP | 0xf0);
                            -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar13) + 8))();
                            cVar5 = CCharString__NotEqual()
                            -- TODO(native): unaff_ESI = CONCAT13(1,(int3)unaff_ESI);
                            CVar10 = unaff_BP
                            if cVar5 == 0 then return end  -- TODO(native): goto LAB_00d4557d
                        end
                        if unaff_BP < 0 then
                            -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP & 0x7f);
                        end
                        if (unaff_BP & 0x40) ~= 0 then
                            -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP & 0xbf);
                        end
                        if (unaff_BP & 0x20) ~= 0 then
                            -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP & 0xdf);
                        end
                        if (unaff_BP & 0x10) ~= 0 then
                            -- TODO(native): unaff_BP = (C3DClothPrimitive)((byte)unaff_BP & 0xef);
                        end
                        if (unaff_ESI >> 0x18) ~= 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then return end  -- TODO(native): goto LAB_00d45346
                            -- TODO(native): (**(code **)(*(int *)((int)puStack_a0 + iVar13) + 0x10c))();
                            quest:EntitySetAsKillable(nil --[[missing]], (iVar13 ~= 0))
                        end
                        uVar11 = uVar11 + 1
                        iVar13 = iVar13 + 0xc
                    until not (uVar11 < ((0x0 - 0x0) / 0xc))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00d45346: (native jump target)
                    return
                end
            end
            piVar6 = quest:GetHero()
            -- TODO(native): ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0xd45675;
            -- TODO(native): MsgHitFriendWithBareHands is not a ForgeFSE binding
            cVar5 = piVar6:MsgHitFriendWithBareHands()
            if not cVar5 then
                piVar6 = quest:GetHero()
                -- TODO(native): MsgHitFriendWithMeleeWeapon is not a ForgeFSE binding
                cVar5 = piVar6:MsgHitFriendWithMeleeWeapon()
                if cVar5 then return end  -- TODO(native): goto LAB_00d456af
                piVar6 = quest:GetHero()
                -- TODO(native): MsgHitFriendWithRangedWeapon is not a ForgeFSE binding
                cVar5 = piVar6:MsgHitFriendWithRangedWeapon()
                uVar11 = unaff_ESI
                if cVar5 then return end  -- TODO(native): goto LAB_00d456af
                -- LAB_00d45782: (native jump target)
                bVar4 = false
            else
                -- LAB_00d456af: (native jump target)
                cVar5 = quest:IsLevelLoaded("HeroGuildComplex")
                uVar11 = unaff_ESI | 0x100
                if not cVar5 then return end  -- TODO(native): goto LAB_00d45782
                native_arg_sequence_1 = false
                if unaff_EBX ~= nil then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    cVar5 = (**(*unaff_EBX + 0x54))()
                    uVar11 = unaff_ESI | 0x300
                    if cVar5 ~= 0 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then return end  -- TODO(native): goto LAB_00d45782
                uVar11 = unaff_ESI | 0x700
                native_arg_sequence_2 = false
                if unaff_EBX ~= nil then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if native_arg_sequence_2 then
                    cVar5 = (**(*unaff_EBX + 0xa8))()
                    if cVar5 ~= 0 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then
                    uVar11 = unaff_ESI | 0xf00
                    native_arg_sequence_3 = false
                    if unaff_EBX == nil then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                    if not native_arg_sequence_3 then
                        cVar5 = (**(*unaff_EBX + 0xa4))()
                        if cVar5 == 0 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                    end
                    if native_arg_sequence_3 then return end  -- TODO(native): goto LAB_00d45782
                end
                bVar4 = true
            end
            if (uVar11 & 0x800) ~= 0 then
                uVar11 = uVar11 & 0xfffff7ff
            end
            if (uVar11 & 0x400) ~= 0 then
                uVar11 = uVar11 & 0xfffffbff
            end
            if (uVar11 & 0x200) ~= 0 then
                uVar11 = uVar11 & 0xfffffdff
            end
            if (uVar11 & 0x100) ~= 0 then
                uVar11 = uVar11 & 0xfffffeff
            end
            if not bVar4 then goto LAB_00d45cae end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            if 2 < quest:GetStateInt("HeroWarnings") then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
                quest:SetMasterGameState("GuildWarningOccuring", true)
                if (quest:GetMasterGameState("SkillTestOccuring") == 0) and (quest:GetMasterGameState("WillTestOccuring") == 0) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_60);
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_60);
                    if bVar4 then
                    end
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: &stack0xffffff50
                    cVar5 = nil --[[unresolved native result]]
                    while cVar5 == 0 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d45db2 end
                        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: &stack0xffffff50
                        cVar5 = nil --[[unresolved native result]]
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_50);
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_50);
                        if bVar4 then
                        end
                        uVar7 = quest:GetHero()
                        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: uVar7
                        cVar5 = nil --[[unresolved native result]]
                        while cVar5 == 0 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d45da9 end
                            uVar7 = quest:GetHero()
                            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: uVar7
                            cVar5 = nil --[[unresolved native result]]
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            -- TODO(native): StdMap_Construct_API();
                            -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aaStack_2c,aCStack_78);
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar14);
                            -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aaStack_2c,(CCharString *)aaStack_90);
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar14);
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_20);
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            uVar7 = quest:GetHero()
                            ppStack_4c = quest:AddNewConversation(uVar7, false, false)
                            iVar12 = *piVar1
                            uVar7 = quest:GetHero()
                            uVar7 = quest:GetHero()
                            ppVar15 = ppStack_4c
                            quest:AddLineToConversation(ppStack_4c, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", uVar7, nil --[[missing]], false)
                            quest:Pause(0x40000000)
                            quest:FixMovieSequenceCamera(true)
                            ppVar15 = 0x0
                            -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): StdMap_Destroy_API();
                            goto LAB_00d45c9d
                        end
                        ::LAB_00d45da9::
                    end
                    ::LAB_00d45db2::
                    -- LAB_00d45dbb: (native jump target)
                    return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
                while (quest:GetMasterGameState("SkillTestOccuring") ~= 0 or (quest:GetMasterGameState("WillTestOccuring") ~= 0)) do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
                ::LAB_00d45c9d::
                quest:SetStateInt("HeroWarnings", 0)
                quest:SetMasterGameState("GuildWarningOccuring", false)
                goto LAB_00d45cae
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d45dbb
            uVar7 = quest:GetHero()
            uVar7 = quest:AddNewConversation(uVar7, (ppVar15 ~= 0), false)
            iVar12 = quest:GetStateInt("HeroWarnings")
            if iVar12 == 0 then
                iVar12 = *piVar6
                r4 = quest:GetHero()
                r5 = quest:GetHero()
                quest:AddLineToConversation(uVar7, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", r5, r4, false)
                -- LAB_00d45930: (native jump target)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            elseif iVar12 == 1 then
                iVar12 = *piVar6
                r6 = quest:GetHero()
                r7 = quest:GetHero()
                quest:AddLineToConversation(uVar7, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", r7, r6, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                if iVar12 ~= 2 then return end  -- TODO(native): goto LAB_00d45930
                iVar12 = *piVar6
                r8 = quest:GetHero()
                r9 = quest:GetHero()
                quest:AddLineToConversation(uVar7, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", r9, r8, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            ::LAB_00d45cae::
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            -- TODO(native): unaff_ESI = uVar11;
        end
        ::LAB_00d45d42::
        return
    end
    while puVar2 ~= puVar3 do
        -- TODO(native): (**(code **)*puVar2)();
        puVar2 = puVar2 + 3
    end
    if puStack_a0 ~= nil then
        -- TODO(native): free(puStack_a0);
    end
    ::LAB_00d45322::
    do return end
    -- TODO(native): joined_r0x00d452d1:
    while puVar2 ~= puVar3 do
        -- TODO(native): (**(code **)*puVar2)();
        puVar2 = puVar2 + 3
    end
    if puStack_a0 ~= nil then
        -- TODO(native): free(puStack_a0);
    end
    goto LAB_00d45322
end

function KeepTabsOnWhisper(quest)
    local bVar2, cVar3, native_arg_sequence_1, pCVar10, pCVar11, piVar4, ppVar5, r1, uVar6, uVar8, uVar9
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    uVar8 = 0
    repeat
        pCVar11 = "GuildWoods"
        uVar9 = uVar8 | 1
        cVar3 = quest:IsLevelLoaded("GuildWoods")
        if not cVar3 then
            -- LAB_00d3cc48: (native jump target)
            bVar2 = false
        else
            uVar9 = uVar8 | 3
            cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
            bVar2 = true
            if not cVar3 then return end  -- TODO(native): goto LAB_00d3cc48
        end
        if (uVar9 & 2) ~= 0 then
            uVar9 = uVar9 & 0xfffffffd
        end
        if (uVar9 & 1) ~= 0 then
            uVar9 = uVar9 & 0xfffffffe
        end
        uVar8 = uVar9
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            pCVar10 = "HeroGuildComplex"
            cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
            while not cVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
            uVar8 = uVar9
            if cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                piVar4 = quest:GetThingWithScriptName("MeleeApprentice")
                cVar3 = (piVar4 ~= nil and piVar4:IsAlive())
                uVar8 = uVar9
                if cVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    ppVar5 = quest:GetThingWithScriptName("MeleeApprentice")
                    quest:RemoveThing(ppVar5)
                    cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                    while cVar3 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        cVar3 = quest:IsQuestActive("Q_GuildTrainingDeparture")
                        native_arg_sequence_1 = false
                        if not cVar3 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if native_arg_sequence_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
                                        while not cVar3 do
                                            alive = quest:NewScriptFrame()
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then
                                                return
                                            end
                                            cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if not bVar2 then
                                            piVar4 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE")
                                            uVar6 = piVar4:GetPos()
                                            r1 = quest:CreateCreature("M_MeleeOpponentStand", uVar6, "MeleeApprentice")
                                        end
                                    end
                                end
                            end
                        end
                    end
                    return
                end
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    until false
end

function WatchForSparrowKilled(quest)
    local bVar2, cVar3
    local alive = true
    local CVar1 = quest:GetStateBool("DisplayBirdKilledMessage")
    -- TODO(native): pCStack_4 = this;
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        CVar1 = quest:GetStateBool("DisplayBirdKilledMessage")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:DisplayGameInfo("TEXT_QST_028_HERO_KILL_BIRD")
        cVar3 = quest:MsgIsGameInfoClickedPast()
        while not cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            cVar3 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
    end
end

function KeepBookcaseExitRemoved(quest)
    local bVar2, cVar3, iVar1, ppVar4, r1, r2
    local alive = true
    r1 = quest:GetThingWithScriptName("SecretBookcase")
    r2 = quest:GetNearestWithDefName(r1, ppVar4)
    iVar1 = quest:GetStateInt("GameState")
    while iVar1 ~= 9 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
        while cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                if (piStack_18 == nil) or (*piStack_18 = *piStack_18 + -1, *piStack_18 ~= 0) then return end  -- TODO(native): goto LAB_00d3cb49
                -- TODO(native): goto LAB_00d3cb41
            end
            cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
        while not cVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00d3cb49: (native jump target)
                return
            end
            cVar3 = quest:IsLevelLoaded("HeroGuildComplex")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        if quest:GetStateInt("GameState") == 9 then break end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        quest:SetRegionExitAsActive(r2, false)
        iVar1 = quest:GetStateInt("GameState")
    end
    alive = not quest:IsActiveThreadTerminating()
    ::LAB_00d3cac6::
end

function RunArrivalCutscene(quest)
    local bVar2, cVar3, piVar4, ppVar6, ppuStack_98, r1, uStack_78
    local alive = true
    quest:SetTimeOfDay(nil --[[missing]])
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    piVar4 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_MAZE_CUTSCENE")
    uStack_78 = piVar4:GetPos()
    r1 = quest:CreateCreature("MK_GTA_MAZE1", uStack_78, "MK_GTA_MAZE1")
    -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_50);
    if bVar2 then
    end
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar3 = nil --[[unresolved native result]]
    while cVar3 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d44ec4 end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar3 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_4c);
        if bVar2 then
        end
        ppuStack_98 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar3 = nil --[[unresolved native result]]
        while cVar3 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d44ebb end
            ppuStack_98 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar3 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff98,(CCharString *)&uStack_78);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffff98,(CCharString *)&uStack_78);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_3c);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(false)
            ppVar6 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera(false)
            quest:FadeScreenOut(0x3f000000, 0)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:SetRegionTextDisplayAsActive(false)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
            quest:RemoveThing(r1, false, true)
            return
        end
        ::LAB_00d44ebb::
    end
    ::LAB_00d44ec4::
end

function GossipSetup(quest)
    local pCVar2 = "Sweet Kid"
    quest:AddRumourCategory("Sweet Kid")
    quest:AddNewRumourToCategory("Sweet Kid", "Sweet Kid")
    quest:AddGossipVillage("Sweet Kid", "Sweet Kid")
    quest:AddGossipFactionToCategory("Sweet Kid", "Sweet Kid")
    quest:AddGossipFactionToCategory("Sweet Kid", "Sweet Kid")
    quest:AddGossipFactionToCategory("Sweet Kid", "Sweet Kid")
    quest:AddNewRumourToCategory("Young Apprentice", "Young Apprentice")
    quest:AddGossipVillage("Young Apprentice", "Young Apprentice")
    quest:AddGossipFactionToCategory("Young Apprentice", "Young Apprentice")
    quest:AddNewRumourToCategory("Young Annoyance", "Young Annoyance")
    quest:AddGossipVillage("Young Annoyance", "Young Annoyance")
    quest:AddGossipFactionToCategory("Young Annoyance", "Young Annoyance")
    quest:AddRumourCategory("Wannabe Hero")
    quest:AddNewRumourToCategory("Wannabe Hero", "Wannabe Hero")
    quest:AddGossipVillage("Wannabe Hero", "Wannabe Hero")
    quest:AddGossipFactionToCategory("Wannabe Hero", "Wannabe Hero")
    quest:AddRumourCategory("Complete Melee")
    quest:AddNewRumourToCategory("Complete Melee", "Complete Melee")
    quest:AddGossipVillage("Complete Melee", "Complete Melee")
    quest:AddGossipFactionToCategory("Complete Melee", "Complete Melee")
    quest:AddRumourCategory("Complete Skill")
    quest:AddNewRumourToCategory("Complete Skill", "Complete Skill")
    quest:AddGossipVillage("Complete Skill", "Complete Skill")
    quest:AddGossipFactionToCategory("Complete Skill", "Complete Skill")
    quest:AddRumourCategory("Complete Will")
    quest:AddNewRumourToCategory("Complete Will", "Complete Will")
    quest:AddGossipVillage("Complete Will", "Complete Will")
    quest:AddGossipFactionToCategory("Complete Will", "Complete Will")
end

function RunCeremonyCutscene(quest)
    local bVar3, cVar2, pCVar8, piVar4, ppVar6, pppuStack_dc, r1, r2, uStack_a0, uStack_bc
    local alive = true
    cVar2 = quest:IsLevelLoaded("FrescoDome")
    while not cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar2 = quest:IsLevelLoaded("FrescoDome")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        piVar4 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
        uStack_a0 = piVar4:GetPos()
        r1 = quest:CreateCreature("MK_GTC_WHISSTART", uStack_a0, "MK_GTC_WHISSTART")
        piVar4 = quest:GetThingWithScriptName("CREATURE_GUILDKEEPER")
        uStack_bc = piVar4:GetPos()
        r2 = quest:CreateCreature("MK_GTC_GMSTART", uStack_bc, "GM")
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&local_7c);
        if bVar3 then
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar2 = nil --[[unresolved native result]]
        while cVar2 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d4a246 end
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar2 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:EntitySetInFaction(r2, "FACTION_HERO")
            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff80);
            if bVar3 then
            end
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar2 = nil --[[unresolved native result]]
            while cVar2 == 0 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4a23d end
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                cVar2 = nil --[[unresolved native result]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_68);
                -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_68);
                if bVar3 then
                end
                pppuStack_dc = quest:GetHero()
                -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                cVar2 = nil --[[unresolved native result]]
                while cVar2 == 0 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4a234 end
                    pppuStack_dc = quest:GetHero()
                    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
                    cVar2 = nil --[[unresolved native result]]
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    -- TODO(native): StdMap_Construct_API();
                    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_a0,(CCharString *)&uStack_bc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5, (CScriptGameResourceObjectScriptedThingBase *)pCVar8);
                    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_a0,(CCharString *)&uStack_bc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar9);
                    -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_a0,(CCharString *)&uStack_bc);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar9);
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_58);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(false)
                    ppVar6 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func(0,0);
                    quest:FixMovieSequenceCamera(false)
                    quest:RemoveThing(r1, false, true)
                    quest:RemoveThing(nil --[[missing]], false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): StdMap_Destroy_API();
                end
                ::LAB_00d4a234::
            end
            ::LAB_00d4a23d::
        end
        ::LAB_00d4a246::
    end
end

function RunSaveXPCutscene(quest)
    local bVar3, cVar4, pCVar7, ppVar6, r1, r2, r3
    local alive = true
    r1 = quest:GetThingWithScriptName("TheRealGuildmaster")
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_38);
    if bVar3 then
    end
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar4 = nil --[[unresolved native result]]
    while cVar4 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_34);
        if bVar3 then
        end
        r2 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
        while cVar4 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d4997a end
            r3 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar4 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffffb0,(CCharString *)&stack0xffffffa0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffffb0,(CCharString *)&stack0xffffffa0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            quest:FixMovieSequenceCamera(false)
            ppVar6 = 0x0
            -- TODO(native): RunCutsceneMacro_Func(0,0,0);
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
        end
        ::LAB_00d4997a::
    end
end

function RunSaveXPCutscene2(quest)
    local bVar3, cVar4, pCVar7, ppVar6, r1, r2, r3
    local alive = true
    r1 = quest:GetThingWithScriptName("TheRealGuildmaster")
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_38);
    if bVar3 then
    end
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    cVar4 = nil --[[unresolved native result]]
    while cVar4 == 0 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_34);
        if bVar3 then
        end
        r2 = quest:GetHero()
        -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
        cVar4 = nil --[[unresolved native result]]
        while cVar4 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d49caa end
            r3 = quest:GetHero()
            -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
            cVar4 = nil --[[unresolved native result]]
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffffb0,(CCharString *)&stack0xffffffa0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            -- TODO(native): pCVar5 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xffffffb0,(CCharString *)&stack0xffffffa0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar5,pCVar7);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            quest:FixMovieSequenceCamera(false)
            ppVar6 = 0x0
            -- TODO(native): RunCutsceneMacro_Func(0,0,0);
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
        end
        ::LAB_00d49caa::
    end
end

