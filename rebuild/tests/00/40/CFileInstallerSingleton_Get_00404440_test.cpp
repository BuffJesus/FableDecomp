#include <stdio.h>

#include "fable_file_installer.h"

CCountedFileInstaller g_CFileInstallerSingleton_013CA818 = {0, 0};

static CFileInstaller g_storage;
static bool g_allocationEnabled = true;
static fable_u32 g_allocationCalls = 0;
static fable_u32 g_allocationSize = 0;
static fable_u32 g_constructorCalls = 0;
static fable_u32 g_assignmentCalls = 0;

void* FABLE_CDECL FableFileInstallerAllocate(fable_u32 size)
{
    ++g_allocationCalls;
    g_allocationSize = size;
    return g_allocationEnabled ? &g_storage : 0;
}

CFileInstaller::CFileInstaller()
{
    ++g_constructorCalls;
}

void CCountedFileInstaller::operator=(CFileInstaller* value)
{
    ++g_assignmentCalls;
    object = value;
    reference = value != 0 ? this : 0;
}

static void ResetFixture(bool allocationEnabled)
{
    g_CFileInstallerSingleton_013CA818.object = 0;
    g_CFileInstallerSingleton_013CA818.reference = 0;
    g_allocationEnabled = allocationEnabled;
    g_allocationCalls = 0;
    g_allocationSize = 0;
    g_constructorCalls = 0;
    g_assignmentCalls = 0;
}

int main()
{
    ResetFixture(true);
    g_CFileInstallerSingleton_013CA818.object = &g_storage;
    CFileInstaller* cached = &CFileInstallerSingleton::Get();
    if (cached != &g_storage ||
        g_allocationCalls != 0 ||
        g_constructorCalls != 0 ||
        g_assignmentCalls != 0)
    {
        printf("FABLETLC_FILE_INSTALLER_GET_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    ResetFixture(true);
    CFileInstaller* created = &CFileInstallerSingleton::Get();
    if (created != &g_storage ||
        g_allocationCalls != 1 ||
        g_allocationSize != sizeof(CFileInstaller) ||
        g_constructorCalls != 1 ||
        g_assignmentCalls != 1 ||
        g_CFileInstallerSingleton_013CA818.reference == 0)
    {
        printf("FABLETLC_FILE_INSTALLER_GET_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    ResetFixture(false);
    CFileInstaller* unavailable = &CFileInstallerSingleton::Get();
    if (unavailable != 0 ||
        g_allocationCalls != 1 ||
        g_constructorCalls != 0 ||
        g_assignmentCalls != 1)
    {
        printf("FABLETLC_FILE_INSTALLER_GET_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS\n");
    return 0;
}
