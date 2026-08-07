char __fastcall AreAllowedToCoexist(int actionA, int actionB){
    switch (actionA) {
    case 0x08: case 0x1f: case 0x2d:
        if (actionB == 0x08 || actionB == 0x1f || actionB == 0x2d) return 1;
        return 0;
    case 0x1a: case 0x56:
        if (actionB == 0x1a || actionB == 0x56) return 1;
        return 0;
    }
    return 0;
}