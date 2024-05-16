// nosmart

#include "includes.h"
#include "hde/hde64.h"

hde64s hs;
ULONG64 offset = 0;

NTSTATUS DriverEntry(PDRIVER_OBJECT, PUNICODE_STRING)
{
    // Hardcoded information as we do not really need to add another function here; the goal is to keep this code as simple as possible

    uintptr_t disk_mapped_region = 0xfffff8073aed0000;
    uintptr_t disk_mapped_offset = disk_mapped_region + 0xee34;
    UCHAR* p = reinterpret_cast<UCHAR*>(disk_mapped_offset);

    // PAGE:00000001C000EE34
    // FUNCTION CHUNK AT PAGE:00000001C00109E4 
    // SIZE 00000028 BYTES
    while (offset < 0x00000028) {
        ULONG len = hde64_disasm(p + offset, &hs);
        if (len > 0) {
            if (hs.len >= 3 && p[offset] == 0x48 && p[offset + 1] == 0x8d && p[offset + 2] == 0x54) {
                // lea     rdx, [rsp+2C0h+Dst
                // 48 8D 54 ?? ?? ?? (opcode for lea rdx, [rsp+...])
                return offset;
            }
            offset += len;
        }
        else {
            break;
        }
    }

    // Now, in order to write to this kernel memory addr, we will need write access
    // as this page is read-only. Either we apply the modifications to the page itself,
    // or modify our permissions using bit 16 in CR0 (WP BIT)

    uintptr_t ControlRegister0 = __readcr0();
    ControlRegister0 &= ~(1ULL << 16);
    __writecr0(ControlRegister0);

    // We are now checking if our disassembly went correct and if our address is all good
    // This is also a stackwalking technique where check if the RIP is within the target module's bounds (AC use this in game process, in their driver, etc)

    if (offset >= disk_mapped_region && offset <= disk_mapped_region)
        return STATUS_MEMBER_NOT_IN_GROUP;

    // Further checks to guarantee the integrity of the funct
#ifdef CHECK_MEMORY_BINDS
    // From the driver's shellcode; checking if the memory matches with the mapped memory
    if ((*((uintptr_t*)offset + 11) & 0x10) != 0
        && *((uintptr_t*)offset + 41) >= 2u)
        return STATUS_NOT_MAPPED_DATA;
#endif

    RtlCopyMemory((void*)offset, (void*)8, sizeof uintptr_t);

    // Restore the modifcations to CR0

    ControlRegister0 = __readcr0();
    ControlRegister0 &= ~(1ULL << 16);
    __writecr0(ControlRegister0);
}