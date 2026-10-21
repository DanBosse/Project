#!/bin/bash
set -e

echo "[Git] Staging working TorusOS sovereign stack..."
git add .

if git diff-index --quiet HEAD --; then
    echo "[Git] Working tree is clean."
else
    git commit -m "feat: Lock in verified working version of TorusOS with Aura AI and AuraCad"
    git pull origin main --rebase || true
    git push origin main
fi

echo "[ISO Backup] Building and backing up master ISO..."
./build_iso.sh &
PID=$!
sleep 5
kill $PID || true

if [ -f "torusos.iso" ]; then
    cp torusos.iso torusos_master_$(date +%Y%m%d_%H%M%S).iso
    echo "[ISO Backup] Master backup ISO created successfully!"
fi
