# LoFS - Unified filesystem interface for embedded firmware

**A PlatformIO / Arduino library** that routes file operations to internal flash (via `FSCommon`) and optional SD card using path prefixes.

LoFS (LittleFS + SD card file system) provides one API for both backends. Paths with `/internal/` use onboard storage; paths with `/sd/` use the SD card when `HAS_SDCARD` is enabled and hardware is present.

## Features

- **Unified API** — Same calls for internal flash and SD
- **Path-based routing** — `/internal/…` and `/sd/…` prefixes
- **Cross-filesystem operations** — Rename/move across backends (copy + delete when needed)
- **Unprefixed paths** — Treated as internal filesystem for compatibility
- **SD detection** — Missing or unsupported SD is handled safely
- **Targets** — Intended for ESP32, STM32WL, RP2040, and nRF52–class Arduino builds

## Installation (PlatformIO)

Add LoFS as a dependency of your firmware project (any PlatformIO `arduino` environment that provides `FSCommon.h` / `configuration.h` from your tree).

**From Git (pinned ref recommended):**

```ini
lib_deps =
  https://github.com/MeshEnvy/lofs.git#v0.2.0
```

Or track a branch:

```ini
lib_deps =
  https://github.com/MeshEnvy/lofs.git#main
```

**From the PlatformIO Registry** (after you publish with `pio pkg publish`):

```ini
lib_deps =
  <publisher>/LoFS @ ^0.1.0
```

Use the exact `publisher/name` shown on [registry.platformio.org](https://registry.platformio.org/).

Then install and build as usual:

```bash
pio pkg install -e <your_env>
pio run -e <your_env>
```

The library manifest is [`library.json`](library.json). Version in `library.json` should match `LOFS_VERSION` in [`include/lofs/LoFS.h`](include/lofs/LoFS.h).

## Usage

Include the public header (no `src/` path):

```cpp
#include <lofs/LoFS.h>
```

Then use static methods on `LoFS`.

### Basic operations

```cpp
#include <lofs/LoFS.h>

File f1 = LoFS::open("/internal/config/settings.txt", FILE_O_READ);
File f2 = LoFS::open("/sd/data/log.txt", FILE_O_WRITE);

if (LoFS::exists("/sd/myfile.txt")) {
  // ...
}

LoFS::mkdir("/internal/my/directory");
LoFS::remove("/sd/oldfile.txt");
LoFS::rename("/internal/old.txt", "/internal/new.txt");
LoFS::rename("/internal/source.txt", "/sd/dest.txt");
```

### Path prefixes

- **`/internal/…`** — Internal filesystem (`FSCom` / `FSCommon`)
- **`/sd/…`** — SD card (when built with SD support and card present)
- **No prefix** — Internal filesystem

### `FSType` enum

```cpp
LoFS::FSType::AUTO
LoFS::FSType::INTERNAL
LoFS::FSType::SD
LoFS::FSType::INVALID
```

Useful when building paths or integrating with other code (e.g. LoDB) that needs an explicit backend choice.

### SD availability

```cpp
if (LoFS::isSDCardAvailable()) {
  File f = LoFS::open("/sd/data.txt", FILE_O_WRITE);
} else {
  File f = LoFS::open("/internal/data.txt", FILE_O_WRITE);
}
```

Operations **lazy-initialize** as needed (including SD on first access to `/sd/…` or when you call `isSDCardAvailable()`).

## API summary

| Method | Description |
|--------|-------------|
| `LoFS::open(path, mode)` | Open file or directory |
| `LoFS::exists(path)` | Existence check |
| `LoFS::mkdir(path)` | Create directory |
| `LoFS::remove(path)` | Delete file |
| `LoFS::rename(old, new)` | Rename or cross-filesystem move |
| `LoFS::rmdir(path, recursive)` | Remove directory |
| `LoFS::isSDCardAvailable()` | SD present / supported |
| `LoFS::totalBytes` / `usedBytes` / `freeBytes` | Space stats by path prefix |

## Implementation notes

- **Internal storage:** Uses `FSCom` from `FSCommon.h` (provided by the host firmware tree).
- **SD:** Arduino `SD` library when `HAS_SDCARD` is defined and soft-SPI is not used.
- **Concurrency:** SPI lock is used where appropriate for SD access.
- **Build context:** This library expects your firmware to supply Meshtastic-compatible headers and defines (`configuration.h`, `FSCommon.h`, `SPILock.h`, etc.).

## License

LoFS is distributed under the MIT license. See `LICENSE`. Combined Meshtastic firmware remains under GPL; follow GPL requirements for combined works.

## Disclaimer

LoFS and MeshForge are independent projects not endorsed by or affiliated with the Meshtastic organization.
