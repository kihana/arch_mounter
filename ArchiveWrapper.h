#pragma once

#include "ArchiveItem.h"


struct archive;


class ArchiveWrapper
{
public:
  static ArchiveWrapper& instance();

  bool build_tree(const std::wstring& archive_path);
  ArchiveItem* get_item(const std::wstring& path);
  unsigned long read(const std::wstring& path, void* buffer, const unsigned long size, const long long offset);
  
  ArchiveWrapper(const ArchiveWrapper& other) = delete;
  ArchiveWrapper(const ArchiveWrapper&& other) = delete;
  ArchiveWrapper& operator=(const ArchiveWrapper& other) = delete;
  ArchiveWrapper& operator=(const ArchiveWrapper&& other) = delete;

private:
  ArchiveWrapper();

  void add_item(const std::wstring& path, archive_entry* entry, ArchiveItem& parent);
  ArchiveItem* get_item(const std::wstring& full_path, ArchiveItem* parent);

private:
  ArchiveItem root_;
  std::wstring archive_path_;
};

