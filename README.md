# Jenny Toolkit CLI

Jenny is a lightweight, local-first system assistant and developer utility designed for performance-focused users. It provides deep hardware insights and automates routine system maintenance directly from the terminal interface.

- **Current Version:** 6.0.0-BETA

---

## Release Notes for Jenny v6.0.0-BETA

**Simplicity**: Jenny will only require a configuration file in the same directory with the main executable file. This change was made to improve the ease of developing another release that will be compatible with Linux operating systems.

---

## Core Features

- **Hardware Diagnostic Engine:** Generates comprehensive hardware utilization reports (CPU, GPU, RAM, Storage, and Display Resolution).
- **Update Management:** Scans and upgrades system applications via native package manager integration (`winget`).
- **Cryptographic Security Vault:** Generates and stores cryptographically sound access credentials locally.
- **Local Execution Architecture:** Designed with zero external API dependencies or cloud latency; all routines execute strictly on the local machine.
- **Sentinel Core:** Performs heuristic process analysis and active network monitoring to identify unsigned binaries and unverified socket connections.
- **CompileX Engine:** High-performance compilation utility built for aggressive execution speed and operational efficiency.
  - *Note:* The CompileX module has been migrated from batch execution scripts to a compiled C++ implementation.

---

## Installation

Refer to the [Installation Manual](docs/INSTALLATION.md) for full deployment instructions.

---

## Configuration

Refer to the [Configuration Guide](docs/CONFIGURATION.md) for system setup and parameter specification.

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

### Jenny v6.0.0-LTS
The binary executables for Jenny on the latest release require approximately **1.25 MB** of local storage space. 
A minimum allocation of **2.50 MB** is recommended to accommodate local logging and configuration caches.

### Jenny v5.1.9-LTS
The binary executables for Jenny on the final supported legacy release require approximately **3.22 MB** of local storage space. 
A minimum allocation of **6.00 MB** is recommended to accommodate local logging and configuration caches.

---

## License

This software is distributed under the **GNU General Public License v3.0**.  
Maintained and engineered by **hypernova-developer**.
