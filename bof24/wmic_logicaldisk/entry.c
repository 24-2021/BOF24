#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "wmi.c"

void showLogicalDisk() {
    HRESULT hr = S_OK;
    WMI wmi = {0};
    BSTR **results = NULL;
    DWORD rows = 0, cols = 0;
    hr = Wmi_Initialize(&wmi);
    if (FAILED(hr)) { return; }
    hr = Wmi_Connect(&wmi, L"\\\\.\\root\\cimv2");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_Query(&wmi, L"SELECT Caption, Size, FreeSpace FROM Win32_LogicalDisk");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_ParseResults(&wmi, L"Caption,Size,FreeSpace", &results, &rows, &cols);
    if (FAILED(hr) || !results || rows < 1) { Wmi_Finalize(&wmi); return; }
    for (DWORD r = 0; r < rows; r++) {
        internal_printf("%S %S %S\n", results[r][0], results[r][1], results[r][2]);
    }
    Wmi_Finalize(&wmi);
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
    showLogicalDisk();
    printoutput(TRUE);
};

#else

int main()
{
    showLogicalDisk();
}

#endif
