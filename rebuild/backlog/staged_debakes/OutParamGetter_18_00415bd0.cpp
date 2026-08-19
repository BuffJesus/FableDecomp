// Getter that materialises a temporary, fills it through an out-param member call, and
// returns one word of it. __fastcall this=ecx, no args.
struct Rect { int width; int height; };
struct T {
    Rect* GetRect(Rect* out);
    int GetWidth();
};
int T::GetWidth() {
    Rect r;
    return this->GetRect(&r)->width;
}
