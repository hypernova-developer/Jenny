# Jenny Toolkit CLI 🌑

Jenny is a lightweight, local-first system assistant and developer utility designed for performance-focused users. It provides deep hardware insights and automates routine system maintenance directly from your terminal.
- Current Version: **5.1.9-LTS**

## ❇️ Release Notes for **Jenny v5.1.9-LTS**:
1. Jenny started using a [configurator file](src/jenny.conf) to access the binaries and files needed to run. Paths belong to me will not be burned into executables starting from [Jenny v5.1.4 Release](https://github.com/hypernova-developer/Jenny/releases/tag/v5.1.4-LTS).
2. This [README.md](README.md) file and [INSTALLATION.md](src/INSTALLATION.md) were edited, cleaned and fixed. Old installation media that belongs to Python era of Jenny is out of [README.md](README.md) now. And configuration settings are at [INSTALLATION.md](src/INSTALLATION.md) now.
3. Added [CONFIGURATION.md](src/CONFIGURATION.md) file to describe the process. 

## 🛠️ Key Features
- **Hardware Detective:** Comprehensive system reports (CPU, GPU, RAM, Storage, and Screen Resolution).
- **Update Manager:** Scan and upgrade applications simultaneously using Winget integration.
- **Security Vault:** Generate and store cryptographically secure passwords locally.
- **Zero Latency:** No cloud, no API delays. Everything runs on your machine.
- **Sentinel Core:** Advanced heuristic and network analysis to detect unsigned processes and suspicious external connections.
- **CompileX Engine:** A high-performance compilation tool optimized for speed and "pure power," designed for developers seeking efficiency.
- **Note**: The CompileX module switched from batch processing to C++ technology.

## 🚀 Installation
Read the [manual](src/INSTALLATION.md) to learn about installation.

## 🛞 Configuration
Read the [manual](src/CONFIGURATION.md) to learn about installation.

## 💻 Usage

| Command | Function |
| :--- | :--- |
| `jenny --hardware` | Display detailed system and hardware report. |
| `jenny --software` | Display OS version, build and kernel information. |
| `jenny --px-e` | (ProtocolX) Encode secure message via external binary. |
| `jenny --px-d` | (ProtocolX) Decode secure message via external binary. |
| `jenny --check-updates` | Scan for application and driver updates. |
| `jenny --upgrade-apps` | Upgrade installed applications via Winget. |
| `jenny --compile` | **(CompileX)** Compile files using the high-performance engine. |
| `jenny --sentinel` | **(Sentinel)** Scan current directory for threats. |
| `jenny --sentinel-network` | **(Sentinel)** Scan active connections for untrusted processes. |
| `jenny --restore` | Restore files from the Sentinel quarantine. |
| `jenny --vault` | Generate and store cryptographically secure passwords. |
| `jenny --help` | Show the full command list. |

## 🏛️ Needed Empty Space
Jenny takes 3.22 megabytes of empty space at [v5.1.8](https://github.com/hypernova-developer/Jenny/releases/tag/v5.1.8-LTS) release in my PC alone itself. It might be different on other devices. Only for Jenny, 6 megabytes of empty space will be enough. 

## 🛡️ License
Protected with **GNU General Public License v3.0**.
Maintained, preserved, coded, kept up and developed by **hypernova-developer**.
