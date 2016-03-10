CC=avr-gcc -O3 -mmcu=atmega1284p -Wl,-u,vfprintf -lprintf_flt -lm
CFLAGS=-g -Os -Wall -mcall-prologues
OBJ2HEX=avr-objcopy
#UISP=/usr/local/bin/uisp
TARGET=test
#ADFLAGS=-p m8 -c FT232 -P /dev/tty.usbserial-A8003POq 
ADFLAGS=-p m1284p -c FT232 -P /dev/ttyUSB0
# ADFLAGS=-p m1284p -c avrispmkii -P usb

.PHONY: fuses flash erase # .PHONY means that these are not a target


flash : $(TARGET).hex $(TARGET).eeprom  #this programs the chip, depends on Target.hex and Target.eeprom
	avrdude $(ADFLAGS) -V -U flash:w:$(TARGET).hex:i
#       avrdude $(ADFLAGS) -U eeprom:w:$(TARGET).eeprom:i

%.obj : %.o   # make .obj (any obj) if corresponding .o is newer
	 $(CC) $(CFLAGS) $< -o $@  # $@ means target of rule, $< means prerequisite of rule

%.hex : %.obj  # similarly, make any .hex if corresponding .obj is newer
	$(OBJ2HEX) -R .eeprom -O ihex $< $@  # remove  eeprom output ihex translate .obj into hex file

%.eeprom : %.obj  # any eeprom depends on same .obj
	$(OBJ2HEX) -j .eeprop -O ihex $< $@ 

erase :   # this target will just erase avr
	avrdude $(ADFLAGS) -E noreset -e
clean :  # this target will remove all non-source files (will cause a complete re-build on next make)
	rm -f *.hex *.obj *.o

fuses: # program the fuses (NOT done by regular make)
	avrdude $(ADFLAGS) -U lfuse:w:0xC2:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m #www.engbedded.com/fusecalc
