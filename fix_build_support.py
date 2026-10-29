with open("build.sh", "r") as f:
    content = f.read()

# Remove kernel_support compilation and linking
content = content.replace('gcc -m32 -c kernel_support.c -o kernel_support.o -ffreestanding -O2 -Wall -Wextra\n', '')
content = content.replace('kernel_support.o ', '')

with open("build.sh", "w") as f:
    f.write(content)

print("Removed duplicate kernel_support.o successfully!")
