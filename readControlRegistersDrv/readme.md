ReadControlRegistersDrv is a Windows Kernel-mode driver that on execution prints out various values from the CR0 and CR4 Intel Control Registers.

Tested on Windows 10 x64 1607 RS1

![f645fee096ea4f39aefed46d3bab0613](https://user-images.githubusercontent.com/70239991/123901932-75690400-d95b-11eb-823b-e64f09474728.png)

Loaded the unsigned kernel driver through exploiting Capcom.sys using drvmap
![302211d220e5412f9a4d712a02ca78ca](https://user-images.githubusercontent.com/70239991/123901943-7b5ee500-d95b-11eb-914a-19bd74d69b8a.png)
