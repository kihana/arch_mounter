#include "DokanOperations.h"
#include "Logger.h"
#include "ArchiveWrapper.h"
#include "Utils.h"
#include "Helpers.h"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <mutex>

using namespace std;


mutex g_read_mutex;

vector<wstring> split(const wstring& str, const wstring& delim)
{
  vector<wstring> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if (pos == wstring::npos)
      pos = str.length();
    
    wstring token = str.substr(prev, pos - prev);
    
    if (!token.empty())
      tokens.push_back(token);
    
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());

  return tokens;
}

void DOKAN_CALLBACK DokanOperations::Cleanup(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"Cleanup");
  logger->debug(L"FileName: {}", FileName);

  wstring file_name = FileName;
  replace(begin(file_name), end(file_name), L'\\', L'/');

  DokanFileInfo->Context = 0;
  ArchiveItem* item = ArchiveWrapper::instance().get_item(file_name);
  if (nullptr != item && !item->data.empty())
    item->data.clear();
}

void DOKAN_CALLBACK DokanOperations::CloseFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"CloseFile");
  logger->debug(L"FileName: {}", FileName);
}

NTSTATUS DOKAN_CALLBACK DokanOperations::DeleteDirectory(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"DeleteDirectory");
  logger->debug(L"FileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::DeleteFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"DeleteFile");
  logger->debug(L"FileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::FindFiles(LPCWSTR FileName, PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"FindFiles");
  logger->debug(L"\tFileName: {}", FileName);
  
  ArchiveItem* archive_item = reinterpret_cast<ArchiveItem*>(DokanFileInfo->Context);
  if (nullptr == archive_item)
    return STATUS_OBJECT_NAME_NOT_FOUND;

  WIN32_FIND_DATA find_data;
  auto& children = archive_item->children;

  for (auto& child : children)
  {
    auto& item = child.second;
    
    find_data.dwFileAttributes = item->attributes;
    find_data.ftCreationTime = item->creation_time;
    find_data.ftLastWriteTime = item->modification_time;
    find_data.ftLastAccessTime = item->last_access_time;
    find_data.nFileSizeHigh = 0;
    find_data.nFileSizeLow = item->size;
    wcscpy_s(find_data.cFileName, item->name.c_str());

    FillFindData(&find_data, DokanFileInfo);
  }
  
  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::FindFilesWithPattern(LPCWSTR PathName, LPCWSTR SearchPattern, PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"FindFilesWithPattern");
  logger->debug(L"\tPathName: {}", PathName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::FindStreams(LPCWSTR FileName, PFillFindStreamData FillFindStreamData, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"FindStreams");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::FlushFileBuffers(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"FlushFileBuffers");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::GetDiskFreeSpace(PULONGLONG FreeBytesAvailable, PULONGLONG TotalNumberOfBytes, PULONGLONG TotalNumberOfFreeBytes, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"GetDiskFreeSpace");

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::GetFileInformation(LPCWSTR FileName, LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"GetFileInformation");
  logger->debug(L"\tFileName: {}", FileName);

  ArchiveItem* archive_item = reinterpret_cast<ArchiveItem*>(DokanFileInfo->Context);
  if (nullptr == archive_item)
    return STATUS_OBJECT_NAME_NOT_FOUND;
   
  HandleFileInformation->dwFileAttributes = archive_item->attributes;
  HandleFileInformation->ftCreationTime = archive_item->creation_time;
  HandleFileInformation->ftLastAccessTime = archive_item->last_access_time;
  HandleFileInformation->ftLastWriteTime = archive_item->modification_time;
  HandleFileInformation->nFileSizeHigh = 0;
  HandleFileInformation->nFileSizeLow = archive_item->size;

  logger->debug(L"\tFileAttributes: {}", HandleFileInformation->dwFileAttributes);
  logger->debug(L"\tCreationTime: {}", utils::filetime_to_string(HandleFileInformation->ftCreationTime));
  logger->debug(L"\tLastAccessTime: {}", utils::filetime_to_string(HandleFileInformation->ftLastAccessTime));
  logger->debug(L"\tLastWriteTime: {}", utils::filetime_to_string(HandleFileInformation->ftLastWriteTime).c_str());
  logger->debug(L"\tSize: {}", HandleFileInformation->nFileSizeLow);

  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::GetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"GetFileSecurity");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::GetVolumeInformation(LPWSTR VolumeNameBuffer, DWORD VolumeNameSize, LPDWORD VolumeSerialNumber, LPDWORD MaximumComponentLength, LPDWORD FileSystemFlags, LPWSTR FileSystemNameBuffer, DWORD FileSystemNameSize, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"GetVolumeInformation");

  wcscpy_s(VolumeNameBuffer, VolumeNameSize, L"ARCH_VOLUME");

  if (nullptr != VolumeSerialNumber)
    *VolumeSerialNumber = 0x19831116;

  if (nullptr != MaximumComponentLength)
    *MaximumComponentLength = 255;
  
  if (nullptr != FileSystemFlags)
    *FileSystemFlags = FILE_CASE_SENSITIVE_SEARCH | FILE_CASE_PRESERVED_NAMES |
    FILE_SUPPORTS_REMOTE_STORAGE | FILE_UNICODE_ON_DISK |
    FILE_PERSISTENT_ACLS | FILE_NAMED_STREAMS;

  wchar_t volume_root[4];
  volume_root[0] = 'd';
  volume_root[1] = ':';
  volume_root[2] = '\\';
  volume_root[3] = '\0';

  DWORD flags = 0;
  if (::GetVolumeInformation(volume_root, nullptr, 0, nullptr, MaximumComponentLength, &flags, FileSystemNameBuffer, FileSystemNameSize))
  {
    if (FileSystemFlags)
      *FileSystemFlags &= flags;
  }
  else
    wcscpy_s(FileSystemNameBuffer, FileSystemNameSize, L"NTFS");

  logger->debug(L"\tVolumeNameBuffer: {}", VolumeNameBuffer);
  logger->debug(L"\tVolumeNameSize: {}", VolumeNameSize);
  logger->debug(L"\tVolumeSerialNumber: {}", *VolumeSerialNumber);
  logger->debug(L"\tMaximumComponentLength: {}", *MaximumComponentLength);
  logger->debug(L"\tFileSystemFlags: {}", *FileSystemFlags);
  logger->debug(L"\tFileSystemNameBuffer: {}", FileSystemNameBuffer);
  logger->debug(L"\tFileSystemNameSize: {}", FileSystemNameSize);

  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::LockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"LockFile");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::MoveFile(LPCWSTR FileName, LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"MoveFile");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::ReadFile(LPCWSTR FileName, LPVOID Buffer, 
                                                  DWORD BufferLength, LPDWORD ReadLength,
                                                  LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo)
{
  wstring file_name = FileName;
  replace(begin(file_name), end(file_name), L'\\', L'/');

  helpers::AutoMutex lock(file_name);
  
  auto& logger = Logger::instance();
  logger->debug(L"ReadFile");
  logger->debug(L"\tFileName: {}", FileName);
  logger->debug(L"\tBufferLength: {}", BufferLength);
  logger->debug(L"\tReadLength: {}", *ReadLength);
  logger->debug(L"\tOffset: {}", Offset);

  ArchiveWrapper& archive = ArchiveWrapper::instance();
  ArchiveItem* archive_item = archive.get_item(file_name);
  if (nullptr == archive_item)
    return STATUS_OBJECT_NAME_NOT_FOUND;

  DWORD read_size = archive.read(file_name, Buffer, BufferLength, Offset);
  *ReadLength = read_size;

  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::SetAllocationSize(LPCWSTR FileName, LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"SetAllocationSize");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::SetEndOfFile(LPCWSTR FileName, LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"SetEndOfFile");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::SetFileAttributes(LPCWSTR FileName, DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"SetFileAttributes");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::SetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"SetFileSecurity");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::SetFileTime(LPCWSTR FileName, CONST FILETIME* CreationTime, CONST FILETIME* LastAccessTime, CONST FILETIME* LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"SetFileTime");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::UnlockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"UnlockFile");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::Mounted(PDOKAN_FILE_INFO DokanFileInfo)
{
  UNREFERENCED_PARAMETER(DokanFileInfo);

  auto& logger = Logger::instance();
  logger->debug(L"Mounted");

  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::Unmounted(PDOKAN_FILE_INFO DokanFileInfo)
{
  UNREFERENCED_PARAMETER(DokanFileInfo);

  auto& logger = Logger::instance();
  logger->debug(L"Unmounted");

  return STATUS_SUCCESS;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::WriteFile(LPCWSTR FileName, LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"WriteFile");
  logger->debug(L"\tFileName: {}", FileName);

  return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DOKAN_CALLBACK DokanOperations::CreateFile(LPCWSTR FileName, PDOKAN_IO_SECURITY_CONTEXT SecurityContext,
                                                    ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess,
                                                    ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo)
{
  auto& logger = Logger::instance();
  logger->debug(L"CreateFile");
  logger->debug(L"\tFileName: {}", FileName);
  logger->debug(L"\tShareAccess: {}", ShareAccess);
  logger->debug(L"\tCreateOptions: {}", CreateOptions);

  wstring file_name = FileName;
  replace(begin(file_name), end(file_name), L'\\', L'/');

  ArchiveItem* archive_item = ArchiveWrapper::instance().get_item(file_name);
  if (nullptr == archive_item)
    return STATUS_OBJECT_NAME_NOT_FOUND;

  DokanFileInfo->Context = reinterpret_cast<ULONG64>(archive_item);

  if (DokanFileInfo->IsDirectory || archive_item->attributes == FILE_ATTRIBUTE_DIRECTORY)
  {
    if (!DokanFileInfo->IsDirectory)
      DokanFileInfo->IsDirectory = TRUE;
    
    logger->debug(L"\tCreateFile for a directory.");
  }
  else
  {
    logger->debug(L"\tCreateFile for a file.");
  }

  return STATUS_SUCCESS;
}