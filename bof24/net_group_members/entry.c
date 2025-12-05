#include <windows.h>
#include <lm.h>
#include "bofdefs.h"
#include "base.c"

static LPWSTR a2w(const char* s, int len) {
    if (!s || len <= 0) return NULL;
    LPWSTR w = (LPWSTR)intAlloc(sizeof(wchar_t) * (len + 1));
    for (int i = 0; i < len; i++) { w[i] = (wchar_t)(unsigned char)s[i]; }
    w[len] = 0;
    return w;
}

void listDomainGroupMembers(LPWSTR group) {
    if (!group) return;
    LPWSTR dc = NULL;
    DWORD s = NETAPI32$NetGetAnyDCName(NULL, NULL, (LPBYTE*)&dc);
    if (s != 0 || !dc) { return; }
    DWORD status = 0, entriesRead = 0, totalEntries = 0; DWORD_PTR resumeHandle = 0;
    LPBYTE buffer = NULL;
    internal_printf("\n%S members:\n", group);
    internal_printf("================================\n");
    do {
        status = NETAPI32$NetGroupGetUsers(
            dc,
            group,
            0,
            &buffer,
            MAX_PREFERRED_LENGTH,
            &entriesRead,
            &totalEntries,
            &resumeHandle);
        if (status == ERROR_SUCCESS || status == ERROR_MORE_DATA) {
            GROUP_USERS_INFO_0 *p = (GROUP_USERS_INFO_0*)buffer;
            for (DWORD i = 0; i < entriesRead; i++) {
                internal_printf("%S\n", p[i].grui0_name);
            }
            if (buffer) { NETAPI32$NetApiBufferFree(buffer); buffer = NULL; }
        } else {
            BeaconPrintf(CALLBACK_ERROR, "NetGroupGetUsers failed: %lu", status);
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
    datap parser = {0};
    BeaconDataParse(&parser, Buffer, Length);
    int sz = 0;
    char* g = BeaconDataExtract(&parser, &sz);
    LPWSTR wg = a2w(g, sz);
    listDomainGroupMembers(wg);
    if (wg) { intFree(wg); }
    printoutput(TRUE);
};

#else

int main()
{
    wchar_t g[] = L"Domain Admins";
    listDomainGroupMembers(g);
}

#endif
