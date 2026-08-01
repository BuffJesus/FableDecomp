#include <wchar.h>
#include <stdio.h>

void __cdecl wsplitpath(const wchar_t *path, wchar_t *drive, wchar_t *dir, wchar_t *fname, wchar_t *ext)
{
    _wsplitpath(path, drive, dir, fname, ext);
}

int main()
{
    wchar_t drive[8];
    wchar_t dir[260];
    wchar_t fname[260];
    wchar_t ext[32];

    drive[0] = 0; dir[0] = 0; fname[0] = 0; ext[0] = 0;

    wsplitpath(L"C:\\folder\\sub\\file.txt", drive, dir, fname, ext);

    if (drive[0] == L'C' && wcsstr(dir, L"folder") != 0 && wcsstr(dir, L"sub") != 0 &&
        wcscmp(fname, L"file") == 0 && wcscmp(ext, L".txt") == 0)
    {
        printf("WSPLITPATH_OK\n");
    }
    else
    {
        printf("WSPLITPATH_BAD\n");
    }

    return 0;
}