# ESP32-S3 Headless Marauder

Known-good firmware source, build configuration, compilation, upload, and recovery notes.

> **Known-good reference:** 13 November 2026  
> **Target:** ESP32-S3 N16R8 hardware, configured as an ESP32-S3 Dev Module  
> **Repository source directory:** `esp32s3_marauder/`  
> **Preserved build directory:** `build_artifacts/known_good_20261113/`

---

## 1. Purpose

This repository contains the ESP32-S3 Headless Marauder firmware source and the configuration used to produce a known-working firmware build.

The November 13, 2026 build is retained as a recovery point. If a later source change, Arduino-ESP32 core update, partition change, or compiler change causes a regression, the preserved binaries can be uploaded again without recompiling.

Keep these two things separate:

- **Source code** is used to inspect, modify, and compile the firmware.
- **Build artifacts** are the exact output files produced by a successful compile. They are used to restore that already-compiled firmware.

Do not overwrite the known-good artifact directory when testing new changes. Compile experiments into a separate, newly named directory.

## 2. Known-good hardware and software configuration

| Item | Known-good setting |
|---|---|
| Target MCU | ESP32-S3 |
| Board definition | `ESP32-S3 Dev Module` |
| FQBN | `esp32:esp32:esp32s3` |
| Flash capacity | 16 MB |
| Flash mode | DIO |
| PSRAM | Disabled in this build |
| ESP32 Arduino core | 2.0.9 |
| ESP-IDF reported by firmware | v4.4.4 |
| Arduino CLI | 1.5.1 |
| Arduino IDE | 2.3.10 (optional when using CLI commands) |
| Compiler optimization | `-Os` |
| Partition property | `build.partitions=default_16MB` |
| Maximum application upload size | 6,553,600 bytes |
| Build date/reference | 2026-11-13 |

The exact compile configuration used was:

```bash
arduino-cli compile \
  --fqbn "esp32:esp32:esp32s3:FlashMode=dio,FlashSize=16M,PSRAM=disabled" \
  --build-property compiler.optimization_flags=-Os \
  --build-property build.partitions=default_16MB \
  --build-property upload.maximum_size=6553600 \
  --output-dir "$OUT" \
  "$CHECK"
```

**Important:** `default_16MB` was supplied as a build property in the successful compile. Do not silently replace it with `fatflash` or another partition scheme. A partition layout changes flash offsets and available storage regions.

## 3. SD-card hardware configuration

The working source configuration uses the Waveshare C5 override for the SD chip-select pin.

The effective SD SPI pins are:

| Signal | GPIO |
|---|---:|
| CS | 9 |
| SCK | 13 |
| MISO | 12 |
| MOSI | 11 |

The base `MARAUDER_MULTIBOARD_S3` definition initially assigns CS to GPIO 10. The later `MARAUDER_WS_C5_35_OVERRIDE` changes the effective CS pin to GPIO 9.

Relevant source locations:

- `configs.h`: board configuration and Waveshare C5 override.
- `SDInterface.cpp`: SPI bus selection, SPI initialization, and SD initialization.

Do not remove or reorder the override without checking the actual target board wiring and retesting SD initialization.

The known-good boot log included:

```text
ESP-ROM:esp32s3-20210327
Build: Mar 27 2021
rst:0x1 (POWERON),boot:0x2b (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
entry 0x403c98bc
Thursday, January 01 1970 00:00:00
ESP-IDF version is: v4.4.4
Thursday, January 01 1970 07:00:00
Using external SPI configuration...
Evil Portal Found 0 HTML files
Could not detect GPS baudrate
GPS Not Found
CLI Ready
```

This boot log confirms that the firmware reached `CLI Ready` and did not report an SD mount failure in that run. It does **not** prove that every SD-card write, rewrite, overwrite, or delete operation works. Those operations must be tested separately.

## 4. Why this project is pinned to ESP32 Arduino core 2.0.9

A newer framework version is not automatically a safer firmware version.

The known-good firmware was compiled and booted using ESP32 Arduino core **2.0.9**, which is based on the older ESP-IDF 4.4 generation. The project contains hardware-specific initialization, SD/SPI operations, partition assumptions, and libraries that were verified with that toolchain.

The Arduino-ESP32 3.x line is a major framework generation based on ESP-IDF 5.x. It includes API and implementation changes, and Espressif publishes a dedicated 2.x-to-3.0 migration guide. The 4.0.0 release candidate line is based on ESP-IDF 6.1 and is a pre-release, not the baseline used to verify this firmware.

Moving to a newer core may require source and library changes and can alter behavior in areas such as:

- SPI and SD-card initialization and timing.
- Filesystem and storage APIs.
- Peripheral and driver interfaces.
- Build flags, linker behavior, and partition integration.
- Third-party library compatibility.

For this project, changing the framework version while also changing SD and flash partition behavior would make failures harder to isolate. Therefore, core 2.0.9 is pinned as the reproducible baseline. This is a project compatibility decision, not a claim that 2.0.9 is the newest or best core for every ESP32 project.

### Arduino IDE versus ESP32 board package

Arduino IDE and the ESP32 Arduino core are different components:

- **Arduino IDE** is the editor and desktop application.
- **Arduino CLI** performs command-line compilation and upload.
- **ESP32 Arduino core / board package** supplies the ESP32 platform, compiler integration, framework, board definitions, and related tools.

The current Arduino IDE 2.3.10 can be used with the pinned ESP32 core 2.0.9. Updating the IDE does not require updating the ESP32 core. The command-line workflow below uses Arduino CLI 1.5.1, so the IDE is not required to compile or upload.

Do not upgrade the ESP32 core as part of an unrelated IDE update. Treat a core upgrade as a separate migration with its own branch, build output directory, and hardware regression tests.

## 5. Required tools and installed platform

Check the command-line tool:

```bash
arduino-cli version
```

Expected baseline:

```text
arduino-cli Version: 1.5.1
```

Check the installed ESP32 platform:

```bash
arduino-cli core list
```

The ESP32 platform version used for the known-good build must be:

```text
esp32:esp32  2.0.9
```

If the required platform is not installed, install the exact pinned version rather than the newest version:

```bash
arduino-cli core update-index
arduino-cli core install esp32:esp32@2.0.9
```

Check that the board definition exists:

```bash
arduino-cli board listall "ESP32-S3 Dev Module"
```

Do not remove other installed board platforms or change the user's global Arduino configuration as part of this project setup.

## 6. Repository layout

The intended layout is:

```text
esp32s3-headless-marauder/
├── README.md
├── esp32s3_marauder/
│   ├── esp32s3_marauder.ino
│   ├── configs.h
│   ├── SDInterface.cpp
│   └── ... remaining firmware source files
└── build_artifacts/
    └── known_good_20261113/
        ├── esp32s3_marauder.ino.bin
        ├── esp32s3_marauder.ino.bootloader.bin
        ├── esp32s3_marauder.ino.partitions.bin
        ├── esp32s3_marauder.ino.elf
        ├── esp32s3_marauder.ino.map
        └── ... other files produced by the successful compile
```

Keep the entire successful compile output directory, not just the application `.bin`. The bootloader and partition binary are important parts of the firmware image layout; the other generated metadata is useful for diagnosis and reproducibility.

Do not put a new compile directly into `known_good_20261113/`. Use a separate directory for every later build.

## 7. Compile the current source into a new build directory

Run from a terminal on the development computer:

```bash
REPO="/mnt/local-ai/Projects/esp32s3-headless-marauder"
SKETCH="$REPO/esp32s3_marauder"
BUILD="$REPO/build_artifacts/rebuild_$(date +%Y%m%d_%H%M%S)"

mkdir -p "$BUILD"

arduino-cli compile \
  --fqbn "esp32:esp32:esp32s3:FlashMode=dio,FlashSize=16M,PSRAM=disabled" \
  --build-property compiler.optimization_flags=-Os \
  --build-property build.partitions=default_16MB \
  --build-property upload.maximum_size=6553600 \
  --output-dir "$BUILD" \
  "$SKETCH"
```

This creates a new timestamped build directory and leaves the known-good artifacts untouched.

The November 13 successful compile reported:

```text
Sketch uses 1433793 bytes (21%) of program storage space. Maximum is 6553600 bytes.
Global variables use 69528 bytes (21%) of dynamic memory, leaving 258152 bytes for local variables. Maximum is 327680 bytes.
```

Those values are a historical reference for that build. New source changes can legitimately change the sizes.

## 8. Upload a newly compiled build

First identify the connected serial port:

```bash
arduino-cli board list
```

Or list stable USB serial device names:

```bash
ls -l /dev/serial/by-id/
```

Set the correct port. Example:

```bash
PORT="/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A5069RR4-if00-port0"
```

Upload the newly compiled build using its output directory:

```bash
REPO="/mnt/local-ai/Projects/esp32s3-headless-marauder"
SKETCH="$REPO/esp32s3_marauder"

# Set this to the exact timestamped build directory created during compilation.
BUILD="$REPO/build_artifacts/rebuild_YYYYMMDD_HHMMSS"

PORT="/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A5069RR4-if00-port0"

arduino-cli upload \
  "$SKETCH" \
  --fqbn "esp32:esp32:esp32s3:FlashMode=dio,FlashSize=16M,PSRAM=disabled" \
  --port "$PORT" \
  --input-dir "$BUILD" \
  --verbose
```

Replace `rebuild_YYYYMMDD_HHMMSS` with the real directory name. Do not guess the timestamp or port.

The `--input-dir` option tells Arduino CLI to use the existing compiled binaries instead of compiling the sketch again. The upload command does not itself compile the sketch.

## 9. Restore the November 13 known-good firmware without recompiling

Use this procedure when a later firmware build needs to be rolled back.

```bash
REPO="/mnt/local-ai/Projects/esp32s3-headless-marauder"
SKETCH="$REPO/esp32s3_marauder"
BUILD="$REPO/build_artifacts/known_good_20261113"

PORT="/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A5069RR4-if00-port0"

test -f "$BUILD/esp32s3_marauder.ino.bin" || {
  echo "ERROR: Known-good application binary is missing."
  exit 1
}

test -f "$BUILD/esp32s3_marauder.ino.bootloader.bin" || {
  echo "ERROR: Known-good bootloader binary is missing."
  exit 1
}

test -f "$BUILD/esp32s3_marauder.ino.partitions.bin" || {
  echo "ERROR: Known-good partition binary is missing."
  exit 1
}

arduino-cli upload \
  "$SKETCH" \
  --fqbn "esp32:esp32:esp32s3:FlashMode=dio,FlashSize=16M,PSRAM=disabled" \
  --port "$PORT" \
  --input-dir "$BUILD" \
  --verbose
```

This reuses the preserved compiled output. It does not rebuild the source.

**Important:** Keep the matching ESP32 core/toolchain installed. A precompiled application binary is not a substitute for preserving the correct board configuration and upload tools. Do not erase the entire flash as a routine rollback step. A full-chip erase can remove stored data and is not required by the normal upload procedure.

## 10. Verify the uploaded firmware

After upload:

1. Open the serial monitor at the baud rate configured by the firmware.
2. Reset or power-cycle the board.
3. Confirm that the ESP32-S3 starts normally.
4. Confirm that the firmware reaches `CLI Ready`.
5. Confirm that SD initialization does not report a mount failure.
6. Test actual file operations on the SD card.

The November 13 boot log is a reference, not a substitute for testing the current board.

### SD-card regression checklist

Test more than one command that writes to the card. The known issue has affected multiple file-saving commands, so investigate shared filesystem and file-write paths if failures recur.

- [ ] SD card initializes and mounts.
- [ ] A new file can be created.
- [ ] Data can be written and the file closed.
- [ ] The file can be read back.
- [ ] An existing file can be reopened and rewritten as intended.
- [ ] An existing file can be overwritten as intended.
- [ ] A file can be deleted and recreated.
- [ ] Repeated saves work across the commands that use SD storage.
- [ ] Files remain readable after reboot.

Do not format the SD card as a troubleshooting shortcut. Preserve existing card data and investigate the common write path first.

## 11. Preserve and verify build artifacts

Before publishing a new known-good build:

1. Compile into a new timestamped build directory.
2. Upload it to the target board.
3. Verify boot and required hardware behavior.
4. Copy the complete successful output directory into a new versioned directory under `build_artifacts/`.
5. Generate checksums for the preserved files.
6. Record the core version, FQBN, partition property, compiler flags, and test results.

Generate a checksum manifest inside a versioned artifact directory:

```bash
ARTIFACTS="/mnt/local-ai/Projects/esp32s3-headless-marauder/build_artifacts/known_good_20261113"

(
  cd "$ARTIFACTS"
  find . -maxdepth 1 -type f -print0 |
    sort -z |
    xargs -0 sha256sum
) > "$ARTIFACTS/SHA256SUMS"
```

Verify the manifest later:

```bash
cd "/mnt/local-ai/Projects/esp32s3-headless-marauder/build_artifacts/known_good_20261113"
sha256sum -c SHA256SUMS
```

Do not regenerate the manifest after an unexplained artifact change. A changed checksum means the stored file is no longer byte-for-byte identical to the recorded artifact.

## 12. GitHub publishing notes

The source files and the known-good artifact directory should be committed on the intended feature/release branch.

Before committing, verify the source export and repository source are identical. A checksum comparison is more reliable than comparing file counts.

Build artifacts can make a Git repository large. Check their total size and individual file sizes before pushing. GitHub blocks ordinary Git files larger than 100 MiB; use Git LFS or a release asset for files that exceed repository limits. Do not omit the bootloader or partition binary merely to reduce size without first choosing and documenting a different recovery method.

Suggested commit subjects:

```text
Fix ESP32-S3 SD and SPIFFS configuration
Preserve known-good ESP32-S3 firmware build artifacts
Document reproducible ESP32-S3 build and recovery process
```

## 13. Important safety rules

- Never overwrite `build_artifacts/known_good_20261113/`.
- Never use the known-good artifact directory as a scratch build directory.
- Use a new timestamped directory for every compile.
- Do not change the partition layout casually; flash offsets and storage capacity depend on it.
- Do not change SD pin definitions without checking the board wiring.
- Do not upgrade the ESP32 core and change storage code in the same test.
- Do not assume a successful compile proves SD-card writes or overwrites work.
- Do not format the SD card or erase the whole flash without a separate backup and explicit reason.
- Keep source, compiled artifacts, and test evidence together for every release.

## 14. Official references

- [Espressif Arduino-ESP32 releases](https://github.com/espressif/arduino-esp32/releases)
- [Arduino-ESP32 migration guide: 2.x to 3.0](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html)
- [Arduino CLI compile command](https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_compile/)
- [Arduino CLI upload command](https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_upload/)
- [Arduino IDE downloads and release information](https://www.arduino.cc/en/software)

---

## Known-good status record

| Check | Status |
|---|---|
| Export contains 105 source files | Verified |
| Export source filenames match original source filenames | Verified |
| Export source contents compile successfully | Verified |
| November 13 build compiles with the pinned configuration | Verified |
| Firmware reaches `CLI Ready` | Verified in the supplied boot log |
| SD mount failure absent in supplied boot log | Verified for that boot |
| All SD file create/rewrite/overwrite operations | Still requires hardware verification |
| Remote GitHub branch contains the final source and artifacts | Verify after push |
