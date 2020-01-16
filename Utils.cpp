#include "Utils.h"

#include <windows.h>


using namespace std;


namespace utils
{
  wstring filetime_to_string(const FILETIME& filetime)
  {
    SYSTEMTIME systemtime;
    FILETIME local_filetime;
    wchar_t szLocalDate[255], szLocalTime[255];

    FileTimeToLocalFileTime(&filetime, &local_filetime);
    FileTimeToSystemTime(&local_filetime, &systemtime);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &systemtime, nullptr, szLocalDate, 255);
    GetTimeFormat(LOCALE_USER_DEFAULT, 0, &systemtime, nullptr, szLocalTime, 255);

    wstring result = szLocalDate;
    result += wstring(L" ") + szLocalTime;

    return result;
  }

  FILETIME time_t_to_filetime(time_t time)
  {
    FILETIME file_time = { 0, 0 };

    unsigned long long nanoseconds = time * 10000000 + 116444736000000000;
    file_time.dwLowDateTime = static_cast<DWORD>(nanoseconds);
    file_time.dwHighDateTime = nanoseconds >> 32;

    return file_time;
  }
}
