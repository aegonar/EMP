################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Hardware/spi.obj: ../Hardware/spi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmsp --abi=coffabi -Ooff -g --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/Hardware" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/NFC" --define=__MSP430G2553__ --define=ENABLE15693 --define=ENABLE14443A --define=ENABLE14443B --define=TRF7970A --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="Hardware/spi.pp" --obj_directory="Hardware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Hardware/trf7970.obj: ../Hardware/trf7970.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmsp --abi=coffabi -Ooff -g --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/Hardware" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/NFC" --define=__MSP430G2553__ --define=ENABLE15693 --define=ENABLE14443A --define=ENABLE14443B --define=TRF7970A --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="Hardware/trf7970.pp" --obj_directory="Hardware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Hardware/trf7970BoosterPack.obj: ../Hardware/trf7970BoosterPack.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmsp --abi=coffabi -Ooff -g --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/Hardware" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/NFC" --define=__MSP430G2553__ --define=ENABLE15693 --define=ENABLE14443A --define=ENABLE14443B --define=TRF7970A --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="Hardware/trf7970BoosterPack.pp" --obj_directory="Hardware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Hardware/uart.obj: ../Hardware/uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmsp --abi=coffabi -Ooff -g --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/Hardware" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/NFC" --define=__MSP430G2553__ --define=ENABLE15693 --define=ENABLE14443A --define=ENABLE14443B --define=TRF7970A --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="Hardware/uart.pp" --obj_directory="Hardware" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


