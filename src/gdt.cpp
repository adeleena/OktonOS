
#include "gdt.h"

/*
Constructor
*/
GlobalDescriptorTable::GlobalDescriptorTable()
       :nullSegmentSelector(0, 0, 0), 
        unusedSegmentSelector(0, 0, 0),
        codeSegmentSelector(0, 64*1024*1024, 0x9A),
        dataSegmentSelector(0, 64*1024*1024, 0x92)
{
    uint32_t i[2];
    i[1] = (uint32_t)this;
    i[0] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)": :"p" (((uint8_t*) i)+2));

}

/*
Destructor
*/
GlobalDescriptorTable::~GlobalDescriptorTable(){}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

//Inner Class
GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type)
{
    uint8_t* target = (uint8_t*)this;
    
    if(limit <= 65536)
    {
        //16 bit address space
        target[6] = 0x40;
    }
    else
    {
        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else 
            limit = limit >> 12;

    target[6] = 0xC00;
    }
    /*
    
    Anding an integer with 0xFF leaves only the least significant byte. For example, to get the first byte in a short s, you can write s & 0xFF. This is typically referred to as "masking". If byte1 is either a single byte type (like uint8_t) or is already less than 256 (and as a result is all zeroes except for the least significant byte) there is no need to mask out the higher bits, as they are already zero.

    */
    //Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    //Encode the base 
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    //Type
    target[5] = type;

}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t r = target[7];
    r = (r << 8) + target[4];
    r = (r << 8) + target[3];
    r = (r << 8) + target[2];

    return r;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t r = target[6] & 0xF;
    r = (r << 8) + target[1];
    r = (r << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)
        r = (r << 12) | 0xFFF;

    return r;
}
