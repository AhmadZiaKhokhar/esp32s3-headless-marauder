#!/usr/bin/env bash
set -e

BASE_DIR="/media/mydata/marauder-final-with-androide-app-and-web-server-UI"
LOCAL_SRC="$BASE_DIR/esp32s3_marauder"
UPSTREAM_DIR="$BASE_DIR/upstream_check_temp"

echo "[*] Ensuring upstream temp workspace is available..."
if [ ! -d "$UPSTREAM_DIR" ]; then
  git clone --depth 1 https://github.com/justcallmekoko/ESP32Marauder.git "$UPSTREAM_DIR"
fi

echo "[*] Step 2 Automation: Copying new features and memory patches..."

# 1. Drone tracking (Remote ID)
if [ -f "$UPSTREAM_DIR/esp32_marauder/RemoteID.cpp" ]; then
  cp "$UPSTREAM_DIR/esp32_marauder/RemoteID."* "$LOCAL_SRC/"
  echo "  [+] Remote ID drone tracking files copied."
fi

# 2. CSI Motion Detector
if [ -f "$UPSTREAM_DIR/esp32_marauder/CSI.cpp" ]; then
  cp "$UPSTREAM_DIR/esp32_marauder/CSI."* "$LOCAL_SRC/"
  echo "  [+] CSI Motion Monitor files copied."
fi

# 3. Buffer and Sniffer memory patches
cp "$UPSTREAM_DIR/esp32_marauder/Buffer."* "$LOCAL_SRC/"
echo "  [+] Memory & Packet Buffer stability fixes updated."

echo -e "\n[*] Step 2 complete! New upstream features merged without touching your custom UART bridge or partition table."
