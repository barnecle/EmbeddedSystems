################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/battery.c \
../Core/Src/interrupt.c \
../Core/Src/main.c \
../Core/Src/queue.c \
../Core/Src/retarget.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/temp.c 

OBJS += \
./Core/Src/battery.o \
./Core/Src/interrupt.o \
./Core/Src/main.o \
./Core/Src/queue.o \
./Core/Src/retarget.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/temp.o 

C_DEPS += \
./Core/Src/battery.d \
./Core/Src/interrupt.d \
./Core/Src/main.d \
./Core/Src/queue.d \
./Core/Src/retarget.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/temp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/battery.d ./Core/Src/battery.o ./Core/Src/interrupt.d ./Core/Src/interrupt.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/queue.d ./Core/Src/queue.o ./Core/Src/retarget.d ./Core/Src/retarget.o ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/temp.d ./Core/Src/temp.o

.PHONY: clean-Core-2f-Src

