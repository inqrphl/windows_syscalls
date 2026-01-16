#include <windows.h>
#include <stdio.h>
#include <system_error>
#include <vector>


// Helper function to build a simple capability list
std::wstring BuildCapabilityListString(DWORD fileSystemFlags) {
    std::wstring result;
    
    struct FlagDescription {
        DWORD flag;
        const wchar_t* name;
        const wchar_t* description;
    };
    
    std::vector<FlagDescription> flagDescriptions = {
        {FILE_CASE_SENSITIVE_SEARCH, L"Case Sensitive Search", L"Supports case-sensitive file names"},
        {FILE_CASE_PRESERVED_NAMES, L"Case Preserved Names", L"Preserves case of file names on disk"},
        {FILE_UNICODE_ON_DISK, L"Unicode On Disk", L"Supports Unicode in file names on disk"},
        {FILE_PERSISTENT_ACLS, L"Persistent ACLs", L"Preserves and enforces access control lists (ACL)"},
        {FILE_FILE_COMPRESSION, L"File Compression", L"Supports file-based compression"},
        {FILE_VOLUME_QUOTAS, L"Volume Quotas", L"Supports disk quotas"},
        {FILE_SUPPORTS_SPARSE_FILES, L"Sparse Files", L"Supports sparse files"},
        {FILE_SUPPORTS_REPARSE_POINTS, L"Reparse Points", L"Supports reparse points"},
        {FILE_SUPPORTS_REMOTE_STORAGE, L"Remote Storage", L"Supports remote storage"},
        {FILE_RETURNS_CLEANUP_RESULT_INFO, L"Cleanup Result Info", L"Returns cleanup result information"},
        {FILE_SUPPORTS_POSIX_UNLINK_RENAME, L"POSIX Unlink/Rename", L"Supports POSIX-style delete and rename operations"},
        {FILE_VOLUME_IS_COMPRESSED, L"Volume Compressed", L"Volume is compressed"},
        {FILE_SUPPORTS_OBJECT_IDS, L"Object IDs", L"Supports object identifiers"},
        {FILE_SUPPORTS_ENCRYPTION, L"Encryption", L"Supports Encrypted File System (EFS)"},
        {FILE_NAMED_STREAMS, L"Named Streams", L"Supports named streams"},
        {FILE_READ_ONLY_VOLUME, L"Read Only Volume", L"Volume is read-only"},
        {FILE_SEQUENTIAL_WRITE_ONCE, L"Sequential Write Once", L"Supports single sequential write"},
        {FILE_SUPPORTS_TRANSACTIONS, L"Transactions", L"Supports transactions"},
        {FILE_SUPPORTS_HARD_LINKS, L"Hard Links", L"Supports hard links"},
        {FILE_SUPPORTS_EXTENDED_ATTRIBUTES, L"Extended Attributes", L"Supports extended attributes"},
        {FILE_SUPPORTS_OPEN_BY_FILE_ID, L"Open by File ID", L"Supports open by FileID"},
        {FILE_SUPPORTS_USN_JOURNAL, L"USN Journal", L"Supports update sequence number (USN) journals"},
        {FILE_SUPPORTS_INTEGRITY_STREAMS, L"Integrity Streams", L"Supports integrity streams"},
        {FILE_SUPPORTS_BLOCK_REFCOUNTING, L"Block Refcounting", L"Supports sharing logical clusters between files"},
        {FILE_SUPPORTS_SPARSE_VDL, L"Sparse VDL", L"Tracks valid data length for sparse files"},
        {FILE_DAX_VOLUME, L"DAX Volume", L"Volume is direct access (DAX)"},
        {FILE_SUPPORTS_GHOSTING, L"Ghosting", L"Supports ghosting"}
    };
    
    result += L"Capabilities:\n";
    result += L"-------------\n";
    
    bool hasCapabilities = false;
    for (const auto& flagDesc : flagDescriptions) {
        if (fileSystemFlags & flagDesc.flag) {
            result += L"";
            result += flagDesc.name;
            result += L" - ";
            result += flagDesc.description;
            result += L"\n";
            hasCapabilities = true;
        }
    }
    
    if (!hasCapabilities) {
        result += L"No capabilities detected\n";
    }
    
    return result;
}

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

    wprintf(L"%s",BuildCapabilityListString(lpFileSystemFlags).c_str());
}


