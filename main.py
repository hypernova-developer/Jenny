import sys
import os
import subprocess
from hardware import HardwareDetective
from security import JennyVault

def main():
    version = "1.1.5-Beta"

    if len(sys.argv) < 2:
        print("\n[!] No command entered. Try: jenny --help")
        return

    command = sys.argv[1].lower()

    if command == "--hardware":
        detective = HardwareDetective()
        data = detective.get_system_summary()
        print("\n" + "="*40)
        print("   JENNY HARDWARE REPORT")
        print("="*40)
        print(f"MOTHERBOARD : {data['motherboard']}")
        print(f"CPU         : {data['cpu']}")
        print(f"RAM         : {data['ram']}")
        print(f"RESOLUTION  : {data['resolution']}")
        print(f"STORAGE     : {data['storage']}")
        for i, gpu in enumerate(data['gpus'], 1):
            print(f"GPU {i}       : {gpu['name']}")
            print(f"DRIVER {i}    : {gpu['driver']}")
        print("="*40 + "\n")

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
        detective = HardwareDetective()
        confirm = input("\n[?] Do you want to upgrade ALL apps? (-y / -n): ").lower()
        if confirm == "-y":
            print("[*] Starting upgrades... This may take a while.")
            detective.upgrade_apps()
            print("[+] Upgrade process completed.")
        else:
            app_id = input("[*] Enter specific App ID to upgrade (or leave blank to cancel): ").strip()
            if app_id:
                detective.upgrade_apps(app_id)
                print(f"[+] Process for {app_id} finished.")

    elif command == "--compile":
        if len(sys.argv) < 3:
            print("\n[!] Error: Please specify a file to compile.")
            return
        
        target_file = os.path.abspath(sys.argv[2])
        compilex_dir = r"C:\Tools\CompileX"
        compilex_bat = os.path.join(compilex_dir, "compilex.bat")
        
        if not os.path.exists(compilex_bat):
            print(f"\n[!] Error: CompileX not found at {compilex_dir}")
            return

        current_dir = os.getcwd()
        try:
            print("\n[*] Initializing CompileX Engine...")
            os.chdir(compilex_dir)
            subprocess.run([compilex_bat, target_file], shell=True)
        finally:
            os.chdir(current_dir)

    elif command == "--vault":
        vault = JennyVault()
        new_pass = vault.generate_password()
        print("\n" + "-"*30 + f"\nGEN-PASS: {new_pass}\n" + "-"*30)
        choice = input("Save this password? (-y / -n): ").lower()
        if choice == "-y":
            file_name = input("Enter file name: ").strip().replace('"', '')
            if " " in file_name:
                print("[!] Error: No spaces allowed!")
            elif vault.save_password(file_name, new_pass):
                print(f"[+] Saved to {file_name}.txt")

    elif command == "--version":
        print(f"\nJenny AI Engine - Version: {version}\nDeveloped by hypernova-developer\n")

    elif command == "--help":
        print("\nJenny AI CLI:")
        print("--hardware      : System components report.")
        print("--check-updates : List available updates.")
        print("--upgrade-apps  : Install app updates.")
        print("--compile       : Compile code using CompileX.")
        print("--vault         : Secure password tools.")
        print("--version       : Version info.")
        print("--help          : Command list.")

if __name__ == "__main__":
    main()
