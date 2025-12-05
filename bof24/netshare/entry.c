#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void listShares() {
    DWORD status = 0;
    DWORD entriesRead = 0;
    DWORD totalEntries = 0;
    DWORD resumeHandle = 0;
    LPBYTE buffer = NULL;

    internal_printf("\nShares:\n");
    internal_printf("================================\n");

    do {
        status = NETAPI32$NetShareEnum(
            NULL,
            2,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);

        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            SHARE_INFO_2 *pInfo = (SHARE_INFO_2 *)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%-24S %-64S\n", pInfo[i].shi2_netname, pInfo[i].shi2_remark ? pInfo[i].shi2_remark : L"" );
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetShareEnum failed: %lu", status);
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
    listShares();
    printoutput(TRUE);
};

#else

int main()
{
    listShares();
}

#endif

