#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <comutil.h>
#include <WbemIdl.h>
#include <softpub.h>
#include <wintrust.h>
#include <wincrypt.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <bcrypt.h>

#pragma comment(lib, "wintrust.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "bcrypt.lib")

namespace fs = std::filesystem;

namespace JennyModules
{
    struct GPUInfo
    {
        std::string name;
        std::string driver;
    };

    class HardwareDetective
    {
    public:
        std::string getMotherboard()
        {
            return queryWMI(L"Win32_BaseBoard", L"Manufacturer") + " " + queryWMI(L"Win32_BaseBoard", L"Product");
        }

        std::string getRAM()
        {
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);
            GlobalMemoryStatusEx(&memInfo);
            double totalRAM = (double)memInfo.ullTotalPhys / (1024 * 1024 * 1024);
            return std::to_string(totalRAM).substr(0, 5) + " GB";
        }

        std::string getResolution()
        {
            int width = GetSystemMetrics(SM_CXSCREEN);
            int height = GetSystemMetrics(SM_CYSCREEN);
            return std::to_string(width) + "x" + std::to_string(height);
        }

        std::vector<GPUInfo> getGPUs()
        {
            std::vector<GPUInfo> gpus;
            gpus.push_back({queryWMI(L"Win32_VideoController", L"Name"), queryWMI(L"Win32_VideoController", L"DriverVersion")});
            return gpus;
        }

        std::string getStorage()
        {
            DWORD dwSize = GetLogicalDriveStringsA(0, NULL);
            std::vector<char> vChars(dwSize);
            GetLogicalDriveStringsA(dwSize, vChars.data());
            std::string storageStr = "";
            for (char* pDrive = vChars.data(); *pDrive; pDrive += strlen(pDrive) + 1)
            {
                ULARGE_INTEGER freeBytes, totalBytes;
                if (GetDiskFreeSpaceExA(pDrive, &freeBytes, &totalBytes, NULL))
                {
                    double gb = (double)totalBytes.QuadPart / (1024 * 1024 * 1024);
                    storageStr += std::string(pDrive) + " (" + std::to_string(gb).substr(0, 5) + " GB) | ";
                }
            }
            return storageStr;
        }

    private:
        std::string bstrToString(BSTR bstr)
        {
            if (!bstr) return "";
            int len = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, NULL, 0, NULL, NULL);
            std::string str(len, 0);
            WideCharToMultiByte(CP_UTF8, 0, bstr, -1, &str[0], len, NULL, NULL);
            return str.c_str();
        }

        std::string queryWMI(const std::wstring& table, const std::wstring& field)
        {
            HRESULT hres;
            hres = CoInitializeEx(0, COINIT_MULTITHREADED);
            hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
            
            IWbemLocator* pLoc = NULL;
            hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
            
            IWbemServices* pSvc = NULL;
            BSTR resource = SysAllocString(L"ROOT\\CIMV2");
            hres = pLoc->ConnectServer(resource, NULL, NULL, 0, 0, 0, 0, &pSvc);
            SysFreeString(resource);
            
            IEnumWbemClassObject* pEnumerator = NULL;
            std::wstring query = L"SELECT " + field + L" FROM " + table;
            BSTR wql = SysAllocString(L"WQL");
            BSTR q = SysAllocString(query.c_str());
            hres = pSvc->ExecQuery(wql, q, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
            SysFreeString(wql);
            SysFreeString(q);
            
            IWbemClassObject* pclsObj = NULL;
            ULONG uReturn = 0;
            std::string result = "Unknown";
            if (pEnumerator)
            {
                hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                if (uReturn != 0)
                {
                    VARIANT vtProp;
                    pclsObj->Get(field.c_str(), 0, &vtProp, 0, 0);
                    if (vtProp.vt == VT_BSTR) result = bstrToString(vtProp.bstrVal);
                    VariantClear(&vtProp);
                    pclsObj->Release();
                }
            }

            if (pSvc) pSvc->Release();
            if (pLoc) pLoc->Release();
            if (pEnumerator) pEnumerator->Release();
            CoUninitialize();
            return result;
        }
    };

    class JennyVault
    {
    public:
        std::string generatePassword(int length = 16)
        {
            const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}|;:,.<>?";
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<> distribution(0, characters.size() - 1);

            std::string password = "";
            for (int i = 0; i < length; ++i)
            {
                password += characters[distribution(generator)];
            }
            return password;
        }

        bool savePassword(std::string filename, std::string password)
        {
            std::ofstream file(filename + ".txt");
            if (file.is_open())
            {
                file << "Password: " << password;
                file.close();
                return true;
            }
            return false;
        }
    };

    class ProtocolX
    {
    public:
        std::string encode(std::string t)
        {
            std::string r = "";
            for (unsigned char c : t)
            {
                std::string s = std::to_string((int)c);
                while (s.length() < 4) s += 'X';
                r += s;
            }
            return r;
        }

        std::string decode(std::string d)
        {
            std::string r = "";
            for (size_t i = 0; i < d.length(); i += 4)
            {
                std::string b = d.substr(i, 4);
                std::string n = "";
                for (char c : b) if (isdigit(c)) n += c;
                if (!n.empty()) r += (char)std::stoi(n);
            }
            return r;
        }
    };

    namespace Sentinel
    {
        struct ThreatInfo
        {
            int Score;
            std::vector<std::string> Reasons;
        };

        inline std::string quarantineDir = "";
        inline std::string currentSelfHash = "";
        inline std::map<std::string, ThreatInfo> detectedThreats;

        inline std::vector<std::string> criticalServices =
        {
            "wpcmonsvc", "svchost", "lsass", "services", "wininit",
            "csrss", "smss", "winlogon", "taskhostw", "spoolsv"
        };

        inline std::vector<std::string> developerWorkspaces =
        {
            "D:\\Tools", "D:\\mingw64", "C:\\msys64", "D:\\Developing"
        };

        inline std::vector<std::string> driverWhitelist =
        {
            "pusat k3", "hid.exe", "mouseconfig", "keyboard driver", "peripheral"
        };

        std::string GetFileHash(std::string filename);
        bool IsFileSigned(std::string path);
        bool IsHardwareDriver(std::string path);
        void ReportAndHandleThreats();
        void ScanDirectoryRecursively(std::string root);
        int AnalyzeFile(std::string fullPath, std::vector<std::string>& reasons);
        bool CheckStartupStatus(std::string path);
        bool IsSimilar(std::string s1, std::string s2);
        void QuarantineFile(std::string sourcePath);
        void RestoreQuarantine();
        void ScanNetworkActivity();

        inline void loadQuarantineConfig()
        {
            std::ifstream file("jenny.conf");
            if (file.is_open())
            {
                std::string dummy;
                for (int i = 0; i < 8; ++i)
                {
                    std::getline(file, dummy);
                }
                std::getline(file, quarantineDir);
                file.close();
            }
            if (quarantineDir.empty())
            {
                quarantineDir = "C:\\Tools\\Jenny\\Quarantine";
            }
        }

        inline std::string GetFileHash(std::string filename)
        {
            BCRYPT_ALG_HANDLE hAlg = NULL;
            BCRYPT_HASH_HANDLE hHash = NULL;
            NTSTATUS status = 0;
            DWORD cbData = 0, cbHash = 0, cbHashObject = 0;
            PBYTE pbHashObject = NULL;
            PBYTE pbHash = NULL;

            std::ifstream file(filename, std::ios::binary);
            if (!file) return "error";

            status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0);
            status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);
            pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
            status = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
            pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
            status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0);

            char buffer[4096];
            while (file.read(buffer, sizeof(buffer)))
            {
                BCryptHashData(hHash, (PBYTE)buffer, (ULONG)file.gcount(), 0);
            }
            BCryptHashData(hHash, (PBYTE)buffer, (ULONG)file.gcount(), 0);

            BCryptFinishHash(hHash, pbHash, cbHash, 0);

            std::string hashStr = "";
            for (DWORD i = 0; i < cbHash; i++)
            {
                char hex[3];
                sprintf(hex, "%02x", pbHash[i]);
                hashStr += hex;
            }

            if (hHash) BCryptDestroyHash(hHash);
            if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
            if (pbHashObject) HeapFree(GetProcessHeap(), 0, pbHashObject);
            if (pbHash) HeapFree(GetProcessHeap(), 0, pbHash);

            return hashStr;
        }

        inline bool IsFileSigned(std::string path)
        {
            std::wstring wpath(path.begin(), path.end());
            WINTRUST_FILE_INFO fileInfo = { 0 };
            fileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
            fileInfo.pcwszFilePath = wpath.c_str();

            GUID actionID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
            WINTRUST_DATA wtData = { 0 };
            wtData.cbStruct = sizeof(WINTRUST_DATA);
            wtData.dwUIChoice = WTD_UI_NONE;
            wtData.fdwRevocationChecks = WTD_REVOKE_NONE;
            wtData.dwUnionChoice = WTD_CHOICE_FILE;
            wtData.pFile = &fileInfo;
            wtData.dwStateAction = WTD_STATEACTION_IGNORE;
            wtData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;

            LONG result = WinVerifyTrust(NULL, &actionID, &wtData);
            return result == ERROR_SUCCESS;
        }

        inline bool IsHardwareDriver(std::string path)
        {
            std::string fileName = fs::path(path).filename().string();
            std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
            std::string pathLower = path;
            std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);

            for (const auto& d : driverWhitelist)
            {
                if (fileName.find(d) != std::string::npos || pathLower.find("\\pusat\\") != std::string::npos || pathLower.find("\\peripheral\\") != std::string::npos)
                {
                    return true;
                }
            }
            return false;
        }

        inline bool CheckStartupStatus(std::string path)
        {
            auto checkRegistry = [&](HKEY hRoot, const char* subKey)
            {
                HKEY hKey;
                if (RegOpenKeyExA(hRoot, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
                {
                    char valueName[1024];
                    DWORD valueNameLen = 1024;
                    char data[1024];
                    DWORD dataLen = 1024;
                    DWORD i = 0;
                    while (RegEnumValueA(hKey, i++, valueName, &valueNameLen, NULL, NULL, (LPBYTE)data, &dataLen) == ERROR_SUCCESS)
                    {
                        std::string sData = data;
                        std::transform(sData.begin(), sData.end(), sData.begin(), ::tolower);
                        std::string sPath = path;
                        std::transform(sPath.begin(), sPath.end(), sPath.begin(), ::tolower);
                        if (sData.find(sPath) != std::string::npos)
                        {
                            RegCloseKey(hKey);
                            return true;
                        }
                        valueNameLen = 1024;
                        dataLen = 1024;
                    }
                    RegCloseKey(hKey);
                }
                return false;
            };
            return checkRegistry(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run") ||
                   checkRegistry(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        }

        inline bool IsSimilar(std::string s1, std::string s2)
        {
            if (s1 == s2 || std::abs((int)s1.length() - (int)s2.length()) > 1) return false;
            int diffs = 0;
            int minLen = std::min((int)s1.length(), (int)s2.length());
            for (int i = 0; i < minLen; i++) if (s1[i] != s2[i]) diffs++;
            return diffs > 0 && diffs <= 2;
        }

        inline void QuarantineFile(std::string sourcePath)
        {
            try
            {
                std::string fileName = fs::path(sourcePath).filename().string();
                std::string destPath = quarantineDir + "\\" + fileName + ".jny_locked";
                std::string mapPath = quarantineDir + "\\" + fileName + ".map";
                if (fs::exists(sourcePath))
                {
                    std::ofstream mapFile(mapPath);
                    mapFile << sourcePath;
                    mapFile.close();
                    fs::rename(sourcePath, destPath);
                }
            }
            catch (...) {}
        }

        inline void RestoreQuarantine()
        {
            for (auto& p : fs::directory_iterator(quarantineDir))
            {
                if (p.path().extension() == ".map")
                {
                    try
                    {
                        std::ifstream mapFile(p.path());
                        std::string originalPath;
                        std::getline(mapFile, originalPath);
                        mapFile.close();
                        std::string lockedFile = p.path().string();
                        size_t pos = lockedFile.find(".map");
                        lockedFile.replace(pos, 4, ".jny_locked");
                        if (fs::exists(lockedFile))
                        {
                            fs::create_directories(fs::path(originalPath).parent_path());
                            fs::rename(lockedFile, originalPath);
                            fs::remove(p.path());
                        }
                    }
                    catch (...) {}
                }
            }
        }

        inline void ScanNetworkActivity()
        {
            std::cout << "\n[SENTINEL NETWORK] Analyzing Unsigned & External Connections..." << std::endl;
            PMIB_TCPTABLE_OWNER_PID pTcpTable;
            DWORD dwSize = 0;
            GetExtendedTcpTable(NULL, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
            pTcpTable = (MIB_TCPTABLE_OWNER_PID*)malloc(dwSize);
            GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

            char winBuf[MAX_PATH];
            GetWindowsDirectoryA(winBuf, MAX_PATH);
            std::string winPath = winBuf;
            std::transform(winPath.begin(), winPath.end(), winPath.begin(), ::tolower);

            HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnap, &pe))
            {
                do
                {
                    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
                    if (hProc)
                    {
                        char pathBuf[MAX_PATH];
                        DWORD dwPathSize = MAX_PATH;
                        if (QueryFullProcessImageNameA(hProc, 0, pathBuf, &dwPathSize))
                        {
                            std::string path = pathBuf;
                            if (!IsHardwareDriver(path) && GetFileHash(path) != currentSelfHash)
                            {
                                bool isSigned = IsFileSigned(path);
                                std::string pathLower = path;
                                std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);
                                bool isTrusted = pathLower.find("windowsapps") != std::string::npos || pathLower.find("winget") != std::string::npos;
                                bool isSys = pathLower.find(winPath) != std::string::npos || pathLower.find("\\windows\\") != std::string::npos;

                                if (!isSigned && !isTrusted && !isSys)
                                {
                                    for (DWORD i = 0; i < pTcpTable->dwNumEntries; i++)
                                    {
                                        if (pTcpTable->table[i].dwOwningPid == pe.th32ProcessID && pTcpTable->table[i].dwState == MIB_TCP_STATE_ESTAB)
                                        {
                                            if (detectedThreats.find(path) == detectedThreats.end())
                                            {
                                                detectedThreats[path] = { 100, {"Unsigned process with active network activity"} };
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        CloseHandle(hProc);
                    }
                }
                while (Process32Next(hSnap, &pe));
            }
            CloseHandle(hSnap);
            free(pTcpTable);
            ReportAndHandleThreats();
        }

        inline int AnalyzeFile(std::string fullPath, std::vector<std::string>& reasons)
        {
            if (IsHardwareDriver(fullPath)) return 0;
            int threatScore = 0;
            std::string fileName = fs::path(fullPath).stem().string();
            std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
            std::string pathLower = fullPath;
            std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);

            bool isSigned = IsFileSigned(fullPath);
            bool isStartup = CheckStartupStatus(fullPath);
            bool isTrusted = pathLower.find("\\windowsapps\\") != std::string::npos || pathLower.find("\\microsoft\\winget\\") != std::string::npos;

            if (!isSigned)
            {
                if (!isTrusted)
                {
                    threatScore += 50;
                    reasons.push_back("No Valid Digital Signature");
                }
                else
                {
                    threatScore += 10;
                    reasons.push_back("Unsigned but verified Package Origin");
                }
            }

            for (const auto& workspace : developerWorkspaces)
            {
                std::string wsLower = workspace;
                std::transform(wsLower.begin(), wsLower.end(), wsLower.begin(), ::tolower);
                if (pathLower.find(wsLower) == 0)
                {
                    threatScore -= 30;
                    reasons.push_back("Safe Zone: Developer Workspace");
                }
            }

            if (isStartup && !isSigned && !isTrusted)
            {
                threatScore += 50;
                reasons.push_back("Persistence Alert: Unsigned file in Startup");
            }

            for (const auto& service : criticalServices)
            {
                if (fileName == service && pathLower.find("c:\\windows\\system32") == std::string::npos)
                {
                    threatScore += 70;
                    reasons.push_back("Location Anomaly: " + service + " masquerading");
                }
                if (IsSimilar(fileName, service))
                {
                    threatScore += 50;
                    reasons.push_back("Typosquatting: Mimicking " + service);
                }
            }
            return threatScore;
        }

        inline void ScanDirectoryRecursively(std::string root)
        {
            std::string pathLower = root;
            std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);
            if (pathLower.find("c:\\windows") != std::string::npos || pathLower.find("winsxs") != std::string::npos) return;

            try
            {
                for (auto& p : fs::directory_iterator(root))
                {
                    if (p.is_directory())
                    {
                        ScanDirectoryRecursively(p.path().string());
                    }
                    else if (p.path().extension() == ".exe")
                    {
                        if (GetFileHash(p.path().string()) == currentSelfHash) continue;
                        std::vector<std::string> reasons;
                        int score = AnalyzeFile(p.path().string(), reasons);
                        if (score >= 40)
                        {
                            detectedThreats[p.path().string()] = { std::min(score, 100), reasons };
                        }
                    }
                }
            }
            catch (...) {}
        }

        inline void ReportAndHandleThreats()
        {
            std::cout << "\n" << std::string(50, '-') << std::endl;
            std::cout << "[+] Analysis Finished. Total Threats: " << detectedThreats.size() << std::endl;

            if (!detectedThreats.empty())
            {
                for (auto const& [path, info] : detectedThreats)
                {
                    std::cout << "\n-> " << fs::path(path).filename().string() << " | TOTAL SCORE: " << info.Score << "/100" << std::endl;
                    for (const auto& reason : info.Reasons) std::cout << "   [!] " << reason << std::endl;
                    std::cout << "   [#] SHA256: " << GetFileHash(path) << std::endl;
                }
                std::cout << "\n[?] Move detected files to secure quarantine? (-y / -n): ";
                std::string choice;
                std::cin >> choice;
                if (choice == "-y")
                {
                    for (auto const& [path, info] : detectedThreats) QuarantineFile(path);
                    std::cout << "\n[+] Isolation complete." << std::endl;
                }
            }
        }
    }

    class CompileX
    {
    public:
        void execute(const std::string& arg1, const std::string& confPath = "jenny.conf")
        {
            struct ConfigLocal
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
                std::string ollama_model;
            };

            ConfigLocal cfg;
            std::ifstream file(confPath);
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
                std::getline(file, cfg.ollama_model);
                file.close();
            }

            auto log_operation = [](const std::string& message)
            {
                std::ofstream log_file("compilex_history.log", std::ios::app);
                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                char buf[26];
                ctime_s(buf, sizeof(buf), &now);
                std::string ts(buf);
                ts.pop_back();
                log_file << "[" << ts << "] " << message << std::endl;
            };

            if (arg1 == "--version")
            {
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << "[CompileX Engine]" << std::endl;
                std::cout << "Version: 3.0.1-LTS \"Character-Safe Edition\"" << std::endl;
                std::cout << "Developer: hypernova-developer" << std::endl;
                std::cout << "Focus: C++, C, Fortran, C#, Python" << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                return;
            }

            fs::path source_path = fs::path(reinterpret_cast<const char8_t*>(arg1.c_str()));

            if (!fs::exists(source_path))
            {
                std::cout << "[CompileX] Error: Source file not found." << std::endl;
                return;
            }

            std::string ext = source_path.extension().string();
            std::string exe_name = source_path.stem().string() + ".exe";
            std::string cmd = "";
            std::string lang = "";

            if (ext == ".cpp")
            {
                lang = "C++";
                cmd = "\"\"" + cfg.gxx + "\" -std=c++23 -O3 -s -static -static-libgcc -static-libstdc++ -I\"" + cfg.syntax_inc + "\" -I\"" + cfg.raylib_inc + "\" \"" + arg1 + "\" -o \"" + exe_name + "\" -L\"" + cfg.raylib_lib + "\" -lraylib -lopengl32 -lgdi32 -lwinmm\"";
            }
            else if (ext == ".c")
            {
                lang = "C";
                cmd = "\"\"" + cfg.gcc + "\" -O3 -s -static -static-libgcc -I\"" + cfg.syntax_inc + "\" \"" + arg1 + "\" -o \"" + exe_name + "\"\"";
            }
            else if (ext == ".f90")
            {
                lang = "Fortran";
                cmd = "\"\"" + cfg.gfortran + "\" -O3 -static \"" + arg1 + "\" -o \"" + exe_name + "\"\"";
            }
            else if (ext == ".cs")
            {
                lang = "C#";
                cmd = "\"\"" + cfg.csc + "\" /nologo /optimize+ /out:\"" + exe_name + "\" \"" + arg1 + "\"\"";
            }
            else if (ext == ".py")
            {
                std::cout << "[CompileX] Executing " << arg1 << " (Python)..." << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                int py_res = system(std::string("\"\"" + cfg.python + "\" \"" + arg1 + "\"\"").c_str());
                std::cout << "---------------------------------------------------" << std::endl;
                log_operation("Executed " + arg1 + " (Python)");
                return;
            }

            std::cout << "[CompileX] Compiling " << arg1 << " (" << lang << ")..." << std::endl;
            int res = system(cmd.c_str());

            if (res == 0)
            {
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << "[CompileX] Success: Build complete." << std::endl;
                
                std::ofstream built_list(".compilex_built", std::ios::app);
                built_list << exe_name << "\n";
                
                log_operation("Compiled " + exe_name + " (" + lang + ")");
                
                std::cout << "[CompileX] Executing binary..." << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                system(std::string("\"" + exe_name + "\"").c_str());
            }
            else
            {
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << "[CompileX] Error: Compilation failed." << std::endl;
                log_operation("Failed to compile " + arg1 + " (" + lang + ")");
            }
        }
    };
}

struct ConfigMain
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

ConfigMain loadConfigMain()
{
    ConfigMain cfg;
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
    ConfigMain cfg = loadConfigMain();

    if (argc < 2)
    {
        std::cout << "\n[!] Jenny " << version << " - No command entered. Try: jenny --help" << std::endl;
        return 0;
    }

    std::string command = argv[1];

    if (command == "--hardware")
    {
        JennyModules::HardwareDetective detective;
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
        JennyModules::ProtocolX px;
        std::cout << "Encoded Output: " << px.encode(argv[2]) << std::endl;
    }
    else if (command == "--px-d" && argc > 2)
    {
        JennyModules::ProtocolX px;
        std::cout << "Decoded Output: " << px.decode(argv[2]) << std::endl;
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
        JennyModules::CompileX compilex;
        compilex.execute(argv[2]);
    }
    else if (command == "--sentinel")
    {
        JennyModules::Sentinel::loadQuarantineConfig();
        char selfPath[MAX_PATH];
        GetModuleFileNameA(NULL, selfPath, MAX_PATH);
        JennyModules::Sentinel::currentSelfHash = JennyModules::Sentinel::GetFileHash(selfPath);

        if (!fs::exists(JennyModules::Sentinel::quarantineDir)) fs::create_directories(JennyModules::Sentinel::quarantineDir);

        std::string target = (argc > 2) ? argv[2] : fs::current_path().string();
        std::cout << "\n[SENTINEL CORE v2.0.0-LTS] Precision Scan Initiated: " << target << std::endl;

        JennyModules::Sentinel::ScanDirectoryRecursively(target);
        JennyModules::Sentinel::ReportAndHandleThreats();

        std::cout << "\n[PRESS ENTER TO EXIT]" << std::endl;
        std::cin.ignore();
        std::cin.get();
    }
    else if (command == "--sentinel-network")
    {
        JennyModules::Sentinel::loadQuarantineConfig();
        char selfPath[MAX_PATH];
        GetModuleFileNameA(NULL, selfPath, MAX_PATH);
        JennyModules::Sentinel::currentSelfHash = JennyModules::Sentinel::GetFileHash(selfPath);
        JennyModules::Sentinel::ScanNetworkActivity();
    }
    else if (command == "--restore")
    {
        JennyModules::Sentinel::loadQuarantineConfig();
        JennyModules::Sentinel::RestoreQuarantine();
    }
    else if (command == "--vault")
    {
        JennyModules::JennyVault vault;
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
