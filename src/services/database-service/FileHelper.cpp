#include	"FileHelper.hh"
#ifdef WIN32
#include <windows.h>
#else
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<cstdlib>
#endif

bool	FileHelper::fileExist(const std::string &path)
{
#ifdef WIN32
  DWORD dwAttrib = GetFileAttributes(path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
  struct stat stats;

  if (stat(path.c_str(), &stats) == -1)
    return (false);
  return (true);
#endif
}

bool	FileHelper::directoryExist(const std::string &path)
{
#ifdef WIN32
  DWORD dwAttrib = GetFileAttributes(path.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
  struct stat stats;

  if (lstat(path.c_str(), &stats) == -1)
    return (false);
  return (true);
#endif
}

bool	FileHelper::createDirectory(const std::string &path)
{
#ifdef WIN32
	if (CreateDirectory(path.c_str(), NULL) == 0)
		return false;
#else
	if (mkdir(path.c_str(), 0777) == -1)
		return false;
#endif
	return true;
}
