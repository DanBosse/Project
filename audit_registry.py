#!/usr/bin/env python3
import os
import re

def audit_codebase():
    defined_tokens = set()
    # Look for #define macros that look like commands, subsystem actions, or identifiers
    macro_pattern = re.compile(r'#define\s+([A-Z0-9_]+)\s+0x[0-9A-Fa-f]+')
    
    for root, _, files in os.walk('.'):
        for file in files:
            if file.endswith(('.h', '.c', '.s')):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                        for line in f:
                            match = macro_pattern.search(line)
                            if match:
                                token = match.group(1)
                                # Capture anything that looks like a command or subsystem macro
                                if any(k in token for k in ['CMD_', 'AURA_', 'AUDIO_', 'SOT_', 'STUDIOLENS_', 'SYNTH_', 'COMPOSITOR_', 'BENCH_']):
                                    defined_tokens.add((token, file))
                except Exception:
                    pass
                    
    print(f"\n--- Extended Subsystem & Command Token Audit ---")
    print(f"Found {len(defined_tokens)} relevant macro definitions across project files:\n")
    
    by_file = {}
    for token, file in sorted(defined_tokens):
        by_file.setdefault(file, []).append(token)
        
    for file, tokens in sorted(by_file.items()):
        print(f"📁 {file} ({len(tokens)} tokens):")
        for t in tokens:
            print(f"   - {t}")
        print()

if __name__ == '__main__':
    audit_codebase()
