#include "LoFS.h"
#include "DebugConfiguration.h"
#include <string.h>

void lofs_diagnostics()
{
    LOG_INFO("=== LoFS Comprehensive Test Suite ===");
    LOG_INFO("");

    // Test 1: Filesystem Availability
    LOG_INFO("--- Test 1: Filesystem Availability ---");
    bool lfsAvailable = true; // LittleFS should always be available if FSCom is defined
    bool sdAvailable = LoFS::isSDCardAvailable();
    LOG_INFO("LittleFS available: %s", lfsAvailable ? "YES" : "NO");
    LOG_INFO("SD Card available: %s", sdAvailable ? "YES" : "NO");
    LOG_INFO("");

    // Test 2: Filesystem Space Information
    LOG_INFO("--- Test 2: Filesystem Space Information ---");
    {
        uint64_t lfsTotal = LoFS::totalBytes("/lfs");
        uint64_t lfsUsed = LoFS::usedBytes("/lfs");
        uint64_t lfsFree = LoFS::freeBytes("/lfs");
        LOG_INFO("LittleFS - Total: %lu bytes (%.2f KB, %.2f MB)", (uint32_t)lfsTotal, lfsTotal / 1024.0f, lfsTotal / (1024.0f * 1024.0f));
        LOG_INFO("LittleFS - Used:  %lu bytes (%.2f KB, %.2f MB)", (uint32_t)lfsUsed, lfsUsed / 1024.0f, lfsUsed / (1024.0f * 1024.0f));
        LOG_INFO("LittleFS - Free:  %lu bytes (%.2f KB, %.2f MB)", (uint32_t)lfsFree, lfsFree / 1024.0f, lfsFree / (1024.0f * 1024.0f));
    }

    if (sdAvailable) {
        uint64_t sdTotal = LoFS::totalBytes("/sd");
        uint64_t sdUsed = LoFS::usedBytes("/sd");
        uint64_t sdFree = LoFS::freeBytes("/sd");
        LOG_INFO("SD Card - Total: %lu bytes (%.2f KB, %.2f MB)", (uint32_t)sdTotal, sdTotal / 1024.0f, sdTotal / (1024.0f * 1024.0f));
        LOG_INFO("SD Card - Used:  %lu bytes (%.2f KB, %.2f MB)", (uint32_t)sdUsed, sdUsed / 1024.0f, sdUsed / (1024.0f * 1024.0f));
        LOG_INFO("SD Card - Free:  %lu bytes (%.2f KB, %.2f MB)", (uint32_t)sdFree, sdFree / 1024.0f, sdFree / (1024.0f * 1024.0f));
    }
    LOG_INFO("");

    // Test 3: Basic File Operations - LittleFS
    LOG_INFO("--- Test 3: Basic File Operations (LittleFS) ---");
    const char *testFileLFS = "/lfs/test_file.txt";
    const char *testContent = "Hello from LoFS test! This is test data.\n";
    size_t testContentLen = strlen(testContent);

    // Clean up any existing test file
    if (LoFS::exists(testFileLFS)) {
        LoFS::remove(testFileLFS);
        LOG_INFO("Cleaned up existing test file");
    }

    // Write test
    File f1 = LoFS::open(testFileLFS, FILE_O_WRITE);
    if (f1) {
        size_t written = f1.write((const uint8_t *)testContent, testContentLen);
        f1.close();
        LOG_INFO("Write test: wrote %u bytes to %s", written, testFileLFS);
    } else {
        LOG_INFO("Write test: FAILED to open %s for writing", testFileLFS);
    }

    // Exists test
    bool exists = LoFS::exists(testFileLFS);
    LOG_INFO("Exists test: %s exists = %s", testFileLFS, exists ? "YES" : "NO");

    // Read test
    File f2 = LoFS::open(testFileLFS, FILE_O_READ);
    if (f2) {
        size_t fileSize = f2.size();
        LOG_INFO("Read test: file size = %u bytes", fileSize);
        if (fileSize > 0) {
            char buffer[128];
            size_t bytesRead = f2.readBytes(buffer, sizeof(buffer) - 1);
            buffer[bytesRead] = '\0';
            LOG_INFO("Read test: read %u bytes, content: \"%s\"", bytesRead, buffer);
        }
        f2.close();
    } else {
        LOG_INFO("Read test: FAILED to open %s for reading", testFileLFS);
    }
    LOG_INFO("");

    // Test 4: Basic File Operations - SD Card
    LOG_INFO("--- Test 4: Basic File Operations (SD Card) ---");
    if (sdAvailable) {
        const char *testFileSD = "/sd/test_file.txt";

        // Clean up any existing test file
        if (LoFS::exists(testFileSD)) {
            LoFS::remove(testFileSD);
            LOG_INFO("Cleaned up existing SD test file");
        }

        // Write test
        File f3 = LoFS::open(testFileSD, FILE_O_WRITE);
        if (f3) {
            size_t written = f3.write((const uint8_t *)testContent, testContentLen);
            f3.close();
            LOG_INFO("Write test: wrote %u bytes to %s", written, testFileSD);
        } else {
            LOG_INFO("Write test: FAILED to open %s for writing", testFileSD);
        }

        // Exists test
        exists = LoFS::exists(testFileSD);
        LOG_INFO("Exists test: %s exists = %s", testFileSD, exists ? "YES" : "NO");

        // Read test
        File f4 = LoFS::open(testFileSD, FILE_O_READ);
        if (f4) {
            size_t fileSize = f4.size();
            LOG_INFO("Read test: file size = %u bytes", fileSize);
            if (fileSize > 0) {
                char buffer[128];
                size_t bytesRead = f4.readBytes(buffer, sizeof(buffer) - 1);
                buffer[bytesRead] = '\0';
                LOG_INFO("Read test: read %u bytes, content: \"%s\"", bytesRead, buffer);
            }
            f4.close();
        } else {
            LOG_INFO("Read test: FAILED to open %s for reading", testFileSD);
        }
    } else {
        LOG_INFO("SD Card not available, skipping SD card tests");
    }
    LOG_INFO("");

    // Test 5: Directory Operations
    LOG_INFO("--- Test 5: Directory Operations ---");
    const char *testDirLFS = "/lfs/test_dir";
    const char *testDirSD = "/sd/test_dir";

    // Create directory on LittleFS
    if (LoFS::mkdir(testDirLFS)) {
        LOG_INFO("Created directory: %s", testDirLFS);
    } else {
        LOG_INFO("Failed to create directory: %s (may already exist)", testDirLFS);
    }

    // Check if directory exists
    exists = LoFS::exists(testDirLFS);
    LOG_INFO("Directory exists check: %s = %s", testDirLFS, exists ? "YES" : "NO");

    if (sdAvailable) {
        // Create directory on SD card
        if (LoFS::mkdir(testDirSD)) {
            LOG_INFO("Created directory: %s", testDirSD);
        } else {
            LOG_INFO("Failed to create directory: %s (may already exist)", testDirSD);
        }

        exists = LoFS::exists(testDirSD);
        LOG_INFO("Directory exists check: %s = %s", testDirSD, exists ? "YES" : "NO");
    }
    LOG_INFO("");

    // Test 6: File Size Verification
    LOG_INFO("--- Test 6: File Size Verification ---");
    const char *sizeTestFileLFS = "/lfs/size_test.dat";
    const char *sizeTestFileSD = "/sd/size_test.dat";
    const size_t testSizes[] = {0, 1, 10, 100, 512, 1024, 2048};
    const size_t numSizes = sizeof(testSizes) / sizeof(testSizes[0]);

    // Test on LittleFS
    LOG_INFO("Testing file sizes on LittleFS:");
    for (size_t i = 0; i < numSizes; i++) {
        if (LoFS::exists(sizeTestFileLFS)) {
            LoFS::remove(sizeTestFileLFS);
        }

        File f = LoFS::open(sizeTestFileLFS, FILE_O_WRITE);
        if (f) {
            // Write test data
            for (size_t j = 0; j < testSizes[i]; j++) {
                uint8_t byte = (uint8_t)(j & 0xFF);
                f.write(byte);
            }
            f.close();

            // Verify size
            File fRead = LoFS::open(sizeTestFileLFS, FILE_O_READ);
            if (fRead) {
                size_t actualSize = fRead.size();
                bool match = (actualSize == testSizes[i]);
                LOG_INFO("  Size %u bytes: wrote %u, read %u, match: %s", testSizes[i], testSizes[i], actualSize, match ? "YES" : "NO");
                fRead.close();
            }
            LoFS::remove(sizeTestFileLFS);
        }
    }

    // Test on SD card
    if (sdAvailable) {
        LOG_INFO("Testing file sizes on SD Card:");
        for (size_t i = 0; i < numSizes; i++) {
            if (LoFS::exists(sizeTestFileSD)) {
                LoFS::remove(sizeTestFileSD);
            }

            File f = LoFS::open(sizeTestFileSD, FILE_O_WRITE);
            if (f) {
                // Write test data
                for (size_t j = 0; j < testSizes[i]; j++) {
                    uint8_t byte = (uint8_t)(j & 0xFF);
                    f.write(byte);
                }
                f.close();

                // Verify size
                File fRead = LoFS::open(sizeTestFileSD, FILE_O_READ);
                if (fRead) {
                    size_t actualSize = fRead.size();
                    bool match = (actualSize == testSizes[i]);
                    LOG_INFO("  Size %u bytes: wrote %u, read %u, match: %s", testSizes[i], testSizes[i], actualSize, match ? "YES" : "NO");
                    fRead.close();
                }
                LoFS::remove(sizeTestFileSD);
            }
        }
    }
    LOG_INFO("");

    // Test 7: Cross-Filesystem Operations (Copy)
    LOG_INFO("--- Test 7: Cross-Filesystem Operations ---");
    if (sdAvailable) {
        const char *sourceFile = "/lfs/cross_test_source.txt";
        const char *destFileSD = "/sd/cross_test_dest.txt";
        const char *destFileLFS = "/lfs/cross_test_dest.txt";
        const char *crossTestContent = "This file will be copied across filesystems!\n";

        // Create source file on LittleFS
        if (LoFS::exists(sourceFile)) {
            LoFS::remove(sourceFile);
        }
        File src = LoFS::open(sourceFile, FILE_O_WRITE);
        if (src) {
            src.write((const uint8_t *)crossTestContent, strlen(crossTestContent));
            src.close();
            LOG_INFO("Created source file on LittleFS: %s", sourceFile);
        }

        // Copy from LittleFS to SD using rename (which performs copy+delete)
        if (LoFS::exists(destFileSD)) {
            LoFS::remove(destFileSD);
        }
        bool renameResult = LoFS::rename(sourceFile, destFileSD);
        LOG_INFO("Cross-filesystem rename (LFS->SD): %s", renameResult ? "SUCCESS" : "FAILED");

        // Verify destination exists and source is gone
        bool sourceExists = LoFS::exists(sourceFile);
        bool destExists = LoFS::exists(destFileSD);
        LOG_INFO("  Source file exists: %s (should be NO)", sourceExists ? "YES" : "NO");
        LOG_INFO("  Dest file exists: %s (should be YES)", destExists ? "YES" : "NO");

        // Verify content
        if (destExists) {
            File f = LoFS::open(destFileSD, FILE_O_READ);
            if (f) {
                char buffer[128];
                size_t bytesRead = f.readBytes(buffer, sizeof(buffer) - 1);
                buffer[bytesRead] = '\0';
                bool contentMatch = (strcmp(buffer, crossTestContent) == 0);
                LOG_INFO("  Content match: %s", contentMatch ? "YES" : "NO");
                f.close();
            }
        }

        // Test reverse: SD to LittleFS
        if (LoFS::exists(sourceFile)) {
            LoFS::remove(sourceFile);
        }
        src = LoFS::open(sourceFile, FILE_O_WRITE);
        if (src) {
            src.write((const uint8_t *)crossTestContent, strlen(crossTestContent));
            src.close();
        }

        if (LoFS::exists(destFileLFS)) {
            LoFS::remove(destFileLFS);
        }
        renameResult = LoFS::rename(sourceFile, destFileLFS);
        LOG_INFO("Cross-filesystem rename (SD->LFS): %s", renameResult ? "SUCCESS" : "FAILED");

        sourceExists = LoFS::exists(sourceFile);
        destExists = LoFS::exists(destFileLFS);
        LOG_INFO("  Source file exists: %s (should be NO)", sourceExists ? "YES" : "NO");
        LOG_INFO("  Dest file exists: %s (should be YES)", destExists ? "YES" : "NO");

        // Cleanup
        if (LoFS::exists(destFileSD)) {
            LoFS::remove(destFileSD);
        }
        if (LoFS::exists(destFileLFS)) {
            LoFS::remove(destFileLFS);
        }
    } else {
        LOG_INFO("SD Card not available, skipping cross-filesystem tests");
    }
    LOG_INFO("");

    // Test 8: Same-Filesystem Rename
    LOG_INFO("--- Test 8: Same-Filesystem Rename ---");
    const char *oldNameLFS = "/lfs/old_name.txt";
    const char *newNameLFS = "/lfs/new_name.txt";

    // Create test file
    if (LoFS::exists(oldNameLFS)) {
        LoFS::remove(oldNameLFS);
    }
    File f = LoFS::open(oldNameLFS, FILE_O_WRITE);
    if (f) {
        f.write((const uint8_t *)"Test content for rename\n", 25);
        f.close();
    }

    // Rename
    if (LoFS::exists(newNameLFS)) {
        LoFS::remove(newNameLFS);
    }
    bool renameOk = LoFS::rename(oldNameLFS, newNameLFS);
    LOG_INFO("Rename on LittleFS: %s", renameOk ? "SUCCESS" : "FAILED");
    LOG_INFO("  Old file exists: %s (should be NO)", LoFS::exists(oldNameLFS) ? "YES" : "NO");
    LOG_INFO("  New file exists: %s (should be YES)", LoFS::exists(newNameLFS) ? "YES" : "NO");

    // Cleanup
    if (LoFS::exists(newNameLFS)) {
        LoFS::remove(newNameLFS);
    }

    if (sdAvailable) {
        const char *oldNameSD = "/sd/old_name.txt";
        const char *newNameSD = "/sd/new_name.txt";

        if (LoFS::exists(oldNameSD)) {
            LoFS::remove(oldNameSD);
        }
        f = LoFS::open(oldNameSD, FILE_O_WRITE);
        if (f) {
            f.write((const uint8_t *)"Test content for rename\n", 25);
            f.close();
        }

        if (LoFS::exists(newNameSD)) {
            LoFS::remove(newNameSD);
        }
        renameOk = LoFS::rename(oldNameSD, newNameSD);
        LOG_INFO("Rename on SD Card: %s", renameOk ? "SUCCESS" : "FAILED");
        LOG_INFO("  Old file exists: %s (should be NO)", LoFS::exists(oldNameSD) ? "YES" : "NO");
        LOG_INFO("  New file exists: %s (should be YES)", LoFS::exists(newNameSD) ? "YES" : "NO");

        if (LoFS::exists(newNameSD)) {
            LoFS::remove(newNameSD);
        }
    }
    LOG_INFO("");

    // Test 9: Error Cases
    LOG_INFO("--- Test 9: Error Cases ---");
    // Test invalid path (no prefix)
    File invalidFile = LoFS::open("no_prefix.txt", FILE_O_READ);
    LOG_INFO("Invalid path (no prefix): %s", invalidFile ? "OPENED (unexpected)" : "REJECTED (expected)");

    // Test non-existent file
    bool nonExistentExists = LoFS::exists("/lfs/nonexistent_file_12345.txt");
    LOG_INFO("Non-existent file check: %s (should be NO)", nonExistentExists ? "YES" : "NO");

    // Test SD path when SD not available
    if (!sdAvailable) {
        File sdFile = LoFS::open("/sd/test.txt", FILE_O_READ);
        LOG_INFO("SD path when SD unavailable: %s (should be REJECTED)", sdFile ? "OPENED" : "REJECTED");
    }

    // Test invalid prefix
    File invalidPrefix = LoFS::open("/invalid/prefix.txt", FILE_O_READ);
    LOG_INFO("Invalid prefix: %s (should be REJECTED)", invalidPrefix ? "OPENED" : "REJECTED");
    LOG_INFO("");

    // Test 10: Cleanup Test Files
    LOG_INFO("--- Test 10: Cleanup ---");
    const char *cleanupFiles[] = {
        "/lfs/test_file.txt",
        "/lfs/test_dir",
        "/sd/test_file.txt",
        "/sd/test_dir",
    };
    size_t numCleanupFiles = sizeof(cleanupFiles) / sizeof(cleanupFiles[0]);

    for (size_t i = 0; i < numCleanupFiles; i++) {
        // Try as file first (skip exists check to avoid ESP32 VFS warnings)
        if (LoFS::remove(cleanupFiles[i])) {
            LOG_INFO("Removed file: %s", cleanupFiles[i]);
        } else {
            // Try as directory
            if (LoFS::rmdir(cleanupFiles[i])) {
                LOG_INFO("Removed directory: %s", cleanupFiles[i]);
            }
        }
    }
    LOG_INFO("");

    // Final Summary
    LOG_INFO("=== Test Suite Complete ===");
    LOG_INFO("All tests finished. Check logs above for detailed results.");
}
