#include <windows.h>
#include "bofdefs.h"
#include "base.c"

typedef struct _DS_DOMAIN_TRUSTS_A {
    LPSTR NetbiosDomainName;
    LPSTR DnsDomainName;
    DWORD Flags;
    DWORD ParentIndex;
    DWORD TrustType;
    DWORD TrustAttributes;
    PVOID DomainSid;
    GUID DomainGuid;
} DS_DOMAIN_TRUSTS, *PDS_DOMAIN_TRUSTS;


static DWORD flagval(const char* s, int len) {
    if (!s || len <= 0) return 0;
    if (len==7 && !MSVCRT$_strnicmp(s, "PRIMARY", 7)) return 0x0008;
    if (len==6 && !MSVCRT$_strnicmp(s, "FOREST", 6)) return 0x0001;
    if (len==10 && !MSVCRT$_strnicmp(s, "DIRECT_OUT", 10)) return 0x0002;
    if (len==10 && !MSVCRT$_strnicmp(s, "ALL_TRUSTS", 10)) return 0;
    return 0;
}

void showDomainTrustsFiltered(DWORD want) {
    PDS_DOMAIN_TRUSTS p = NULL;
    DWORD c = 0;
    DWORD r = NETAPI32$DsEnumerateDomainTrustsA(NULL, 0, (PVOID*)&p, &c);
    if (r == 0 && p) {
        for (DWORD i = 0; i < c; i++) {
            if (want && !(p[i].Flags & want)) { continue; }
            char *name = p[i].DnsDomainName ? p[i].DnsDomainName : p[i].NetbiosDomainName;
            internal_printf("%s\n", name ? name : "");
        }
        NETAPI32$NetApiBufferFree(p);
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
    DWORD want = 0;
    datap parser = {0};
    BeaconDataParse(&parser, Buffer, Length);
    int sl = 0; char* s = BeaconDataExtract(&parser, &sl);
    want = flagval(s, sl);
    showDomainTrustsFiltered(want);
    printoutput(TRUE);
};

#else

int main()
{
    showDomainTrustsFiltered(0);
}

#endif
