################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/CUG_Init.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Image_Elements.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Image_Process.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Menu.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Speed_Judge.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/motor.c \
C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/pid_positional.c 

OBJS += \
./code/CUG_Init.o \
./code/Image_Elements.o \
./code/Image_Process.o \
./code/Menu.o \
./code/Speed_Judge.o \
./code/motor.o \
./code/pid_positional.o 

C_DEPS += \
./code/CUG_Init.d \
./code/Image_Elements.d \
./code/Image_Process.d \
./code/Menu.d \
./code/Speed_Judge.d \
./code/motor.d \
./code/pid_positional.d 


# Each subdirectory must supply rules for building sources it contributes
code/CUG_Init.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/CUG_Init.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Image_Elements.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Image_Elements.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Image_Process.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Image_Process.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Menu.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Menu.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Speed_Judge.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/Speed_Judge.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/motor.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/motor.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/pid_positional.o: C:/Users/Joker/Desktop/Æøµæ´¬2024.6.9/my_project/code/pid_positional.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\Libraries\doc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_components" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Core" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Ld" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Peripheral" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\sdk\Startup" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\user\inc" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_common" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_device" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\my_project\code" -I"C:\Users\Joker\Desktop\Æøµæ´¬2024.6.9\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

