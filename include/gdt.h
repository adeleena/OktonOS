#ifndef __GDT_H
#define __GDT_H

#include"types.h"

class GlobalDescriptorTable
{
    public:
        class SegmentDescriptor
        {
            private:
                uint16_t limit_lo;  
                uint16_t base_lo; 
                uint8_t base_hi; //Byte extension for the pointer
                uint8_t type;
                uint8_t limit_hi;
                uint8_t base_vhi;

            public:
                //Ctor  
                SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t Base();
                uint32_t Limit();
        } __attribute__((packed));
        
        /*_attribute__((packed)) prevents the compile to move around the objects*/

    private:
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        //Ctor
        GlobalDescriptorTable();
        //Dtor
        ~GlobalDescriptorTable();

        //Functii
        uint16_t CodeSegmentSelector();
        uint16_t DataSegmentSelector();
            
};


#endif
