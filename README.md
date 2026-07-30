# Jenny Toolkit CLI

Jenny is a lightweight, local-first system assistant and developer utility designed for performance-focused users. It provides deep hardware insights and automates routine system maintenance directly from the terminal interface.

- **Current Version:** 7.0.0-LTS

---

## Release Notes for Jenny v7.0.0-LTS

**Compatibility**: Jenny is compatible with Linux distributions starting from this release. The features on the Linux side will be less problematic in time with the next long-term support release. 

---

## Core Features

- **Hardware Diagnostic Engine:** Generates comprehensive hardware utilization reports (CPU, GPU, RAM, Storage, and Display Resolution).
- **Update Management:** Scans and upgrades system applications via native package manager integration (`winget`).
- **Cryptographic Security Vault:** Generates and stores cryptographically sound access credentials locally.
- **Local Execution Architecture:** Designed with zero external API dependencies or cloud latency; all routines execute strictly on the local machine.
- **Sentinel Core:** Performs heuristic process analysis and active network monitoring to identify unsigned binaries and unverified socket connections.
- **CompileX Engine:** High-performance compilation utility built for aggressive execution speed and operational efficiency.
  - *Note:* The CompileX module has been migrated from batch execution scripts to a compiled C++ implementation.
  - *NOTE:* The CompileX module used in Jenny Toolkit CLI project is a different edition from the original one. The module from this project has the ability to be configured out of the code and be changed without being compiled again.

---

## Installation

Refer to the [Installation Manual for Linux](docs/INSTALLATION-ON-LINUX.md) [Installation Manual for Windows](docs/INSTALLATION-ON-WINDOWS.md) for full deployment instructions.

---

## Configuration

Refer to the [Configuration Guide](docs/CONFIGURATION.md) for system setup and parameter specification.

---

## Information About Releases
Refer to the [Releases Support Policy](docs/RELEASES.md) for information for all of the releases.

---

## Information About the Roadmap of Jenny Toolkit CLI Project
Refer to the [Roadmap Plan](docs/ROADMAP.md) for information for all the soon releases.

---

## Command Reference

| Command | Function |
| :--- | :--- |
| `jenny --hardware` | Displays detailed system and hardware diagnostic metrics. |
| `jenny --software` | Displays OS build, versioning, and kernel metadata. |
| `jenny --px-e` | (ProtocolX) Encodes secure payloads using external binaries. |
| `jenny --px-d` | (ProtocolX) Decodes secure payloads using external binaries. |
| `jenny --check-updates` | Scans for application and driver updates. |
| `jenny --upgrade-apps` | Upgrades installed software packages via Winget. |
| `jenny --compile` | Executes the high-performance CompileX build system. |
| `jenny --sentinel` | Runs Sentinel heuristic scans on the current working directory. |
| `jenny --sentinel-network` | Scans active network connections for unverified process sockets. |
| `jenny --restore` | Restores isolated files from the Sentinel quarantine vault. |
| `jenny --vault` | Generates and manages encrypted local credential stores. |
| `jenny --help` | Displays full CLI option documentation. |

---

## System Storage Requirements

### Jenny v7.0.0-LTS
The Windows binary executable for Jenny on the latest release require approximately **1.24 MB** of local storage space.
A minimum allocation of **2.50 MB** is recommended to accommodate local logging and configuration caches.

### Jenny v6.0.0-LTS
The Windows binary executable for Jenny on the latest release require approximately **1.25 MB** of local storage space.
A minimum allocation of **2.50 MB** is recommended to accommodate local logging and configuration caches.

### Jenny v5.1.9-LTS
The Windows binary executable(s) for Jenny on the final supported legacy release require approximately **3.22 MB** of local storage space.
A minimum allocation of **6.00 MB** is recommended to accommodate local logging and configuration caches.

---

## License

This software is distributed under the **GNU General Public License v3.0**.  
Maintained and engineered by **hypernova-developer**.
