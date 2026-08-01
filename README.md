# JTK

Jenny is a lightweight, local-first system assistant and developer utility designed for performance-focused users. It provides deep hardware insights and automates routine system maintenance directly from the terminal interface.

- **Current Version:** 7.3.5-LTS

---

## Release Notes for JTK v7.3.5-LTS

**Compatibility**: The CompileX module had such issues like the hard-coded config file path being wrong and Raylib being a must on Linux side. These were fixed on this release.
**Final Dot Mark**: JTK v7.3.5-LTS is planned to be the last release having the tag `v7`.

---

## Core Features

- **Hardware Diagnostic Engine:** Generates comprehensive hardware utilization reports (CPU, GPU, RAM, Storage, and Display Resolution).
- **Cryptographic Security Vault:** Generates and stores cryptographically sound access credentials locally.
- **Local Execution Architecture:** Designed with zero external API dependencies or cloud latency; all routines execute strictly on the local machine.
- **Sentinel Core:** Performs heuristic process analysis and active network monitoring to identify unsigned binaries and unverified socket connections.
- **CompileX Engine:** High-performance compilation utility built for aggressive execution speed and operational efficiency.
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
| `jenny --compile` | Executes the high-performance CompileX build system. |
| `jenny --sentinel` | Runs Sentinel heuristic scans on the current working directory. |
| `jenny --sentinel-network` | Scans active network connections for unverified process sockets. |
| `jenny --vault` | Generates and manages encrypted local credential stores. |
| `jenny --help` | Displays full CLI option documentation. |

---

## System Storage Requirements

### JTK v7.3.5-LTS
The Linux binary executable for Jenny on the latest release require approximately **133,7 KB** of local storage space.
A minimum allocation of **2.50 MB** is recommended to accommodate local logging and configuration caches.

### Jenny Toolkit CLI v6.0.0-LTS
The Windows binary executable for Jenny on this release require approximately **1.25 MB** of local storage space.
A minimum allocation of **2.50 MB** is recommended to accommodate local logging and configuration caches.

---

## License

This software is distributed under the **GNU General Public License v3.0**.  
Maintained and engineered by **hypernova-developer**.
