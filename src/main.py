import sys
import os
import subprocess
import ctypes
import platform
from hardware import HardwareDetective
from security import JennyVault

def get_processor_name():
    try:
        command = "wmic cpu get name"
        name = subprocess.check_output(command, shell=True).decode().strip().split('\n')[1]
        return name
    except:
        return "Unknown Processor"

def get_software_info():
    system = platform.system()
    if system == "Windows":
        try:
            build = subprocess.check_output("powershell (Get-ItemProperty 'HKLM:\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion').CurrentBuild", shell=True).decode().strip()
            version = subprocess.check_output("powershell (Get-ItemProperty 'HKLM:\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion').DisplayVersion", shell=True).decode().strip()
            return f"Windows {platform.release()} {version} (Build {build})"
        except:
            return f"Windows {platform.release()}"
    elif system == "Linux":
        try:
            with open("/etc/os-release") as f:
                lines = f.readlines()
                for line in lines:
                    if line.startswith("PRETTY_NAME"):
                        return line.split("=")[1].strip().replace('"', '')
            return "GNU/Linux"
        except:
            return "GNU/Linux"
    return f"{system} {platform.release()}"

def run_protocolx(mode, input_text):
    px_path = os.path.join(os.path.dirname(__file__), "ProtocolX.exe")
    if not os.path.exists(px_path):
        print("\n[!] ProtocolX.exe missing.")
        return
    try:
        result = subprocess.run([px_path, mode, input_text], capture_output=True, text=True, shell=True)
        print(f"\n{result.stdout.strip()}")
    except Exception as e:
        print(f"\n[!] ProtocolX execution error: {e}")

def run_sentinel(mode_arg=None, target_path=None):
    sentinel_path = os.path.join(os.path.dirname(__file__), "SentinelCore.exe")
    if not os.path.exists(sentinel_path):
        print("\n[!] SentinelCore.exe (C++ Engine) missing.")
        return
    cmd = [sentinel_path]
    if mode_arg: cmd.append(mode_arg)
    elif target_path: cmd.append(target_path)
    try:
        subprocess.run(cmd, shell=True)
    except Exception as e:
        print(f"\n[!] Sentinel execution error: {e}")

def main():
    version = "v4.1.1-LTS"

    if len(sys.argv) < 2:
        print(f"\n[!] Jenny {version} - No command entered. Try: jenny --help")
        return

    command = sys.argv[1].lower()

    if command == "--hardware":
        detective = HardwareDetective()
        data = detective.get_system_summary()
        cpu_name = get_processor_name()
        print("\n" + "="*45)
        print("      JENNY HARDWARE REPORT")
        print("="*45)
        print(f"MOTHERBOARD : {data['motherboard']}")
        print(f"CPU         : {cpu_name}")
        print(f"RAM         : {data['ram']}")
        print(f"RESOLUTION  : {data['resolution']}")
        print(f"STORAGE     : {data['storage']}")
        for i, gpu in enumerate(data['gpus'], 1):
            print(f"GPU {i}       : {gpu['name']}")
            print(f"DRIVER {i}    : {gpu['driver']}")
        print("="*45 + "\n")

    elif command == "--software":
        info = get_software_info()
        print("\n" + "="*45)
        print("      JENNY SOFTWARE REPORT")
        print("="*45)
        print(f"OS VERSION  : {info}")
        print(f"KERNEL      : {platform.version()}")
        print(f"ARCH        : {platform.machine()}")
        print("="*45 + "\n")

    elif command == "--px-e":
        if len(sys.argv) < 3:
            print("\n[!] Message required.")
            return
        run_protocolx("--encode", sys.argv[2])

    elif command == "--px-d":
        if len(sys.argv) < 3:
            print("\n[!] Encoded code required.")
            return
        run_protocolx("--decode", sys.argv[2])

    elif command == "--check-updates":
        detective = HardwareDetective()
        print("\n[*] Scanning for application and driver updates...")
        app_data = detective.check_app_updates()
        print("\n[ APPLICATION UPDATES ]\n" + (app_data if app_data.strip() else "All apps up to date."))
        print("\n[ DRIVER STATUS ]")
        for driver in detective.check_driver_updates():
            print(f"- {driver}")

    elif command == "--upgrade-apps":
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if not is_admin:
            ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
            return
        detective = HardwareDetective()
        confirm = input("\n[?] Upgrade ALL applications via Winget? (-y / -n): ").lower()
        if confirm == "-y":
            detective.upgrade_apps()
        else:
            app_id = input("[*] Enter App ID to upgrade: ").strip()
            if app_id: detective.upgrade_apps(app_id)

    elif command == "--compile":
        if len(sys.argv) < 3:
            print("\n[!] Specify a file to compile.")
            return
        target_file = os.path.abspath(sys.argv[2])
        current_dir = os.path.dirname(os.path.abspath(__file__))
        compilex_exe = os.path.join(current_dir, "compilex_engine.exe")
        if not os.path.exists(compilex_exe):
            print(f"\n[!] CompileX Engine missing at: {compilex_exe}")
            return
        print(f"[*] Summoning CompileX Engine v3.0-LTS...")
        try:
            subprocess.run([compilex_exe, target_file], shell=True)
        except Exception as e:
            print(f"\n[!] Compilation error: {e}")

    elif command == "--sentinel":
        target = sys.argv[2] if len(sys.argv) > 2 else os.getcwd()
        run_sentinel(target_path=target)

    elif command == "--sentinel-network":
        run_sentinel(mode_arg="--network-scan")

    elif command == "--restore":
        run_sentinel(mode_arg="--restore")

    elif command == "--vault":
        vault = JennyVault()
        print("\n[ JENNY VAULT SECURITY ]")
        action = input("[?] Generate password or view stored? (gen/view): ").lower()
        if action == "gen":
            site = input("Site Name: ")
            vault.generate_and_store(site)
        elif action == "view":
            vault.display_vault()

    elif command == "--version":
        print(f"\nJenny AI Engine - Version: {version} (Raylib Edition)\n")

    elif command == "--help":
        print(f"\nJenny AI CLI {version} - Available Commands:")
        print("-" * 75)
        print(f"{'--hardware':<22} | Display detailed system and hardware report")
        print(f"{'--software':<22} | Display OS version, build and kernel information")
        print(f"{'--px-e':<22} | (ProtocolX) Encode secure message via external binary")
        print(f"{'--px-d':<22} | (ProtocolX) Decode secure message via external binary")
        print(f"{'--check-updates':<22} | Scan for application and driver updates")
        print(f"{'--upgrade-apps':<22} | Upgrade installed applications via Winget")
        print(f"{'--compile':<22} | (CompileX) Compile files using the high-performance engine")
        print(f"{'--sentinel':<22} | (SyntaX) Scan current directory for threats")
        print(f"{'--sentinel-network':<22} | (SyntaX) Scan active connections for untrusted processes")
        print(f"{'--restore':<22} | Restore files from the Sentinel quarantine")
        print(f"{'--vault':<22} | Generate and store cryptographically secure passwords")
        print(f"{'--version':<22} | Show current engine version")
        print(f"{'--help':<22} | Show the full command list")
        print("-" * 75)

if __name__ == "__main__":
    main()
