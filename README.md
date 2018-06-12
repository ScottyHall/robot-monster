# 🦄 robot-monster
## Robot Monster 🦖 is a 'furby'-esque hardware/software project

![alt text][headerpic]

[headerpic]: https://i.imgur.com/hAnCs4R.png?1 "RobotMonster Header"


The Robot Monster is rechargeable with a standard phone charger/computer USB port.
###### Controls:
- 2X Buttons (one in each fin):
	- Right Arm = Jokes
	- Left Arm Quick Press = Quotes (Some are mispronounced by the voice synthesizer, but it’s a little comical, so it was not corrected)
	- Left Arm Hold Press = Oregon Trail game

###### Components:
- Tilt sensor
- 2x Cherry MX Blue keyboard switches
- 3x JST Switches
- Speaker
- Photo-resistor
- Microphone with breakout board
- Multicolor LED

![alt text][design]

[design]: https://i.imgur.com/UUB2Ms0.jpg?1 "Robot monster blueprint"

## HARDWARE 🎛

###### PROCESSING 💻

The section of the PharmMonster responsible for running the code is a base, modified board from PJRC utilizing a 32-bit ARM processor.
Unmodified Teensy 3.2 from PJRC https://www.pjrc.com

###### AUDIO 🎵
The audio processing is done by a Freescale SGTL5000. Analog Inputs
- Stereo LINEIN - Support for external analog input
- Stereo LINEIN - Codec bypass for low power
- MIC bias provided
- Programmable MIC gain
- ADC - 85 dB SNR (-60 dB input) and -73 dB THD+N (VDDA = 1.8 V) Analog Outputs
- HP Output - Capless design
- HP Output - 62.5 mW max, 1.02 kHz sine into 16 W load at 3.3 V • HP Output - 100 dB SNR (-60 dB input) and -80 dB THD+N (VDDA = 1.8 V, 16 W load, DAC to headphone)
- LINEOUT - 100 dB SNR (-60 dB input) and -85 dB THD+N (VDDIO = 3.3 V) Digital I/O
- I2S port to allow routing to Application Processor Integrated Digital Processing
- Freescale surround, Freescale bass, tone control/ parametric equalizer/graphic equalizer clocking/control
- PLL allows input of an 8.0 MHz to 27 MHz system clock - standard audio clocks are derived from PLL Power Supplies
- Designed to operate from 1.62 to 3.6 volts

###### WAV FILES 〰️

Export
Export each clip as an uncompressed WAV at 44.1kHz.
He will not accept compressed audio files
The audio codec, in its current form, can only process lossless WAV files at 44.1kHz. Nothing else.

###### WIRING ⌁

The diagram on the following page shows the wiring from the main board, to the audio board along with basic input listed in the key. The number corresponds to the PJRC documentation for the Teensy 3.2. The following number scheme directly corresponds with the code for the PharmMonster.

![alt text][wiring]

[wiring]: https://i.imgur.com/VG19hHe.png?1 "Wiring diagram"

###### Control Interface 🎚

3X Switches in a grid pattern
located on back along with USB charging cable

| Switches             |           |
| -------------------- |:---------:|
| Easter Egg switch    | no switch |
| Disable Mic reaction | Power     |

## FEATURES

###### OREGON TRAIL GAME 🂡

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

###### 8 BALL MODE №8

8 Ball Mode turns the monster into an 8 Ball.

It responds to the tilt sensor if you hold him like Simba from the Lion King

![alt text][lionking]

[lionking]: https://i.imgur.com/C2NS3yv.png?1 "Naaaaaa seveenyyaaaaaaa"

To use 8 ball mode, hold hammy like Simba with his head to the sky and tail to the ground.
Once it notifies you that 8 ball mode is active, the light will shine. Give him a slight up and down shake to get your answer. (All answers are from the original 8 ball).
