#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // for size_t

void serial_print(const char *str);

// ELF32 Header Structures
#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    uint32_t      e_entry;
    uint32_t      e_phoff;
    uint32_t      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} __attribute__((packed)) Elf32_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} __attribute__((packed)) Elf32_Phdr;

#define PT_LOAD 1

static void *elf_memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

static void *elf_memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    return s;
}

bool elf_load_and_run(const uint8_t *file_buffer) {
    serial_print("[ElfLoader] Inspecting binary headers...\n");

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)file_buffer;

    if (ehdr->e_ident[0] != 0x7F || 
        ehdr->e_ident[1] != 'E'  || 
        ehdr->e_ident[2] != 'L'  || 
        ehdr->e_ident[3] != 'F') {
        serial_print("[ElfLoaderError] Invalid ELF magic signature.\n");
        return false;
    }

    if (ehdr->e_ident[4] != 1) { 
        serial_print("[ElfLoaderError] Unsupported ELF class (Not 32-bit).\n");
        return false;
    }

    serial_print("[ElfLoader] Valid ELF32 binary verified. Parsing program headers...\n");

    Elf32_Phdr *phdr = (Elf32_Phdr *)(file_buffer + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            uint8_t *dest = (uint8_t *)(uintptr_t)phdr[i].p_vaddr;
            const uint8_t *src = file_buffer + phdr[i].p_offset;

            serial_print("[ElfLoader] Loading segment to memory address...\n");
            elf_memcpy(dest, src, phdr[i].p_filesz);

            if (phdr[i].p_memsz > phdr[i].p_filesz) {
                elf_memset(dest + phdr[i].p_filesz, 0, phdr[i].p_memsz - phdr[i].p_filesz);
            }
        }
    }

    serial_print("[ElfLoader] All segments loaded successfully. Jumping to module entry point...\n");

    int (*entry_point)(void) = (int (*)(void))(uintptr_t)ehdr->e_entry;
    int result = entry_point();

    if (result == 0) {
        serial_print("[ElfLoader] Dynamic ELF module executed successfully.\n");
    } else {
        serial_print("[ElfLoader] Module execution returned non-zero status.\n");
    }

    return true;
}
