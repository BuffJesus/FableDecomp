local MY_SCRIPT_NAME = "M_FishermanWaspSpawn"
local WASP_DEF_NAME = "CREATURE_HORNET_01"

function Main(quest, me)
    quest:Log("M_FishermanWaspSpawn: Main() started.")

    -- Wait until the intro cutscene has finished playing
    while not quest:GetStateBool("IntroPlayed") do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    -- If the hero hasn't saved the fisherman yet, spawn the combat wasp
    if not quest:GetStateBool("HeroHasSavedFishermanFromWasps") then
        local myPos = me:GetPos()
        quest:CreateCreature(WASP_DEF_NAME, myPos, "FishermanWasp")
        quest:Log("M_FishermanWaspSpawn: Spawned combat wasp at position.")
    end

    quest:Log("M_FishermanWaspSpawn: Main() completed.")
end
