# Spinner
Spinning Wheel Arduino firmware - now with analog footpedal!

This is a fork of the project that's hardware specifications and build steps are located on thingiverse (https://www.thingiverse.com/thing:2603677).

This fork changes the following:

- Omit the footswitch parts and wiring when building the wheel. The SW pin on the KY-040 rotary encoder is wired directly to D9.
  - Note: the wheel will work fine with just the knob, if you want to be sure everything works well before adding on this addition. 
- Buy a guitar footpedal like this one with a TRS jack https://www.amazon.com/AKAI-Professional-Expression-Pedal-Lightweight/dp/B09MRRPVL4
- Buy a 6.35mm jack socket like this one https://www.amazon.com/KAISH-Premium-American-Thread-Microphone/dp/B0BBVRLHVJ 
- Solder wires to the jack socket such that tip is signal, ring is GND and sleeve is 5V 
  - Note: I would recommend playing with this with a simple Arduino sketch that just Serial prints the `digitalRead` result of the signal pin before wiring it up to the spinning wheel
  - Note 2: There is a knob on the side of the foot pedal that adjusts how the values change as you press down on the pedal. The symbol on my pedal is like a little volume nob with a line thats thick on one end and gets thinner. **You will want it to be on the thin end**. Being on the thin end means the number changes steadily as youd expect. Being on the thick end seems to mean that the analog number change looks more like a curve as you press down, getting smaller until the midway point then getting bigger.
- Cut a hole for the jack socket and use included nut to hold in place. I put mine back by the motor.
- 5V and GND can go on the existing 5V and GND wires from the driver board. Signal wire goes to A1.

The program included operates where going clockwise is default and counter clockwise needs an action. The knob on it will adjust the direction, and also allows for faster spinning. The higher the max speed for the footpedal, the harder it is to fine tune the speed with your foot, so I set the max speed the pedal allows for to be comparitively low compared to the max speed the knob can do.

So, to spin counterclockwise, you will need to use the knob to set that direction and press the knob to stop, and then you can use your footpedal to alter the speed.

Thanks so much to [Christoph Pech](https://github.com/ChristophPech) for the original project! 
