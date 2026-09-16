"""Native presented-item responses, including acquisition and movie ownership."""
from tools.script_recovery.teddy_girl_health import prove
SOURCE='''function TeddyGirlWithMovie(quest, resources, body)
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, result = xpcall(function()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        return body()
    end, function(err) return err end)
    local cleanupError
    if pauseAttempted then
        local closed, err = pcall(function() quest:PauseAllNonScriptedEntities(false) end)
        if not closed then cleanupError = err end
    end
    local closed, err = pcall(function() resources:DestroyMovie(movie) end)
    if not closed and cleanupError == nil then cleanupError = err end
    if not ok then error(result, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return result
end

function TeddyGirlAcquire(quest, resources, me, control)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function TeddyGirlPresentedResponse(quest, resources, me, control, state, kind, given)
    if kind == "none" then return true end
    if quest:IsActiveThreadTerminating() then return false end
    if kind == "other" then
        if state:GetStateBool("FoundTeddy") then return true end
        if quest:IsActiveThreadTerminating() then return false end
    end
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    if kind == "teddy" then state:SetStateBool("DoneIntro", true) end
    return TeddyGirlWithMovie(quest, resources, function()
        local key = kind == "teddy" and "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY" or "TEXT_QST_048_TEDDYGIRL_DONT_WANT"
        local complete = TeddyGirlSpeak(quest, resources, control, key)
        if complete and kind == "teddy" then given() end
        return complete
    end)
end
'''

def recover(data=None):
    w=prove(data)
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'starts':{'other':0xdaf74d,'teddy':0xdaf87a},
        'end':0xdafa9b,'movieOffsets':{'other':200,'teddy':168},'priority':4,
        'limitations':['The enclosing owner must unwind control and temporary Things after callback errors.','Prepare and acquisition are required raw resource capabilities.','Lua callback-error cleanup is an adapter policy; native phase comparisons cover normal and cancellation paths.']}
