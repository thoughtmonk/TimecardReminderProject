import serial
import time

class NeopixelCtrl:
    """This class acts as an interface for the MSP430 controlling the neopixel 
    strip. It acts as a serial controller, sending serial packets that sets 
    individual neopixels."""

    def __init__(self, port, stripLength):
        self.ser = serial.Serial(port, 9600)
        self.stripLength = stripLength

    def SetPixel(self, pixel, red, green, blue):
        base = [255, pixel, red, green, blue, 255]
        values = bytearray(base)
        self.ser.write(values)

    def SetStrip(self, red, green, blue):
        for x in range(0, self.stripLength):
            base = [255, x, red, green, blue, 255]
            values = bytearray(base)
            self.ser.write(values)
    
    def DrawStrip(self):
        base = [255,255,0,0,0,255]
        values = bytearray(base)
        self.ser.write(values)


strip = NeopixelCtrl('/dev/ttyUSB0', 19)

steps = 255 / 2

for brightness in range(steps):
    strip.SetStrip( brightness*2, 0, 0)
    strip.DrawStrip()

strip.SetPixel(0, 255, 0, 0)
strip.SetPixel(1, 150, 150, 0)
strip.SetPixel(2, 0, 255, 0)
strip.SetPixel(3, 0, 150, 150)
strip.SetPixel(4, 0, 0, 255)
strip.SetPixel(5, 150, 0, 150)
strip.SetPixel(6, 255, 0, 0)
strip.SetPixel(7, 150, 150, 0)
strip.SetPixel(8, 0, 255, 0)
strip.SetPixel(9, 0, 150, 150)
strip.SetPixel(10, 0, 0, 255)
strip.SetPixel(11, 150, 0, 150)
strip.SetPixel(12, 255, 0, 0)
strip.SetPixel(13, 150, 150, 0)
strip.SetPixel(14, 0, 255, 0)
strip.SetPixel(15, 0, 150, 150)
strip.SetPixel(16, 0, 0, 255)
strip.SetPixel(17, 150, 0, 150)
strip.DrawStrip();

        
