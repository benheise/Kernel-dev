# Kernel-dev

This repository contains random code snippets related to kernel driver development

- **getVersion**: Determines information about the systems verion via `RtlGetVersion`
- **readControlRegistersDrv**: Checks if CR registers are enabling SMEP, SMAP, PE, WP, etc
- **getInfoDrvIoctl**: Prints information about the drivers DriverSection, DriverUnload etc via an IOCTL call
