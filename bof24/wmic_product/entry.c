#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "wmi.c"

void listProducts() {
    HRESULT hr = S_OK;
    WMI wmi = {0};
    BSTR **results = NULL;
    DWORD rows = 0, cols = 0;

    hr = Wmi_Initialize(&wmi);
    if (FAILED(hr)) { return; }

    hr = Wmi_Connect(&wmi, L"\\\\.\\root\\cimv2");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }

    hr = Wmi_Query(&wmi, L"SELECT Name, Version FROM Win32_Product");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }

    hr = Wmi_ParseResults(&wmi, L"Name,Version", &results, &rows, &cols);
    if (FAILED(hr) || !results || rows < 1) { Wmi_Finalize(&wmi); return; }

    if (cols >= 2) {
        internal_printf("%-64S %-24S\n", results[0][0], results[0][1]);
        for (DWORD r = 1; r < rows; r++) {
            internal_printf("%-64S %-24S\n", results[r][0], results[r][1]);
        }
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
    listProducts();
    printoutput(TRUE);
};

#else

int main()
{
    listProducts();
}

#endif

