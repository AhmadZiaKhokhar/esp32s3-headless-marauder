#!/usr/bin/env bash
set -e

BASE_DIR="/media/mydata/marauder-final-with-androide-app-and-web-server-UI"
LOCAL_SRC="$BASE_DIR/esp32s3_marauder"
UPSTREAM_DIR="$BASE_DIR/upstream_check_temp"

echo "[*] Step 1: Checking upstream repository status..."
if [ ! -d "$UPSTREAM_DIR" ]; then
  git clone --depth 1 https://github.com/justcallmekoko/ESP32Marauder.git "$UPSTREAM_DIR"
else
  cd "$UPSTREAM_DIR" && git pull origin master
fi

echo -e "\n[*] Step 2: Key Upstream Source Files Detected:"
ls -la "$UPSTREAM_DIR/esp32_marauder/" | grep -E "(CSI|RemoteID|Buffer|WiFiScan|Aps)"

echo -e "\n[*] Step 3: Checking custom UART bridge dependencies in local setup..."
grep -rn "Serial2" "$LOCAL_SRC" || echo "[!] Notice: Verify Serial2 definition for ESP32-CAM link."

echo -e "\n[*] Audit complete. Workspace prepared at: $UPSTREAM_DIR"
