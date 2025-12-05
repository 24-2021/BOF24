#include <windows.h>
#include "bofdefs.h"
#include "base.c"

 

void showRdpPort() {
    HKEY hKey = NULL;
    LONG ret = ADVAPI32$RegOpenKeyExW(
        HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",
        0,
        KEY_READ,
        &hKey);
    if (ret != ERROR_SUCCESS) {
        BeaconPrintf(CALLBACK_ERROR, "RegOpenKeyExW failed: %ld", ret);
        return;
    }
    DWORD type = 0;
    DWORD port = 0;
    DWORD cb = sizeof(DWORD);
    ret = ADVAPI32$RegQueryValueExW(hKey, L"PortNumber", NULL, &type, (LPBYTE)&port, &cb);
    if (ret == ERROR_SUCCESS && type == REG_DWORD) {
        internal_printf("RDP PortNumber: %lu\n", port);
    } else {
        BeaconPrintf(CALLBACK_ERROR, "RegQueryValueExW failed: %ld", ret);
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
    showRdpPort();
    printoutput(TRUE);
};

#else

int main()
{
    showRdpPort();
}

#endif
