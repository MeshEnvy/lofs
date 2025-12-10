# Changelog

All notable changes to LoFS will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2025-01-XX

### Major
- Initial release of LoFS (LittleFS + SD card File System)
- Unified filesystem interface for LittleFS and SD card
- Path-based routing with `/lfs/` and `/sd/` prefixes
- Cross-filesystem file operations (copy, move, rename)
- SD card availability detection
- Platform support for ESP32, STM32WL, RP2040, and NRF52
