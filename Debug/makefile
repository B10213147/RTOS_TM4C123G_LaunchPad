################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_9-2015q3

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.o" \
"./tm4c123gh6pm_startup_ccs_gcc.o" \
"./driverlib/adc.o" \
"./driverlib/aes.o" \
"./driverlib/can.o" \
"./driverlib/comp.o" \
"./driverlib/cpu.o" \
"./driverlib/crc.o" \
"./driverlib/des.o" \
"./driverlib/eeprom.o" \
"./driverlib/emac.o" \
"./driverlib/epi.o" \
"./driverlib/flash.o" \
"./driverlib/fpu.o" \
"./driverlib/gpio.o" \
"./driverlib/hibernate.o" \
"./driverlib/i2c.o" \
"./driverlib/interrupt.o" \
"./driverlib/lcd.o" \
"./driverlib/mpu.o" \
"./driverlib/onewire.o" \
"./driverlib/pwm.o" \
"./driverlib/qei.o" \
"./driverlib/shamd5.o" \
"./driverlib/ssi.o" \
"./driverlib/sw_crc.o" \
"./driverlib/sysctl.o" \
"./driverlib/sysexc.o" \
"./driverlib/systick.o" \
"./driverlib/timer.o" \
"./driverlib/uart.o" \
"./driverlib/udma.o" \
"./driverlib/usb.o" \
"./driverlib/watchdog.o" \
-Wl,-T"../tm4c123gh6pm.lds" \
$(GEN_CMDS__FLAG) \
-Wl,--start-group \
-lc \
-lgcc \
-lnosys \
-Wl,--end-group \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include driverlib/subdir_vars.mk
-include subdir_rules.mk
-include driverlib/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
test.out \

EXE_OUTPUTS__QUOTED += \
"test.out" \

BIN_OUTPUTS += \
test.hex \

BIN_OUTPUTS__QUOTED += \
"test.hex" \


# All Target
all: test.out

# Tool invocations
test.out: $(OBJS) $(LDS_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Linker'
	"C:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc.exe" -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -Dgcc -DTARGET_IS_TM4C123_RB1 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -Wl,-Map,"test.map" -o"test.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

test.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: GNU Objcopy Utility'
	"C:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-objcopy.exe"  $(EXE_OUTPUTS__QUOTED) "test.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main.d" "tm4c123gh6pm_startup_ccs_gcc.d" "driverlib\adc.d" "driverlib\aes.d" "driverlib\can.d" "driverlib\comp.d" "driverlib\cpu.d" "driverlib\crc.d" "driverlib\des.d" "driverlib\eeprom.d" "driverlib\emac.d" "driverlib\epi.d" "driverlib\flash.d" "driverlib\fpu.d" "driverlib\gpio.d" "driverlib\hibernate.d" "driverlib\i2c.d" "driverlib\interrupt.d" "driverlib\lcd.d" "driverlib\mpu.d" "driverlib\onewire.d" "driverlib\pwm.d" "driverlib\qei.d" "driverlib\shamd5.d" "driverlib\ssi.d" "driverlib\sw_crc.d" "driverlib\sysctl.d" "driverlib\sysexc.d" "driverlib\systick.d" "driverlib\timer.d" "driverlib\uart.d" "driverlib\udma.d" "driverlib\usb.d" "driverlib\watchdog.d" 
	-$(RM) "main.o" "tm4c123gh6pm_startup_ccs_gcc.o" "driverlib\adc.o" "driverlib\aes.o" "driverlib\can.o" "driverlib\comp.o" "driverlib\cpu.o" "driverlib\crc.o" "driverlib\des.o" "driverlib\eeprom.o" "driverlib\emac.o" "driverlib\epi.o" "driverlib\flash.o" "driverlib\fpu.o" "driverlib\gpio.o" "driverlib\hibernate.o" "driverlib\i2c.o" "driverlib\interrupt.o" "driverlib\lcd.o" "driverlib\mpu.o" "driverlib\onewire.o" "driverlib\pwm.o" "driverlib\qei.o" "driverlib\shamd5.o" "driverlib\ssi.o" "driverlib\sw_crc.o" "driverlib\sysctl.o" "driverlib\sysexc.o" "driverlib\systick.o" "driverlib\timer.o" "driverlib\uart.o" "driverlib\udma.o" "driverlib\usb.o" "driverlib\watchdog.o" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

