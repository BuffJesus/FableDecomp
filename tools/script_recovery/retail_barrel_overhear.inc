    bool ShouldBarrelOverhear(CScriptThing* actor,bool alreadyHeard) {
        CheckOpen();
        if(alreadyHeard) {
            using Random=int(__cdecl*)();
            const int random=ASLR<Random>(0x00BFEB16)();
            const int divisor=*ASLR<const volatile int*>(0x013AC854);
            // Native IDIV faults for these operands; report an error rather than
            // accepting the event or executing undefined C++ arithmetic.
            if(divisor==0 || (random==(-2147483647-1) && divisor==-1))
                throw std::runtime_error("Retail Barrel overhear division fault");
            if(random%divisor!=0)return false;
        }
        if(!m_game || !GetHero_API || !IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Retail Barrel overhear APIs unavailable");
        auto* hero=GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(actor,hero,15.0f);
    }
