#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "HardwareDetective.hpp"
#include "JennyVault.hpp"

namespace fs = std::filesystem;

struct Config
{
    std::string gcc;
    std::string gxx;
    std::string gfortran;
    std::string csc;
    std::string python;
    std::string syntax_inc;
    std::string raylib_inc;
    std::string raylib_lib;
    std::string quarantine_dir;
};

Config loadConfig()
{
    Config cfg;
    std::ifstream file("jenny.conf");
    if (file.is_open())
    {
        std::getline(file, cfg.gcc);
        std::getline(file, cfg.gxx);
        std::getline(file, cfg.gfortran);
        std::getline(file, cfg.csc);
        std::getline(file, cfg.python);
        std::getline(file, cfg.syntax_inc);
        std::getline(file, cfg.raylib_inc);
        std::getline(file, cfg.raylib_lib);
        std::getline(file, cfg.quarantine_dir);
        file.close();
    }
    return cfg;
}

std::string getProcessorName()
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen("wmic cpu get name", "r");
    if (!pipe) return "Unknown Processor";
    while (fgets(buffer, 128, pipe) != NULL) result += buffer;
    _pclose(pipe);
    
    size_t pos = result.find('\n');
    if (pos != std::string::npos)
    {
        std::string line2 = result.substr(pos + 1);
        line2.erase(std::remove(line2.begin(), line2.end(), '\r'), line2.end());
        line2.erase(std::remove(line2.begin(), line2.end(), '\n'), line2.end());
        return line2;
    }
    return "Unknown Processor";
}

std::string getSoftwareInfo()
{
    char buffer[128];
    std::string osName = "Unknown Windows";
    
    FILE* pipe = _popen("wmic os get Caption", "r");
    if (pipe)
    {
        std::string wmicOutput = "";
        while (fgets(buffer, 128, pipe) != NULL) wmicOutput += buffer;
        _pclose(pipe);
        
        size_t pos = wmicOutput.find('\n');
        if (pos != std::string::npos)
        {
            osName = wmicOutput.substr(pos + 1);
        }
    }
    
    osName.erase(std::remove(osName.begin(), osName.end(), '\n'), osName.end());
    osName.erase(std::remove(osName.begin(), osName.end(), '\r'), osName.end());
    while(!osName.empty() && osName.back() == ' ') osName.pop_back();

    std::string build = "Unknown";
    FILE* pipeBuild = _popen("powershell (Get-ItemProperty 'HKLM:\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion').CurrentBuild", "r");
    if (pipeBuild)
    {
        if (fgets(buffer, 128, pipeBuild) != NULL) build = buffer;
        _pclose(pipeBuild);
    }
    
    build.erase(std::remove(build.begin(), build.end(), '\n'), build.end());
    build.erase(std::remove(build.begin(), build.end(), '\r'), build.end());

    return osName + " (Build " + build + ")";
}

void runExternal(std::string exeName, std::string args)
{
    std::string fullCmd = exeName + " " + args;
    system(fullCmd.c_str());
}

int main(int argc, char* argv[])
{
    std::string version = "v5.1.9-LTS";
    Config cfg = loadConfig();

    if (argc < 2)
    {
        std::cout << "\n[!] Jenny " << version << " - No command entered. Try: jenny --help" << std::endl;
        return 0;
    }

    std::string command = argv[1];

    if (command == "--hardware")
    {
        HardwareDetective detective;
        std::cout << "\n" << std::string(45, '=') << std::endl;
        std::cout << "      JENNY HARDWARE REPORT" << std::endl;
        std::cout << std::string(45, '=') << std::endl;
        std::cout << "MOTHERBOARD : " << detective.getMotherboard() << std::endl;
        std::cout << "CPU         : " << getProcessorName() << std::endl;
        std::cout << "RAM         : " << detective.getRAM() << std::endl;
        std::cout << "RESOLUTION  : " << detective.getResolution() << std::endl;
        std::cout << "STORAGE     : " << detective.getStorage() << std::endl;
        auto gpus = detective.getGPUs();
        for (size_t i = 0; i < gpus.size(); ++i)
        {
            std::cout << "GPU " << i + 1 << "       : " << gpus[i].name << std::endl;
            std::cout << "DRIVER " << i + 1 << "    : " << gpus[i].driver << std::endl;
        }
        std::cout << std::string(45, '=') << "\n" << std::endl;
    }
    else if (command == "--software")
    {
        std::cout << "\n" << std::string(45, '=') << std::endl;
        std::cout << "      JENNY SOFTWARE REPORT" << std::endl;
        std::cout << std::string(45, '=') << std::endl;
        std::cout << "OS VERSION  : " << getSoftwareInfo() << std::endl;
        std::cout << std::string(45, '=') << "\n" << std::endl;
    }
    else if (command == "--px-e" && argc > 2)
    {
        runExternal("ProtocolX.exe", "--encode " + std::string(argv[2]));
    }
    else if (command == "--px-d" && argc > 2)
    {
        runExternal("ProtocolX.exe", "--decode " + std::string(argv[2]));
    }
    else if (command == "--check-updates")
    {
        std::cout << "\n[*] Scanning for updates via Winget..." << std::endl;
        system("winget upgrade");
    }
    else if (command == "--upgrade-apps")
    {
        if (argc > 2) system(("winget upgrade --id " + std::string(argv[2])).c_str());
        else system("winget upgrade --all");
    }
    else if (command == "--compile" && argc > 2)
    {
        runExternal("compilex_engine.exe", argv[2]);
    }
    else if (command == "--sentinel")
    {
        std::string target = (argc > 2) ? argv[2] : ".";
        runExternal("SentinelCore.exe", target);
    }
    else if (command == "--sentinel-network")
    {
        runExternal("SentinelCore.exe", "--network-scan");
    }
    else if (command == "--restore")
    {
        runExternal("SentinelCore.exe", "--restore");
    }
    else if (command == "--vault")
    {
        JennyVault vault;
        std::string action;
        std::cout << "\n[ JENNY VAULT SECURITY ]\n[?] gen / view: ";
        std::cin >> action;
        if (action == "gen")
        {
            std::string site;
            std::cout << "Site Name: ";
            std::cin >> site;
            std::string pwd = vault.generatePassword();
            vault.savePassword(site, pwd);
            std::cout << "[+] Stored: " << pwd << std::endl;
        }
    }
    else if (command == "--help")
    {
        std::cout << "\nJenny AI CLI " << version << " - Help Menu\n" << std::string(75, '-') << std::endl;
        printf("%-22s | %s\n", "--hardware", "Display detailed system and hardware report");
        printf("%-22s | %s\n", "--software", "Display OS version, build and kernel information");
        printf("%-22s | %s\n", "--px-e", "(ProtocolX) Encode secure message");
        printf("%-22s | %s\n", "--px-d", "(ProtocolX) Decode secure message");
        printf("%-22s | %s\n", "--check-updates", "Scan for updates");
        printf("%-22s | %s\n", "--upgrade-apps", "Upgrade via Winget");
        printf("%-22s | %s\n", "--compile", "(CompileX) Compile files");
        printf("%-22s | %s\n", "--sentinel", "(Sentinel) Threat scan");
        printf("%-22s | %s\n", "--sentinel-network", "(Sentinel) Network scan");
        printf("%-22s | %s\n", "--vault", "Password manager");
        std::cout << std::string(75, '-') << std::endl;
    }

    return 0;
}
