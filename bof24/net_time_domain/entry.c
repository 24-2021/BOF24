#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

void showDomainTime() {
    LPWSTR dc = NULL;
    DWORD s = NETAPI32$NetGetAnyDCName(NULL, NULL, (LPBYTE*)&dc);
    if (s != 0 || !dc) { return; }
    LPBYTE buf = NULL;
    DWORD r = NETAPI32$NetRemoteTOD(dc, &buf);
    if (r == 0 && buf) {
        TIME_OF_DAY_INFO* t = (TIME_OF_DAY_INFO*)buf;
        internal_printf("%04u-%02u-%02u %02u:%02u:%02u\n", (unsigned)(t->tod_year), (unsigned)(t->tod_month), (unsigned)(t->tod_day), (unsigned)(t->tod_hours), (unsigned)(t->tod_mins), (unsigned)(t->tod_secs));
        NETAPI32$NetApiBufferFree(buf);
    }
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
    showDomainTime();
    printoutput(TRUE);
};

#else

int main()
{
    showDomainTime();
}

#endif
