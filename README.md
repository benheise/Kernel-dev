# Kernel-dev

This repository contains random code snippets related to kernel driver development

|  **Driver Name**   |   **Description**  |
| --- | --- |
| getVersion.sys | Prints system version information from kernel-mode using RtlGetVersion |
| readControlRegistersDrv.sys | Checks if CR registers are enabling SMEP, SMAP, PE, WP, etc |
| getInfoDrvIoctl.sys | Prints information about the driver (DriverSection, etc) via an IOCTL |
| dkomHideDriver.sys | Hides the current driver via DKOM manipulation |
| execShellcode.sys | Disables SMEP and execute user-mode provided shellcode (used for LPE) |
