#include <ntddk.h>

void SampleDriverUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	KdPrint(("[>] Driver has been unloaded\n"));
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = SampleDriverUnload;

	KdPrint(("[>] Driver has been loaded successfully\n"));
	KdPrint(("[+] Getting Windows OS version information....\n"));

	RTL_OSVERSIONINFOW rOsInformation = { 0 };
	rOsInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	NTSTATUS getVersion = RtlGetVersion(&rOsInformation);

	KdPrint(("[*] RtlGetVersion result:  %d\n", getVersion));

	KdPrint(("[*] RTL_OSVERSIONINFOW members:\n"));
	KdPrint(("\t\t[dwMajorVersion] - %d \n", rOsInformation.dwMajorVersion));
	KdPrint(("\t\t[dwMinorVersion] - %d \n", rOsInformation.dwMinorVersion));
	KdPrint(("\t\t[dwBuildNumber]  - %d \n", rOsInformation.dwBuildNumber));
	KdPrint(("\t\t[dwPlatformId]   - %d \n", rOsInformation.dwPlatformId));

	if (NT_SUCCESS(getVersion)) {
		switch (rOsInformation.dwMajorVersion) {

		case 10:
			KdPrint(("[+] OS Version - Windows 10 \n"));
			break;
		case 6:
			if (rOsInformation.dwMinorVersion == 3) {
				KdPrint(("[+] OS Version - Windows 8.1\n"));
			}
			else if (rOsInformation.dwMinorVersion == 2) {
				KdPrint(("[+] OS Version - Windows 8\n"));
			}
			else if (rOsInformation.dwMinorVersion == 1) {
				KdPrint(("[+] OS Version - Windows 7\n"));
			}
			else {
				KdPrint(("[+] OS Version - Windows Vista\n"));
			}
		case 5:
			if (rOsInformation.dwMinorVersion == 2) {
				KdPrint(("[+] OS Version - Windows XP Professional x64 Edition\n"));
			}
			else if (rOsInformation.dwMinorVersion == 1) {
				KdPrint(("[+] OS Version - Windows XP\n"));
			}
			else {
				KdPrint(("[+] OS Version - Windows 2000\n"));
			}
		}
	}
	else {
		KdPrint(("[!] Error getting the OS version with RtlGetVersion()"));
	}
	return STATUS_SUCCESS;
}
