#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void listLocalUsers() {
    DWORD status = 0;
    DWORD entriesRead = 0;
    DWORD totalEntries = 0;
    DWORD resumeHandle = 0;
    LPBYTE buffer = NULL;

    internal_printf("\nLocal Users:\n");
    internal_printf("================================\n");

    do {
        status = NETAPI32$NetUserEnum(
            NULL,
            0,
            FILTER_NORMAL_ACCOUNT,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);

        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            USER_INFO_0 *pInfo = (USER_INFO_0 *)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%S\n", pInfo[i].usri0_name);
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetUserEnum failed: %lu", status);
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
    listLocalUsers();
    printoutput(TRUE);
};

#else

int main()
{
    listLocalUsers();
}

#endif

