#include <iostream>
#include <string>
using namespace std;

#include <windows.h>
#include <winioctl.h>
#include <tchar.h>

#include "RawStorage.h"

void ProgressProc(ULONG TotalSector, ULONG RemainSector, LPVOID lpParam) {
    char back[4] = { 0x08, 0x08, 0x08, 0x08 };
    fwrite(back, 1, 4, stdout);
    printf("%3d%%", ((TotalSector - RemainSector) * 100) / TotalSector);
}

int main(int argc, char* argv[]) {
    TCHAR Logical = TEXT('D');

    CRawStorage     RawStorage(Logical);
    StorageInfo_t   StorageInfo;

    RawStorage.Get_Storage_Info(&StorageInfo);

    printf("%-25s: %u\n",  "BusType",               StorageInfo.BusType);
    printf("%-25s: %lu\n", "BytePerSector",         StorageInfo.BytePerSector);
    printf("%-25s: %s\n",  "CommandQueueing",       StorageInfo.CommandQueueing ? "Yes" : "No");
    printf("%-25s: %u\n",  "DeviceType",            StorageInfo.DeviceType);
    printf("%-25s: %u\n",  "DeviceTypeModifier",    StorageInfo.DeviceTypeModifier);
    printf("%-25s: %lu\n", "MaximumPhysicalPages",  StorageInfo.MaximumPhysicalPages);
    printf("%-25s: %lu\n", "MaximumTransferLength", StorageInfo.MaximumTransferLength);
    printf("%-25s: %u\n",  "PartitionStyle",        StorageInfo.PartitionStyle);
    printf("%-25s: %s\n",  "ProductID",             StorageInfo.ProductID.c_str());
    printf("%-25s: %s\n",  "ProductRevision",       StorageInfo.ProductRevision.c_str());
    printf("%-25s: %s\n",  "RemovableMedia",        StorageInfo.RemovableMedia ? "Yes" : "No");
    printf("%-25s: %s\n",  "SerialNumber",          StorageInfo.SerialNumber.c_str());
    printf("%-25s: %u\n",  "StorageNumber",         StorageInfo.StorageNumber);
    printf("%-25s: %s\n\n",  "VendorID",              StorageInfo.VendorID.c_str());

    TCHAR File[] = _T("C:\\shobjidl.h");
    LARGE_INTEGER FileSize;
    HANDLE hFile = CreateFile(File, FILE_READ_ACCESS, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE) {
        cout << "File Open Fail..." << endl;
    }

    GetFileSizeEx(hFile, &FileSize);

    cout << "File Size: " << FileSize.QuadPart << endl;
    LPBYTE pwBuff = new BYTE[FileSize.QuadPart];
    LPBYTE prBuff = new BYTE[FileSize.QuadPart];
    DWORD Junk;
    DWORD SectorCount;
    BOOL bResult;
    bool bReturn;

    SectorCount = (FileSize.QuadPart % 512 != 0) ? ((FileSize.QuadPart / 512) + 1) : FileSize.QuadPart / 512;
    bResult = ReadFile(hFile, pwBuff, SectorCount, &Junk, NULL);
    if(bResult != TRUE) {
        cout << "File Read Fail..." << endl;
    }
    
    CloseHandle(hFile);

    cout << "[Write]" << endl;
    bReturn = RawStorage.Write(0, pwBuff, SectorCount, ProgressProc);
    if(bReturn != true) {
        cout << "Write Fail" << endl;
    }
    cout << "\nDone\n" << endl;

    cout << "[Read]" << endl;
    bReturn = RawStorage.Read(0, prBuff, SectorCount, ProgressProc);
    if(bReturn != true) {
        cout << "Read Fail" << endl;
    }
    cout << "\nDone\n" << endl;

    pwBuff[4000] = '1';

    cout << "[Verify]" << endl;
    bReturn = RawStorage.Verify(prBuff, pwBuff, SectorCount, ProgressProc);
    if(bReturn != true) {
        cout << "Verify Fail" << endl;
    }
    cout << "\nDone\n" << endl;

    delete[] pwBuff;
    delete[] prBuff;
    pwBuff = nullptr;
    prBuff = nullptr;

    return 0;
}