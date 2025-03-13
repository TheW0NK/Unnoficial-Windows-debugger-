#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void modify_registry() {
    // Load SOFTWARE hive
    system("reg load HKLM\\TEMP C:\\Windows\\System32\\config\\SOFTWARE");

    // Modify the Shell value to cmd.exe
    system("reg add HKLM\\TEMP\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon /v Shell /t REG_SZ /d cmd.exe /f");

    // Unload the hive
    system("reg unload HKLM\\TEMP");
}

void reboot_system() {
    printf("Rebooting...\n");
    system("shutdown /r /t 0");
}

void shutdown_system() {
    printf("Shutting down...\n");
    system("shutdown /s /t 0");
}

int main() {
    printf("=============================================\n"
           "| Unofficial Windows Debugger Boot Sequence |\n"
           "=============================================\n"
           "[Boot options]\n"
           "[1] Modify Windows to Boot into CMD\n"
           "[2] Reboot\n"
           "[3] Shutdown\n");

    printf("Select an option: ");
    int option;
    scanf("%d", &option);

    switch (option) {
        case 1:
            printf("Applying jailbreak...\n");
            modify_registry();
            printf("Done! Rebooting into jailbreak mode...\n");
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
