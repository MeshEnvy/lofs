# Changelog

All notable changes to LoFS will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Changed

- Public headers live under **`include/lofs/`** — use **`#include <lofs/LoFS.h>`** (or **`#include <lofs/lofs.h>`**); the implementation stays in **`src/LoFS.cpp`**.
- Ship as a **PlatformIO library** (`library.json`); install via `lib_deps` instead of Mesh Plugin Manager.
- Remove Meshtastic module / MPM surface (`plugin.h`, `MPM_REGISTER_MESHTASTIC_MODULE`).
- Add `LOFS_VERSION` in `LoFS.h`.

### Removed

- `LoFSModule.{h,cpp}` and `diagnostics.cpp` (Meshtastic module / optional test hook; not part of the library API).

### Minor
- Changed path prefix from `/lfs/` to `/internal/` to better reflect that it represents the internal filesystem abstraction (FSCommon), not specifically LittleFS
- Added recursive directory removal support to `LoFS::rmdir()` with optional `recursive` parameter
- Initial release of LoFS (LittleFS + SD card File System)
- Unified filesystem interface for LittleFS and SD card
- Path-based routing with `/internal/` and `/sd/` prefixes
- Cross-filesystem file operations (copy, move, rename)
- SD card availability detection
- Platform support for ESP32, STM32WL, RP2040, and NRF52

## [0.0.1]

Initial release