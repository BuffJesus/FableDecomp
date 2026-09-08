struct CRegisteredMusicEntry {
    void* SrcFileName;
    void* DstFileName;
    float Gain;
    float Offset;
    bool CacheToHD;
    unsigned char _pad_0x11[3];
};

struct CRegisteredMusicPair {
    long MusicID;
    CRegisteredMusicEntry Entry;
};

struct CRegisteredMusicVector {
    CRegisteredMusicPair* Begin;
    CRegisteredMusicPair* End;
};

struct CMusicManager {
    unsigned char CurrentStateAndSoundDef[0x1c];
    CRegisteredMusicVector RegisteredMusic;

    void ResetMusicEntriesOffsets();
};

void CMusicManager::ResetMusicEntriesOffsets() {
    for (CRegisteredMusicPair* music = RegisteredMusic.Begin;
         music != RegisteredMusic.End; ++music) {
        music->Entry.Offset = 0.0f;
    }
}