#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "wmi.c"

void showVideoController() {
    HRESULT hr = S_OK;
    WMI wmi = {0};
    BSTR **results = NULL;
    DWORD rows = 0, cols = 0;
    hr = Wmi_Initialize(&wmi);
    if (FAILED(hr)) { return; }
    hr = Wmi_Connect(&wmi, L"\\\\.\\root\\cimv2");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_Query(&wmi, L"SELECT Name FROM Win32_VideoController");
    if (FAILED(hr)) { Wmi_Finalize(&wmi); return; }
    hr = Wmi_ParseResults(&wmi, L"Name", &results, &rows, &cols);
    if (FAILED(hr) || !results || rows < 1) { Wmi_Finalize(&wmi); return; }
    for (DWORD r = 0; r < rows; r++) {
        internal_printf("%S\n", results[r][0]);
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
    showVideoController();
    printoutput(TRUE);
};

#else

int main()
{
    showVideoController();
}

#endif
