#include <windows.h>
#include <stdio.h>
#include <system_error>
#include <vector>

void main(int argc, char* argv[]) {
    WCHAR lpszVolumeMountPoint[MAX_PATH + 5] = L"";
    const DWORD lpszVolumeNameBufferSize{MAX_PATH+5};
    WCHAR lpszVolumeName[lpszVolumeNameBufferSize] = L"";

    size_t converted_wchar{ 0 };
    switch (argc){
        case 2:
            mbstowcs_s(&converted_wchar, lpszVolumeMountPoint, MAX_PATH + 5, argv[1], _TRUNCATE);
            if (converted_wchar == 0) {
                printf("Failed to convert argument\n");
                return;
            }
            break;
        default:
            printf("Usage: get_volume_name <path>\n");
            return;
    }

    DWORD Error{ERROR_SUCCESS};
    BOOL getVolumeNameForVolumeMountPointSuccess = GetVolumeNameForVolumeMountPointW(lpszVolumeMountPoint,lpszVolumeName,lpszVolumeNameBufferSize);

    if (!getVolumeNameForVolumeMountPointSuccess) {
        Error = GetLastError();
        wprintf(L"GetVolumeNameForVolumeMountPoint failed with error code %d\n", Error);
        printf("Error code explanation: %s\n", std::system_category().message(Error).c_str());
        return;
    }

    // Print the volume information
    wprintf(L"Volume Mount Point: %s\n", lpszVolumeMountPoint);
    wprintf(L"Volume Name: %s\n", lpszVolumeName);
}


