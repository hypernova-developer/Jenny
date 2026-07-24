# Jenny Refactoring - Completed ✓

## Summary of Changes

- [x] Step 1: Created TODO.md
- [x] Step 2: Removed all comment lines from jenny.cpp
- [x] Step 3: Applied Allman brace style throughout
- [x] Step 4: Added `#ifdef _WIN32` cross-platform guards
  - [x] 4a: Replaced Windows-only includes with cross-platform alternatives (added `<sys/utsname.h>` on Linux)
  - [x] 4b: Wrapped `#pragma comment` in `#ifdef _WIN32`
  - [x] 4c: Added Linux alternative for HardwareDetective (using `/sys/class/dmi/id/`, `sysinfo()`, `lspci`, `df`)
  - [x] 4d: Added Linux alternatives for Sentinel functions:
    - GetFileHash: `sha256sum` command via popen
    - IsFileSigned: Returns false on Linux
    - CheckStartupStatus: Autostart dirs + crontab check
    - ScanNetworkActivity: `/proc/net/tcp` parsing + `readlink`
  - [x] 4e: Added Linux alternatives for getProcessorName() and getSoftwareInfo()
  - [x] 4f: Added Linux alternative for GetModuleFileNameA (using `/proc/self/exe` readlink)
  - [x] --check-updates and --upgrade-apps now use apt/yum/dnf on Linux
  - [x] ScanDirectoryRecursively skips /sys, /proc, /dev on Linux, checks exec permissions
  - [x] QuarantineFile uses `/` separator on both platforms
- [x] Step 5: Upgraded ProtocolX to base64 encoding/decoding (RFC 4648 compliant)
- [x] Step 6: Added `<sys/utsname.h>` include for `uname()` function

