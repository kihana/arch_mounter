#include "ArchiveWrapper.h"
#include "Logger.h"
#include "Utils.h"

#include "archive.h"
#include "archive_entry.h"

#include <fstream>
#include <filesystem>
#include <memory>
#include <algorithm>


using namespace std;
namespace fs = std::filesystem;

ArchiveWrapper::ArchiveWrapper() :
  root_(&utils::PATH_DELIMITER, nullptr)
{}

ArchiveWrapper& ArchiveWrapper::instance()
{
  static ArchiveWrapper archive;

  return archive;
}

bool ArchiveWrapper::build_tree(const wstring& archive_path)
{
  archive_path_ = archive_path;
  auto& logger = Logger::instance();
  bool result = true;

  archive* a = archive_read_new();
  try
  {
    if (nullptr == a)
      throw exception("Failed to create archive struct.");
    
    if (ARCHIVE_OK != archive_read_support_filter_all(a))
      throw exception("Failed to initialize filter.");

    if (ARCHIVE_OK != archive_read_support_format_all(a))
      throw exception("Failed to initialize format.");

    if (ARCHIVE_OK != archive_read_open_filename_w(a, archive_path.c_str(), 10240))
      throw exception("Failed to open archive.");

    archive_entry* entry = nullptr;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
      const wstring pathname = archive_entry_pathname_w(entry);
      add_item(pathname, entry, root_);

      if (ARCHIVE_FATAL == archive_read_data_skip(a))
        break;
    }

    if (0 != archive_errno(a))
      throw exception("Failed to read archive correctly.");

  }
  catch (const exception& e)
  {
    logger->debug(e.what());
    result = false;
  }

  if (nullptr != a && ARCHIVE_OK != archive_read_free(a))
    logger->debug("Error during releasing archive resources. {}", archive_error_string(a));


  return result;
}

ArchiveItem* ArchiveWrapper::get_item(const wstring& path)
{
  return get_item(path, &root_);
}

void ArchiveWrapper::add_item(const std::wstring& path, archive_entry* entry, ArchiveItem& parent)
{
  size_t pos = path.find(utils::PATH_DELIMITER);

  if (wstring::npos != pos)
  {
    wstring name = path.substr(0, pos);
    if (end(parent.children) == parent.children.find(name))
      parent.children.insert({ name, make_unique<ArchiveItem>(name, (pos + 1) == path.length() ? entry : nullptr) });

    if ((pos + 1) != path.length())
    {
      wstring tail = path.substr(pos + 1);
      add_item(tail, entry, *parent.children.at(name));
    }
  }
  else
    parent.children.insert({ path, make_unique<ArchiveItem>(path, entry) });
}

ArchiveItem* ArchiveWrapper::get_item(const wstring& full_path, ArchiveItem* parent)
{
  if (full_path.empty())
    return nullptr;

  if (parent->name == full_path)
    return parent;

  wstring path = full_path;
  if (path.front() == utils::PATH_DELIMITER)
    path = path.substr(1);

  size_t pos = path.find(utils::PATH_DELIMITER);
  if (wstring::npos == pos)
    return (end(parent->children) == parent->children.find(path)) ? nullptr : parent->children.at(path).get();

  wstring folder_name = path.substr(0, pos);
  if (end(parent->children) == parent->children.find(folder_name))
    return nullptr;

  return get_item(path.substr(pos), parent->children.at(folder_name).get());
}

unsigned long ArchiveWrapper::read(const wstring& path, void* buffer, const unsigned long size, const long long offset)
{
  auto& logger = Logger::instance();
  
  ArchiveItem* item = get_item(path);
  if (item->data.empty())
  {
    archive* a = archive_read_new();
    try
    {
      if (nullptr == a)
        throw exception("Failed to create archive struct.");

      if (ARCHIVE_OK != archive_read_support_filter_all(a))
        throw exception("Failed to initialize filter.");

      if (ARCHIVE_OK != archive_read_support_format_all(a))
        throw exception("Failed to initialize format.");

      if (ARCHIVE_OK != archive_read_open_filename_w(a, archive_path_.c_str(), 10240))
        throw exception("Failed to open archive.");

      archive_entry* entry = nullptr;
      while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
      {
        const wstring pathname = wstring(&utils::PATH_DELIMITER) + archive_entry_pathname_w(entry);
        if (path == pathname)
          break;

        if (ARCHIVE_FATAL == archive_read_data_skip(a))
          break;
      }

      const void* buff;
      size_t size;
      la_int64_t offset;
      int result;

      while ((result = archive_read_data_block(a, &buff, &size, &offset)) != ARCHIVE_EOF)
      {
        if (ARCHIVE_FATAL == result)
          break;

        if (ARCHIVE_OK == result)
        {
          const std::byte* bytes = reinterpret_cast<const std::byte*>(buff);
          item->data.insert(end(item->data), bytes, bytes + size);
        }
      }

      if (0 != archive_errno(a))
        throw exception("Failed to read archive correctly.");
    }
    catch (const exception & e)
    {
      logger->debug(e.what());
    }

    if (nullptr != a && ARCHIVE_OK != archive_read_free(a))
      logger->debug("Error during releasing archive resources. {}", archive_error_string(a));
  }
  
  unsigned long size_read_buffer = min(size, static_cast<unsigned long>(item->data.size() - offset));
  auto start = begin(item->data) + offset;
  copy(start, start + size_read_buffer, reinterpret_cast<std::byte*>(buffer));
  
  return size_read_buffer;
}