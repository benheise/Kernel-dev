### Capture Debugging Information with DebugView

1. Set up the `Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter` Registry key
2. Use `DbgPrintEx` to have debug statements appear in DebugView

After loading the driver and executing the client, the `INFO_IOCTL` IOCTL will cause the driver to print information such as the drivers base address.

![image](https://user-images.githubusercontent.com/54753063/143798850-ff69af63-2a2e-4e88-8eda-3aa782151d9f.png)

### InfoDevice.sys

1. Defines the unloader function and sets up the rest of the IRP table
2. Registers a device under the name of "InfoDevice"
3. Accepts a `IRP_MJ_DEVICE_CONTROL` request from user-mode
4. Checks the inbound IOCTL code
5. Prints information about the driver based on the correct IOCTL code


### InfoDevice_client.exe

1. Obtains a HANDLE to the user-mode exposed driver device
2. Sends a request through `DeviceIoControl` which an IOCTL that matches what's defined in the driver
3. Triggers the `DriverInformation` function to print information about the driver
