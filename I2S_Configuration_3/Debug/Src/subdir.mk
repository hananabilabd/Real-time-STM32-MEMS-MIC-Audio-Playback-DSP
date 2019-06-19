################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/arm_fir_data.c \
../Src/arm_fir_example_f32.c \
../Src/arm_graphic_equalizer_data.c \
../Src/crc.c \
../Src/dac.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2s.c \
../Src/main.c \
../Src/math_helper.c \
../Src/pdm2pcm.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/arm_fir_data.o \
./Src/arm_fir_example_f32.o \
./Src/arm_graphic_equalizer_data.o \
./Src/crc.o \
./Src/dac.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2s.o \
./Src/main.o \
./Src/math_helper.o \
./Src/pdm2pcm.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/arm_fir_data.d \
./Src/arm_fir_example_f32.d \
./Src/arm_graphic_equalizer_data.d \
./Src/crc.d \
./Src/dac.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2s.d \
./Src/main.d \
./Src/math_helper.d \
./Src/pdm2pcm.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Inc" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Middlewares/ST/STM32_Audio/Addons/PDM/Inc" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/Drivers/CMSIS/Include" -I"F:/ITI/Digital-Hearing-Aid-STM32/I2S_Configuration_3/FilteringFunctionInclude"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


