################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Joker/Desktop/���洬2024.6.9/libraries/sdk/Core/core_riscv.c 

OBJS += \
./sdk/Core/core_riscv.o 

C_DEPS += \
./sdk/Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/Core/core_riscv.o: C:/Users/Joker/Desktop/���洬2024.6.9/libraries/sdk/Core/core_riscv.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\���洬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\���洬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\���洬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\���洬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

