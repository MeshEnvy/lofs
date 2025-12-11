#pragma once

#include "FSCommon.h"
#include "configuration.h"
#include <Stream.h>

#if defined(HAS_SDCARD) && !defined(SDCARD_USE_SOFT_SPI)
#include <SD.h>
#include <FS.h>
// SD library uses FILE_READ and FILE_WRITE constants
#ifndef FILE_READ
#define FILE_READ O_READ
#endif
#ifndef FILE_WRITE
#define FILE_WRITE O_WRITE
#endif
#endif

/**
 * @brief Unified filesystem interface that routes paths to appropriate backends
 * 
 * Supports path prefixes:
 * - /lfs/... -> routes to LittleFS (internal flash)
 * - /sd/...  -> routes to SD card (if available and HAS_SDCARD is defined)
 * 
 * Paths without prefix default to LittleFS for backward compatibility.
 * 
 * Usage examples:
 *   // Open from LittleFS
 *   File f1 = LoFS::open("/lfs/config/settings.txt", FILE_O_READ);
 *   
 *   // Open from SD card
 *   File f2 = LoFS::open("/sd/data/log.txt", FILE_O_WRITE);
 *   
 *   // Check existence
 *   if (LoFS::exists("/sd/myfile.txt")) {
 *       // file exists on SD card
 *   }
 *   
 *   // Create directory
 *   LoFS::mkdir("/lfs/my/directory");
 *   
 *   // Copy between filesystems
 *   File src = LoFS::open("/lfs/source.txt", FILE_O_READ);
 *   File dst = LoFS::open("/sd/dest.txt", FILE_O_WRITE);
 *   // ... copy data ...
 * 
 * Note: Both filesystems use the same File interface, so you can use
 * the returned File objects interchangeably for reading/writing.
 */
class LoFS
{
  public:
    /**
     * @brief Open a file or directory
     * @param filepath Path with prefix (/lfs/... or /sd/...)
     * @param mode File mode (uint8_t: 0=read, 1=write for STM32WL/NRF52)
     * @return File object (platform-specific type)
     */
    static File open(const char *filepath, uint8_t mode);

    /**
     * @brief Open a file or directory (string mode version)
     * @param filepath Path with prefix (/lfs/... or /sd/...)
     * @param mode File mode string ("r" for read, "w" for write on ESP32/RP2040)
     * @return File object (platform-specific type)
     */
    static File open(const char *filepath, const char *mode);

    /**
     * @brief Check if file or directory exists
     * @param filepath Path with prefix
     * @return true if exists
     */
    static bool exists(const char *filepath);

    /**
     * @brief Create a directory
     * @param filepath Path with prefix
     * @return true if successful
     */
    static bool mkdir(const char *filepath);

    /**
     * @brief Remove a file
     * @param filepath Path with prefix
     * @return true if successful
     */
    static bool remove(const char *filepath);

    /**
     * @brief Rename a file (or move between filesystems)
     * @param oldfilepath Source path with prefix
     * @param newfilepath Destination path with prefix
     * @return true if successful
     * 
     * If both paths are on the same filesystem, performs a simple rename.
     * If paths are on different filesystems (e.g., /lfs/file -> /sd/file),
     * performs a copy + delete operation.
     */
    static bool rename(const char *oldfilepath, const char *newfilepath);

    /**
     * @brief Remove a directory
     * @param filepath Path with prefix
     * @return true if successful
     */
    static bool rmdir(const char *filepath);

    /**
     * @brief Check if SD card is available (compile-time and runtime check)
     * @return true if SD card is supported and present, false otherwise
     * 
     * This function works even when HAS_SDCARD is false (returns false).
     * Useful for checking SD card availability before attempting operations.
     */
    static bool isSDCardAvailable();

    /**
     * @brief Get total space in bytes for the filesystem
     * @param filepath Path with prefix (/lfs/... or /sd/...) - prefix determines filesystem
     * @return Total space in bytes, or 0 if filesystem is invalid/unavailable
     */
    static uint64_t totalBytes(const char *filepath);

    /**
     * @brief Get used space in bytes for the filesystem
     * @param filepath Path with prefix (/lfs/... or /sd/...) - prefix determines filesystem
     * @return Used space in bytes, or 0 if filesystem is invalid/unavailable
     */
    static uint64_t usedBytes(const char *filepath);

    /**
     * @brief Get free space in bytes for the filesystem
     * @param filepath Path with prefix (/lfs/... or /sd/...) - prefix determines filesystem
     * @return Free space in bytes (totalBytes - usedBytes), or 0 if filesystem is invalid/unavailable
     */
    static uint64_t freeBytes(const char *filepath);

    /**
     * @brief Filesystem type constants for specifying which filesystem to use
     */
    enum FilesystemType {
        LFS = 0,  ///< LittleFS (internal flash)
        SD = 1    ///< SD Card (if available)
    };

  private:
    enum class FSType { LFS, SD, INVALID };

    /**
     * @brief Parse path prefix and return filesystem type and stripped path
     * @param filepath Full path with prefix
     * @param strippedPath Output buffer for path without prefix (must be at least strlen(filepath)+1)
     * @return Filesystem type
     */
    static FSType parsePath(const char *filepath, char *strippedPath, size_t bufferSize);

    /**
     * @brief Get stripped path (helper that allocates buffer)
     */
    static FSType parsePath(const char *filepath, char **strippedPath);
};
