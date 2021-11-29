#include <windows.h>
#include <iostream>

#define INFO_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define STRING_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1338, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define DRIVER_PATH "\\\\.\\InfoDevice"

int main()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	BOOL result = FALSE;
	DWORD lpBytesReturned = 0;
	CHAR inputBuffer[128] = "This is data being sent to the kernel";

	hFile = CreateFileA(DRIVER_PATH, GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "[!] Failed to open a HANDLE to the device\n";
		CloseHandle(hFile);
		exit(1);
	}
	
	std::cout << "[+] Successfully opened a HANDLE to the device\n";

	std::string user_selection;
	std::cout << "Select an IOCTL:\n\t1. INFO_IOCTL\n\t2. STRING_IOCTL\nPrompt: ";
	std::cin >> user_selection;

	if (user_selection == "INFO_IOCTL")
	{
		result = DeviceIoControl(hFile, INFO_IOCTL, NULL, 0, NULL, 0, &lpBytesReturned, NULL);
	} 
	if (user_selection == "STRING_IOCTL")
	{
		result = DeviceIoControl(hFile, STRING_IOCTL, inputBuffer, sizeof(inputBuffer), NULL, 0, &lpBytesReturned, NULL);
		
		if (outputBuffer != NULL)
		{
			std::cout << "Kernel buffer: " << outputBuffer << std::endl;
		}
	}

	if (!result)
	{
		std::cout << "[!] Failed to interact with the IOCTL\n";
		exit(1);
	} 

	CloseHandle(hFile);

	return 0;
}
