        if __native_entity_state:GetStateInt("MyPhase") == 0 then goto FLOW_native_label_1 end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        native_arg_switch_2 = __native_entity_state:GetStateInt("MyPhase")
        repeat
            if native_arg_switch_2 == 1 then
                quest:SetTimer(quest:GetStateInt("WatchTimer"), 0x2d)
                quest:EntitySetTargetable(me, false)
                if not acquireBarrelControl() then goto LAB_00db6afd end
                if not walkOffFromWarehouse() then goto LAB_00db6afd end
                break
            else
                if native_arg_switch_2 == 2 then
                    if not teleportWalkOff() then goto LAB_00db6afd end
                    break
                else
                    if native_arg_switch_2 == 3 then
                        if not acquireBarrelControl() then goto LAB_00db6afd end
                        quest:EntitySetCutsceneBehaviour(me, 1)
                        quest:EntitySetTargetable(me, false)
                        if not returnToWarehouse(warehouseStartMarker) then goto LAB_00db6afd end
                        break
                    else
                        if native_arg_switch_2 == 4 then
                            quest:SetStateBool("BarrelManSpokenToHeroOnReturn", true)
                            resources:FaceBarrelManTowardsHero(me)
                            quest:EntitySetCutsceneBehaviour(me, 2)
                            quest:EntitySetTargetable(me, true)
                            if resources:ShouldBarrelManThankHero(me) then
                                -- LAB_00db5c28: (native jump target)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db6afd end
                                if not acquireBarrelControl() then goto LAB_00db6afd end
                                if not playThanksMovie() then goto LAB_00db6afd end
                                -- LAB_00db5daf: (native jump target)
                                quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
                                __native_entity_state:SetStateInt("MyPhase", 5)
                                break
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db6afd end
                            resources:AddBarrelConversation(me, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE")
                            __native_entity_state:SetStateBool("HeroLetMeDown", true)
                            if not showWarehouseFailure() then goto LAB_00db6afd end
                            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
                            __native_entity_state:SetStateInt("MyPhase", 5)
                            break
                        end
                    end
                end
            end
        until not (false)
        ::FLOW_native_label_1::
