-- progress belongs to this Main invocation; it is not persistent quest state.
local function commitTheresaChocolates(quest, me, resources, progress)
    quest:SetStateBool("GivenTheresaChocs", true)
    resources:TakeTheresaChocolatesAndUpdateObjective()
    progress.givenChocolates = true
    resources:ClearTheresaInformation(me)
end

return commitTheresaChocolates
