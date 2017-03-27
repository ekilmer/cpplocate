
#include <cpplocate/utils.h>

#include <algorithm>
#include <sstream>

#ifdef SYSTEM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else
    #include <sys/stat.h>
#endif

#include <cpplocate/ModuleInfo.h>


namespace
{


#ifdef SYSTEM_WINDOWS
    const char pathDelim = '\\';
    const char pathsDelim = ';';
#else
    const char pathDelim = '/';
    const char pathsDelim = ':';
#endif


} // namespace


namespace cpplocate
{


namespace utils
{


void replace(std::string & str, const std::string & original, const std::string & substitute)
{
    // Count number of substring occurrences
    auto pos = str.find(original, 0);
    auto count = size_t(0);

    while (pos != std::string::npos)
    {
        ++count;
        pos += original.size();

        pos = str.find(original, pos);
    }

    auto result = std::string(str.size() + count * (substitute.size() - original.size()), 0);

    // Build string with replacements
    auto lastPos = size_t(0);
    /*auto*/ pos = str.find(original, 0);
    auto current = result.begin();

    while (pos != std::string::npos)
    {
        std::copy(str.begin()+lastPos, str.begin()+pos, current);

        current += pos - lastPos;

        std::copy(substitute.begin(), substitute.end(), current);

        current += substitute.size();
        pos += original.size();
        lastPos = pos;

        pos = str.find(original, lastPos);
    }

    std::copy(str.begin()+lastPos, str.end(), current);

    // Swap
    std::swap(str, result);
}

void trim(std::string & str)
{
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
}

std::string trimPath(const std::string & path)
{
    return trimPath(std::string(path));
}

std::string trimPath(std::string && path)
{
    auto trimmed = std::move(path);

    trimmed.erase(0, trimmed.find_first_not_of(' '));
    trimmed.erase(trimmed.find_last_not_of(' ') + 1);
    trimmed.erase(trimmed.find_last_not_of(pathDelim) + 1);

    return trimmed;
}

std::string unifiedPath(const std::string & path)
{
    return unifiedPath(std::string(path));
}

std::string unifiedPath(std::string && path)
{
    std::string str = std::move(path);
    std::replace(str.begin(), str.end(), '\\', '/');

    return str;
}

std::string getDirectoryPath(const std::string & fullpath)
{
    if (fullpath.empty())
    {
        return "";
    }

    auto pos           = fullpath.rfind("/");
    const auto posBack = fullpath.rfind("\\");

    if (pos == std::string::npos || (posBack != std::string::npos && posBack > pos))
    {
        pos = posBack;
    }

    return fullpath.substr(0, pos);
}

size_t posAfterString(const std::string & str, const std::string & substr)
{
    size_t pos = str.rfind(substr);

    if (pos != std::string::npos)
    {
        pos += substr.size();
    }

    return pos;
}

std::string getSystemBasePath(const std::string & path)
{
    size_t pos;

    if ((pos = posAfterString(path, "/usr/bin/")) != std::string::npos)
    {
        return path.substr(0, pos - 4);
    }

    else if ((pos = posAfterString(path, "/usr/local/bin/")) != std::string::npos)
    {
        return path.substr(0, pos - 4);
    }

    else if ((pos = posAfterString(path, "/usr/lib/")) != std::string::npos)
    {
        return path.substr(0, pos - 4);
    }

    else if ((pos = posAfterString(path, "/usr/lib32/")) != std::string::npos)
    {
        return path.substr(0, pos - 6);
    }

    else if ((pos = posAfterString(path, "/usr/lib64/")) != std::string::npos)
    {
        return path.substr(0, pos - 6);
    }

    else if ((pos = posAfterString(path, "/usr/local/lib/")) != std::string::npos)
    {
        return path.substr(0, pos - 4);
    }

    else if ((pos = posAfterString(path, "/usr/local/lib32/")) != std::string::npos)
    {
        return path.substr(0, pos - 6);
    }

    else if ((pos = posAfterString(path, "/usr/local/lib64/")) != std::string::npos)
    {
        return path.substr(0, pos - 6);
    }

    return "";
}

void split(const std::string & str, const char delim, std::vector<std::string> & values)
{
    std::stringstream stream(str);

    std::string item;
    while (std::getline(stream, item, delim))
    {
        if (!item.empty())
        {
            values.push_back(item);
        }
    }
}

void getPaths(const std::string & paths, std::vector<std::string> & values)
{
    split(paths, pathsDelim, values);
}

std::string getEnv(const std::string & name)
{
    const auto value = std::getenv(name.c_str());

    return value ? std::string(value) : std::string();
}

bool fileExists(const std::string & path)
{
#ifdef SYSTEM_WINDOWS

    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    return (GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fileInfo) != 0);

#else

    struct stat fileInfo;
    return (stat(path.c_str(), &fileInfo) == 0);

#endif
}

bool loadModule(const std::string & directory, const std::string & name, ModuleInfo & info)
{
    // Validate directory path
    const auto dir = trimPath(directory);

    // Try to load module
    const auto path = dir + pathDelim + name + ".modinfo";

    return info.load(path);
}


} // namespace utils


} // namespace cpplocate
