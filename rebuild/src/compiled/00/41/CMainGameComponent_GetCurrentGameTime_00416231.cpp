struct CMainGameComponent {
    char pad[0x60];
    double gameTimeBase;
    double GetCurrentGameTime();
};
extern double __fastcall ReadCurrentGameTime(CMainGameComponent* self);
double CMainGameComponent::GetCurrentGameTime() {
    return ReadCurrentGameTime(this) - gameTimeBase;
}