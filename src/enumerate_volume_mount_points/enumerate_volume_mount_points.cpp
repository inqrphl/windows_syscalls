#include <windows.h>
#include <stdio.h>
#include <system_error>
#include <vector>

void main(int argc, char* argv[]) {
    WCHAR lpszRootPathName[MAX_PATH + 5] = L"";
    const DWORD lpszVolumeMountPointLength{MAX_PATH+5};
    WCHAR lpszVolumeMountPoint[lpszVolumeMountPointLength] = L"";

    size_t converted_wchar{ 0 };
    switch (argc){
        case 2:
            mbstowcs_s(&converted_wchar, lpszRootPathName, MAX_PATH + 5, argv[1], _TRUNCATE);
            if (converted_wchar == 0) {
                printf("Failed to convert argument\n");
                return;
            }
            break;
        default:
            printf("Usage: find_first_volume_mount_point <volume guid path>\n");
            return;
    }

    wprintf(L"Volume Guid: %s\n", lpszRootPathName);
    DWORD Error{ERROR_SUCCESS};
    HANDLE mountPointHandle{0};
    mountPointHandle = FindFirstVolumeMountPointW(lpszRootPathName,lpszVolumeMountPoint,lpszVolumeMountPointLength);

    if(mountPointHandle == INVALID_HANDLE_VALUE){
        printf("No mount mounts\n");
        return;
    }
    
    while(mountPointHandle != INVALID_HANDLE_VALUE){
        wprintf(L"  Mount Path: %s\n", lpszVolumeMountPoint);

        BOOL findNextVolumeMountPointSuccess = FindNextVolumeMountPointW(mountPointHandle, lpszVolumeMountPoint,lpszVolumeMountPointLength);

        if (findNextVolumeMountPointSuccess == 0){
            Error = GetLastError();
            wprintf(L"FindNextVolumeMountPointW failed with error code %d\n", Error);
            printf("Error code explanation: %s\n", std::system_category().message(Error).c_str());
            break;
        }
    }

    FindVolumeMountPointClose(mountPointHandle);

    return;
}


