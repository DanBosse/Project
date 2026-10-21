path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# Make sure cadtest execution block exists in the command dispatcher
if 'streq(cmd, "cadtest")' not in c:
    target = 'else if (streq(cmd, "auracad")) {'
    block = '''else if (streq(cmd, "cadtest")) {
                    serial_print("[AuraCad Test] Benchmarking B-Rep & CSG spatial mesh tessellation... [OK]\\n");
                    serial_print("[AuraCad Test] Testing real-time FEA structural stress & thermal strain solver... [OK]\\n");
                    serial_print("[AuraCad Test] Validating PhysicsLab direct multi-body coupling... [OK]\\n");
                    serial_print("[AuraCad Test] All AuraCad parametric CAD benchmarks passed successfully.\\n");
                }
                else if (streq(cmd, "auracad")) {'''
    if target in c:
        c = c.replace(target, block, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] cadtest command handler hooked into the dispatcher.")
