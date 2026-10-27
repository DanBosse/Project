#include <stdint.h>
#include <stddef.h>

#define TORUS_ARCHIVE_MAGIC 0x544F5253 // "TORS"
#define MAX_ARCHIVE_FILES 16
#define MAX_FILENAME_LEN 32
#define MAX_PAYLOAD_SIZE 2048

typedef struct {
    char filename[MAX_FILENAME_LEN];
    uint32_t original_size;
    uint32_t compressed_size;
    uint32_t checksum;
    uint32_t data_offset;
} archive_file_header_t;

typedef struct {
    uint32_t magic;
    uint32_t file_count;
    archive_file_header_t headers[MAX_ARCHIVE_FILES];
    uint8_t payload_pool[MAX_PAYLOAD_SIZE];
    size_t total_payload_bytes;
} torus_archive_t;

static torus_archive_t active_archive;

void torus_archive_init(void) {
    active_archive.magic = TORUS_ARCHIVE_MAGIC;
    active_archive.file_count = 0;
    active_archive.total_payload_bytes = 0;
}

int torus_archive_pack_file(const char *name, const uint8_t *data, size_t size) {
    if (!name || !data || size == 0 || active_archive.file_count >= MAX_ARCHIVE_FILES) {
        return -1; // Invalid input or archive full
    }

    if (active_archive.total_payload_bytes + size > MAX_PAYLOAD_SIZE) {
        return -2; // Archive payload pool capacity exceeded
    }

    archive_file_header_t *hdr = &active_archive.headers[active_archive.file_count];
    
    // Copy filename
    int j = 0;
    while (name[j] != '\0' && j < MAX_FILENAME_LEN - 1) {
        hdr->filename[j] = name[j];
        j++;
    }
    hdr->filename[j] = '\0';

    hdr->original_size = (uint32_t)size;
    hdr->data_offset = (uint32_t)active_archive.total_payload_bytes;

    // Perform kernel-native RLE compression into payload pool
    size_t dest_idx = hdr->data_offset;
    size_t i = 0;
    uint32_t checksum = 0;

    while (i < size && dest_idx + 2 < MAX_PAYLOAD_SIZE) {
        uint8_t cur = data[i];
        uint8_t count = 1;

        checksum += cur;

        while ((i + 1 < size) && (data[i + 1] == cur) && (count < 255)) {
            count++;
            i++;
        }

        active_archive.payload_pool[dest_idx++] = count;
        active_archive.payload_pool[dest_idx++] = cur;
        i++;
    }

    hdr->compressed_size = (uint32_t)(dest_idx - hdr->data_offset);
    hdr->checksum = checksum;
    active_archive.total_payload_bytes = dest_idx;
    active_archive.file_count++;

    return 0;
}

int torus_archive_unpack_file(const char *name, uint8_t *out_buffer, size_t *out_size) {
    if (!name || !out_buffer || !out_size) return -1;

    archive_file_header_t *target_hdr = NULL;
    for (uint32_t f = 0; f < active_archive.file_count; f++) {
        int match = 1;
        for (int k = 0; k < MAX_FILENAME_LEN; k++) {
            if (active_archive.headers[f].filename[k] != name[k]) {
                match = 0;
                break;
            }
            if (name[k] == '\0') break;
        }
        if (match) {
            target_hdr = &active_archive.headers[f];
            break;
        }
    }

    if (!target_hdr) return -3; // File not found in archive
    if (*out_size < target_hdr->original_size) return -4; // Output buffer too small

    // Decompress payload (RLE Unpack)
    size_t src_idx = target_hdr->data_offset;
    size_t src_end = target_hdr->data_offset + target_hdr->compressed_size;
    size_t dest_idx = 0;

    while (src_idx < src_end) {
        if (src_idx + 1 >= src_end) return -5; // Corrupted archive stream

        uint8_t count = active_archive.payload_pool[src_idx++];
        uint8_t cur = active_archive.payload_pool[src_idx++];

        for (uint8_t c = 0; c < count; c++) {
            if (dest_idx >= target_hdr->original_size) return -6;
            out_buffer[dest_idx++] = cur;
        }
    }

    *out_size = dest_idx;
    return 0;
}
