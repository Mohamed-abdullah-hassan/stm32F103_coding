################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/I2C.c \
../Src/LED.c \
../Src/font.c \
../Src/gpio.c \
../Src/hmc3883l.c \
../Src/main.c \
../Src/ssd1306.c \
../Src/ssd1306_test.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system.c \
../Src/time.c \
../Src/uart.c 

OBJS += \
./Src/I2C.o \
./Src/LED.o \
./Src/font.o \
./Src/gpio.o \
./Src/hmc3883l.o \
./Src/main.o \
./Src/ssd1306.o \
./Src/ssd1306_test.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system.o \
./Src/time.o \
./Src/uart.o 

C_DEPS += \
./Src/I2C.d \
./Src/LED.d \
./Src/font.d \
./Src/gpio.d \
./Src/hmc3883l.d \
./Src/main.d \
./Src/ssd1306.d \
./Src/ssd1306_test.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system.d \
./Src/time.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../mcal -I../Inc -I"F:/MyWork/Code/STM32/RTOS_I/CUBE_IDE/5_I2C_Driver/Chip_Headers/core" -I"F:/MyWork/Code/STM32/RTOS_I/CUBE_IDE/5_I2C_Driver/Chip_Headers/mcu" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/I2C.cyclo ./Src/I2C.d ./Src/I2C.o ./Src/I2C.su ./Src/LED.cyclo ./Src/LED.d ./Src/LED.o ./Src/LED.su ./Src/font.cyclo ./Src/font.d ./Src/font.o ./Src/font.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/hmc3883l.cyclo ./Src/hmc3883l.d ./Src/hmc3883l.o ./Src/hmc3883l.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/ssd1306.cyclo ./Src/ssd1306.d ./Src/ssd1306.o ./Src/ssd1306.su ./Src/ssd1306_test.cyclo ./Src/ssd1306_test.d ./Src/ssd1306_test.o ./Src/ssd1306_test.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system.cyclo ./Src/system.d ./Src/system.o ./Src/system.su ./Src/time.cyclo ./Src/time.d ./Src/time.o ./Src/time.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

