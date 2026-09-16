template<class T> static T BarrelMethod(CGameScriptInterfaceBase* game,unsigned slot) {
    if(!game || !*reinterpret_cast<void***>(game))throw std::runtime_error("Barrel interface unavailable");
    auto method=reinterpret_cast<T>((*reinterpret_cast<void***>(game))[slot/4]);
    if(!method)throw std::runtime_error("Barrel method unavailable");return method;
}
static CGameScriptInterfaceBase* BarrelTimerInterface(){return *ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);}
static int BarrelStateRead(sol::protected_function read) {
    auto value=read();if(!value.valid()){sol::error e=value;throw std::runtime_error(e.what());}return value.get<int>();
}
int ReadBarrelWatchTimer(int id) {
    CheckOpen();auto* game=BarrelTimerInterface();return BarrelMethod<tGetTimer>(game,0x168)(game,id);
}
void AddBarrelTimerBar(sol::protected_function publish) {
    CheckOpen();FableString text("");FableString icon("HUD_CLOCK_ICON");
    CRGBColour first{0,255,0,255},second{0,255,0,255};
    const int id=BarrelMethod<tAddQuestInfoBar>(m_game,0x510)(m_game,45.f,0.f,&first,&second,icon,text,1.f);
    auto result=publish(id);if(!result.valid()){sol::error e=result;throw std::runtime_error(e.what());}
}
bool IsHeroNearBarrelGuard(unsigned id) {
    auto& guard=Get(id,Kind::Thing).thing;
    if(!IsDistanceBetweenThingsUnder_API)throw std::runtime_error("Barrel distance API unavailable");
    auto* hero=BarrelMethod<tGetHero>(m_game,0x118)(m_game);
    return IsDistanceBetweenThingsUnder_API(hero,&guard,2.f);
}
void ColourBarrelTimer(int id,bool within) {
    CheckOpen();CRGBColour first=within?CRGBColour{0,255,0,255}:CRGBColour{0,0,255,255};auto second=first;
    BarrelMethod<tChangeQuestInfoBarColour>(m_game,0x534)(m_game,id,&first,&second);
}
void UpdateBarrelTimer(sol::protected_function readTimer,sol::protected_function readBar) {
    CheckOpen();auto update=BarrelMethod<tUpdateQuestInfoBar>(m_game,0x530);
    auto* global=BarrelTimerInterface();const int timer=BarrelStateRead(readTimer);
    const int remaining=BarrelMethod<tGetTimer>(global,0x168)(global,timer);
    const int bar=BarrelStateRead(readBar);update(m_game,bar,static_cast<float>(remaining),-1.f,-1.f);
}
void RemoveBarrelTimer(int id) {CheckOpen();BarrelMethod<tRemoveQuestInfoElement>(m_game,0x548)(m_game,id);}
