################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Service/CpuGeneric/SysSe/Time/Ifx_DateTime.c 

OBJS += \
./Libraries/Service/CpuGeneric/SysSe/Time/Ifx_DateTime.o 

COMPILED_SRCS += \
./Libraries/Service/CpuGeneric/SysSe/Time/Ifx_DateTime.src 

C_DEPS += \
./Libraries/Service/CpuGeneric/SysSe/Time/Ifx_DateTime.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Service/CpuGeneric/SysSe/Time/%.src: ../Libraries/Service/CpuGeneric/SysSe/Time/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gpt12" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Platform/Tricore/Compilers" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Multican/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Platform" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cif/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Hssl/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cpu/Trap" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/If/Ccu6If" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dsadc/Dsadc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Port" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Stm/Timer" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dts/Dts" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eth" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Flash" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Vadc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Msc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Qspi/SpiMaster" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Scu/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe/Comm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe/Math" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Platform/Tricore" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Trig" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tim" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/TimerWithTrigger" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Emem" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Mtu" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fft" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/I2c/I2c" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Asclin/Asc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Flash/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/If" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cpu" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fce/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Stm/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Msc/Msc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Vadc/Adc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Asclin" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Pwm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Atom" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Port/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5/Psi5" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eray" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Qspi/SpiSlave" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/Icu" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cpu/CStart" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Hssl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cif" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eth/Phy_Pef7071" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Hssl/Hssl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Iom/Driver" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Multican/Can" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5s/Psi5s" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fft/Fft" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmHl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Iom/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/_Lib" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Sent" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eray/Eray" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gpt12/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dma" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fce/Crc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Qspi" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Sfr" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Sfr/TC26B" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe/Bsp" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe/General" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cpu/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dts" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Src" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dma/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cif/Cam" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Src/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Asclin/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/I2c/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Configurations" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/_Lib/DataHandling" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Sent/Sent" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/Timer" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5s" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Emem/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmBc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Iom" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/TPwm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Multican" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Mtu/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Infra/Sfr/TC26B/_Reg" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/PwmHl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dma/Dma" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Timer" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/SysSe/Time" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dsadc/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Cpu/Irq" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Ccu6" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gpt12/IncrEnc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Psi5s/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Scu" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/_Lib/InternalMux" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Stm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dsadc/Rdc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Vadc/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dts/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eth/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Smu" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/_PinMap" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Asclin/Lin" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/StdIf" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Dsadc" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fce" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/PwmHl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Qspi/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tom" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Tim/In" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Msc/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Configurations/Debug" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Fft/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Pwm" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/Service/CpuGeneric/_Utilities" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Gtm/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Smu/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/I2c" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Asclin/Spi" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Eray/Std" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Port/Io" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/_Impl" -I"E:/Users/10947/AURIX-v1.2.4-workspace/CAN_transmit/Libraries/iLLD/TC26B/Tricore/Sent/Std" --iso=99 --c++14 --language=+volatile --anachronisms --fp-model=3 --fp-model=c --fp-model=f --fp-model=l --fp-model=n --fp-model=r --fp-model=z -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file=$(@:.src=.d) --misrac-version=2012 -N0 -Z0 -Y0 2>&1; sed -i -e '/ctc\\include/d' -e '/Libraries\\iLLD/d' -e '/Libraries\\Infra/d' -e 's/\(.*\)".*\\CAN_transmit\(\\.*\)"/\1\.\.\2/g' -e 's/\\/\//g' $(@:.src=.d) && \
	echo $(@:.src=.d) generated
	@echo 'Finished building: $<'
	@echo ' '

Libraries/Service/CpuGeneric/SysSe/Time/%.o: ./Libraries/Service/CpuGeneric/SysSe/Time/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


