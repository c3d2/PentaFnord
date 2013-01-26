PROJECT=pentafnord

OPTIMIZATION = -Os
MCU = atmega48
F_CPU = 20000000
QUARZ = 20MHZ
FUSES = -U lfuse:w:0xc7:m -U hfuse:w:0xdc:m -U efuse:w:0x01:m

#########################################################################

SRC=$(wildcard lib/*.c *.c) 
OBJECTS= $(SRC:.c=.o) 
LSTFILES= $(SRC:.c=.lst)
HEADERS=$(wildcard lib/*.h *.h)

#  Compiler Options
GCFLAGS = -ffreestanding -std=gnu99 -mmcu=$(MCU) $(OPTIMIZATION) -Wl,-gc-sections -nostdlib -I. 
# Warnings
GCFLAGS += -Wstrict-prototypes -Wundef -Wall -Wextra -Wunreachable-code  
# Optimizazions
GCFLAGS += -fsingle-precision-constant -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-builtin -ffunction-sections -fno-common -fdata-sections 
# Debug stuff
GCFLAGS += -Wa,-adhlns=$(<:.c=.lst),-gstabs -g 

GCFLAGS += -DF_CPU=$(F_CPU) -DQUARZ=$(QUARZ)

LDFLAGS = -mmcu=$(MCU) $(OPTIMIZATION) -gc-sections


#  Paths
GCC = avr-gcc
OBJCOPY = avr-objcopy
REMOVE = rm -f
SIZE = avr-size
AVRDUDE = avrdude

#########################################################################

all: $(PROJECT).hex Makefile 
	@$(SIZE) --mcu=$(MCU) --format=avr $(PROJECT).elf

$(PROJECT).hex: $(PROJECT).elf Makefile
	@echo "  \033[1;34mOBJCOPY\033[0m $(PROJECT).elf -> $(PROJECT).hex"
	@$(OBJCOPY) -O ihex -R .eeprom $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJECTS) Makefile
	@echo "  \033[1;34mLink   \033[0m (\033[1;33m $(OBJECTS)\033[0m) -> $(PROJECT).elf"
	@$(GCC) $(OBJECTS) $(LDFLAGS)  -o $(PROJECT).elf

clean:
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(LSTFILES)
	$(REMOVE) $(PROJECT).hex
	$(REMOVE) $(PROJECT).elf

#########################################################################

%.o: %.c Makefile $(HEADERS)
	@echo "  \033[1;34mCompile\033[0m $<"
	@$(GCC) $(GCFLAGS) -o $@ -c $<


#########################################################################

fuse: 
	$(AVRDUDE) -p m48 -F -c usbtiny  -v -v  $(FUSES)

flash: $(PROJECT).hex Makefile
	$(AVRDUDE) -p m48 -F -c usbtiny  -v -v  -B 1 -U flash:w:$(PROJECT).hex


flashmart: $(PROJECT).hex Makefile
	$(AVRDUDE) -p m48 -F  -P /dev/ttyUSB0 -c stk500v2 -v -v  -U flash:w:$(PROJECT).hex


.PHONY : clean all fuse flash flashsmart

