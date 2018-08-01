
#pragma once


#include <liblocate/liblocate_api.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
*  @brief
*    Get path to the current executable
*
*  @param[out] path
*    Path to executable (including filename)
*  @param[out] pathLength
*    Number of characters of path without null byte
*
*  @remark
*    The path is returned in native format, e.g., backslashes on Windows.
*
*  @remark
*    The caller takes memory ownership over *path.
*/
LIBLOCATE_API void getExecutablePath(char ** path, unsigned int * pathLength);

/**
*  @brief
*    Get path to the current application bundle
*
*  @param[out] path
*    Path to bundle (including filename)
*  @param[out] pathLength
*    Number of characters of path without null byte
*
*  @remark
*    The path is returned in unified format (forward slashes).
*
*  @remark
*    If the current executable is part of a macOS application bundle,
*    this function returns the part to the bundle. Otherwise, an
*    empty string is returned.
*
*  @remark
*    The caller takes memory ownership over *path.
*/
LIBLOCATE_API void getBundlePath(char ** path, unsigned int * pathLength);

/**
*  @brief
*    Get path to the current module
*
*  @param[out] path
*    Path to module (directory in which the executable is located)
*  @param[out] pathLength
*    Number of characters of path without null byte
*
*  @remark
*    The path is returned in unified format (forward slashes).
*
*  @remark
*    The caller takes memory ownership over *path.
*/
LIBLOCATE_API void getModulePath(char ** path, unsigned int * pathLength);

/**
*  @brief
*    Get path to dynamic library
*
*  @param[in] symbol
*    A symbol from the library, e.g., a function or variable pointer
*  @param[out] path
*    Path to library (including filename)
*  @param[out] pathLength
*    Length of path
*
*  @remark
*    The path is returned in unified format (forward slashes).
*
*  @remark
*    If symbol is null pointer, an empty string is returned.
*
*  @remark
*    The caller takes memory ownership over *path.
*/
LIBLOCATE_API void getLibraryPath(void * symbol, char ** path, unsigned int * pathLength);

/**
*  @brief
*    Locate path to a file or directory
*
*  @param[out] path
*    Path to file or directory
*  @param[out] pathLength
*    Length of path
*  @param[in] relPath
*    Relative path to a file or directory (e.g., 'data/logo.png')
*  @param[in] relPathLength
*    Length of relPath
*  @param[in] systemDir
*    Subdirectory for system installs (e.g., 'share/myappname')
*  @param[in] systemDirLength
*    Length of systemDir
*  @param[in] symbol
*    A symbol from the library, e.g., a function or variable pointer
*
*  @remark
*    This function tries to locate the named file or directory based
*    on the location of the current executable or library. If the
*    file or directory could be found, the base path from which the
*    relative path can be resolved is returned. Otherwise, an empty
*    string is returned.
*
*  @remark
*    The path is returned in unified format (forward slashes).
*
*  @remark
*    The caller takes memory ownership over *path.
*/
LIBLOCATE_API void locatePath(char ** path, unsigned int * pathLength, const char * relPath, unsigned int relPathLength,
    const char * systemDir, unsigned int systemDirLength, void * symbol);


#ifdef __cplusplus
}
#endif
