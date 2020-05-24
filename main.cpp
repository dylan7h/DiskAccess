/* #include <windows.h>
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
 */

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // 두 번째 함수 원형. (주의깊게 봐야 할 함수)

// 전역 변수 선언
HINSTANCE g_hInst;
LPSTR lpszClass = "First";   // 이 문자열은 윈도우 클래스를 정의하는데 사용된다.

// 첫 번째 함수 : WinMain의 모양은 대체로 일정
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;        // WinMain의 인수로 전달된 hInstance값을 전역 변수 g_hInst에 대입.
								// hInstance 인수는 기억부류가 지역 변수이기 때문에 WinMain의 밖에서는 사용할 수 없기 때문이다.

	WndClass.cbClsExtra = 0;        // 윈도우즈가 (특수한 목적에 의해) 내부적으로 사용하는 일종의 예약 영역이다.
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// 윈도우의 배경 색상을 채색할 브러시를 지정하는 멤버이다.
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);        // 윈도우가 사용할 마우스 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);        // 최소화 되었을 때 출력될 아이콘
	WndClass.hInstance = hInstance;
	// 이 윈도우 클래스를 사용하는 프로그램의 번호이다. (이 값은 WinMain의 인수로 전달된 hInstance값을 그대로 대입)
	WndClass.lpfnWndProc = (WNDPROC)WndProc;        // 이 멤버는 윈도우의 메시지 처리 함수를 지정한다.
													// (메시지가 발생할 때마다 여기서 지정한 함수가 호출되며 이 함수가 모든 메시지를 처리)
	WndClass.lpszClassName = lpszClass;        // 윈도우 클래스의 이름을 정의한다. (지정한 이름은 CreateWindow 함수에 전달)
											   // CreateWindow 함수는 윈도우 클래스에서 정의한 특성값을 참조하여 윈도우를 만든다.
	WndClass.lpszMenuName = NULL;        // 이 프로그램이 사용할 메뉴를 지정
	WndClass.style = CS_HREDRAW | CS_VREDRAW;        // 윈도우가 어떤 형태를 가질 것인가를 지정하는 멤버이다.
	RegisterClass(&WndClass);        // RegisterClass 함수의 인수로 WndClass 구조체의 번지를 넘겨주면 된다.
									 // 이런 이런 특성을 가진 윈도우를 앞으로 사용하겠다는 등록 과정이다.

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	// CreateWindow 함수는 윈도우에 관한 모든 정보를 메모리에 만든 후 윈도우 핸들을 리턴값으로 넘겨준다.
	// 넘겨지는 윈도우 핸들은 hWnd라는 지역 변수에 저장되었다가 윈도우를 참조하는 모든 함수의 인수로 사용된다.
	ShowWindow(hWnd, nCmdShow);        // 화면에 표시

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}



// 두 번째 함수 : WinMain에서는 윈도우를 만들고 화면에 출력하기만 할 뿐이며 대부분의 일은 WndProc에서 이루어진다.

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
