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
            build = subprocess.check_output("powershell (Get-ItemProperty 'HKLM:\\SOFTWARE\\Microslop\\Windows NT\\CurrentVersion').CurrentBuild", shell=True).decode().strip()
            version = subprocess.check_output("powershell (Get-ItemProperty 'HKLM:\\SOFTWARE\\Microslop\\Windows NT\\CurrentVersion').DisplayVersion", shell=True).decode().strip()
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

def main():
    version = "3.0.0-LTS"

    if len(sys.argv) < 2:
        print("\n[!] No command entered. Try: jenny --help")
        return

    command = sys.argv[1].lower()

    if command == "--hardware":
        detective = HardwareDetective()
        data = detective.get_system_summary()
        cpu_name = get_processor_name()
        print("\n" + "="*40)
        print("    JENNY HARDWARE REPORT")
        print("="*40)
        print(f"MOTHERBOARD : {data['motherboard']}")
        print(f"CPU         : {cpu_name}")
        print(f"RAM         : {data['ram']}")
        print(f"RESOLUTION  : {data['resolution']}")
        print(f"STORAGE     : {data['storage']}")
        for i, gpu in enumerate(data['gpus'], 1):
            print(f"GPU {i}       : {gpu['name']}")
            print(f"DRIVER {i}    : {gpu['driver']}")
        print("="*40 + "\n")

    elif command == "--software":
        info = get_software_info()
        print("\n" + "="*40)
        print("    JENNY SOFTWARE REPORT")
        print("="*40)
        print(f"OS VERSION  : {info}")
        print(f"KERNEL      : {platform.version()}")
        print(f"ARCH        : {platform.machine()}")
        print("="*40 + "\n")

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
        print("\n[*] Scanning for system and app updates...")
        app_data = detective.check_app_updates()
        print("\n[ APPLICATION UPDATES ]\n" + (app_data if app_data.strip() else "All apps up to date."))
        print("\n[ DRIVER STATUS ]")
        for driver in detective.check_driver_updates():
            print(f"- {driver}")
        print("\n[?] Use 'jenny --upgrade-apps' to update everything.")

    elif command == "--upgrade-apps":
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if not is_admin:
            ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
            return
        detective = HardwareDetective()
        confirm = input("\n[?] Do you want to upgrade ALL apps? (-y / -n): ").lower()
        if confirm == "-y":
            detective.upgrade_apps()
        else:
            app_id = input("[*] Enter App ID: ").strip()
            if app_id: detective.upgrade_apps(app_id)

    elif command == "--compile":
        if len(sys.argv) < 3:
            print("\n[!] Specify a file.")
            return
        target_file = os.path.abspath(sys.argv[2])
        output_dir = os.getcwd()
        compilex_dir = r"D:\Tools\CompileX"
        compilex_bat = os.path.join(compilex_dir, "compilex.bat")
        try:
            subprocess.run([compilex_bat, target_file, output_dir], shell=True)
        except Exception as e:
            print(f"\n[!] Compilation error: {e}")

    elif command == "--sentinel":
        target = sys.argv[2] if len(sys.argv) > 2 else os.getcwd()
        quoted_target = f'"{target}"'
        core_path = os.path.join(os.path.dirname(__file__), "SentinelCore.exe")
        if not os.path.exists(core_path):
            print("\n[!] SentinelCore.exe missing.")
            return
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if is_admin:
            subprocess.run([core_path, target])
        else:
            ctypes.windll.shell32.ShellExecuteW(None, "runas", core_path, quoted_target, None, 1)

    elif command == "--sentinel-network":
        core_path = os.path.join(os.path.dirname(__file__), "SentinelCore.exe")
        if not os.path.exists(core_path):
            print("\n[!] SentinelCore.exe missing.")
            return
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if is_admin:
            subprocess.run([core_path, "--network-scan"])
        else:
            ctypes.windll.shell32.ShellExecuteW(None, "runas", core_path, "--network-scan", None, 1)

    elif command == "--restore":
        core_path = os.path.join(os.path.dirname(__file__), "SentinelCore.exe")
        if not os.path.exists(core_path):
            print("\n[!] SentinelCore.exe missing.")
            return
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if is_admin:
            subprocess.run([core_path, "--restore"])
        else:
            ctypes.windll.shell32.ShellExecuteW(None, "runas", core_path, "--restore", None, 1)

    elif command == "--vault":
        vault = JennyVault()
        new_pass = vault.generate_password()
        print("\n" + "-"*30 + f"\nGEN-PASS: {new_pass}\n" + "-"*30)
        choice = input("Save? (-y / -n): ").lower()
        if choice == "-y":
            file_name = input("File name: ").strip()
            if vault.save_password(file_name, new_pass):
                print(f"[+] Saved to {file_name}.txt")

    elif command == "--version":
        print(f"\nJenny AI Engine - Version: {version}\n")

    elif command == "--help":
        print("\nJenny AI CLI - Available Commands:")
        print("-" * 65)
        print(f"{'--hardware':<22} | Display detailed system and hardware report")
        print(f"{'--software':<22} | Display OS version, build and kernel information")
        print(f"{'--px-e':<22} | (ProtocolX) Encode secure message via external binary")
        print(f"{'--px-d':<22} | (ProtocolX) Decode secure message via external binary")
        print(f"{'--check-updates':<22} | Scan for application and driver updates")
        print(f"{'--upgrade-apps':<22} | Upgrade installed applications via Winget")
        print(f"{'--compile':<22} | Compile files using CompileX engine")
        print(f"{'--sentinel':<22} | Scan directory for threats (Sentinel Core)")
        print(f"{'--sentinel-network':<22} | Scan active network connections for untrusted processes")
        print(f"{'--restore':<22} | Restore files from Sentinel quarantine")
        print(f"{'--vault':<22} | Generate and store secure passwords")
        print(f"{'--version':<22} | Show current engine version")
        print(f"{'--help':<22} | Show this help menu")
        print("-" * 65)

if __name__ == "__main__":
    main()
