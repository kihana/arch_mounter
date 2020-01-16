#pragma once

#include <windows.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>

struct archive_entry;


struct ArchiveItem
{
  ArchiveItem(const std::wstring& name, archive_entry* entry);

  std::wstring name;
  DWORD attributes;
  DWORD size;
  FILETIME creation_time;
  FILETIME modification_time;
  FILETIME last_access_time;
  std::unordered_map<std::wstring, std::unique_ptr<ArchiveItem>> children;
  std::vector<std::byte> data;
};

