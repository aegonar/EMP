################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/bin/cl430" -vmsp --abi=coffabi -Ooff -g --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.6/include" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/Hardware" --include_path="C:/Users/Alejandro/workspace_v6_1/TRF7970ABP_Demo/NFC" --define=__MSP430G2553__ --define=ENABLE15693 --define=ENABLE14443A --define=ENABLE14443B --define=TRF7970A --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


