#include <ntddk.h>
#include <wdm.h>

#define INFO_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_ANY_ACCESS)

UNICODE_STRING SYM_NAME = RTL_CONSTANT_STRING(L"\\Device\\InfoDevice");
UNICODE_STRING DEV_NAME = RTL_CONSTANT_STRING(L"\\??\\InfoDevice");
DRIVER_DISPATCH HandleIoctls;

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("Driver has been unloaded");

    IoDeleteDevice(DriverObject->DeviceObject);
    IoDeleteSymbolicLink(&SYM_NAME);
}

NTSTATUS HandleIoctls(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION stack_location;
    NTSTATUS status = STATUS_SUCCESS;

    stack_location = IoGetCurrentIrpStackLocation(Irp);

    switch (stack_location->MajorFunction)
    {
    case IRP_MJ_DEVICE_CONTROL:

        DbgPrint("IOCTL was recieved in kernel-mode");

        if (stack_location->Parameters.DeviceIoControl.IoControlCode == INFO_IOCTL)
        {
            DbgPrint("Hit IOCTL call");
        }

    case IRP_MJ_CREATE:

        DbgPrint("Handle to the symolic link was created");
        status = STATUS_SUCCESS;

    case IRP_MJ_CLOSE:

        DbgPrint("Handle to the symolic link was closed");
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
    // basic info
    DbgPrint("DriverStart Address 0x%x", DriverObject->DriverStart);
    DbgPrint("DriverSize 0x%d", DriverObject->DriverSize);
    DbgPrint("DriverSection Address 0x%x", DriverObject->DriverSection);
    DbgPrint("DriverUnload Address 0x%x", DriverObject->DriverUnload);

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
        DbgPrint("Failed to create the device");
    }
    else {
        status = IoCreateSymbolicLink(&SYM_NAME, &DEV_NAME);
        if (NT_SUCCESS(status))
        {
            DbgPrint("Symbolic link successfully created");
        }
        else {
            DbgPrint("Symbolic link not created");
        }

    }

    return STATUS_SUCCESS;

}
