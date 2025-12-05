#include <windows.h>
#include "bofdefs.h"
#include "base.c"

void showRecent() {
    wchar_t p[1024];
    KERNEL32$ExpandEnvironmentStringsW(L"%APPDATA%\\Microsoft\\Windows\\Recent\\*", p, 1023);
    WIN32_FIND_DATAW fd;
    HANDLE h = KERNEL32$FindFirstFileW(p, &fd);
    if (h == INVALID_HANDLE_VALUE) { return; }
    do {
        internal_printf("%S\n", fd.cFileName);
    } while (KERNEL32$FindNextFileW(h, &fd));
    KERNEL32$FindClose(h);
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
    showRecent();
    printoutput(TRUE);
};

#else

int main()
{
    showRecent();
}

#endif
