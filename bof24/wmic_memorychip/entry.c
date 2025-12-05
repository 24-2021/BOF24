#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "wmi.c"

void showMemoryChip() {
    HRESULT hr = S_OK;
    WMI wmi = {0};
    BSTR **results = NULL;
    DWORD rows = 0, cols = 0;
    hr = Wmi_Initialize(&wmi);
    if (FAILED(hr)) { return; }
    hr = Wmi_Connect(&wmi, L"\\\\.\\root\\cimv2");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_Query(&wmi, L"SELECT Capacity, Speed FROM Win32_PhysicalMemory");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_ParseResults(&wmi, L"Capacity,Speed", &results, &rows, &cols);
    if (FAILED(hr) || !results || rows < 1) { Wmi_Finalize(&wmi); return; }
    for (DWORD r = 0; r < rows; r++) {
        internal_printf("%S %S\n", results[r][0], results[r][1]);
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
    showMemoryChip();
    printoutput(TRUE);
};

#else

int main()
{
    showMemoryChip();
}

#endif
