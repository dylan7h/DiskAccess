#include <windows.h>

#include <iostream>
#include <string>
#include <tchar.h>
using namespace std;

#include "Disk.h"

int main(int argc, char* argv[])
{
    CDiskAccess Disk;
    DWORD DeviceID;
    STORAGE_DEVICE_DESCRIPTOR Storage_Desc;

    Disk.GetHandleOnVolume(_T('D'));
    Disk.GetDeviceID(DeviceID);
    Disk.GetDeviceDescriptor(Storage_Desc);

    cout<< "[ Storage_Desc ]" << endl;
    printf("  %-25s: %s\n", "BusType",                 CDiskAccess::Get_Storage_Bus_Type(Storage_Desc.BusType).c_str());
    printf("  %-25s: %d\n", "DeviceType",              Storage_Desc.DeviceType);
    printf("  %-25s: %d\n", "DeviceTypeModifier",      Storage_Desc.DeviceTypeModifier);
    printf("  %-25s: %d\n", "ProductIdOffset",         Storage_Desc.ProductIdOffset);
    printf("  %-25s: %d\n", "ProductRevisionOffset",   Storage_Desc.ProductRevisionOffset);
    printf("  %-25s: %d\n", "RawDeviceProperties",     Storage_Desc.RawDeviceProperties);
    printf("  %-25s: %d\n", "RawPropertiesLength",     Storage_Desc.RawPropertiesLength);
    printf("  %-25s: %d\n", "RemovableMedia",          Storage_Desc.RemovableMedia);
    printf("  %-25s: %d\n", "SerialNumberOffset",      Storage_Desc.SerialNumberOffset);
    printf("  %-25s: %d\n", "Size",                    Storage_Desc.Size);
    printf("  %-25s: %d\n", "VendorIdOffset",          Storage_Desc.VendorIdOffset);
    printf("  %-25s: %d\n", "Version",                 Storage_Desc.Version);

    return 0;
}