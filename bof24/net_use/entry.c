#include <windows.h>
#include <winnetwk.h>
#include "bofdefs.h"
#include "base.c"

void listNetUseScope(DWORD scope) {
    HANDLE h = NULL;
    DWORD r = MPR$WNetOpenEnumW(scope, RESOURCETYPE_ANY, 0, NULL, &h);
    if (r != 0) { return; }
    DWORD size = 65536;
    LPBYTE buf = (LPBYTE)intAlloc(size);
    DWORD count = 0xFFFFFFFF;
    r = MPR$WNetEnumResourceW(h, &count, buf, &size);
    if (r == 0) {
        LPNETRESOURCEW nr = (LPNETRESOURCEW)buf;
        for (DWORD i = 0; i < count; i++) {
            internal_printf("%S %S\n", nr[i].lpLocalName ? nr[i].lpLocalName : L"", nr[i].lpRemoteName ? nr[i].lpRemoteName : L"");
        }
    }
    intFree(buf);
    MPR$WNetCloseEnum(h);
}

void showNetUse() {
    listNetUseScope(RESOURCE_CONNECTED);
    listNetUseScope(RESOURCE_REMEMBERED);
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
    showNetUse();
    printoutput(TRUE);
};

#else

int main()
{
    showNetUse();
}

#endif
