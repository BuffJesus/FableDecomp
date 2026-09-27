-- Generated from the same native helper bodies as the quest draft.
local helper_E55C60
function helper_E55C60(quest, me, native_arg_strParam_1, native_arg_strParam_2, native_arg_strParam_3, native_arg_strParam_4)
    quest:AddRumourCategory(native_arg_strParam_1)
    quest:AddNewRumourToCategory(native_arg_strParam_1, native_arg_strParam_2)
    quest:AddGossipVillage(native_arg_strParam_1, native_arg_strParam_3)
    quest:AddGossipFactionToCategory(native_arg_strParam_1, native_arg_strParam_4)
end

return {helper_E55C60 = helper_E55C60}
