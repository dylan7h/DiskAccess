#include <iostream>
#include <string>
using namespace std;

#include <windows.h>
#include <winioctl.h>
#include <tchar.h>

int main(int argc, char* argv[]) {
    HANDLE hDevice = CreateFile(_T("\\\\.\\PhysicalDrive1"), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(hDevice == INVALID_HANDLE_VALUE) {
        cout << "INVALID_HANDLE_VALUE" << endl;
        return 0;
    }

    STORAGE_PROPERTY_QUERY storagePropertyQuery;
    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    PSTORAGE_DEVICE_DESCRIPTOR pStorageDeviceDescriptor = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 0xFFF];
    DWORD Junk;

    DeviceIoControl(
        _In_        (HANDLE)       hDevice,                            // handle to a partition
        _In_        (DWORD) IOCTL_STORAGE_QUERY_PROPERTY,              // dwIoControlCode
        _In_        (LPVOID)       &storagePropertyQuery,              // input buffer - STORAGE_PROPERTY_QUERY structure
        _In_        (DWORD)        sizeof(STORAGE_PROPERTY_QUERY),     // size of input buffer
        _Out_opt_   (LPVOID)       pStorageDeviceDescriptor,           // output buffer - see Remarks
        _In_        (DWORD)        sizeof(STORAGE_DEVICE_DESCRIPTOR) + 0xFFF,  // size of output buffer
        _Out_opt_   (LPDWORD)      &Junk,                              // number of bytes returned
        _Inout_opt_ (LPOVERLAPPED) NULL                                // OVERLAPPED structure
    );

    printf("%-25s: %lu\n", "Version",               pStorageDeviceDescriptor->Version);
    printf("%-25s: %lu\n", "Size",                  pStorageDeviceDescriptor->Size);
    printf("%-25s: %u\n", "DeviceType",             pStorageDeviceDescriptor->DeviceType);
    printf("%-25s: %u\n", "DeviceTypeModifier",     pStorageDeviceDescriptor->DeviceTypeModifier);
    printf("%-25s: %s\n", "RemovableMedia",         pStorageDeviceDescriptor->RemovableMedia ? "TRUE" : "FALSE");
    printf("%-25s: %s\n", "CommandQueueing",        pStorageDeviceDescriptor->CommandQueueing ? "TRUE" : "FALSE");
    printf("%-25s: %lu\n", "VendorIdOffset",        pStorageDeviceDescriptor->VendorIdOffset);
    printf("%-25s: %lu\n", "ProductIdOffset",       pStorageDeviceDescriptor->ProductIdOffset);
    printf("%-25s: %lu\n", "ProductRevisionOffset", pStorageDeviceDescriptor->ProductRevisionOffset);
    printf("%-25s: %lu\n", "SerialNumberOffset",    pStorageDeviceDescriptor->SerialNumberOffset);
    printf("%-25s: %u\n\n", "BusType",               pStorageDeviceDescriptor->BusType);

    ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
    storagePropertyQuery.PropertyId = StorageAdapterProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    STORAGE_ADAPTER_DESCRIPTOR descriptor;

    DeviceIoControl(
        _In_        (HANDLE)       hDevice,                            // handle to a partition
        _In_        (DWORD) IOCTL_STORAGE_QUERY_PROPERTY,              // dwIoControlCode
        _In_        (LPVOID)       &storagePropertyQuery,              // input buffer - STORAGE_PROPERTY_QUERY structure
        _In_        (DWORD)        sizeof(STORAGE_PROPERTY_QUERY),     // size of input buffer
        _Out_opt_   (LPVOID)       &descriptor,           // output buffer - see Remarks
        _In_        (DWORD)        sizeof(STORAGE_ADAPTER_DESCRIPTOR),  // size of output buffer
        _Out_opt_   (LPDWORD)      &Junk,                              // number of bytes returned
        _Inout_opt_ (LPOVERLAPPED) NULL                                // OVERLAPPED structure
    );

    printf("%-30s: %lu\n", "MaximumTransferLength",               descriptor.MaximumTransferLength);
    printf("%-30s: %lu\n", "MaximumPhysicalPages",               descriptor.MaximumPhysicalPages);
    printf("%-30s: %lu\n", "AlignmentMask",               descriptor.AlignmentMask);
    printf("%-30s: %s\n", "AdapterUsesPio",               descriptor.AdapterUsesPio ? "TRUE" : "FALSE");
    printf("%-30s: %s\n", "AdapterScansDown",               descriptor.AdapterScansDown ? "TRUE" : "FALSE");
    printf("%-30s: %s\n", "CommandQueueing",               descriptor.CommandQueueing ? "TRUE" : "FALSE");
    printf("%-30s: %s\n", "AcceleratedTransfer",               descriptor.AcceleratedTransfer ? "TRUE" : "FALSE");

    CloseHandle(hDevice);

    return 0;
}