#pragma once


#include <windows.h>

#include <string>


namespace helpers
{
  class AutoMutex
  {
  public:
    AutoMutex(const std::wstring& name);
    ~AutoMutex();

  private:
    HANDLE handle_;
  };
}
