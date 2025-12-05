#include <windows.h>
#include <wlanapi.h>
#include "bofdefs.h"
#include "base.c"

typedef DWORD (WINAPI *pWlanOpenHandle)(DWORD, PVOID, PDWORD, PHANDLE);
typedef DWORD (WINAPI *pWlanEnumInterfaces)(HANDLE, PVOID, PWLAN_INTERFACE_INFO_LIST*);
typedef DWORD (WINAPI *pWlanGetProfileList)(HANDLE, const GUID*, PVOID, PWLAN_PROFILE_INFO_LIST*);
typedef VOID  (WINAPI *pWlanFreeMemory)(PVOID);
typedef DWORD (WINAPI *pWlanCloseHandle)(HANDLE, PVOID);

void showWlanProfiles() {
    HMODULE mod = KERNEL32$LoadLibraryA("wlanapi.dll");
    if (!mod) { return; }

    pWlanOpenHandle WlanOpenHandle = (pWlanOpenHandle)KERNEL32$GetProcAddress(mod, "WlanOpenHandle");
    pWlanEnumInterfaces WlanEnumInterfaces = (pWlanEnumInterfaces)KERNEL32$GetProcAddress(mod, "WlanEnumInterfaces");
    pWlanGetProfileList WlanGetProfileList = (pWlanGetProfileList)KERNEL32$GetProcAddress(mod, "WlanGetProfileList");
    pWlanFreeMemory WlanFreeMemory = (pWlanFreeMemory)KERNEL32$GetProcAddress(mod, "WlanFreeMemory");
    pWlanCloseHandle WlanCloseHandle = (pWlanCloseHandle)KERNEL32$GetProcAddress(mod, "WlanCloseHandle");

    if (!WlanOpenHandle || !WlanEnumInterfaces || !WlanGetProfileList || !WlanFreeMemory || !WlanCloseHandle) {
        KERNEL32$FreeLibrary(mod);
        return;
    }

    DWORD ver = 0;
    HANDLE h = NULL;
    if (WlanOpenHandle(2, NULL, &ver, &h) != 0) {
        KERNEL32$FreeLibrary(mod);
        return;
    }

    PWLAN_INTERFACE_INFO_LIST ifs = NULL;
    if (WlanEnumInterfaces(h, NULL, &ifs) == 0 && ifs) {
        for (DWORD i = 0; i < ifs->dwNumberOfItems; i++) {
            PWLAN_PROFILE_INFO_LIST plist = NULL;
            if (WlanGetProfileList(h, &ifs->InterfaceInfo[i].InterfaceGuid, NULL, &plist) == 0 && plist) {
                for (DWORD j = 0; j < plist->dwNumberOfItems; j++) {
                    internal_printf("%S\n", plist->ProfileInfo[j].strProfileName);
                }
                WlanFreeMemory(plist);
            }
        }
        WlanFreeMemory(ifs);
    }
    WlanCloseHandle(h, NULL);
    KERNEL32$FreeLibrary(mod);
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
    showWlanProfiles();
    printoutput(TRUE);
};

#else

int main()
{
    showWlanProfiles();
}

#endif
