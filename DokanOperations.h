#pragma once

#include "dokan/dokan.h"

class DokanOperations
{
public:
  static void DOKAN_CALLBACK Cleanup(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo);
  static void DOKAN_CALLBACK CloseFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK DeleteDirectory(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK DeleteFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK FindFiles(LPCWSTR FileName, PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK FindFilesWithPattern(LPCWSTR PathName, LPCWSTR SearchPattern, PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK FindStreams(LPCWSTR FileName, PFillFindStreamData FillFindStreamData, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK FlushFileBuffers(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK GetDiskFreeSpace(PULONGLONG FreeBytesAvailable, PULONGLONG TotalNumberOfBytes, PULONGLONG TotalNumberOfFreeBytes, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK GetFileInformation(LPCWSTR FileName, LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK GetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK GetVolumeInformation(LPWSTR VolumeNameBuffer, DWORD VolumeNameSize, LPDWORD VolumeSerialNumber, LPDWORD MaximumComponentLength, LPDWORD FileSystemFlags, LPWSTR FileSystemNameBuffer, DWORD FileSystemNameSize, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK LockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK MoveFile(LPCWSTR FileName, LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK ReadFile(LPCWSTR FileName, LPVOID Buffer, DWORD BufferLength, LPDWORD ReadLength, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK SetAllocationSize(LPCWSTR FileName, LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK SetEndOfFile(LPCWSTR FileName, LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK SetFileAttributes(LPCWSTR FileName, DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK SetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK SetFileTime(LPCWSTR FileName, CONST FILETIME* CreationTime, CONST FILETIME* LastAccessTime, CONST FILETIME* LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK UnlockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK Mounted(PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK Unmounted(PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK WriteFile(LPCWSTR FileName, LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo);
  static NTSTATUS DOKAN_CALLBACK CreateFile(LPCWSTR FileName, PDOKAN_IO_SECURITY_CONTEXT SecurityContext, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo);
};

