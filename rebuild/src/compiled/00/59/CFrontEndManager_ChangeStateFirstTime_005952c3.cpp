// NUISystem::CFrontEndManager::ChangeStateFirstTime @ 0x005952C3
//
// The stack helper is a one-argument fastcall in the retail ABI.  The
// current stack component exposes the state transition at vtable slot C0.
// Declaring the known vtable prefix lets VC7.1 emit the retail virtual call
// directly; no assembly is needed here.

extern "C" void** __fastcall
FableFrontEndFirstStateStackTop(void* stack);

struct FableFrontendStackComponent
{
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void Slot24();
    virtual void Slot25();
    virtual void Slot26();
    virtual void Slot27();
    virtual void Slot28();
    virtual void Slot29();
    virtual void Slot30();
    virtual void Slot31();
    virtual void Slot32();
    virtual void Slot33();
    virtual void Slot34();
    virtual void Slot35();
    virtual void Slot36();
    virtual void Slot37();
    virtual void Slot38();
    virtual void Slot39();
    virtual void Slot40();
    virtual void Slot41();
    virtual void Slot42();
    virtual void Slot43();
    virtual void Slot44();
    virtual void Slot45();
    virtual void Slot46();
    virtual void Slot47();
    virtual void SetState(long state);
};

extern "C" void __fastcall
CFrontEndManager_ChangeStateFirstTime_005952c3(void* manager, void*)
{
    void** stackTop = FableFrontEndFirstStateStackTop(
        reinterpret_cast<unsigned char*>(manager) + 0x20);
    FableFrontendStackComponent* component =
        reinterpret_cast<FableFrontendStackComponent*>(*stackTop);
    component->SetState(5);
}
