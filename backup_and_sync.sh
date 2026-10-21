#!/bin/bash
set -e

echo "[Git] Staging latest TorusOS subsystems and kernel updates..."
git add .

# Check if there are changes to commit
if git diff-index --quiet HEAD --; then
    echo "[Git] No uncommitted changes detected."
else
    echo "[Git] Committing changes..."
    git commit -m "feat: Integrate video, audio, graphics studios, office suite, and physicslab into TorusOS StudioLens"
    
    echo "[Git] Pushing to remote repository..."
    git push origin main || git push origin master || echo "[Git] Push skipped or configured branch differs. Please check remote."
fi

echo "[ISO Backup] Creating secure backup of master ISO..."
if [ -f "torusos.iso" ]; then
    cp torusos.iso torusos_backup_$(date +%Y%m%d_%H%M%S).iso
    echo "[ISO Backup] Backup ISO successfully generated!"
else
    echo "[ISO Backup] torusos.iso not found. Running build pipeline first..."
    ./build_iso.sh
    cp torusos.iso torusos_backup_$(date +%Y%m%d_%H%M%S).iso
    echo "[ISO Backup] Built and backed up successfully!"
fi

echo "[Sovereign Stack] Sync & backup sequence completed."
