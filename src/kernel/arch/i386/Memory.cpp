#include "kernel/arch/Memory.h"
#include "kernel/arch/tty.h"

MemoryMap Memory::memoryMap;
size_t Memory::upperMemorySize;
MemorySegment* Memory::firstSegment;
size_t Memory::availableMemory;
size_t Memory::allocatedMemory;

void Memory::Initialize(multiboot_info_t *mbd, uint32_t upperMemoryOffset) {
    memoryMap.map = (multiboot_memory_map_t*)mbd->mmap_addr;
    memoryMap.length = mbd->mmap_length;
    memoryMap.size = mbd->mmap_addr + memoryMap.length;
    upperMemorySize = mbd->mem_upper;
    firstSegment = nullptr; // Invalid location
    availableMemory = 0;
    allocatedMemory = 0;

    for(multiboot_mmap_entry* mmap_entry = memoryMap.map;
        (uint32_t)mmap_entry < (memoryMap.size);
        mmap_entry = (multiboot_memory_map_t*)((uint32_t)mmap_entry + mmap_entry->size + sizeof(mmap_entry->size))) {
        if(mmap_entry->addrL == 0x100000) {
            // Upper memory section found
            // TODO check that a single segment struct can also fit
            if(mmap_entry->lenL < upperMemoryOffset) {
                // Fatal error stop execution
                Terminal::writeline("Memory offset is larger than the available memory");
                while(true) {
                    
                }
            }
            firstSegment = (MemorySegment*)mmap_entry->addrL;
            firstSegment->next = nullptr;
            firstSegment->prev = nullptr;
            firstSegment->allocated = false;
            firstSegment->size = mmap_entry->lenL - upperMemoryOffset - sizeof(MemorySegment);
            availableMemory = firstSegment->size;
            allocatedMemory = sizeof(MemorySegment); // Theres always atleas 1 segment present that marks the unallocated memory
            // Address to the free data in this segment is MemorySegment* + sizeof(MemorySegment)
        }

    }
}

void* Memory::malloc(size_t size) {
    MemorySegment* allocatedSegment = nullptr;

    for(MemorySegment* segment = firstSegment; segment != nullptr && allocatedSegment == nullptr; segment = segment->next) {
        if(segment->size > size && !segment->allocated) {
            allocatedSegment = segment;
        }
    }

    if(allocatedSegment == nullptr) {
        return nullptr;
    }

    // Only create a new unallocated segment if we can fit a segment header.
    if(!allocatedSegment->size < size + sizeof(MemorySegment)) {
        // Address to the memory segment that contains the data that was not allocated
        // This address is allocatedSegmentAddress + sizeOfAllocation + sizeof(MemorySegment)
        // because we store the segment info before the allocated data
        MemorySegment* unallocatedSegment = (MemorySegment*)((size_t)allocatedSegment) + sizeof(MemorySegment) + size;
        unallocatedSegment->allocated = false;
        unallocatedSegment->prev = allocatedSegment;
        unallocatedSegment->next = allocatedSegment->next;
        if(unallocatedSegment->next != nullptr) {
            unallocatedSegment->next->prev = unallocatedSegment;
        }
        unallocatedSegment->size = allocatedSegment->size - size - sizeof(MemorySegment); // The new unnallocated memory segment has the size of the old segment minus the allocated size minus the new segment header

        allocatedSegment->next = unallocatedSegment;
        allocatedSegment->size = size;
        // Allocates size + sizeof(MemorySegment)
        allocatedMemory += allocatedSegment->size + sizeof(MemorySegment);
    } else {
        allocatedMemory += allocatedSegment->size;
        // Allocates size
    }
    allocatedSegment->allocated = true;
    return (void*)(allocatedSegment + sizeof(MemorySegment)); // The pointer to the allocated data is the address of the segment + the segment header size
}

void Memory::free(void* ptr) {
    MemorySegment* segment = (MemorySegment*)((size_t)ptr - sizeof(MemorySegment)); // Retrieve the memory segment header
    segment->allocated = false;

    // Unallocates size
    allocatedMemory -= segment->size;

    if(segment->next != nullptr && !segment->next->allocated) {
        MemorySegment* rightSegment = segment->next;
        // The segment to the right if this newly unallocated segment is not allocated merge that segment into this segment
        segment->size = segment->size + sizeof(MemorySegment) + rightSegment->size;
        segment->next = rightSegment->next;
        if(rightSegment->next != nullptr) {
            rightSegment->next->prev = segment;
        }
        // The right segment has now effectivly been merged into segment

        // Unallocated a segment header
        allocatedMemory -= sizeof(MemorySegment);
    }
}

size_t Memory::GetAllocatedMemory() {
    return allocatedMemory;
}

size_t Memory::GetAvailableMemory() {
    return availableMemory;
}

void Memory::PrintMemoryMap() {
    for(multiboot_mmap_entry* mmap_entry = memoryMap.map;
        (uint32_t)mmap_entry < (memoryMap.size);
        mmap_entry = (multiboot_memory_map_t*)((uint32_t)mmap_entry + mmap_entry->size + sizeof(mmap_entry->size))) {
            Terminal::writeline("Grub memory map entry");
            //Terminal::writehex(mmap->addrH);
            Terminal::writehex32(mmap_entry->addrL);

            Terminal::write(" ", 1);
            
            //Terminal::writehex(mmap->lenH);
            Terminal::write("Size: ", 6);
            Terminal::writeint32(mmap_entry->lenL/1024);
            Terminal::write("KB", 2);
            //Terminal::writehex(mmap->lenL);
            
            Terminal::write(" ", 1);
            Terminal::write("Type: ", 6);
            switch (mmap_entry->type)
            {
            case MULTIBOOT_MEMORY_AVAILABLE:
                Terminal::write("Unallocated ", 12);
                break;
            
            case MULTIBOOT_MEMORY_RESERVED:
                Terminal::write("Reserved" , 9);
                break;

            default:
                Terminal::write("Unkown type ", 12);
                break;
            }
            //Terminal::writehex(mmap->type);
            Terminal::newline();
    }
}