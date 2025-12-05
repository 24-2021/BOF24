#include <windows.h>
#include <winsvc.h>
#include "bofdefs.h"
#include "base.c"

 

void listStartedServices() {
    SC_HANDLE scm = ADVAPI32$OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!scm) {
        BeaconPrintf(CALLBACK_ERROR, "OpenSCManager failed: %u", KERNEL32$GetLastError());
        return;
    }

    DWORD bytesNeeded = 0, servicesReturned = 0, resumeHandle = 0;
    LPBYTE buffer = NULL;

    BOOL ok = ADVAPI32$EnumServicesStatusExA(
        scm,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        buffer,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL);

    if (!ok && KERNEL32$GetLastError() != ERROR_MORE_DATA) {
        BeaconPrintf(CALLBACK_ERROR, "EnumServicesStatusExA init failed: %u", KERNEL32$GetLastError());
        ADVAPI32$CloseServiceHandle(scm);
        return;
    }

    buffer = (LPBYTE)intAlloc(bytesNeeded);
    ok = ADVAPI32$EnumServicesStatusExA(
        scm,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        buffer,
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL);

    if (!ok) {
        BeaconPrintf(CALLBACK_ERROR, "EnumServicesStatusExA failed: %u", KERNEL32$GetLastError());
        intFree(buffer);
        ADVAPI32$CloseServiceHandle(scm);
        return;
    }

    ENUM_SERVICE_STATUS_PROCESS *services = (ENUM_SERVICE_STATUS_PROCESS*)buffer;
    internal_printf("\nStarted Services:\n");
    internal_printf("================================\n");
    for (DWORD i = 0; i < servicesReturned; i++) {
        internal_printf("%s\n", services[i].lpDisplayName ? services[i].lpDisplayName : services[i].lpServiceName);
    }

    intFree(buffer);
    ADVAPI32$CloseServiceHandle(scm);
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
    listStartedServices();
    printoutput(TRUE);
};

#else

int main()
{
    listStartedServices();
}

#endif
