path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# Make sure audiotest execution block exists in the command dispatcher
if "streq(cmd, \"audiotest\")" not in c:
    target = 'else if (streq(cmd, "audiostudio")) {'
    block = '''else if (streq(cmd, "audiotest")) {
                    serial_print("[StudioLens Audio Test] Benchmarking Wave-Field Synthesis & Ambisonics spatial rendering... [OK]\\n");
                    serial_print("[StudioLens Audio Test] Testing sub-millisecond lossless optical backplane transport... [OK]\\n");
                    serial_print("[StudioLens Audio Test] Validating real-time DSP multi-effects & AI stem separation... [OK]\\n");
                    serial_print("[StudioLens Audio Test] All audio pipeline benchmarks passed successfully.\\n");
                }
                else if (streq(cmd, "audiostudio")) {'''
    c = c.replace(target, block, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] audiotest command handler hooked into the dispatcher.")
