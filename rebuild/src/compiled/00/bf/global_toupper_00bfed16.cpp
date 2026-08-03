extern "C" __declspec(dllimport) int __cdecl toupper(int c);

int __cdecl toupper_thunk(int c)
{
    return toupper(c);
}