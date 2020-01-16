#include "Helpers.h"


using namespace std;


namespace helpers
{
  AutoMutex::AutoMutex(const wstring& name)
  {
    handle_ = CreateMutex(nullptr, FALSE, name.c_str());
    if (nullptr != handle_)
      DWORD wait_result = WaitForSingleObject(handle_, INFINITE);
  }

  AutoMutex::~AutoMutex()
  {
    ReleaseMutex(handle_);
    CloseHandle(handle_);
  }
}