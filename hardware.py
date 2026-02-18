import wmi
import psutil
import platform
import tkinter
import subprocess

class HardwareDetective:
    def __init__(self):
        self.w = wmi.WMI()

    def get_system_summary(self):
        info = {}
        for board in self.w.Win32_BaseBoard():
            info["motherboard"] = f"{board.Manufacturer} {board.Product}"
        info["cpu"] = platform.processor()
        total_ram = round(psutil.virtual_memory().total / (1024**3), 2)
        info["ram"] = f"{total_ram} GB"
        root = tkinter.Tk()
        width = root.winfo_screenwidth()
        height = root.winfo_screenheight()
        info["resolution"] = f"{width}x{height}"
        root.destroy()
        gpus = []
        for gpu in self.w.Win32_VideoController():
            gpus.append({
                "name": gpu.Name,
                "driver": gpu.DriverVersion
            })
        info["gpus"] = gpus
        storage_info = []
        for disk in self.w.Win32_LogicalDisk(DriveType=3):
            total_space = round(int(disk.Size) / (1024**3), 2)
            storage_info.append(f"{disk.DeviceID} ({total_space} GB)")
        info["storage"] = " | ".join(storage_info)
        return info

    def check_app_updates(self):
        try:
            result = subprocess.run(['winget', 'upgrade'], capture_output=True, text=True, encoding='utf-8')
            return result.stdout
        except Exception:
            return "Winget could not be accessed."

    def upgrade_apps(self, app_id=None):
        try:
            command = ['winget', 'upgrade']
            if app_id:
                command.extend(['--id', app_id])
            else:
                command.append('--all')
            
            subprocess.run(command)
            return True
        except Exception:
            return False

    def check_driver_updates(self):
        updates = []
        for gpu in self.w.Win32_VideoController():
            updates.append(f"{gpu.Name}: Current v{gpu.DriverVersion} (Check manufacturer site for updates)")
        return updates
