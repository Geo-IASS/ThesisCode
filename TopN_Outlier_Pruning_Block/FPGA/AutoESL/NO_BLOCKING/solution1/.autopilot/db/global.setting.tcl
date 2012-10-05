
set TopModule "distance_squared"
set ClockPeriod "10.000000"
set ClockList {ap_clk}
set multiClockList {}
set CombLogicFlag 0
set PipelineFlag 0
set DataflowTaskPipelineFlag  0
set TrivialPipelineFlag 0
set FloatingPointFlag 1
set ResetLevelFlag 1
set ResetStyle "control"
set ResetSyncFlag 1
set RtlPrefix ""
set ExtraCCFlags ""
set ExtraCLdFlags ""
set SynCheckOptions ""
set PresynOptions ""
set PreprocOptions ""
set SchedOptions ""
set BindOptions ""
set RtlGenOptions ""
set RtlWriterOptions ""
set CbcGenFlag ""
set CasGenFlag ""
set CasMonitorFlag ""
set AutoSimOptions {}
set ExportMCPathFlag "0"
set SCTraceFileName "mytrace"
set SCTraceFileFormat "vcd"
set SCTraceOption "all"
set TargetInfo "xc6vlx240t:ff1156:-1"
set SourceFiles {sc {} c ../../../source/top_n_outlier_pruning_block.cpp}
set SourceFlags {sc {} c {{-D__AUTOESL__ -DSORTED_INSERT -DNO_BLOCKING -DUSE_DYNAMIC_ARRAY_SIZE=0 -USE_INLINE=0 -I../include}}}
set DirectiveFile {/home/joshua/Documents/University/Thesis/Code/TopN_Outlier_Pruning_Block/AutoESL/NO_BLOCKING/solution1/solution1.directive}
set TBFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TVInFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TVOutFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TBTops {bc "" c "" sc "" cas "" vhdl "" verilog ""}
set TBInstNames {bc "" c "" sc "" cas "" vhdl "" verilog ""}
set ExtraGlobalOptions {"area_timing" 1 "clock_gate" 1 "impl_flow" map "power_gate" 0}
set PlatformFiles {{DefaultPlatform {xilinx/virtex6/virtex6 xilinx/virtex6/virtex6_fpv6}}}
set DefaultPlatform "DefaultPlatform"
set TBTVFileNotFound ""
set AppFile "../autopilot.app"
set ApsFile "solution1.aps"
set AvePath "../.."
