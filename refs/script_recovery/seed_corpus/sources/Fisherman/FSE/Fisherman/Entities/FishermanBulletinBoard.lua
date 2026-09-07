local MY_SCRIPT_NAME = "FishermanBulletinBoard"

function Main(quest, me)
    quest:Log("FishermanBulletinBoard: Main() started.")

    me:SetReadableText("TEXT_QST_023_SIGN_WASP_PLAGUE")

    while not quest:GetStateBool("HeroHasSavedFishermanFromWasps") do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    me:SetReadableText("TEXT_QST_023_SIGN_COME_FISHING")
    quest:Log("FishermanBulletinBoard: Updated sign to COME_FISHING.")
end
