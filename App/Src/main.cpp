#include <windows.h>
#include <winioctl.h>
#include <tchar.h>

#include <cstdlib>
#include <cstdio>

#define PHY_DRV_PATH "\\\\.\\PhysicalDrive1"

BOOL GetDrive_Geometry(LPCSTR lpszPath, PDISK_GEOMETRY pDG)
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;
    BOOL bResult = FALSE;
    DWORD Junk = 0;
    
    hDevice = CreateFile(
        lpszPath,                           // Physical Drive Path
        0,                                  // No Access To the Drive,
        FILE_SHARE_READ | FILE_SHARE_WRITE, // Share Mode
        NULL,                               // Defailt Security Attributes
        OPEN_EXISTING,                      // Diposition
        0,                                  // File Attributes
        NULL                                // Do not copy attributes
    );
    if(hDevice == INVALID_HANDLE_VALUE) {
        return FALSE;
    }
    bResult = DeviceIoControl(
        hDevice,                        // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
        NULL, 0,                        // no input buffer
        pDG, sizeof(DISK_GEOMETRY),     // output buffer
        &Junk,                          // # bytes returned
        (LPOVERLAPPED)NULL              // synchronous I/O
    );
    CloseHandle(hDevice);
    return bResult;
}

void ShowDrive_Geometry(void)
{
    DISK_GEOMETRY DG = { 0 };   // disk drive geometry structure
    BOOL bResult = FALSE;       // generic results flag
    ULONGLONG DiskSize = 0;     // size of the drive, in bytes
    bResult = GetDrive_Geometry(PHY_DRV_PATH, &DG);
    if (bResult) {
        printf("Drive path      = %s\n",   PHY_DRV_PATH);
        printf("Cylinders       = %I64d\n", DG.Cylinders);
        printf("Tracks/cylinder = %ld\n",   (ULONG) DG.TracksPerCylinder);
        printf("Sectors/track   = %ld\n",   (ULONG) DG.SectorsPerTrack);
        printf("Bytes/sector    = %ld\n",   (ULONG) DG.BytesPerSector);
        DiskSize = DG.Cylinders.QuadPart * (ULONG)DG.TracksPerCylinder *
                (ULONG)DG.SectorsPerTrack * (ULONG)DG.BytesPerSector;
        printf(
            "Disk size       = %I64d (Bytes)\n"
            "                = %.2f (Gb)\n", 
            DiskSize, (double) DiskSize / (1024 * 1024 * 1024)
        );
    } 
    else 
    {
        printf ("GetDriveGeometry failed. Error %ld.\n", GetLastError ());
    }
}

#define PHY "\\\\.\\PhysicalDrive1"

#define SECTOR_SIZE 512U
#define SECTOR_COUNT 2U

#define COL_CNT 48U
static void display_buff(const BYTE* pBuff, UINT Size)
{
	UINT i, j;
	printf("-------------------------------------------------------------");
	for (i = 0U; i < Size; i++)
	{
		if (i % COL_CNT == 0)
		{
			printf("\n[%02u]: ", i / COL_CNT);
		}
		printf("%02X ", pBuff[i]);
	}
	printf("\n-------------------------------------------------------------\n\n");
}

BOOL WriteSector(DWORD dwSectorOffset, LPBYTE lpBuffer, int cbSector)
{
	HANDLE hDisk = CreateFile(PHY, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	if (hDisk == INVALID_HANDLE_VALUE) {
		printf("Get Volume Handle - Failed\n");
		return FALSE;
	}

	LONG    hipart = dwSectorOffset >> (32 - 9);
	LONG    lopart = dwSectorOffset << 9;
	LONG	err;

	SetLastError(0);       // needed before SetFilePointer post err detection
	lopart = SetFilePointer(hDisk, lopart, &hipart, FILE_BEGIN);

	if (lopart == -1 && NO_ERROR != (err = GetLastError()))
	{
		printf("HWWrite: error %d\n", err);
		return FALSE;
	}

	DWORD   n;

	if (!WriteFile(hDisk, lpBuffer, cbSector * 512, &n, NULL))
	{
		err = GetLastError();
		printf("HWWrite: error %d\n", err);

		return FALSE;
	}

	CloseHandle(hDisk);

	return TRUE;
}

BOOL ReadSector(DWORD dwSectorOffset, LPBYTE lpBuffer, int cbSector)
{
	HANDLE hDisk = CreateFile(PHY, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	if (hDisk == INVALID_HANDLE_VALUE) {
		printf("Get Volume Handle - Failed\n");
		return FALSE;
	}

	LONG    hipart = dwSectorOffset >> (32 - 9);
	LONG    lopart = dwSectorOffset << 9;
	LONG	err;

	SetLastError(0);       // needed before SetFilePointer post err detection
	lopart = SetFilePointer(hDisk, lopart, &hipart, FILE_BEGIN);

	if (lopart == -1 && NO_ERROR != (err = GetLastError()))
	{
		printf("HRead: error %d\n", err);
		return FALSE;
	}

	DWORD   n;

	if (!ReadFile(hDisk, lpBuffer, cbSector * 512, &n, NULL))
	{
		err = GetLastError();
		printf("HRead: error %d\n", err);

		return FALSE;
	}

	CloseHandle(hDisk);

	return TRUE;
}

int main(void) {
	BYTE ReadBuff[SECTOR_SIZE*SECTOR_COUNT];
	BYTE WriteBuff[SECTOR_SIZE*SECTOR_COUNT];

	ZeroMemory(ReadBuff, SECTOR_SIZE*SECTOR_COUNT);
	ZeroMemory(WriteBuff, SECTOR_SIZE*SECTOR_COUNT);

	for (UINT i = 0; i < SECTOR_SIZE*SECTOR_COUNT; i++) {
		WriteBuff[i] = (BYTE)(i % 255U);
	}

	printf("Before Write\n");
	WriteSector(0, ReadBuff, SECTOR_COUNT);
	ReadSector(0, ReadBuff, SECTOR_COUNT);
	display_buff(ReadBuff, SECTOR_SIZE*SECTOR_COUNT);

	ZeroMemory(ReadBuff, SECTOR_SIZE*SECTOR_COUNT);

	printf("\nAfter Write\n");
	WriteSector(0, WriteBuff, SECTOR_COUNT);
	ReadSector(0, ReadBuff, SECTOR_COUNT);
	display_buff(ReadBuff, SECTOR_SIZE*SECTOR_COUNT);

	return 0;
}
