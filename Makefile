include $(DEVKITARM)/gba_rules

PATH := $(DEVKITARM)/bin:$(PATH)

# --- Project details -------------------------------------------------

PROJ    := $(notdir $(CURDIR))
TITLE   := $(PROJ)
SOURCE  := source
Build 	:= build
COBJS   := main.o spaceship.o

OBJS	:= $(COBJS)

# --- boot type (MB=0 : normal. MB=1 : multiboot) ---

MB = 0

ifeq ($(MB),1)

TARGET	:= $(PROJ).mb
SPECS	:= -specs=gba_mb.specs

else

TARGET	:= $(PROJ)
SPECS	:= -specs=gba.specs

endif

# --- Compiling -------------------------------------------------------

CROSS	?= arm-none-eabi-
AS		:= $(CROSS)as
CC		:= $(CROSS)gcc
LD		:= $(CROSS)gcc
OBJCOPY	:= $(CROSS)objcopy


ARCH	:= -mthumb-interwork -mthumb

LIBTONC = /opt/devkitpro/libtonc

ASFLAGS	:= -mthumb-interwork
CFLAGS	:= $(ARCH) -O2 -Wall -fno-strict-aliasing -I$(LIBTONC)/include
LDFLAGS	:= $(ARCH) $(SPECS) -L $(LIBTONC)/lib -ltonc

#LIBS	:= -ltonc
#devkitpro is a path
#LIBDIRS	:= $(DEVKITPRO)/libtonc

.PHONY : build clean

# --- Build -----------------------------------------------------------

build : $(TARGET).gba

#take relocatable obj and link
$(TARGET).gba : $(TARGET).elf
	#what does objcopy do here?
	$(OBJCOPY) -v -O binary $< $@
	#gbafix puts hash at the top of ROM
	-@gbafix $@ -t$(TITLE)

#take objs and turn into relocatable obj(s?)
#uses linker
$(TARGET).elf : $(OBJS)
	$(LD)  $^ $(LDFLAGS) -o $@
	#arm-none-eabi-gcc -mthumb-interwork -mthumb -O2 -Wall -fno-strict-aliasing $^ 

#compile f files intoclea objs
$(COBJS) : %.o : $(SOURCE)/%.c
	#gcc  -using all flags -c first prereq file -o target(cobjs)
	$(CC) $(CFLAGS) -c $< -o $@
#	arm-none-eabi-gcc -I $(DEVKITPRO)/libtonc/include -mthumb-interwork -mthumb -O2 -Wall -fno-strict-aliasing -c $< -o $@
	
# --- Clean -----------------------------------------------------------

clean : 
	@rm -fv *.gba
	@rm -fv *.elf
	@rm -fv *.o

#EOF
