#ifndef __RAW_STORAGE_H__
#define __RAW_STORAGE_H__

enum class eStorageIO {
    INFO, READ, WRITE
};

typedef struct {
    BYTE    DeviceType;
    BYTE    DeviceTypeModifier;
    bool    RemovableMedia;
    bool    CommandQueueing;
    TCHAR*  VendorID;
    TCHAR*  ProductID;
    TCHAR*  ProductRevision;
    TCHAR*  SerialNumber;
    STORAGE_BUS_TYPE BusType;
} Storage_Info_t;

typedef void(*ProgressCallback)(DWORD TotalSector, DWORD RemainSector);

class CRawStorage {
private:
    TCHAR* m_LogicalName;

public:
    CRawStorage(TCHAR LogicalName[]);
    CRawStorage(const TCHAR LogicalName[]);
    ~CRawStorage();

public:
    bool Open(eStorageIO& eMode);
    void Close(void);
    
public:
    bool GetStorageInfo(OUT Storage_Info_t& Info);

public:
};

#endif