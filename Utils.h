#pragma once


#include <windows.h>

#include <string>


namespace utils
{
  const wchar_t PATH_DELIMITER = L'/';

  std::wstring filetime_to_string(const FILETIME& filetime);
  FILETIME time_t_to_filetime(time_t time);
};

