#include <stdint.h>
#include <stddef.h>

#define MAX_CLUSTER_NODES 16
#define MAX_SCHED_JOBS 64

typedef struct {
    uint32_t node_id;
    uint32_t available_cpu_cores;
    uint64_t available_memory_mb;
    uint32_t active_workloads;
    uint8_t online;
} cluster_node_info_t;

typedef struct {
    uint32_t job_id;
    uint32_t required_cores;
    uint64_t required_memory_mb;
    uint32_t assigned_node_id;
    uint8_t scheduled;
} sched_workload_t;

static cluster_node_info_t cluster_nodes[MAX_CLUSTER_NODES];
static sched_workload_t sched_jobs[MAX_SCHED_JOBS];
static uint32_t next_sched_job_id = 20000;

void cluster_scheduler_init(void) {
    for (int i = 0; i < MAX_CLUSTER_NODES; i++) {
        cluster_nodes[i].online = 0;
        cluster_nodes[i].active_workloads = 0;
    }
    for (int i = 0; i < MAX_SCHED_JOBS; i++) {
        sched_jobs[i].scheduled = 0;
    }
}

int cluster_scheduler_register_node(uint32_t node_id, uint32_t cores, uint64_t memory_mb) {
    for (int i = 0; i < MAX_CLUSTER_NODES; i++) {
        if (!cluster_nodes[i].online) {
            cluster_nodes[i].node_id = node_id;
            cluster_nodes[i].available_cpu_cores = cores;
            cluster_nodes[i].available_memory_mb = memory_mb;
            cluster_nodes[i].active_workloads = 0;
            cluster_nodes[i].online = 1;
            return 0;
        }
    }
    return -1; // Cluster node registry full
}

int cluster_scheduler_submit_job(uint32_t cores, uint64_t memory_mb, uint32_t *out_job_id, uint32_t *out_assigned_node) {
    if (!out_job_id || !out_assigned_node) return -1;

    // Find best fit online node
    int best_node_idx = -1;
    uint32_t min_remaining_cores = 0xFFFFFFFF;

    for (int i = 0; i < MAX_CLUSTER_NODES; i++) {
        if (cluster_nodes[i].online) {
            if (cluster_nodes[i].available_cpu_cores >= cores && 
                cluster_nodes[i].available_memory_mb >= memory_mb) {
                
                if (cluster_nodes[i].available_cpu_cores < min_remaining_cores) {
                    min_remaining_cores = cluster_nodes[i].available_cpu_cores;
                    best_node_idx = i;
                }
            }
        }
    }

    if (best_node_idx == -1) {
        return -2; // Insufficient cluster resources for workload placement
    }

    for (int i = 0; i < MAX_SCHED_JOBS; i++) {
        if (!sched_jobs[i].scheduled) {
            sched_workload_t *job = &sched_jobs[i];
            job->job_id = next_sched_job_id++;
            job->required_cores = cores;
            job->required_memory_mb = memory_mb;
            job->assigned_node_id = cluster_nodes[best_node_idx].node_id;
            job->scheduled = 1;

            // Update node capacity
            cluster_nodes[best_node_idx].available_cpu_cores -= cores;
            cluster_nodes[best_node_idx].available_memory_mb -= memory_mb;
            cluster_nodes[best_node_idx].active_workloads++;

            *out_job_id = job->job_id;
            *out_assigned_node = job->assigned_node_id;
            return 0;
        }
    }
    return -3; // Scheduler job queue full
}
