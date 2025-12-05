#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void showWorkstation() {
    LPBYTE buf = NULL;
    DWORD r = NETAPI32$NetWkstaGetInfo(NULL, 102, &buf);
    if (r == 0 && buf) {
        WKSTA_INFO_102 *p = (WKSTA_INFO_102*)buf;
        internal_printf("ComputerName: %S\n", p->wki102_computername);
        internal_printf("Domain: %S\n", p->wki102_langroup);
        internal_printf("Version: %lu.%lu\n", p->wki102_ver_major, p->wki102_ver_minor);
        NETAPI32$NetApiBufferFree(buf);
    }
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
    showWorkstation();
    printoutput(TRUE);
};

#else

int main()
{
    showWorkstation();
}

#endif
