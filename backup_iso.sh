#!/bin/bash
set -e
echo "[Backup] Archiving current TorusOS build..."
if [ -f "torusos.iso" ]; then
    cp torusos.iso torusos_studio_backup.iso
    echo "[Success] Created backup ISO: torusos_studio_backup.iso"
else
    echo "[Error] torusos.iso not found. Run ./build_iso.sh first."
fi
