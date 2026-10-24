import os
import glob

upstream_dir = "/media/mydata/marauder-final-with-androide-app-and-web-server-UI/upstream_check_temp/esp32_marauder"
local_dir = "/media/mydata/marauder-final-with-androide-app-and-web-server-UI/esp32s3_marauder"

# Find settings header file in upstream
setting_headers = glob.glob(os.path.join(upstream_dir, "*[sS]ettings.h"))
if not setting_headers:
    setting_headers = glob.glob(os.path.join(upstream_dir, "*setting*.h"))

print(f"Found upstream setting headers: {setting_headers}")

if setting_headers:
    with open(setting_headers[0], 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
        print("--- Upstream Settings Header Snippet ---")
        lines = content.splitlines()
        for line in lines:
            if "class " in line or "void " in line or "int " in line:
                print(line)

# Let's inspect how settings_obj or equivalent is used in upstream ino file if present
upstream_ino = glob.glob(os.path.join(upstream_dir, "*.ino"))
if upstream_ino:
    with open(upstream_ino[0], 'r', encoding='utf-8', errors='ignore') as f:
        ino_content = f.read()
        for line in ino_content.splitlines():
            if "settings" in line.lower():
                print(f"Upstream ino reference: {line}")
