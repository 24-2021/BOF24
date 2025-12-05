#include <windows.h>
#include "bofdefs.h"
#include "base.c"
WINBASEAPI DWORD WINAPI KERNEL32$GetLogicalDrives(VOID);

void showDrives() {
    DWORD mask = KERNEL32$GetLogicalDrives();
    for (int i = 0; i < 26; i++) {
        if (mask & (1 << i)) {
            char n[4] = { (char)('A' + i), ':', '\\', 0 };
            internal_printf("%s\n", n);
        }
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
    showDrives();
    printoutput(TRUE);
};

#else

int main()
{
    showDrives();
}

#endif
