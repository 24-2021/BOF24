#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void listDomainGroups() {
    LPWSTR dc = NULL;
    DWORD s = NETAPI32$NetGetAnyDCName(NULL, NULL, (LPBYTE*)&dc);
    if (s != 0 || !dc) { return; }
    DWORD status = 0, entriesRead = 0, totalEntries = 0; DWORD_PTR resumeHandle = 0;
    LPBYTE buffer = NULL;
    internal_printf("\nDomain Groups:\n");
    internal_printf("================================\n");
    do {
        status = NETAPI32$NetGroupEnum(
            dc,
            0,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);
        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            GROUP_INFO_0 *p = (GROUP_INFO_0*)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%S\n", p[i].grpi0_name);
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetGroupEnum failed: %lu", status);
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
    listDomainGroups();
    printoutput(TRUE);
};

#else

int main()
{
    listDomainGroups();
}

#endif
