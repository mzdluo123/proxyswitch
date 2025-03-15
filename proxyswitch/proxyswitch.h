// proxyswitch.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <wininet.h>

// Function to set an environment variable for the current user
bool SetUserEnvironmentVariable(const std::wstring& name, const std::wstring& value);

// Function to get an environment variable for the current user
std::wstring GetUserEnvironmentVariable(const std::wstring& name);

// Function to check if proxy settings are already set
bool IsProxySet();

// Function to set Windows Internet proxy settings
bool SetWindowsInternetProxy(const std::wstring& proxyServer);

// Function to clear Windows Internet proxy settings
bool ClearWindowsInternetProxy();

// Function to set proxy settings (both env vars and Windows Internet)
bool SetProxySettings(const std::wstring& proxyAddress, const std::wstring& proxyPort);

// Function to clear proxy settings (both env vars and Windows Internet)
bool ClearProxySettings();

// TODO: Reference additional headers your program requires here.
