#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <VersionHelpers.h>

#pragma comment(lib, "advapi32.lib")

// Structure for OS version info
typedef struct _OSVERSIONINFOEXW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[128];
    WORD  wServicePackMajor;
    WORD  wServicePackMinor;
    WORD  wSuiteMask;
    BYTE  wProductType;
    BYTE  wReserved;
} OSVERSIONINFOEXW;

typedef LONG (WINAPI* RtlGetVersionPtr)(OSVERSIONINFOEXW*);

// Get actual Windows version without compatibility lies
int get_windows_version(OSVERSIONINFOEXW* osInfo) {
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != NULL) {
            osInfo->dwOSVersionInfoSize = sizeof(*osInfo);
            return fxPtr(osInfo) == 0;
        }
    }
    return 0;
}

int is_windows_10_or_less() {
    OSVERSIONINFOEXW osInfo;
    if (get_windows_version(&osInfo)) {
        printf("Detected Windows Version: %d.%d (Build %d)\n",
               osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);
        // Windows 10 is version 10.x, Windows 11 starts at build 22000
        if (osInfo.dwMajorVersion < 10 || 
            (osInfo.dwMajorVersion == 10 && osInfo.dwBuildNumber <= 19044)) {
            return 1; // Windows 10 or older
        }
    }
    return 0; // Windows 11 or newer
}

void modify_registry() {
    HKEY hKey;
    LONG result;

    // Load registry hive for temporary modification
    system("reg load HKLM\\TEMP C:\\Windows\\System32\\config\\SOFTWARE");

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "TEMP\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", 0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "Shell", 0, REG_SZ, (const BYTE *)"cmd.exe", sizeof("cmd.exe"));
        RegCloseKey(hKey);
        printf("Registry modified to boot into CMD.\n");
    } else {
        printf("Failed to modify the registry. Error code: %ld\n", result);
    }

    system("reg unload HKLM\\TEMP");
}

void reboot_system() {
    system("shutdown /r /t 0");
}

void shutdown_system() {
    system("shutdown /s /t 0");
}

int main() {
    printf("=============================================\n"
           "| Unofficial Windows Debugger Boot Sequence |\n"
           "=============================================\n");

    // Enforce Windows 10 or older
    if (!is_windows_10_or_less()) {
        printf("This program is restricted to Windows 10 or older.\n");
        return 1;
    }

    printf("[Boot options]\n"
           "[1] Modify Windows to Boot into CMD\n"
           "[2] Reboot\n"
           "[3] Shutdown\n");

    printf("Select an option: ");
    int option;
    scanf("%d", &option);

    switch (option) {
        case 1:
            printf("Applying jailbreak for Windows 10 or older...\n");
            modify_registry();
            reboot_system();
            break;
        case 2:
            reboot_system();
            break;
        case 3:
            shutdown_system();
            break;
        default:
            printf("Invalid option selected.\n");
    }

    return 0;
}
