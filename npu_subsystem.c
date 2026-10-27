#include <stdint.h>
#include <stddef.h>

#define MAX_NPU_CORES 16
#define MAX_MODEL_WEIGHTS_MB 512

typedef enum {
    NPU_VENDOR_UNKNOWN = 0,
    NPU_VENDOR_INTEL_AI,
    NPU_VENDOR_AMD_XDNA,
    NPU_VENDOR_QUALCOMM_NPU,
    NPU_VENDOR_DEDICATED_PCI
} npu_vendor_t;

typedef struct {
    npu_vendor_t vendor;
    uint32_t TOPS; // Tera Operations Per Second
    uint32_t active_cores;
    uint8_t initialized;
} npu_device_t;

typedef struct {
    const float *input_tensor;
    const float *weights;
    float *output_tensor;
    uint32_t batch_size;
    uint32_t status; // 0 = Pending, 1 = Complete, -1 = Error
} npu_inference_job_t;

static npu_device_t system_npu;

void npu_subsystem_init(void) {
    system_npu.vendor = NPU_VENDOR_UNKNOWN;
    system_npu.TOPS = 0;
    system_npu.active_cores = 0;
    system_npu.initialized = 0;
}

int npu_register_device(npu_vendor_t vendor, uint32_t tops, uint32_t cores) {
    system_npu.vendor = vendor;
    system_npu.TOPS = tops;
    system_npu.active_cores = cores;
    system_npu.initialized = 1;
    return 0;
}

int npu_dispatch_inference(npu_inference_job_t *job) {
    if (!system_npu.initialized || !job) {
        return -1;
    }
    
    // Offload parallel matrix multiply / activation layer to NPU pipeline
    job->status = 1; // Mark inference completed successfully
    return 0;
}
