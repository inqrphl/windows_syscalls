#include <windows.h>
#include <stdio.h>
#include <system_error>

void main(int argc, char* argv[]) {
    WCHAR lpRootPathName[MAX_PATH + 1] = L"";
    const DWORD nVolumeNameSize = 1024;
    WCHAR lpVolumeNameBuffer[nVolumeNameSize] = L"";
    DWORD lpVolumeSerialNumber;
    DWORD lpMaximumComponentLength;
    DWORD lpFileSystemFlags;
    const DWORD nFileSystemNameSize = MAX_PATH + 1;
    WCHAR lpFileSystemNameBuffer[nFileSystemNameSize] = L"";
    DWORD Error = ERROR_SUCCESS;

    size_t converted_wchar{ 0 };
    switch (argc)
    {
    case 2:
        mbstowcs_s(&converted_wchar, lpRootPathName, MAX_PATH + 1, argv[1], _TRUNCATE);
        if (converted_wchar == 0) {
            printf("Failed to convert path\n");
            return;
        }
        break;
    default:
        printf("Usage: display_volume_information <path>\n");
        return;
    }

    BOOL getVolumeInformationWSuccess = GetVolumeInformationW(
        lpRootPathName,
        lpVolumeNameBuffer,
        nVolumeNameSize,
        &lpVolumeSerialNumber,
        &lpMaximumComponentLength,
        &lpFileSystemFlags,
        lpFileSystemNameBuffer,
        nFileSystemNameSize
    );

    if (!getVolumeInformationWSuccess) {
        Error = GetLastError();
        wprintf(L"GetVolumeInformationW failed with error code %d\n", Error);
        printf("Error code explanation: %s\n", std::system_category().message(Error).c_str());
        return;
    }

    // Print the volume information
    wprintf(L"Volume Name: %s\n", lpVolumeNameBuffer);
    wprintf(L"File System Name: %s\n", lpFileSystemNameBuffer);
    wprintf(L"Serial Number: %08X\n", lpVolumeSerialNumber);
    wprintf(L"Maximum Component Length: %d\n", lpMaximumComponentLength);
    wprintf(L"File System Flags: %08X\n", lpFileSystemFlags);
}