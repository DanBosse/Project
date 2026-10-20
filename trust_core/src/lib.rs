#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {
        unsafe {
            core::arch::asm!("hlt");
        }
    }
}

#[repr(C)]
pub struct SandboxInstructionRust {
    pub opcode: u8,
    pub reg: u8,
    pub immediate: u16,
}

/// 1. Memory-safe Epistemic Sandbox Verifier
#[no_mangle]
pub extern "C" fn trust_sandbox_verify(program_ptr: *const SandboxInstructionRust, length: usize) -> bool {
    if program_ptr.is_null() || length == 0 || length > 64 {
        return false;
    }

    let program = unsafe { core::slice::from_raw_parts(program_ptr, length) };

    for ins in program {
        if ins.opcode > 0x05 {
            return false;
        }
        if (ins.opcode == 0x01 || ins.opcode == 0x02) && ins.reg >= 16 {
            return false;
        }
    }

    true
}

/// 2. Memory-safe FlowScript IR Stream Verifier
#[no_mangle]
pub extern "C" fn trust_flowscript_verify(stream_ptr: *const u8, length: usize) -> bool {
    if stream_ptr.is_null() || length == 0 || length > 256 {
        return false;
    }

    let stream = unsafe { core::slice::from_raw_parts(stream_ptr, length) };

    for &byte in stream {
        if byte == 0xFF {
            return false;
        }
    }

    true
}

/// 3. 32-Tensor Index Bounds Verifier
#[no_mangle]
pub extern "C" fn trust_tensor_bounds_verify(tensor_index: u32) -> bool {
    tensor_index < 32
}

/// 4. Ring 3 Secure Syscall & Argument Verifier
#[no_mangle]
pub extern "C" fn trust_syscall_verify(syscall_id: u32, arg1: u64) -> bool {
    // Restrict valid system call numbers (0 to 15)
    if syscall_id > 15 {
        return false;
    }
    // If syscall is TENSOR_MAP (ID = 3), verify arg1 is within 32-tensor bounds
    if syscall_id == 3 && arg1 >= 32 {
        return false;
    }
    true
}
