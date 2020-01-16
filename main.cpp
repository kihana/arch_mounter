#include "DokanOperations.h"
#include "ArchiveWrapper.h"

#include "dokan/dokan.h"

#include <memory>
#include <iostream>


using namespace std;


unique_ptr<DOKAN_OPERATIONS> create_dokan_operations()
{
  unique_ptr<DOKAN_OPERATIONS> dokan_operations(new DOKAN_OPERATIONS);
  if (nullptr == dokan_operations)
    throw exception("Failed to create DOKAN_OPERATIONS");

  memset(dokan_operations.get(), 0, sizeof(DOKAN_OPERATIONS));

  dokan_operations->Cleanup = DokanOperations::Cleanup;
  dokan_operations->CloseFile = DokanOperations::CloseFile;
  dokan_operations->DeleteDirectory = DokanOperations::DeleteDirectory;
  dokan_operations->DeleteFile = DokanOperations::DeleteFile;
  dokan_operations->FindFiles = DokanOperations::FindFiles;
  dokan_operations->FindFilesWithPattern = DokanOperations::FindFilesWithPattern;
  dokan_operations->FindStreams = DokanOperations::FindStreams;
  dokan_operations->FlushFileBuffers = DokanOperations::FlushFileBuffers;
  dokan_operations->GetDiskFreeSpace = DokanOperations::GetDiskFreeSpace;
  dokan_operations->GetFileInformation = DokanOperations::GetFileInformation;
  dokan_operations->GetFileSecurity = DokanOperations::GetFileSecurity;
  dokan_operations->GetVolumeInformation = DokanOperations::GetVolumeInformation;
  dokan_operations->LockFile = DokanOperations::LockFile;
  dokan_operations->Mounted = DokanOperations::Mounted;
  dokan_operations->MoveFile = DokanOperations::MoveFile;
  dokan_operations->ReadFile = DokanOperations::ReadFile;
  dokan_operations->SetAllocationSize = DokanOperations::SetAllocationSize;
  dokan_operations->SetEndOfFile = DokanOperations::SetEndOfFile;
  dokan_operations->SetFileAttributes = DokanOperations::SetFileAttributes;
  dokan_operations->SetFileSecurity = DokanOperations::SetFileSecurity;
  dokan_operations->SetFileTime = DokanOperations::SetFileTime;
  dokan_operations->UnlockFile = DokanOperations::UnlockFile;
  dokan_operations->Unmounted = DokanOperations::Unmounted;
  dokan_operations->WriteFile = DokanOperations::WriteFile;
  dokan_operations->ZwCreateFile = DokanOperations::CreateFile;

  return dokan_operations;
}

unique_ptr<DOKAN_OPTIONS> create_dokan_options(const wstring& mount_point)
{
  unique_ptr<DOKAN_OPTIONS> dokan_options(new DOKAN_OPTIONS);
  if (nullptr == dokan_options)
    throw exception("Failed to create DOKAN_OPTIONS");

  memset(dokan_options.get(), 0, sizeof(DOKAN_OPTIONS));

  dokan_options->Version = DOKAN_VERSION;
  dokan_options->ThreadCount = 0;
  dokan_options->GlobalContext = 0;
  dokan_options->MountPoint = mount_point.c_str();
  dokan_options->Timeout = 0;
  dokan_options->AllocationUnitSize = 0;
  dokan_options->SectorSize = 0;

  dokan_options->Options |= DOKAN_OPTION_STDERR;
  //dokan_options->Options |= DOKAN_OPTION_DEBUG;
  dokan_options->Options |= DOKAN_OPTION_ALT_STREAM;

  return dokan_options;
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
      return EXIT_FAILURE;

    const string archive_path = argv[1];
    const string mount_point = argv[2];

    const wstring a_p(begin(archive_path), end(archive_path));
    const wstring m_p(begin(mount_point), end(mount_point));

    ArchiveWrapper& archive_wrapper = ArchiveWrapper::instance();
    //if (!archive_wrapper.build_tree(L"d:\\temp\\libarchive-3.4.0.zip"))
    if (!archive_wrapper.build_tree(a_p))
      throw exception("Failed to build tree struct of archive.");

    unique_ptr<DOKAN_OPERATIONS> dokan_operations = create_dokan_operations();
    unique_ptr<DOKAN_OPTIONS> dokan_options = create_dokan_options(m_p);

    int status = DokanMain(dokan_options.get(), dokan_operations.get());
    switch (status)
    {
      case DOKAN_SUCCESS:
        cerr << "Success" << endl;
        break;

      case DOKAN_ERROR:
        cerr << "Error" << endl;
        break;

      case DOKAN_DRIVE_LETTER_ERROR:
        cerr << "Bad Drive letter" << endl;
        break;

      case DOKAN_DRIVER_INSTALL_ERROR:
        cerr << "Can't install driver" << endl;
        break;

      case DOKAN_START_ERROR:
        cerr << "Driver something wrong" << endl;
        break;

      case DOKAN_MOUNT_ERROR:
        cerr << "Can't assign a drive letter" << endl;
        break;

      case DOKAN_MOUNT_POINT_ERROR:
        cerr << "Mount point error" << endl;
        break;

      case DOKAN_VERSION_ERROR:
        cerr << "Version error" << endl;
        break;

      default:
        fprintf(stderr, "Unknown error: %d\n", status);
        break;
    }
  }
  catch (const exception & exception)
  {
    cout << exception.what() << endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}