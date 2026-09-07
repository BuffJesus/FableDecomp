function Main(quest, me)
    quest:Log("GratefulVillagerSpawn: Main() started. Waiting for PanickedVillagersScene...")

    while not quest:GetStateBool("PanickedVillagersScene") do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    quest:Log("GratefulVillagerSpawn: PanickedVillagersScene triggered. Activating 'Post waspboss killed' category.")
    quest:SetCategoryActivity("Post waspboss killed", true)
end
