#include "ArchiveItem.h"
#include "Utils.h"

#include "archive_entry.h"


using namespace std;


ArchiveItem::ArchiveItem(const wstring& name, archive_entry* entry) :
  name(name)
  , size(0)
  , creation_time({ 0, 0 })
  , modification_time({ 0, 0 })
  , last_access_time({ 0, 0 })
{
  if (entry != nullptr)
  {
    const struct stat* stat = archive_entry_stat(entry);

    if ((stat->st_mode & S_IFMT) == S_IFREG)
      attributes = FILE_ATTRIBUTE_NORMAL;
    else if ((stat->st_mode & S_IFMT) == S_IFDIR)
      attributes = FILE_ATTRIBUTE_DIRECTORY;
    else
      attributes = FILE_ATTRIBUTE_NORMAL;

    size = stat->st_size;
    creation_time = utils::time_t_to_filetime(stat->st_ctime);
    modification_time = utils::time_t_to_filetime(stat->st_mtime);
    last_access_time = utils::time_t_to_filetime(stat->st_atime);
  }
  else
    attributes = FILE_ATTRIBUTE_DIRECTORY;  
}