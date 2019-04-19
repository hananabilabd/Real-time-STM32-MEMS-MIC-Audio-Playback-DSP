################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f446xx.S 

OBJS += \
./startup/startup_stm32f446xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32f446xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F446xx -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\CMSIS\Include" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Drivers\STM32F4xx_HAL_Driver\Inc" -I"D:\ITI_Intake_39\Embedded_Systems\Intro_To_Embedded_Systems_Ahmed_Assaf\Device_Drivers\02-Projects\GP_Test_1\Inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


