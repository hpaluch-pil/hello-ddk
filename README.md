# hello-ddk

It is an attempt to create simplest possible Windows 10 Driver to learn
Windows kernel Debugging and other skills necessary when working
with Windows Drivers.

Tested Requirements (from https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads):
- OS: `Windows 10 21H2` (command `winver`)
- `WDK for Windows 10, version 2004`:
  - https://go.microsoft.com/fwlink/?linkid=2128854 (Web installer that is capable to download files
    for offline install)
- Visual Studio 2019 Professional
 
# Building

- Double click on ioctl.sln to Open this project in Visual Studio 2019 Professional.
- Select
  - Configuration: `Debug` (should be default)
  - Platform: `x64`
- Rebuild whole Solution

# Installing driver

After Rebuild you have to install Driver in Elevated CMD prompt using:
```cmd
cd /d sys\x64\Debug
"c:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe" install sioctl.inf _SIOCTL0000

Device node created. Install is complete when drivers are installed...
Updating drivers for _SIOCTL0000 from .....\sys\x64\Debug\sioctl.inf.
Drivers installed successfully.
```

Also in Device Manager (`devmgmt.msc`) you should see new device:
- System devices -> `Pickering SIOCTL Driver`

Or in CLI:
```cmd
"c:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe" status *SIOCTL*

ROOT\SYSTEM\0002
    Name: Pickering SIOCTL Driver
    Driver is running.
1 matching device(s) found.
```

# Running

When driver is already insalle dyou can just
run application `ioctlapp.exe` directly from Visual Studio
or from CLI as
```cmd
exe\x64\Debug\ioctlapp.exe
```

Example output
```
Connecting to device '\\.\IoctlTest'...
InputBuffer Pointer = 00007FF76C588530, BufLength = 100
OutputBuffer Pointer = 00007FF76C5884C0 BufLength = 100

Calling DeviceIoControl METHOD_BUFFERED:
    OutBuffer (38): This String is from Device Driver !!!

Calling DeviceIoControl METHOD_NEITHER
    OutBuffer (38): This String is from Device Driver !!!

Calling DeviceIoControl METHOD_IN_DIRECT
    Number of bytes transfered from OutBuffer: 100

Calling DeviceIoControl METHOD_OUT_DIRECT
    OutBuffer (38): This String is from Device Driver !!!
Closing handle 00000000000000A4 from device '\\.\IoctlTest'...
```

# Watch kernel Debug messages

To see *Driver* debug messages (not just application messages) you have to:
- build Solution in `Debug` mode
- download MS DebugView from:
  - https://docs.microsoft.com/en-us/sysinternals/downloads/debugview
- run DebugView as Administrator
- in menu enable Capture -> Capture Kernel
- also enable Capture -> Enable Verbose Kernel Output
- run above example `ioctlapp.exe` again
- watch for messages beginning with `SIOCTL.SYS` (there will be also many other messages)

NOTE: Unfortunately I was unable to use filter `SIOCTL.SYS` because it will then print only 1st line of
debug message which is not much useful.


# Resources

This sample loosely is based on:
- https://github.com/microsoft/Windows-driver-samples/tree/main/general/ioctl/wdm

