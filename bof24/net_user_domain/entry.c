#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void listDomainUsers() {
    LPWSTR dc = NULL;
    DWORD s = NETAPI32$NetGetAnyDCName(NULL, NULL, (LPBYTE*)&dc);
    if (s != 0 || !dc) { return; }
    DWORD status = 0, entriesRead = 0, totalEntries = 0, resumeHandle = 0;
    LPBYTE buffer = NULL;
    internal_printf("\nDomain Users:\n");
    internal_printf("================================\n");
    do {
        status = NETAPI32$NetUserEnum(
            dc,
            0,
            FILTER_NORMAL_ACCOUNT,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);
        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            USER_INFO_0 *p = (USER_INFO_0*)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%S\n", p[i].usri0_name);
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetUserEnum failed: %lu", status);
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
            break;
        }
    } while (status == ERROR_MORE_DATA);
    if (dc) { NETAPI32$NetApiBufferFree(dc); }
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
    listDomainUsers();
    printoutput(TRUE);
};

#else

int main()
{
    listDomainUsers();
}

#endif
