#include <ntddk.h>
#include <intrin.h>

NTSTATUS CrmDriverCR0Read() {

	NTSTATUS status = STATUS_SUCCESS;

	UINT64 readCr0 = __readcr0();

	BOOLEAN ProtectedMode = readCr0 & 0x0; // CR0.PE
	BOOLEAN WriteProtect = readCr0 & 0x16; // CR0.WP
	BOOLEAN CacheDisabled = readCr0 & 0x30; // CR0.NW
	BOOLEAN Paging = readCr0 & 0x31; // CR0.PG

	DbgPrint("[Is the system in protected mode?] CR0.PE flag 0th bit is set to - %c\n", ProtectedMode ? '1' : '0');
	DbgPrint("[Is the CPU able to write to read-only pages?] CR0.WP flag 16th bit is set to - %c\n", WriteProtect ? '1' : '0');
	DbgPrint("[Is the memory cache enabled/disabled?] CR0.NW flag 30th bit is set to - %c\n", CacheDisabled ? '1' : '0');
	DbgPrint("[Is Paging enabled?] CR0.PG flag 31st bit is set to - %c\n", Paging ? '1' : '0');

	return status;
}

NTSTATUS CrmDriverCR4Read() {

	NTSTATUS status = STATUS_SUCCESS;

	UINT64 readCr4 = __readcr4();

	BOOLEAN SmapEnabled = readCr4 & 0x20; // CR4.SMAP
	BOOLEAN SmepEnabled = readCr4 & 0x21; // CR4.SMEP

	DbgPrint("[Is SMAP enabled?] CR4.SMAP flag 20th bit is set to - %c\n", SmapEnabled ? '1' : '0');
	DbgPrint("[Is SMEP enabled?] CR4.SMEP flag 21st bit is set to - %c\n", SmepEnabled ? '1' : '0');
	return status;
}

void DriverUnload(PDRIVER_OBJECT DriverObject) {

	UNREFERENCED_PARAMETER(DriverObject);

}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath){

	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;

	DriverObject->DriverUnload = DriverUnload;

	DbgPrint("[>] Reading values from the CR0 Control Register\n\n");

	CrmDriverCR0Read();

	DbgPrint("\n\n[>] Reading values from the CR4 Control Register\n\n");

	CrmDriverCR4Read();

	return status;
}
