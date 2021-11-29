#include <ntddk.h>
#include <wdm.h>

#define INFO_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_ANY_ACCESS)

UNICODE_STRING SYM_NAME = RTL_CONSTANT_STRING(L"\\Device\\InfoDevice");
UNICODE_STRING DEV_NAME = RTL_CONSTANT_STRING(L"\\??\\InfoDevice");
DRIVER_DISPATCH HandleIoctls;

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Driver has been unloaded", 0);

    IoDeleteDevice(DriverObject->DeviceObject);
    IoDeleteSymbolicLink(&SYM_NAME);
}

NTSTATUS HandleIoctls(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION stack_location = NULL;
    NTSTATUS status = STATUS_SUCCESS;

    stack_location = IoGetCurrentIrpStackLocation(Irp);

    switch (stack_location->MajorFunction)
    {
    case IRP_MJ_DEVICE_CONTROL:

      
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "IOCTL was recieved in kernel-mode", 0);

        if (stack_location->Parameters.DeviceIoControl.IoControlCode == INFO_IOCTL)
        {
            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Hit IOCTL call", 0);

            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "DriverStart: 0x%p", DeviceObject->DriverObject->DriverStart);
            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "DriverSize: 0x%d", DeviceObject->DriverObject->DriverSize);
            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "DriverSection: 0x%p", DeviceObject->DriverObject->DriverSection);
        }

    case IRP_MJ_CREATE:

        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Handle to the symolic link was created", 0);
        status = STATUS_SUCCESS;

    case IRP_MJ_CLOSE:

        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Handle to the symolic link was closed", 0);
        status = STATUS_SUCCESS;

    case IRP_MJ_WRITE:
        status = STATUS_SUCCESS;
    case IRP_MJ_READ:
        status = STATUS_SUCCESS;
    }

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS status = 0;

    UNREFERENCED_PARAMETER(RegistryPath);

    // Handle unloading the driver
    DriverObject->DriverUnload = DriverUnload;

    // When we pass an IOCTL from usermode
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HandleIoctls;

    // For opening and closing the devices symlink
    DriverObject->MajorFunction[IRP_MJ_CREATE] = HandleIoctls;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = HandleIoctls;

    IoCreateDevice(DriverObject, 0, &DEV_NAME, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DriverObject->DeviceObject);
    if (!NT_SUCCESS(status))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Failed to create the device", 0);
    }
    else {
        status = IoCreateSymbolicLink(&SYM_NAME, &DEV_NAME);
        if (NT_SUCCESS(status))
        {
            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Symbolic link successfully created", 0);
        }
        else {
            DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Symbolic link not created", 0);
        }

    }

    return STATUS_SUCCESS;

}
