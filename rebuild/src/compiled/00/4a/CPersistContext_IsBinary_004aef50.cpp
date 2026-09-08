// Retail 0x004AEF50. Ego_r.pdb identifies Mode at +0x18. The symbolic
// names for EMode values 1 and 3 were not present in the recovered type data,
// so retain their numeric values rather than inventing misleading labels.
struct CPersistContext {
    void* PDefinitionManager;
    void* PSymbolMap;
    void* PParser;
    void* ErrorCallback;
    void* PStringToClean;
    bool UseDefaults;
    bool UseCRCCheck;
    bool SingleTransfer;
    bool Finished;
    long Mode;
    bool Binary;
    bool IsBinary() const;
};

bool CPersistContext::IsBinary() const {
    return Mode == 1 || Mode == 3;
}