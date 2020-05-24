#include <windows.h>
#include <winioctl.h>

#include <iostream>
#include <string>
using namespace std;

#include "Disk.h"

CDiskAccess::CDiskAccess()
    : m_hVolume(INVALID_HANDLE_VALUE), m_hFile(INVALID_HANDLE_VALUE), m_hRawDisk(INVALID_HANDLE_VALUE)
{
}

CDiskAccess::~CDiskAccess()
{
    if(m_hVolume != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hVolume);
        cout << "Closed -- Volume Handle" << endl;
    }
}

HANDLE CDiskAccess::GetHandleOnVolume(const TCHAR& Char_Volume)
{
    string VolumeName = "\\\\.\\A:";
    DWORD   dwAccessMode = GENERIC_READ;
    DWORD   dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

    VolumeName.at(4) = Char_Volume;

    if(m_hVolume == INVALID_HANDLE_VALUE)
    {
        m_hVolume = CreateFile(
            VolumeName.c_str(), 
            dwAccessMode,
            dwShareMode,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if(m_hVolume == INVALID_HANDLE_VALUE) {
            cout << "Fail -- Get Volume Handle" << endl;
        }
        else {
            cout << "Success -- Get Volume Handle" << endl;
        }
    }

    return m_hVolume;
}

bool CDiskAccess::GetDeviceID(DWORD& DiskNumber)
{
    bool bReturn = false;

    VOLUME_DISK_EXTENTS Volume_Disk;
    DWORD dwBytesReturned;
    BOOL bResult;

    bResult = DeviceIoControl(
        m_hVolume, 
        IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 
        NULL, 0, 
        &Volume_Disk, sizeof(Volume_Disk), 
        &dwBytesReturned, 
        NULL
    );

    if(bResult == FALSE) {
        cout << "Faile -- DeviceIoControl() " << endl;
    }
    else {
        cout << "Success -- DeviceIoControl()" << endl;
        bReturn = true;
    }

    if(Volume_Disk.NumberOfDiskExtents < 1) {
        cout << "Error -- NumberOfDiskExtents: " << Volume_Disk.NumberOfDiskExtents << endl;
        bReturn = false;
    }
    else {
        DiskNumber = Volume_Disk.Extents[0].DiskNumber;
        cout << "Success -- Get Disk Number" << endl;
    }
    
    return bReturn;
}
bool CDiskAccess::GetDeviceID(void)
{
    DWORD dwTemp;
    return this->GetDeviceID(dwTemp);
}

bool CDiskAccess::GetDeviceDescriptor(STORAGE_DEVICE_DESCRIPTOR& Storage_Desc)
{
    HANDLE hDevice;
    STORAGE_PROPERTY_QUERY Storage_Query;
    DWORD dwOut;
    BOOL bRet;
    
    ZeroMemory(&m_Storage_Desc, sizeof(STORAGE_DEVICE_DESCRIPTOR));
    
    // Device Io Control을 호출한다.
    Storage_Query.PropertyId = StorageDeviceProperty;
    Storage_Query.QueryType = PropertyStandardQuery;
    bRet = DeviceIoControl(
        m_hVolume, IOCTL_STORAGE_QUERY_PROPERTY,
        &Storage_Query, sizeof(STORAGE_PROPERTY_QUERY),
        &m_Storage_Desc, sizeof(STORAGE_DEVICE_DESCRIPTOR), 
        &dwOut, 
        NULL
    );    

    if(bRet == FALSE) {
        cout << "Faile -- DeviceIoControl() " << endl;
    }             

    return (bRet == FALSE) ? false : true;
}
bool CDiskAccess::GetDeviceDescriptor(void)
{
    STORAGE_DEVICE_DESCRIPTOR Junk;
    return this->GetDeviceDescriptor(Junk);
}

string CDiskAccess::Get_Storage_Bus_Type(STORAGE_BUS_TYPE& eBusType)
{
    string strType;

    switch(eBusType) {
    case STORAGE_BUS_TYPE::BusTypeUnknown:     
        strType = "Unknown";     
        break;
    case STORAGE_BUS_TYPE::BusTypeScsi: 
        strType = "Scsi";     
        break;
    case STORAGE_BUS_TYPE::BusTypeAtapi:            
        strType = "Atapi";     
        break;
    case STORAGE_BUS_TYPE::BusTypeAta:             
        strType = "Ata";     
        break;
    case STORAGE_BUS_TYPE::BusType1394:             
        strType = "1394";     
        break;
    case STORAGE_BUS_TYPE::BusTypeSsa:            
        strType = "Ssa";     
        break;
    case STORAGE_BUS_TYPE::BusTypeFibre:            
        strType = "Fibre";     
        break;
    case STORAGE_BUS_TYPE::BusTypeUsb:              
        strType = "Usb";     
        break;
    case STORAGE_BUS_TYPE::BusTypeRAID:             
        strType = "RAID";     
        break;
    case STORAGE_BUS_TYPE::BusTypeiScsi:            
        strType = "iScsi";     
        break;
    case STORAGE_BUS_TYPE::BusTypeSas:              
        strType = "Sas";     
        break;
    case STORAGE_BUS_TYPE::BusTypeSata:             
        strType = "Sata";     
        break;
    case STORAGE_BUS_TYPE::BusTypeSd:               
        strType = "Sd";     
        break;
    case STORAGE_BUS_TYPE::BusTypeMmc:              
        strType = "Mmc";     
        break;
    case STORAGE_BUS_TYPE::BusTypeVirtual:          
        strType = "Virtual";     
        break;
    case STORAGE_BUS_TYPE::BusTypeFileBackedVirtual:
        strType = "FileBackedVirtual";     
        break;
    default:
        strType = "Reserved";
        break;
    }
       
    return strType;
}

