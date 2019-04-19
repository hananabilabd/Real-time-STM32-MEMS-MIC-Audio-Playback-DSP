################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/crc.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2s.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/crc.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2s.o \
./Src/main.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/crc.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2s.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F446xx -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\CMSIS\Include" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\STM32F4xx_HAL_Driver\Inc" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


