#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void listAdministrators() {
    DWORD status = 0;
    DWORD entriesRead = 0;
    DWORD totalEntries = 0;
    DWORD_PTR resumeHandle = 0;
    LPBYTE buffer = NULL;

    internal_printf("\nAdministrators members:\n");
    internal_printf("================================\n");

    do {
        status = NETAPI32$NetLocalGroupGetMembers(
            NULL,
            L"Administrators",
            2,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);

        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            LOCALGROUP_MEMBERS_INFO_2 *pInfo = (LOCALGROUP_MEMBERS_INFO_2 *)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%S\n", pInfo[i].lgrmi2_domainandname);
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetLocalGroupGetMembers failed: %lu", status);
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
            break;
        }
    } while (status == ERROR_MORE_DATA);
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
    listAdministrators();
    printoutput(TRUE);
};

#else

int main()
{
    listAdministrators();
}

#endif

