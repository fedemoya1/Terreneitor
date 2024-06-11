################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/util/DWT_Delay.c 

OBJS += \
./Core/Src/util/DWT_Delay.o 

C_DEPS += \
./Core/Src/util/DWT_Delay.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/util/%.o Core/Src/util/%.su Core/Src/util/%.cyclo: ../Core/Src/util/%.c Core/Src/util/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-util

clean-Core-2f-Src-2f-util:
	-$(RM) ./Core/Src/util/DWT_Delay.cyclo ./Core/Src/util/DWT_Delay.d ./Core/Src/util/DWT_Delay.o ./Core/Src/util/DWT_Delay.su

.PHONY: clean-Core-2f-Src-2f-util

