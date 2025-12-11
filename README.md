# LoFS - Unified Filesystem Interface for Meshtastic

**A Meshtastic firmware plugin providing a unified filesystem interface for LittleFS and SD card**

LoFS (LittleFS + SD card File System) provides a unified API that routes file operations to the appropriate backend filesystem based on path prefixes. Write code once and it works seamlessly with both internal flash (LittleFS) and external SD card storage.

## Features

- **Unified API** - Same interface for both LittleFS and SD card
- **Path-based routing** - Use `/lfs/` prefix for LittleFS, `/sd/` prefix for SD card
- **Cross-filesystem operations** - Copy and move files between filesystems transparently
- **Backward compatible** - Paths without prefix default to LittleFS
- **SD card detection** - Automatically handles missing SD cards gracefully
- **Platform agnostic** - Works across ESP32, STM32WL, RP2040, and NRF52

## Installation

### Using Mesh Forge (easy)

Use our [Mesh Forge build profile](https://meshforge.org/builds/new/?plugin=lofs) to flash a LoFS-enabled version of Meshtastic to your device.

### Build it yourself

LoFS is a [Meshtastic plugin](https://meshforge.org/plugins) that is automatically discovered and integrated by the [Mesh Plugin Manager](https://pypi.org/project/mesh-plugin-manager/) (MPM). To install LoFS:

1. **Install the Mesh Plugin Manager:**

```bash
pip install mesh-plugin-manager
```

2. **Install LoFS:**

```bash
cd /path/to/meshtastic/firmware
mpm init
mpm install lofs
```

3. **Build and flash:**

```bash
pio run -e esp32 -t upload
```

## Usage

### Basic Operations

```cpp
#include "LoFS.h"

// Open from LittleFS (internal flash)
File f1 = LoFS::open("/lfs/config/settings.txt", FILE_O_READ);

// Open from SD card
File f2 = LoFS::open("/sd/data/log.txt", FILE_O_WRITE);

// Check if file exists
if (LoFS::exists("/sd/myfile.txt")) {
    // file exists on SD card
}

// Create directory
LoFS::mkdir("/lfs/my/directory");

// Remove file
LoFS::remove("/sd/oldfile.txt");

// Rename file (same filesystem - fast)
LoFS::rename("/lfs/old.txt", "/lfs/new.txt");

// Move file between filesystems (copy + delete)
LoFS::rename("/lfs/source.txt", "/sd/dest.txt");
```

### Path Prefixes

LoFS uses path prefixes to route operations to the correct filesystem:

- **`/lfs/...`** - Routes to LittleFS (internal flash)
- **`/sd/...`** - Routes to SD card (if available)
- **No prefix** - Defaults to LittleFS for backward compatibility

The path prefixes (`/lfs/` and `/sd/`) are string constants that must be used exactly as shown - they are parsed by LoFS to determine which filesystem backend to use.

### LoFS Constants

LoFS provides constants for filesystem type identification:

```cpp
#include "LoFS.h"

// FilesystemType enum constants
LoFS::FilesystemType::LFS  // Value: 0 (LittleFS)
LoFS::FilesystemType::SD    // Value: 1 (SD Card)
```

These constants are available in the `LoFS::FilesystemType` enum and can be used for type checking or conditional logic when working with filesystems programmatically.

When building paths programmatically:

```cpp
const char* lfsPath = "/lfs/data/config.txt";
const char* sdPath = "/sd/data/log.txt";

// Or build paths dynamically
char path[64];
snprintf(path, sizeof(path), "%s%s", "/lfs/", "myfile.txt");
File f = LoFS::open(path, FILE_O_READ);
```

### SD Card Availability

```cpp
// Check if SD card is available before operations
if (LoFS::isSDCardAvailable()) {
    File f = LoFS::open("/sd/data.txt", FILE_O_WRITE);
    // ... use file
} else {
    // Fallback to LittleFS
    File f = LoFS::open("/lfs/data.txt", FILE_O_WRITE);
}
```

### Cross-Filesystem Copy Example

```cpp
// Copy file from LittleFS to SD card
File src = LoFS::open("/lfs/source.txt", FILE_O_READ);
File dst = LoFS::open("/sd/dest.txt", FILE_O_WRITE);

if (src && dst) {
    uint8_t buffer[64];
    while (src.available()) {
        size_t bytesRead = src.read(buffer, sizeof(buffer));
        dst.write(buffer, bytesRead);
    }
    src.close();
    dst.close();
}
```

## API Reference

All methods are static and can be called directly on the `LoFS` class:

- `LoFS::open(filepath, mode)` - Open a file or directory
- `LoFS::exists(filepath)` - Check if file/directory exists
- `LoFS::mkdir(filepath)` - Create directory (with parent directories)
- `LoFS::remove(filepath)` - Delete a file
- `LoFS::rename(oldpath, newpath)` - Rename/move file (works across filesystems)
- `LoFS::rmdir(filepath)` - Remove directory
- `LoFS::isSDCardAvailable()` - Check SD card availability

## Implementation Details

- **LittleFS**: Uses `FSCom` from `FSCommon.h` (platform-specific abstraction)
- **SD Card**: Uses Arduino SD library (when `HAS_SDCARD` is defined)
- **Thread Safety**: All operations use SPI lock for thread safety
- **Error Handling**: Returns `false` or invalid `File` objects on failure
- **Memory**: Automatically manages path buffer allocation/deallocation

## Diagnostic Mode

LoFS includes a comprehensive diagnostic test suite that can be enabled by defining `LOFS_PLUGIN_DIAGNOSTICS` during compilation. When enabled, the diagnostic suite runs automatically at plugin initialization and performs extensive testing of all filesystem operations.

The diagnostic mode will:

- **Test filesystem availability** - Verifies LittleFS and SD card detection
- **Display filesystem space information** - Shows total, used, and free space for both filesystems
- **Test basic file operations** - Write, read, and existence checks on both filesystems
- **Test directory operations** - Create and verify directories
- **Test file size verification** - Validates file sizes across different data sizes (0 bytes to 2KB)
- **Test cross-filesystem operations** - Verifies copy/move operations between LittleFS and SD card
- **Test same-filesystem rename** - Validates rename operations within the same filesystem
- **Test error cases** - Verifies proper handling of invalid paths and missing files
- **Cleanup test files** - Removes all test files created during diagnostics

All test results are logged to the Meshtastic console/log output. This mode is useful for debugging filesystem issues and verifying that LoFS is working correctly on your hardware platform.

## License

LoFS is distributed under the MIT license. See the accompanying `LICENSE` file within this module for full text. While LoFS is MIT, it must be combined with Meshtastic source code which is GPL. You must therefore follow all GPL guidelines regarding the combined source and binary distributions of Meshtastic. The LoFS source code may be distributed independently under MIT.

## Disclaimer

LoFS and MeshForge are independent projects not endorsed by or affiliated with the Meshtastic organization.
