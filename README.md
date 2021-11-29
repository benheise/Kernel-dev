# Kernel-dev

This repository contains random code snippets related to kernel driver development

|  **Driver Name**   |   **Description**  |
| --- | --- |
| getVersion | Prints system version information from kernel-mode using RtlGetVersion |
| readControlRegistersDrv | Checks if CR registers are enabling SMEP, SMAP, PE, WP, etc |
| getInfoDrvIoctl | Prints information about the drivers DriverSection, DriverUnload etc via an IOCTL call |
| dkomHideDriver | Hides the current driver via DKOM manipulation |
| execShellcode | Disables SMEP and execute user-mode provided shellcode (used for LPE) |
