#include <windows.h>
#include <iostream>

#define INFO_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DRIVER_PATH "\\\\.\\InfoDevice"

int main()
{
	DWORD lpBytesReturned = 0;
	HANDLE hFile;
	BOOL result;

	hFile = CreateFileA(DRIVER_PATH, GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "[!] Failed to open a HANDLE to the device\n";
		CloseHandle(hFile);
		exit(1);
	}
	
	std::cout << "[+] Successfully opened a HANDLE to the device\n";

	result = DeviceIoControl(hFile, INFO_IOCTL, NULL, 0, NULL, 0, &lpBytesReturned, NULL);

	if (!result)
	{
		std::cout << "[!] Failed to interact with the IOCTL\n";
		exit(1);
	} 

	std::cout << "[+] Successfully issued the IOCTL\n";
	CloseHandle(hFile);

	return 0;
}
