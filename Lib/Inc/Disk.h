#ifndef __DISK_H__
#define __DISK_H__


class CDiskAccess {
private:
    HANDLE m_hVolume;
    HANDLE m_hFile;
    HANDLE m_hRawDisk;

private:
    DWORD m_dwDiskNumber;
    STORAGE_DEVICE_DESCRIPTOR m_Storage_Desc;

public:
    CDiskAccess();
    ~CDiskAccess();

public:
    HANDLE GetHandleOnVolume(const TCHAR& Char_Volume);
    bool GetDeviceID(DWORD& DiskNumber);
    bool GetDeviceID(void);

    bool GetDeviceDescriptor(STORAGE_DEVICE_DESCRIPTOR& Storage_Desc);
    bool GetDeviceDescriptor(void);

public:
    static std::string Get_Storage_Bus_Type(STORAGE_BUS_TYPE& eBusType);

};

#endif