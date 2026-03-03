# Jenny AI CLI 🌑🚀

Jenny is a lightweight, local-first system assistant and developer utility designed for performance-focused users. It provides deep hardware insights and automates routine system maintenance directly from your terminal.
- Current Version: 2.0.0-LTS

## 🛠️ Key Features
- **Hardware Detective:** Comprehensive system reports (CPU, GPU, RAM, Storage, and Screen Resolution).
- **Update Manager:** Scan and upgrade applications simultaneously using Winget integration.
- **Security Vault:** Generate and store cryptographically secure passwords locally.
- **Zero Latency:** No cloud, no API delays. Everything runs on your machine.
- **SyntaX (Sentinel Core):** Advanced heuristic and network analysis to detect unsigned processes and suspicious external connections.
- **CompileX Engine:** A high-performance compilation tool optimized for speed and "pure power," designed for developers seeking efficiency.

## 🚀 Installation

1. Clone the repository:
[https://github.com/hypernova-developer/Jenny.git](https://github.com/hypernova-developer/Jenny.git)

2. Install dependencies:
```bash
pip install wmi psutil
```

3. Add the `jenny.bat` path to your system environment variables.

## 💻 Usage

| Command | Function |
| :--- | :--- |
| `jenny --hardware` | Display detailed system and hardware report. |
| `jenny --check-updates` | Scan for application and driver updates. |
| `jenny --upgrade-apps` | Upgrade installed applications via Winget. |
| `jenny --compile` | **(CompileX)** Compile files using the high-performance engine. |
| `jenny --sentinel` | **(SyntaX)** Scan current directory for threats. |
| `jenny --sentinel-network` | **(SyntaX)** Scan active connections for untrusted processes. |
| `jenny --restore` | Restore files from the Sentinel quarantine. |
| `jenny --vault` | Generate and store cryptographically secure passwords. |
| `jenny --version` | Show current engine version. |
| `jenny --help` | Show the full command list. |

## 🛡️ License
Protected with **GNU General Public License v3.0**.
Developed by **hypernova-developer**.
