    bool WithTimer(sol::protected_function callback) {
        CheckOpen();
        return WithRetailOwnedTimer(callback);
    }
