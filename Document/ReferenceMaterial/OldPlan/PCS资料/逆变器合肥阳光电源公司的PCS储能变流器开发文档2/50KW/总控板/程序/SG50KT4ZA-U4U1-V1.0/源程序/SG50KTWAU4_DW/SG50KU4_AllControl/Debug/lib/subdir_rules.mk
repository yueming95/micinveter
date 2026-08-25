################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
lib/IQmath.exe: E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW/lib/IQmath.lib $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --display_error_number --issue_remarks --diag_wrap=off --asm_directory="E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW/Debug" --obj_directory="E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW/Debug" -z -m"E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW/lib/Debug/SG100KT.map" --stack_size=0x400 --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" -i"E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW/SG50KU4_AllControl" -i"E:/work/SG50KT4ZA/program/SPWM/SG50KTWAU4_DW" -i"E:/work/SG50KT4ZA/program/SPWM/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="SG50KU4_AllControl_linkInfo.xml" --rom_model -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


