    // Explicit native resource actions: no acquisition, wait or termination policy.
    // DB0B30..DB0C1C: short-circuit query with one, two or three CCharStrings.
    // Keeping each constructed string alive to return preserves reverse cleanup.
    bool IsHitByHeroExceptAbility(CScriptThing* actor, int excludedAbility) {
        CheckOpen();
        if (!actor || !actor->pVTable)
            throw std::runtime_error("Retail hit query requires an actor");
        auto* table = reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
        if (!table->MsgIsHitBy || !table->MsgIsHitByAnySpecialAbilityFrom || !table->MsgIsHitBySpecialAbilityFrom)
            throw std::runtime_error("Retail hit query slots are unavailable");
        FableString ordinary("SCRIPT_NAME_HERO");
        if (table->MsgIsHitBy(actor, ordinary.get())) return true;
        FableString anyAbility("SCRIPT_NAME_HERO");
        if (!table->MsgIsHitByAnySpecialAbilityFrom(actor, anyAbility.get())) return false;
        FableString excluded("SCRIPT_NAME_HERO");
        return !table->MsgIsHitBySpecialAbilityFrom(actor, static_cast<EHeroAbility>(excludedAbility), excluded.get());
    }
    // The native husband loads this byte at DB1697/DB1BEB/DB1C41 into
    // PlayAnimation's fifth boolean. Its gameplay meaning is not yet named.
    bool ReadAnimationArgument5() {
        CheckOpen();
        auto* argument = ASLR<const volatile unsigned char*>(0x01375748);
        if (!argument) throw std::runtime_error("Retail animation argument is unavailable");
        return *argument != 0;
    }
    void MoveToPosition(unsigned id, sol::table position, float radius, int moveType,
                        bool avoidObstacles, bool ignorePathPreferability) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->MoveToPosition)
            throw std::runtime_error("Retail resource MoveToPosition is unavailable");
        // A missing coordinate is unresolved input, not a zero-vector fallback.
        C3DVector pos = { position["x"].get<float>(), position["y"].get<float>(), position["z"].get<float>() };
        table->MoveToPosition(expert, &pos, radius, static_cast<EScriptEntityMoveType>(moveType),
                              avoidObstacles, ignorePathPreferability);
    }
    void PlayAnimation(unsigned id, const std::string& name,
                       bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->PlayAnimation)
            throw std::runtime_error("Retail resource PlayAnimation is unavailable");
        FableString animation(name.c_str());
        table->PlayAnimation(expert, animation.get(), b1, b2, b3, b4, b5, b6, b7);
    }
    void ClearCommands(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->ClearCommands)
            throw std::runtime_error("Retail resource ClearCommands is unavailable");
        table->ClearCommands(expert);
    }
    void ClearAllActions(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->ClearAllActions)
            throw std::runtime_error("Retail resource ClearAllActions is unavailable");
        table->ClearAllActions(expert);
    }
    // Explicit query on a resource-returned Thing: retail helper 0x00CBE45C,
    // __fastcall bool(CScriptThing*, const C3DVector*, float). Native husband
    // home checks (DB1729/DB17D3) call it on the Thing from GetScriptThing, not on
    // the cached entity handle. No fallback for a missing coordinate.
    bool ThingIsDistanceFromPositionOver(unsigned id, sol::table position, float distance) {
        auto& e = Get(id, Kind::Thing);
        using tIsDistanceFromThingToPositionOver = bool(__fastcall*)(CScriptThing*, const C3DVector*, float);
        auto retailDistance = ASLR<tIsDistanceFromThingToPositionOver>(0x00CBE45C);
        if (!retailDistance) throw std::runtime_error("Retail Thing distance helper is unavailable");
        C3DVector pos = { position["x"].get<float>(), position["y"].get<float>(), position["z"].get<float>() };
        return retailDistance(&e.thing, &pos, distance);
    }
