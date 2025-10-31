################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c 

S_UPPER_SRCS += \
../utilities/fsl_memcpy.S 

C_DEPS += \
./utilities/fsl_assert.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_memcpy.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180 -DCPU_LPC54628J512ET180_cm4 -DMCUXPRESSO_SDK -DCPU_LPC54628 -D__USE_CMSIS -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DLPC54628_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\source" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\drivers" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\CMSIS" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\CMSIS\m-profile" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\device" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\device\periph" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities\str" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities\debug_console_lite" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\component\uart" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/%.o: ../utilities/%.S utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\source" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\drivers" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\CMSIS" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\CMSIS\m-profile" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\device" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\device\periph" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities\str" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\utilities\debug_console_lite" -I"C:\Users\alexa\Documents\MCUXpressoIDE_25.6.136\workspace\lpcxpresso54628_sctimer_simple_pwm\component\uart" -g3 -gdwarf-4 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_memcpy.o

.PHONY: clean-utilities

