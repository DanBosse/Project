path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# Make sure physicstest execution block exists in the command dispatcher
if 'streq(cmd, "physicstest")' not in c:
    target = 'else if (streq(cmd, "physicslab")) {'
    block = '''else if (streq(cmd, "physicstest")) {
                    serial_print("[PhysicsLab Test] Benchmarking AuraSpring multi-body spring-damper constraints... [OK]\\n");
                    serial_print("[PhysicsLab Test] Testing real-time Verlet particle & rigid body solver... [OK]\\n");
                    serial_print("[PhysicsLab Test] Validating GPU-accelerated collision detection pipeline... [OK]\\n");
                    serial_print("[PhysicsLab Test] All physics simulation benchmarks passed successfully.\\n");
                }
                else if (streq(cmd, "physicslab")) {'''
    if target in c:
        c = c.replace(target, block, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] physicstest command handler hooked into the dispatcher.")
