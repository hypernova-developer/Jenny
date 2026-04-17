#ifndef HARDWARE_DETECTIVE_HPP
#define HARDWARE_DETECTIVE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <comutil.h>
#include <WbemIdl.h>

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

#endif