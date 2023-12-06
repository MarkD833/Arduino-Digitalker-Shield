# Arduino UNO National Semiconductor Digitalker Shield
I recently discovered a National Semiconductor Digitalker IC in my collection. This one is packaged in a yellow box supplied by RS Componenets in the UK. I also have 2 of the Digitalker speech ROMS.

![Yellow boxes](https://github.com/MarkD833/Arduino-Digitalker-Shield/blob/main/images/YellowBoxes.JPG)

Now I'm more confident with using Eagle PCB (v7.7.0) and producing gerbers for JLCPCB I figured I'd have a go at producing a Digitalker shield for an Arduino UNO.

![Digitalker Shield](https://github.com/MarkD833/Arduino-Digitalker-Shield/blob/main/images/Board_2.JPG)

My main inspiration for this project is a web page published by [Dr. Scott M. Baker](https://www.smbaker.com/this-is-digitalker-and-jameco-je520-too-vintage-speech-synthesis) and the recent discovery of quite a few Digitalker ROMs that had been uploaded the the [Internet Archive](https://archive.org/details/digitalker).

You can read more about the Digitalker chip on [Dr. Scott M. Baker's](https://www.smbaker.com/this-is-digitalker-and-jameco-je520-too-vintage-speech-synthesis) web page.

## Description

The aim of this project is to create a shield for an Arduino UNO to bring to life a Digitalker speech IC and to have a large non-volatile storage device to store the contents of several speech ROMs in.

## Design

In order to speak a word (or phrase or sound), the Digitalker chip needs to know which word to speak and this is defined by the binary value on the SW1 (LSB) to SW8 (MSB) inputs of the Digitalker chip.

There's also Chip Select (/CS) which in a minimum configuration can be grounded along with the Command Select (/CMS) signal.

Finally, there is the Write Strobe (/WR) which initiates the generation of the speech on the rising edge.

### Word Selection

The Digitialker chip has 14 address lines (A0..A13) to address up to 16Kbytes of storage. The original speech ROMs had a storage capacity of 8Kbytes each. Address line A13 was used to select the lower or upper ROM. The lower ROM CS signal was active low whereas the upper ROM CS signal was active high.

I'm using an AM29F040B flash chip with a capacity of 512Kbytes to store the contents of the various speech ROMs currently available.

### Interface

I've chosen to use the SPI interface of the ATMEGA328P on the UNO and a couple of 74HCT595 serial-in / parallel-out shift registers to transfer information to the Digitalker chip. One shift register is dedicated to providing the 8-bit word number to speak. The other shift register handles address lines A14..A18 of the flash chip in order to select the pair of speech ROMs required.

I'm using the SPI signals MOSI and SCK from the ICSP header on the UNO rather than D11 and D13 in the hope that this board can also be used with an Arduino MEGA2560 as well. The SPI signals on the Arduino MEGA2560 board are on completely different pins, but they are also present on the ICSP header in the same physical location as on the UNO.

The SPI SS signal is routed to the D10 pin on the UNO and the Write Strobe is routed to the A1 pin on the UNO.   

### Power

The Digitalker chip requires a supply voltage of between 7V and 11V. I'm not a power supply designer so I used an off the shelf DC/DC boost module designed around the SDB628 IC and configured its output for 9V.

### Audio

I'm also not an audio designer so I used an off the shelf audio amplifier module - HXJ8002 - to handle the audio amplification.

# Assembly

Start with the SMD components - there are a few resistors and capacitors which should be easy to hand solder. Then add the various IC sockets and the header pins.

I mounted the DC/DC convertor and the audio module on pin headers so I could easily remove them if necessary.

When you've installed all the basic components, then before fitting any chips, check that you have 9V on the VCC pin of the Digitalker chip. These chips are rare and expensive to replace so best to check first.

Install the rest of the chips, excluding the flash memory as you will need to program it with your chosen speech ROMs. 

# Programming the speech ROMs

The speech ROMs are generally designed to be used in pairs.

As a basic check, start with the SSR1 & SSR2 ROMs. The files for these ROMs were named SSR1.bin and SSR2.bin in the archive I used.

Using a PROM programmer of your choice, load the SSR1.bin file into memory starting at address 0x0000. Next, load the SSR2.bin file but this time you need to make sure that it is loaded into your PROM programmer memory starting at address 0x2000.

The memory map in your PROM programmer should be similar to this if you also want to include the SSR5 & SSR6 ROMs as well:

 | ROM Bank | ROM 1 | ROM 2 |
 | -------- | ----- | ----- |
 | 0        | SSR1.bin (0000-1FFF) | SSR2.bin (2000-3FFF) |
 | 1        | SSR5.bin (4000-5FFF) | SSR6.bin (6000-7FFF) |
 
Then program the flash chip with the loaded data and install it in your board.

# Speaking the first words

Digitalker has it's own equivalent of [Hello World](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program). With the SSR1 and SSR2 ROMs programmed into the flash chip, word number 0 is the phrase "THIS IS DIGITALKER".

I've created a simple test sketch that you can use to speak any word in any ROM.

The commands are entered into the Arduino IDE serial monitor. There are only 2 commands:
* n : a number, for example - 0, 1 or 25 to speek word or phrase associated with that number
* Rn : R followed by a number will switch ROM sets - R2 switches to ROM set #2. Any words or phrases will now come from ROM set #2.

# Further information

Have a look in the datasheets folder for information on the Digitalker chips.

| Kit ID | Description |
| --- | --- |
| DT1000 | Speech synthesis evaluation board |
| DT1050(3) | Standard vocabulary kit - details the words in ROMs SSR1 & SSR2 |
| DT1051(4) | Speech evaluation kit - details the words in ROMs SSR3 & SSR4 |
| DT1052(5) | Basic numbers kit |
| DT1056(7) | Standard vocabulary kit - details the words in ROMs SSR5 & SSR6 |
| DTSW-500  | Digitalker vocabulary selection system (DVSS) (CP/M 2.2 application) |

The SSR3 and SSR4 ROMs have yet to be found despite the best searches of other Digitalker enthusiasts. The user manual for the DVSS also is proving elusive. The SSR3 and SSR4 ROMs could potentially be recreated using the DVSS software as the datasheet indicates that the DVSS software includes 500 words that could be used to create custom ROMs.

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE.md file for details

## Acknowledgments

Inspiration etc.
* [Dr. Scott M. Baker](https://www.smbaker.com/this-is-digitalker-and-jameco-je520-too-vintage-speech-synthesis)
