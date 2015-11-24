################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LCD.obj: ../LCD.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="LCD.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

button_if.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/button_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="button_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

gpio_if.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="gpio_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

jsmn.obj: ../jsmn.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="jsmn.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

network_if.obj: ../network_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="network_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer_if.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/timer_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="timer_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="C:/Users/Alejandro/workspace_v6_1/nfc_reader" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/nfc_reader/nfc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/smtp/client" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib" -g --define=cc3200 --define=__CCS__ --define=ENABLE14443A --define=ENABLE_HOST --define=ENABLE15693 --define=TRF7970A --define=ccs --define=SL_PLATFORM_MULTI_THREADED --define=USE_TIRTOS --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


