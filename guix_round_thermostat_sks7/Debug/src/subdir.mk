################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/guix_round_thermostat_resources.c \
../src/guix_round_thermostat_specifications.c \
../src/guix_round_thermostat_thread_entry.c \
../src/lcd_setup.c 

OBJS += \
./src/guix_round_thermostat_resources.o \
./src/guix_round_thermostat_specifications.o \
./src/guix_round_thermostat_thread_entry.o \
./src/lcd_setup.o 

C_DEPS += \
./src/guix_round_thermostat_resources.d \
./src/guix_round_thermostat_specifications.d \
./src/guix_round_thermostat_thread_entry.d \
./src/lcd_setup.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio_5.3.1.002\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy_cfg\ssp_cfg\bsp" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy_cfg\ssp_cfg\driver" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\bsp" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\driver\api" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\driver\instances" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\src" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\src\synergy_gen" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy_cfg\ssp_cfg\framework" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\framework\api" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\framework\instances" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\framework\tes" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy_cfg\ssp_cfg\framework\el" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\inc\framework\el" -I"C:\synergy\e2studio_531002\workspaces\tech_support\guix_round_thermostat_sks7\synergy\ssp\src\framework\el\tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


