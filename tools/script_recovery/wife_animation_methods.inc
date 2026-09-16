// Both native argument gestures use identical flags and separate CString locals.
void PlayWifeArgumentAnimation(unsigned control,const std::string& name) {
    auto& e=Get(control,Kind::Resource);
    struct Key {
        CCharString value{};
        explicit Key(const char* text){CCharString_Construct_Literal(&value,text,-1);}
        ~Key(){CCharString_Destroy(&value);}
    } key(name.c_str());
    const unsigned char raw=*ASLR<const volatile unsigned char*>(0x1375748);
    auto* expert=e.resource.pImp.Data;
    if(!expert) return;
    auto* table=reinterpret_cast<void**>(expert->pVTable);
    if(!table || !table[0x48/4]) throw std::runtime_error("Wife argument animation slot unavailable");
    using RawAnimation=void(__thiscall*)(void*,const CCharString*,bool,bool,bool,bool,unsigned char,bool,bool);
    reinterpret_cast<RawAnimation>(table[0x48/4])(expert,&key.value,false,false,false,true,raw,false,false);
}
