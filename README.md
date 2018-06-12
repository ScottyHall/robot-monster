# robot-monster
## Robot Monster is a 'furby'-esque hardware/software project

The Robot Monster is rechargeable with a standard phone charger/computer USB port.
Features:
	- 2X BUTTONS:
	- Right Arm = Jokes
	- Left Arm Quick Press = Quotes (Some are mispronounced by the voice synthesizer, but it’s a little comical, so it was not corrected)
	- Left Arm Hold Press = Oregon Trail game

![alt text][design]

[design]: https://i.imgur.com/UUB2Ms0.jpg?1 "Robot monster blueprint"

## Hardware

###### Processing

The section of the PharmMonster responsible for running the code is a base, modified board from PJRC utilizing a 32-bit ARM processor.
Unmodified Teensy 3.2 from PJRC

###### Audio
The audio processing is done by a Freescale SGTL5000. Analog Inputs
• Stereo LINEIN - Support for external analog input
• Stereo LINEIN - Codec bypass for low power
• MIC bias provided
• Programmable MIC gain
• ADC - 85 dB SNR (-60 dB input) and -73 dB THD+N (VDDA = 1.8 V) Analog Outputs
• HP Output - Capless design
• HP Output - 62.5 mW max, 1.02 kHz sine into 16 W load at 3.3 V • HP Output - 100 dB SNR (-60 dB input) and -80 dB THD+N (VDDA = 1.8 V, 16 W load, DAC to headphone)
• LINEOUT - 100 dB SNR (-60 dB input) and -85 dB THD+N (VDDIO = 3.3 V) Digital I/O
• I2S port to allow routing to Application Processor Integrated Digital Processing
• Freescale surround, Freescale bass, tone control/ parametric equalizer/graphic equalizer clocking/control
• PLL allows input of an 8.0 MHz to 27 MHz system clock - standard audio clocks are derived from PLL Power Supplies
• Designed to operate from 1.62 to 3.6 volts

###### Wiring

![alt text][wiring]

[wiring]: https://i.imgur.com/VG19hHe.png?1 "Wiring diagram"

## OREGON TRAIL GAME

![alt text][logo]

[logo]: https://i.imgur.com/4lCi1CB.png?1 "Oregon Trail Sign"

Let the directions finish, listen for the choices, the monster will state each choice again separately and after you hear the choice you wish to choose, make a loud sound.
3 Decisions:
- Trade choice:
	- Banker
	- Carpenter
	- Farmer

These change the odds, banker being the highest success rate.

Although the success rate is higher, the score multiplier is lowest for the banker.
Ford the River: Ford
Float the wagon Bear Trap:
	- Chop their arm off
	- Attempt to free their arm
	- Leave them for dead

After this choice, your score will be calculated. Each choice you made has different percent chances of success based at random. The higher the risk, the great the point reward.
Can you become a ‘True Trail champion’?
