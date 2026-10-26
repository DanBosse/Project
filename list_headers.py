import os

print("--- Header Files in Project ---")
for root, _, files in os.walk('.'):
    for f in files:
        if f.endswith('.h'):
            print(os.path.join(root, f))
