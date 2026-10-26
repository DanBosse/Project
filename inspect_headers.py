import os

headers = ['studiolens.h', 'physics_studio.h', 'synthesizer.h', 'spatial_compositor.h', 'audio_studio.h']
for h in headers:
    if os.path.exists(h):
        print(f"\n--- Contents of {h} ---")
        with open(h, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
            for line in lines[:30]: # print first 30 lines
                print(line, end='')
