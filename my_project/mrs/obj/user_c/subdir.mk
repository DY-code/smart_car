################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/user/src/isr.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/user/src/main.c 

OBJS += \
./user_c/isr.o \
./user_c/main.o 

C_DEPS += \
./user_c/isr.d \
./user_c/main.d 


# Each subdirectory must supply rules for building sources it contributes
user_c/isr.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/user/src/isr.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/main.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/user/src/main.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

