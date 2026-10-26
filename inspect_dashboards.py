import os

print("--- Searching for Dashboard & View Headers ---")
for root, _, files in os.walk('.'):
    for f in files:
        if 'dashboard' in f.lower() or 'view' in f.lower() or 'ui' in f.lower() or 'studio' in f.lower():
            print(os.path.join(root, f))
            
if os.path.exists('studiolens_dashboard.h'):
    print("\n--- Contents of studiolens_dashboard.h ---")
    with open('studiolens_dashboard.h', 'r', encoding='utf-8', errors='ignore') as f:
        print(f.read())
