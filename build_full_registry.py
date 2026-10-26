#!/usr/bin/env python3
import os
import re

def scan_all_commands():
    command_entries = set()
    # Match any macro definition containing CMD or common subsystem action prefixes
    define_pattern = re.compile(r'#define\s+([A-Z0-9_]*(?:CMD_|STUDIOLENS_|PHYSICS_|AUDIO_|AURA_|SYNTH_|COMPOSITOR_|SOT_|VPU_|VIDEO_|OFFICE_|ACTION_|OP_)[A-Z0-9_]*)\b', re.IGNORECASE)
    
    for root, _, files in os.walk('.'):
        for file in files:
            if file.endswith(('.h', '.c')):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                        matches = define_pattern.findall(content)
                        for m in matches:
                            command_entries.add((m, file))
                except Exception:
                    pass
                    
    print(f"\n--- Broadened Subsystem Command Scan ---")
    print(f"Total matching command tokens found: {len(command_entries)}")
    
    by_file = {}
    for name, f in command_entries:
        by_file.setdefault(f, []).append(name)
        
    for f, cmds in sorted(by_file.items(), key=lambda x: len(x[1]), reverse=True):
        print(f"\n📁 {f} ({len(cmds)} tokens):")
        for name in sorted(cmds)[:10]:
            print(f"   - {name}")
        if len(cmds) > 10:
            print(f"   ... and {len(cmds) - 10} more")

if __name__ == '__main__':
    scan_all_commands()
