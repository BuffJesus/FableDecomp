struct CGameScriptInterface {
    void helperA();
    void helperB(int f);
    void EndCutFade();
};

void CGameScriptInterface::EndCutFade() {
    this->helperA();
    this->helperB(0x3f000000);
}