#ifndef __RAW_STORAGE_H__
#define __RAW_STORAGE_H__

#ifdef UNICODE
    #define _tstring wstring
#else
    #define _tstring string
#endif

enum class eStorageIO_Mode {
    INFO = 0,
    READ = 1,
    WRITE = 2,
};

typedef struct _StorageInfo {
    DWORD               StorageNumber;
    DWORD               BytePerSector;
    LONGLONG            StorageSize;
    BYTE                DeviceType;
    BYTE                DeviceTypeModifier;
    bool                RemovableMedia;
    bool                CommandQueueing;  
    std::_tstring       VendorID;
    std::_tstring       ProductID;
    std::_tstring       ProductRevision;
    std::_tstring       SerialNumber;
    STORAGE_BUS_TYPE    BusType;
    DWORD               MaximumTransferLength;
    DWORD               MaximumPhysicalPages;
    PARTITION_STYLE     PartitionStyle;
} StorageInfo_t, *LPStorageInfo_t;

typedef void(*ProgressCallback)(ULONG TotalSector, ULONG RemainSector, LPVOID lpParam);

class CRawStorage {
private:
    bool            m_bInfoInit;
    HANDLE          m_hStorage;
    eStorageIO_Mode m_IO_Mode;
    TCHAR*          m_LogicalDevice;
    TCHAR*          m_PhysicalDevice;
    StorageInfo_t   m_StorageInfo;

public:
    CRawStorage(TCHAR& LogicalName);
    ~CRawStorage();

private:
    bool Get_VolumeNumber();
    bool Get_Geometry();
    bool Get_Descriptor();
    bool Get_Adapter_Descriptor();
    bool Get_Partition_Info();      

public:
    bool Get_Storage_Info(LPStorageInfo_t pStorageInfo = nullptr);

public:
    bool Open(eStorageIO_Mode m_IO_Mode);

private:

public:
    bool Write(DWORD SectorBase, LPBYTE pBuff, DWORD cbSector, ProgressCallback Proc = nullptr, LPVOID lpParam = nullptr);
    bool Read(DWORD SectorBase, LPBYTE pBuff, DWORD cbSector, ProgressCallback Proc = nullptr, LPVOID lpParam = nullptr);
    DWORD Verify(LPBYTE pDest, LPBYTE pSrc, DWORD cbSector, ProgressCallback Proc = nullptr, LPVOID lpParam = nullptr);
};

#endif