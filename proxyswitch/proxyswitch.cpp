// proxyswitch.cpp : Defines the entry point for the application.
//

#include "proxyswitch.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <wininet.h>
#include <fcntl.h>
#include <io.h>
#include <shellapi.h> // Add this include for ShellExecuteEx
#include <Psapi.h>
#include <tlhelp32.h> // Add this include for process snapshot functions


using namespace std;

std::wstring GetUserEnvironmentVariable(const std::wstring& name) {
    HKEY hKey;
    std::wstring keyPath = L"Environment";
    
    // Open the current user's environment key
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        keyPath.c_str(),
        0,
        KEY_READ,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        return L"";
    }

    DWORD type;
    DWORD dataSize = 0;
    
    // Get the size of the value
    result = RegQueryValueExW(
        hKey,
        name.c_str(),
        nullptr,
        &type,
        nullptr,
        &dataSize
    );

    if (result != ERROR_SUCCESS || type != REG_SZ) {
        RegCloseKey(hKey);
        return L"";
    }

    std::wstring value;
    value.resize(dataSize / sizeof(wchar_t));

    // Get the value
    result = RegQueryValueExW(
        hKey,
        name.c_str(),
        nullptr,
        &type,
        reinterpret_cast<LPBYTE>(&value[0]),
        &dataSize
    );

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        return L"";
    }

    // Remove the null terminator if present
    if (!value.empty() && value.back() == L'\0') {
        value.pop_back();
    }

    return value;
}

bool IsProxySet() {
    std::wstring httpProxy = GetUserEnvironmentVariable(L"HTTP_PROXY");
    std::wstring httpsProxy = GetUserEnvironmentVariable(L"HTTPS_PROXY");
    
    return !httpProxy.empty() || !httpsProxy.empty();
}

bool SetUserEnvironmentVariable(const std::wstring& name, const std::wstring& value) {
    HKEY hKey;
    std::wstring keyPath = L"Environment";
    
    // Open the current user's environment key
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        keyPath.c_str(),
        0,
        KEY_WRITE,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to open registry key. Error code: " << result << std::endl;
        return false;
    }

    // If value is empty, delete the environment variable
    if (value.empty()) {
        result = RegDeleteValueW(hKey, name.c_str());
        RegCloseKey(hKey);

        if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND) {
            std::wcerr << L"Failed to delete registry value. Error code: " << result << std::endl;
            return false;
        }
    } else {
        // Set the environment variable in the registry
        result = RegSetValueExW(
            hKey,
            name.c_str(),
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(value.c_str()),
            static_cast<DWORD>((value.length() + 1) * sizeof(wchar_t))
        );

        RegCloseKey(hKey);

        if (result != ERROR_SUCCESS) {
            std::wcerr << L"Failed to set registry value. Error code: " << result << std::endl;
            return false;
        }
    }

    // Broadcast WM_SETTINGCHANGE message to notify the system of the environment change
    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SETTINGCHANGE,
        0,
        reinterpret_cast<LPARAM>(L"Environment"),
        SMTO_ABORTIFHUNG,
        5000,
        nullptr
    );

    return true;
}

bool SetWindowsInternetProxy(const std::wstring& proxyServer) {
    HKEY hKey;
    std::wstring keyPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
    
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        keyPath.c_str(),
        0,
        KEY_WRITE,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to open Internet Settings registry key. Error code: " << result << std::endl;
        return false;
    }

    // Enable proxy
    DWORD proxyEnable = 1;
    result = RegSetValueExW(
        hKey,
        L"ProxyEnable",
        0,
        REG_DWORD,
        reinterpret_cast<const BYTE*>(&proxyEnable),
        sizeof(proxyEnable)
    );

    if (result != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    // Set proxy server
    result = RegSetValueExW(
        hKey,
        L"ProxyServer",
        0,
        REG_SZ,
        reinterpret_cast<const BYTE*>(proxyServer.c_str()),
        static_cast<DWORD>((proxyServer.length() + 1) * sizeof(wchar_t))
    );

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        return false;
    }

    // Notify the system of the change
    InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);

    return true;
}

bool ClearWindowsInternetProxy() {
    HKEY hKey;
    std::wstring keyPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
    
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        keyPath.c_str(),
        0,
        KEY_WRITE,
        &hKey
    );

    if (result != ERROR_SUCCESS) {
        return false;
    }

    // Disable proxy
    DWORD proxyEnable = 0;
    result = RegSetValueExW(
        hKey,
        L"ProxyEnable",
        0,
        REG_DWORD,
        reinterpret_cast<const BYTE*>(&proxyEnable),
        sizeof(proxyEnable)
    );

    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        return false;
    }

    // Notify the system of the change
    InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);

    return true;
}

// Function to get the parent process ID
DWORD GetParentProcessId(DWORD processId) {
    DWORD parentProcessId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (pe.th32ProcessID == processId) {
                    parentProcessId = pe.th32ParentProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    return parentProcessId;
}

#include <algorithm> // Add this include for std::transform

// Function to check if the program was launched via double-click
bool IsDoubleClickLaunch() {
    DWORD processId = GetCurrentProcessId();
    DWORD parentProcessId = GetParentProcessId(processId);

    HANDLE hParentProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, parentProcessId);
    if (hParentProcess == NULL) {
        return false;
    }

    WCHAR parentExePath[MAX_PATH];
    if (GetModuleFileNameExW(hParentProcess, NULL, parentExePath, MAX_PATH) == 0) {
        CloseHandle(hParentProcess);
        return false;
    }

    CloseHandle(hParentProcess);

    std::wstring parentExePathStr(parentExePath);
    std::transform(parentExePathStr.begin(), parentExePathStr.end(), parentExePathStr.begin(), ::tolower);

    // Check if the parent process is explorer.exe
    if (parentExePathStr.find(L"explorer.exe") != std::wstring::npos) {
        return true;
    }

    return false;
}

bool SetProxySettings(const std::wstring& proxyAddress, const std::wstring& proxyPort) {
    std::wstring proxyUrl = L"http://" + proxyAddress + L":" + proxyPort;
    std::wstring proxyServer = proxyAddress + L":" + proxyPort;
    
    bool success = true;
    success &= SetUserEnvironmentVariable(L"HTTP_PROXY", proxyUrl);
    success &= SetUserEnvironmentVariable(L"HTTPS_PROXY", proxyUrl);
    success &= SetWindowsInternetProxy(proxyServer);
    
    if (success) {
        std::wcout << L"Proxy settings have been set to: " << proxyUrl << std::endl;
        std::wcout << L"Windows Internet proxy has been configured." << std::endl;
        if (IsDoubleClickLaunch()) {
            MessageBoxW(NULL, L"Proxy settings have been set.", L"ProxySwitch", MB_OK);
        }
    } else {
        std::wcerr << L"Failed to set proxy settings." << std::endl;
    }
    
    return success;
}

bool ClearProxySettings() {
    bool success = true;
    success &= SetUserEnvironmentVariable(L"HTTP_PROXY", L"");
    success &= SetUserEnvironmentVariable(L"HTTPS_PROXY", L"");
    success &= ClearWindowsInternetProxy();
    
    if (success) {
        std::wcout << L"All proxy settings have been cleared." << std::endl;
        if (IsDoubleClickLaunch()) {
            MessageBoxW(NULL, L"Proxy settings have been cleared.", L"ProxySwitch", MB_OK);
        }
    } else {
        std::wcerr << L"Failed to clear proxy settings." << std::endl;
    }
    
    return success;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    // Set console to UTF-16 mode for proper display of wide characters
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    // Default proxy settings from the requirement
    const std::wstring defaultProxyAddress = L"127.0.0.1";
    const std::wstring defaultProxyPort = L"7897";

    int argc;
    LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);

    if (argc > 0) {
        std::wstring command = argv[0];
        if (command == L"clear") {
            return ClearProxySettings() ? 0 : 1;
        }
        // Additional commands can be added here
    }

    // Check if proxy is already set
    if (IsProxySet()) {
        std::wcout << L"Proxy settings already exist. Clearing them..." << std::endl;
        return ClearProxySettings() ? 0 : 1;
    }

    // Set default proxy if no command is provided and proxy is not already set
    return SetProxySettings(defaultProxyAddress, defaultProxyPort) ? 0 : 1;
}
