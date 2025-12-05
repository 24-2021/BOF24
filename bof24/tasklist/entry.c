#include <windows.h>
#include "bofdefs.h"
#include "base.c"

void listProcesses() {
    HANDLE snap = KERNEL32$CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        BeaconPrintf(CALLBACK_ERROR, "CreateToolhelp32Snapshot failed: %u", KERNEL32$GetLastError());
        return;
    }
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (!KERNEL32$Process32First(snap, &pe)) {
        BeaconPrintf(CALLBACK_ERROR, "Process32First failed: %u", KERNEL32$GetLastError());
        KERNEL32$CloseHandle(snap);
        return;
    }
    internal_printf("%-32s %10s %10s\n", "Image Name", "PID", "PPID");
    do {
        internal_printf("%-32s %10u %10u\n", pe.szExeFile, pe.th32ProcessID, pe.th32ParentProcessID);
    } while (KERNEL32$Process32Next(snap, &pe));
    KERNEL32$CloseHandle(snap);
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
    listProcesses();
    printoutput(TRUE);
};

#else

int main()
{
    listProcesses();
}

#endif

