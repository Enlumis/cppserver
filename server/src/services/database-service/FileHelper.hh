#pragma once
# include <string>

class FileHelper
{
public:
	static bool	fileExist(const std::string &path);
	static bool	directoryExist(const std::string &path);
	static bool	createDirectory(const std::string &path);
};
