#include <windows.h>
#include <winioctl.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include "RawStorage.h"

using namespace std;

#define SAFE_DELETE(PTR)    \
if(PTR != nullptr) {        \
    delete PTR;             \
    PTR = nullptr;          \
}

#define SAFE_DELETE_ARRAY(PTR)      \
if(PTR != nullptr) {                \
    delete[] PTR;                   \
    PTR = nullptr;                  \
}

#define SAFE_CLOSE(PTR)             \
if(PTR != nullptr) {                \
    CloseHandle(PTR);               \
    PTR = INVALID_HANDLE_VALUE;     \
}

bool CRawStorage::Get_VolumeNumber() {
    BOOL                    bResult;
    HANDLE                  hDevice;
    DWORD                   Junk;
    STORAGE_DEVICE_NUMBER   Device_Number;
    TCHAR                   PhysicalDevice[] = _T("\\\\.\\PhysicalDrive00");

    hDevice = CreateFile(
        m_LogicalDevice,
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    if(hDevice == INVALID_HANDLE_VALUE) {
        return false;
    }

    bResult = DeviceIoControl(
        (HANDLE)hDevice,
        IOCTL_STORAGE_GET_DEVICE_NUMBER,
        NULL, 0,                                
        (LPVOID)&Device_Number, (DWORD)sizeof(STORAGE_DEVICE_NUMBER),          
        (LPDWORD)&Junk,
        (LPOVERLAPPED)NULL
    );
    CloseHandle(hDevice);

    if(bResult  == 0) {
        return false;
    }

    if(Device_Number.DeviceNumber < 10) {
        PhysicalDevice[17] += (TCHAR)Device_Number.DeviceNumber; 
        PhysicalDevice[18] = _T('\0'); 
    }
    else if(Device_Number.DeviceNumber < 100) {
        PhysicalDevice[17] += (TCHAR)Device_Number.DeviceNumber / 10; 
        PhysicalDevice[18] += (TCHAR)Device_Number.DeviceNumber % 10; 
        PhysicalDevice[19] = _T('\0'); 
    }
    else {
        return false;
    }

    m_StorageInfo.StorageNumber = Device_Number.DeviceNumber;

    m_PhysicalDevice = new TCHAR[lstrlen(PhysicalDevice) + 1];
    lstrcpy(m_PhysicalDevice, PhysicalDevice);
    
    return true;
}

CRawStorage::CRawStorage(TCHAR& LogicalName) 
    : m_LogicalDevice(nullptr), m_PhysicalDevice(nullptr), m_bInfoInit(false)
{
    TCHAR LogicalDevice[] = _T("\\\\.\\A:");

    LogicalDevice[4] = LogicalName;
    m_LogicalDevice = new TCHAR[lstrlen(LogicalDevice) + 1];

    lstrcpy(m_LogicalDevice, LogicalDevice);

    m_hStorage = INVALID_HANDLE_VALUE;

    m_bInfoInit = Get_Storage_Info();
}

CRawStorage::~CRawStorage() {
    SAFE_CLOSE(m_hStorage);
    SAFE_DELETE(m_LogicalDevice);
    SAFE_DELETE(m_PhysicalDevice);
}

bool CRawStorage::Get_Geometry() {
    bool bReturn;
    BOOL bResult;
    DWORD Junk;
    DISK_GEOMETRY_EX DiskGeometry;

    if(this->Open(eStorageIO_Mode::INFO) != true) {
        return false;
    }

    bResult = DeviceIoControl(
        this->m_hStorage,                 
        IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, 
        NULL, 0,                                
        &DiskGeometry, sizeof(DISK_GEOMETRY_EX),           
        &Junk,        
        NULL       
    );
    if(bResult  == 0) {
        bReturn = false;
        goto CLEAN_UP;
    }
    else {
        bReturn = true;
    }

    m_StorageInfo.StorageSize = DiskGeometry.DiskSize.QuadPart;
    m_StorageInfo.BytePerSector = DiskGeometry.Geometry.BytesPerSector;

CLEAN_UP:
    SAFE_CLOSE(m_hStorage);

    return bReturn;
}

bool CRawStorage::Get_Descriptor() {
    bool bReturn;
    BOOL bResult;
    DWORD Junk;
    
    STORAGE_PROPERTY_QUERY storagePropertyQuery;

    if(this->Open(eStorageIO_Mode::INFO) != true) {
        return false;
    }

    union {
        LPBYTE pBuff;
        TCHAR* strInfo;
        PSTORAGE_DEVICE_DESCRIPTOR pStorageDeviceDescriptor;
    };

    pBuff = new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 0xFFFF];
    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    bResult = DeviceIoControl(
        this->m_hStorage,                 
        IOCTL_STORAGE_QUERY_PROPERTY, 
        &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),                                
        pStorageDeviceDescriptor, sizeof(STORAGE_DEVICE_DESCRIPTOR) + 0xFFFF,           
        &Junk,        
        NULL       
    );
    if(bResult  == 0) {
        bReturn = false;
        goto CLEAN_UP;
    }
    else {
        bReturn = true;
    }

    m_StorageInfo.BusType               = pStorageDeviceDescriptor->BusType;
    m_StorageInfo.CommandQueueing       = pStorageDeviceDescriptor->CommandQueueing;
    m_StorageInfo.DeviceType            = pStorageDeviceDescriptor->DeviceType;
    m_StorageInfo.DeviceTypeModifier    = pStorageDeviceDescriptor->DeviceTypeModifier;
    m_StorageInfo.RemovableMedia        = pStorageDeviceDescriptor->RemovableMedia;
    m_StorageInfo.ProductID             = _tstring((TCHAR*)(strInfo + pStorageDeviceDescriptor->ProductIdOffset));
    m_StorageInfo.ProductRevision       = _tstring((TCHAR*)(strInfo + pStorageDeviceDescriptor->ProductRevisionOffset));
    m_StorageInfo.SerialNumber          = _tstring((TCHAR*)(strInfo + pStorageDeviceDescriptor->SerialNumberOffset));
    m_StorageInfo.VendorID              = _tstring((TCHAR*)(strInfo + pStorageDeviceDescriptor->VendorIdOffset));

CLEAN_UP:
    SAFE_DELETE(pStorageDeviceDescriptor);
    SAFE_CLOSE(m_hStorage);

    return bReturn;
}

bool CRawStorage::Get_Adapter_Descriptor() {
    bool bReturn;
    BOOL bResult;
    DWORD Junk;
    
    STORAGE_ADAPTER_DESCRIPTOR Adapter_Descriptor;
    STORAGE_PROPERTY_QUERY storagePropertyQuery;
    

    if(this->Open(eStorageIO_Mode::INFO) != true) {
        return false;
    }

    storagePropertyQuery.PropertyId = StorageAdapterProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    bResult = DeviceIoControl(
        this->m_hStorage,                 
        IOCTL_STORAGE_QUERY_PROPERTY, 
        &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),                                
        &Adapter_Descriptor, sizeof(STORAGE_ADAPTER_DESCRIPTOR),           
        &Junk,        
        NULL       
    );
    if(bResult  == 0) {
        bReturn = false;
        goto CLEAN_UP;
    }
    else {
        bReturn = true;
    }

    m_StorageInfo.MaximumTransferLength = Adapter_Descriptor.MaximumTransferLength;
    m_StorageInfo.MaximumPhysicalPages = Adapter_Descriptor.MaximumPhysicalPages;

CLEAN_UP:
    SAFE_CLOSE(m_hStorage);

    return bReturn;
}

bool CRawStorage::Get_Partition_Info() {
    bool bReturn;
    BOOL bResult;
    DWORD Junk;
    PARTITION_INFORMATION_EX PartitionInfo;

    if(this->Open(eStorageIO_Mode::INFO) != true) {
        return false;
    }

    bResult = DeviceIoControl(
        this->m_hStorage,                 
        IOCTL_DISK_GET_PARTITION_INFO_EX, 
        NULL, 0,                                
        &PartitionInfo, sizeof(PARTITION_INFORMATION_EX),           
        &Junk,        
        NULL       
    );
    if(bResult  == 0) {
        bReturn = false;
        goto CLEAN_UP;
    }
    else {
        bReturn = true;
    }

    m_StorageInfo.PartitionStyle = PartitionInfo.PartitionStyle;

CLEAN_UP:
    SAFE_CLOSE(m_hStorage);

    return bReturn;
}   

DWORD CRawStorage::Get_BytePerSector(void) const {
    return m_StorageInfo.BytePerSector;
}

DWORD CRawStorage::Get_StorageSize(void) const {
    return m_StorageInfo.StorageSize;
}

bool CRawStorage::Get_Storage_Info(LPStorageInfo_t pStorageInfo) {
    bool bResult;
    if(m_bInfoInit != true) {
        bResult = Get_Geometry();
        if(bResult != true) {
            return false;
        }

        bResult = Get_Descriptor();
        if(bResult != true) {
            return false;
        }

        bResult = Get_Adapter_Descriptor();
        if(bResult != true) {
            return false;
        }

        bResult = Get_Partition_Info();   
        if(bResult != true) {
            return false;
        }
    }

    if(pStorageInfo != nullptr) {
        pStorageInfo->StorageNumber           = m_StorageInfo.StorageNumber;
        pStorageInfo->BytePerSector           = m_StorageInfo.BytePerSector;
        pStorageInfo->StorageSize             = m_StorageInfo.StorageSize;
        pStorageInfo->DeviceType              = m_StorageInfo.DeviceType;
        pStorageInfo->DeviceTypeModifier      = m_StorageInfo.DeviceTypeModifier;
        pStorageInfo->RemovableMedia          = m_StorageInfo.RemovableMedia;
        pStorageInfo->CommandQueueing         = m_StorageInfo.CommandQueueing;  
        pStorageInfo->VendorID                = m_StorageInfo.VendorID;
        pStorageInfo->ProductID               = m_StorageInfo.ProductID;
        pStorageInfo->ProductRevision         = m_StorageInfo.ProductRevision;
        pStorageInfo->SerialNumber            = m_StorageInfo.SerialNumber;
        pStorageInfo->BusType                 = m_StorageInfo.BusType;
        pStorageInfo->MaximumTransferLength   = m_StorageInfo.MaximumTransferLength;
        pStorageInfo->MaximumPhysicalPages    = m_StorageInfo.MaximumPhysicalPages;
        pStorageInfo->PartitionStyle          = m_StorageInfo.PartitionStyle;
    }
    
    return true;
}

bool CRawStorage::Open(eStorageIO_Mode m_IO_Mode) {
    DWORD   ModeIdx;
    DWORD   DesiredAccess;
    BOOL    bResult;
    TCHAR*  pVolumeName;

    switch(m_IO_Mode) {
    case eStorageIO_Mode::INFO:
        DesiredAccess = 0;
        SAFE_CLOSE(m_hStorage);
        pVolumeName = m_PhysicalDevice;
        break;

    case eStorageIO_Mode::READ:
        DesiredAccess = GENERIC_READ;
        SAFE_CLOSE(m_hStorage);
        pVolumeName = m_LogicalDevice;
        break;

    case eStorageIO_Mode::WRITE:
        DesiredAccess = GENERIC_WRITE;
        SAFE_CLOSE(m_hStorage);
        pVolumeName = m_LogicalDevice;
        break;

    default:
        return INVALID_HANDLE_VALUE;
    }

    if(m_PhysicalDevice == nullptr) {
        if(this->Get_VolumeNumber() != true) {
            return INVALID_HANDLE_VALUE;
        }
    }

    m_hStorage = CreateFile(
        pVolumeName,
        DesiredAccess,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
    );
    if(m_hStorage == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

void CRawStorage::Close() {
    SAFE_CLOSE(m_hStorage);
}


bool CRawStorage::Write(DWORD SectorBase, LPBYTE pBuff, DWORD cbSector, ProgressCallback Proc, LPVOID lpParam) {
    BOOL bResult;
    bool bReturn;
    DWORD Junk;
    DWORD TransferUnit;
    DWORD RemainSector;
    LPBYTE lpPos;
    LARGE_INTEGER Distance;
    LARGE_INTEGER NewDistance;

    bReturn = this->Open(eStorageIO_Mode::WRITE);
    if(bReturn != true) {
        return false;
    }

    /* Lock On Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    /* Unmount Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    Distance.QuadPart = (LONGLONG)SectorBase * Get_BytePerSector();
    SetFilePointerEx(this->m_hStorage, Distance, &NewDistance, FILE_BEGIN);
    if(Distance.QuadPart != NewDistance.QuadPart) {
        bReturn = false;
        goto CLENA_UP;
    }

    RemainSector = cbSector;
    lpPos = pBuff;
    do {
        TransferUnit = (RemainSector > 1024) ? 1024 : 1;
        bResult = WriteFile(this->m_hStorage, lpPos, TransferUnit * m_StorageInfo.BytePerSector, &Junk, NULL);
        if(bResult != TRUE) {
            bReturn = false;
            goto CLENA_UP;
        }

        lpPos += TransferUnit * m_StorageInfo.BytePerSector;
        RemainSector -= TransferUnit;

        if(Proc != nullptr) {
            Proc(cbSector, RemainSector, lpParam);
        }
    } while(RemainSector != 0);

    bReturn = true;

CLENA_UP:
    /* Unlock On Volume */
    DeviceIoControl(this->m_hStorage, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    SAFE_CLOSE(m_hStorage);

    return bReturn;
}

bool CRawStorage::Read(DWORD SectorBase, LPBYTE pBuff, DWORD cbSector, ProgressCallback Proc, LPVOID lpParam) {
    BOOL bResult;
    bool bReturn;
    DWORD Junk;
    DWORD TransferUnit;
    DWORD RemainSector;
    LPBYTE lpPos;
    LARGE_INTEGER Distance;
    LARGE_INTEGER NewDistance;

    bReturn = this->Open(eStorageIO_Mode::READ);
    if(bReturn != true) {
        return false;
    }

    /* Lock On Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    /* Unmount Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    Distance.QuadPart = (LONGLONG)SectorBase * Get_BytePerSector();
    SetFilePointerEx(this->m_hStorage, Distance, &NewDistance, FILE_BEGIN);
    if(Distance.QuadPart != NewDistance.QuadPart) {
        bReturn = false;
        goto CLENA_UP;
    }

    RemainSector = cbSector;
    lpPos = pBuff;
    do {
        TransferUnit = (RemainSector > 1024) ? 1024 : 1;
        bResult = ReadFile(this->m_hStorage, lpPos, TransferUnit * m_StorageInfo.BytePerSector, &Junk, NULL);
        if(bResult != TRUE) {
            bReturn = false;
            goto CLENA_UP;
        }

        lpPos += TransferUnit * m_StorageInfo.BytePerSector;
        RemainSector -= TransferUnit;

        if(Proc != nullptr) {
            Proc(cbSector, RemainSector, lpParam);
        }
    } while(RemainSector != 0);

    bReturn = true;

CLENA_UP:
    /* Unlock On Volume */
    DeviceIoControl(this->m_hStorage, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    SAFE_CLOSE(m_hStorage)

    return bReturn;
}

bool CRawStorage::Clean(DWORD SectorBase, DWORD cbSector, ProgressCallback Proc = nullptr, LPVOID lpParam = nullptr) {
    BOOL bResult;
    bool bReturn;
    DWORD Junk;
    DWORD TransferUnit;
    DWORD RemainSector;
    LPBYTE pBuff = nullptr;
    LPBYTE lpPos;
    LARGE_INTEGER Distance;
    LARGE_INTEGER NewDistance;

    bReturn = this->Open(eStorageIO_Mode::WRITE);
    if(bReturn != true) {
        return false;
    }

    /* Lock On Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    /* Unmount Volume */
    bResult = DeviceIoControl(this->m_hStorage, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    if(bResult  == 0) {
        bReturn = false;
        goto CLENA_UP;
    }

    Distance.QuadPart = (LONGLONG)SectorBase * Get_BytePerSector();
    SetFilePointerEx(this->m_hStorage, Distance, &NewDistance, FILE_BEGIN);
    if(Distance.QuadPart != NewDistance.QuadPart) {
        bReturn = false;
        goto CLENA_UP;
    }
    
    pBuff = new BYTE[cbSector * this->Get_BytePerSector()];
    ZeroMemory(pBuff, cbSector * this->Get_BytePerSector());

    RemainSector = cbSector;
    lpPos = pBuff;
    do {
        TransferUnit = (RemainSector > 1024) ? 1024 : 1;
        bResult = WriteFile(this->m_hStorage, lpPos, TransferUnit * m_StorageInfo.BytePerSector, &Junk, NULL);
        if(bResult != TRUE) {
            bReturn = false;
            goto CLENA_UP;
        }

        lpPos += TransferUnit * m_StorageInfo.BytePerSector;
        RemainSector -= TransferUnit;

        if(Proc != nullptr) {
            Proc(cbSector, RemainSector, lpParam);
        }
    } while(RemainSector != 0);


    bReturn = true;

CLENA_UP:
    /* Unlock On Volume */
    DeviceIoControl(this->m_hStorage, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &Junk, NULL);
    SAFE_DELETE_ARRAY(pBuff);
    SAFE_CLOSE(m_hStorage);

    return bReturn;    
}


DWORD CRawStorage::Verify(LPBYTE pDest, LPBYTE pSrc, DWORD cbSector, ProgressCallback Proc, LPVOID lpParam) {
    int nResult;
    bool bReturn = true;
    DWORD TransferUnit;
    DWORD RemainSector;
    LPBYTE lpDstPos;
    LPBYTE lpSrcPos;

    RemainSector = cbSector;
    lpDstPos = pDest;
    lpSrcPos = pSrc;
    do {
        TransferUnit = (RemainSector > 1024) ? 1024 : 1;
        nResult = memcmp(lpSrcPos, lpDstPos, TransferUnit * m_StorageInfo.BytePerSector);
        if(nResult != 0) {
            bReturn = false;
            break;
        }

        lpDstPos += TransferUnit * m_StorageInfo.BytePerSector;
        lpSrcPos += TransferUnit * m_StorageInfo.BytePerSector;
        RemainSector -= TransferUnit;

        if(Proc != nullptr) {
            Proc(cbSector, RemainSector, lpParam);
        }
    } while(RemainSector != 0);

    return bReturn;
}
