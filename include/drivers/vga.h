#ifndef __MYOS__DRIVERS__VGA__H
#define __MYOS__DRIVERS__VGA__H

#include <common/types.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>

namespace myos
{
    namespace drivers
    {
        class VGA // Video Graphics Array
        {
            protected:
                myos::hardwareCommunication::Port8Bit miscPort;
                myos::hardwareCommunication::Port8Bit crtcIndexPort;
                myos::hardwareCommunication::Port8Bit crtcDataPort;
                myos::hardwareCommunication::Port8Bit sequencerIndexPort;
                myos::hardwareCommunication::Port8Bit sequencerDataPort;
                myos::hardwareCommunication::Port8Bit graphicsControllerIndexPort;
                myos::hardwareCommunication::Port8Bit graphicsControllerDataPort;
                myos::hardwareCommunication::Port8Bit attributeControllerIndexPort;
                myos::hardwareCommunication::Port8Bit attributeControllerReadPort;
                myos::hardwareCommunication::Port8Bit attributeControllerWritePort;
                myos::hardwareCommunication::Port8Bit attributeControllerResetPort;

                void WriteRegisters(myos::common::uint8_t* registers);
                myos::common::uint8_t* GetFrameBufferSegment();
                virtual myos::common::uint8_t GetColorIndex(myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
            
            public:
                VGA();
                ~VGA();
                virtual bool SupportsMode(myos::common::uint32_t width, myos::common::uint32_t height, myos::common::uint32_t colorDepth);
                virtual bool SetMode(myos::common::uint32_t width, myos::common::uint32_t height, myos::common::uint32_t colorDepth);
                virtual void PutPixel(myos::common::uint32_t x, myos::common::uint32_t y, myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
                virtual void PutPixel(myos::common::uint32_t x, myos::common::uint32_t y, myos::common::uint8_t colorIndex);
                
                virtual void FillRectangle(myos::common::uint32_t x, myos::common::uint32_t y, myos::common::uint32_t w, myos::common::uint32_t h, myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
        };
    }
}


#endif