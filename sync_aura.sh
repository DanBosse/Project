#!/bin/bash
set -e

echo "[Git] Staging Aura AI, AuraCad, and all TorusOS updates..."
git add .

if git diff-index --quiet HEAD --; then
    echo "[Git] No uncommitted changes."
else
    git commit -m "feat: Integrate Aura OS AI core and AuraCad parametric modeling suite into TorusOS"
    git pull origin main --rebase || true
    git push origin main
fi

echo "[ISO Backup] Generating backup ISO..."
if [ -f "torusos.iso" ]; then
    cp torusos.iso torusos_backup_$(date +%Y%m%d_%H%M%S).iso
    echo "[ISO Backup] Master backup created successfully!"
fi
