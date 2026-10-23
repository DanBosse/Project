[1mdiff --git a/kernel.c b/kernel.c[m
[1mindex 06d3ceb..9787375 100644[m
[1m--- a/kernel.c[m
[1m+++ b/kernel.c[m
[36m@@ -112,6 +112,7 @@[m [mvoid kernel_main(void) {[m
     serial_print("=========================================================\n\n");[m
 [m
     serial_print("[BOOT] Initializing Global Descriptor Table (GDT)...\n");[m
[32m+[m[32m    init_gdt();[m
     serial_print("[BOOT] Initializing Interrupt Descriptor Table (IDT)...\n");[m
     serial_print("[BOOT] Remapping PIC and configuring hardware IRQs...\n");[m
 [m
[1mdiff --git a/kernel.h b/kernel.h[m
[1mindex 447e2a2..7c2028c 100644[m
[1m--- a/kernel.h[m
[1m+++ b/kernel.h[m
[36m@@ -31,3 +31,5 @@[m [mstruct multiboot_info {[m
     uint8_t  framebuffer_type;[m
     uint8_t  color_info[5];[m
 };[m
[32m+[m
[32m+[m[32mextern void init_gdt(void);[m
[1mdiff --git a/torus.h b/torus.h[m
[1mindex 0a14e88..3cd6e68 100644[m
[1m--- a/torus.h[m
[1m+++ b/torus.h[m
[36m@@ -14,3 +14,5 @@[m [mvoid server_daemon_init(void);[m
 void physics_cad_init(void);[m
 [m
 #endif[m
[32m+[m
[32m+[m[32mextern void init_gdt(void);[m
