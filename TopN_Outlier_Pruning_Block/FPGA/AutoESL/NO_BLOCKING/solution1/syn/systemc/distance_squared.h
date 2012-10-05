// ==============================================================
// RTL generated by AutoESL - High-Level Synthesis System (C, C++, SystemC)
// Version: 2012.1
// Copyright (C) 2012 Xilinx Inc. All rights reserved.
// 
// ===========================================================

#ifndef _distance_squared_HH_
#define _distance_squared_HH_

#include "systemc.h"
#include "AESL_pkg.h"

#include "distance_squared_grp_fu_219_ACMP_dadddsub_1.h"
#include "distance_squared_grp_fu_223_ACMP_dadd_2.h"
#include "distance_squared_grp_fu_228_ACMP_dmul_3.h"
#include "distance_squared_grp_fu_232_ACMP_dcmp_4.h"
#include "distance_squared_grp_fu_238_ACMP_sitodp_5.h"
#include "distance_squared_grp_fu_241_ACMP_sitodp_6.h"
#include "distance_squared_sum_of_squares_split.h"

namespace ap_rtl {

struct distance_squared : public sc_module {
    // Port declarations 24
    sc_in_clk ap_clk;
    sc_in< sc_logic > ap_rst;
    sc_in< sc_logic > ap_start;
    sc_out< sc_logic > ap_done;
    sc_out< sc_logic > ap_idle;
    sc_in< sc_lv<64> > vector1_data_V_dout;
    sc_in< sc_logic > vector1_data_V_empty_n;
    sc_out< sc_logic > vector1_data_V_read;
    sc_in< sc_lv<8> > vector1_keep_V_dout;
    sc_in< sc_logic > vector1_keep_V_empty_n;
    sc_out< sc_logic > vector1_keep_V_read;
    sc_in< sc_lv<1> > vector1_last_V_dout;
    sc_in< sc_logic > vector1_last_V_empty_n;
    sc_out< sc_logic > vector1_last_V_read;
    sc_in< sc_lv<64> > vector2_data_V_dout;
    sc_in< sc_logic > vector2_data_V_empty_n;
    sc_out< sc_logic > vector2_data_V_read;
    sc_in< sc_lv<8> > vector2_keep_V_dout;
    sc_in< sc_logic > vector2_keep_V_empty_n;
    sc_out< sc_logic > vector2_keep_V_read;
    sc_in< sc_lv<1> > vector2_last_V_dout;
    sc_in< sc_logic > vector2_last_V_empty_n;
    sc_out< sc_logic > vector2_last_V_read;
    sc_out< sc_lv<64> > sum;


    // Module declarations
    distance_squared(sc_module_name name);
    SC_HAS_PROCESS(distance_squared);

    ~distance_squared();

    sc_trace_file* mVcdFile;

    distance_squared_sum_of_squares_split* sum_of_squares_split_U;
    distance_squared_grp_fu_219_ACMP_dadddsub_1<1,5,64,64,64>* distance_squared_grp_fu_219_ACMP_dadddsub_1_U;
    distance_squared_grp_fu_223_ACMP_dadd_2<2,5,64,64,64>* distance_squared_grp_fu_223_ACMP_dadd_2_U;
    distance_squared_grp_fu_228_ACMP_dmul_3<3,6,64,64,64>* distance_squared_grp_fu_228_ACMP_dmul_3_U;
    distance_squared_grp_fu_232_ACMP_dcmp_4<4,3,64,64,1>* distance_squared_grp_fu_232_ACMP_dcmp_4_U;
    distance_squared_grp_fu_238_ACMP_sitodp_5<5,4,64,64>* distance_squared_grp_fu_238_ACMP_sitodp_5_U;
    distance_squared_grp_fu_241_ACMP_sitodp_6<6,4,64,64>* distance_squared_grp_fu_241_ACMP_sitodp_6_U;
    sc_signal< sc_lv<6> > ap_CS_fsm;
    sc_signal< sc_lv<8> > dim_reg_207;
    sc_signal< sc_lv<64> > grp_fu_219_p2;
    sc_signal< sc_lv<64> > reg_244;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it10;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it0;
    sc_signal< sc_lv<1> > exitcond1_reg_326;
    sc_signal< bool > ap_sig_bdd_100;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it1;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it2;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it3;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it4;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it5;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it6;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it7;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it8;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it9;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it11;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it12;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it13;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it14;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it15;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it16;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it17;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it18;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it19;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it20;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it21;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it22;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it9;
    sc_signal< sc_lv<64> > sum_of_squares_split_q0;
    sc_signal< sc_lv<64> > reg_251;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it15;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_gep_fu_125_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_reg_286;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_1_gep_fu_139_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_1_reg_291;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_2_gep_fu_152_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_2_reg_296;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_3_gep_fu_160_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_3_reg_301;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_4_gep_fu_168_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_4_reg_306;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_5_gep_fu_176_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_5_reg_311;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_6_gep_fu_184_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_6_reg_316;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_7_gep_fu_192_p3;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_7_reg_321;
    sc_signal< sc_lv<1> > exitcond1_fu_258_p2;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it1;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it2;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it3;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it4;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it5;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it6;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it7;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it8;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it10;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it11;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it12;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it13;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it14;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it16;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it17;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it18;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it19;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it20;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond1_reg_326_pp0_it21;
    sc_signal< sc_lv<3> > dim_cast2_reg_335;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it1;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it2;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it3;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it4;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it5;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it6;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it7;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it8;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it9;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it10;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it11;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it12;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it13;
    sc_signal< sc_lv<3> > ap_reg_ppstg_dim_cast2_reg_335_pp0_it14;
    sc_signal< sc_lv<64> > vector1_data_V_read_reg_340;
    sc_signal< sc_lv<64> > vector2_data_V_read_reg_345;
    sc_signal< sc_lv<64> > grp_fu_238_p1;
    sc_signal< sc_lv<64> > vector1_data_reg_350;
    sc_signal< sc_lv<64> > grp_fu_241_p1;
    sc_signal< sc_lv<64> > vector2_data_reg_355;
    sc_signal< sc_lv<3> > sum_of_squares_split_addr_8_reg_360;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it16;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it17;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it18;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it19;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it20;
    sc_signal< sc_lv<3> > ap_reg_ppstg_sum_of_squares_split_addr_8_reg_360_pp0_it21;
    sc_signal< sc_lv<64> > grp_fu_228_p2;
    sc_signal< sc_lv<64> > diff_squared_reg_366;
    sc_signal< sc_lv<64> > grp_fu_223_p2;
    sc_signal< sc_lv<64> > tmp_3_reg_371;
    sc_signal< sc_lv<64> > sum_of_squares_reg_376;
    sc_signal< sc_lv<3> > sum_of_squares_split_address0;
    sc_signal< sc_logic > sum_of_squares_split_ce0;
    sc_signal< sc_logic > sum_of_squares_split_we0;
    sc_signal< sc_lv<64> > sum_of_squares_split_d0;
    sc_signal< sc_lv<3> > sum_of_squares_split_address1;
    sc_signal< sc_logic > sum_of_squares_split_ce1;
    sc_signal< sc_logic > sum_of_squares_split_we1;
    sc_signal< sc_lv<64> > sum_of_squares_split_d1;
    sc_signal< sc_lv<64> > tmp_2_fu_274_p1;
    sc_signal< sc_lv<64> > grp_fu_219_p0;
    sc_signal< sc_lv<64> > grp_fu_219_p1;
    sc_signal< sc_lv<64> > grp_fu_223_p0;
    sc_signal< sc_lv<64> > grp_fu_223_p1;
    sc_signal< sc_lv<64> > grp_fu_228_p0;
    sc_signal< sc_lv<64> > grp_fu_228_p1;
    sc_signal< sc_lv<64> > grp_fu_232_p0;
    sc_signal< sc_lv<64> > grp_fu_232_p1;
    sc_signal< sc_lv<64> > grp_fu_238_p0;
    sc_signal< sc_lv<64> > grp_fu_241_p0;
    sc_signal< sc_lv<1> > grp_fu_232_p2;
    sc_signal< sc_lv<2> > grp_fu_219_opcode;
    sc_signal< sc_logic > grp_fu_219_ce;
    sc_signal< sc_logic > grp_fu_223_ce;
    sc_signal< sc_logic > grp_fu_228_ce;
    sc_signal< sc_logic > grp_fu_232_ce;
    sc_signal< sc_lv<5> > grp_fu_232_opcode;
    sc_signal< sc_logic > grp_fu_238_ce;
    sc_signal< sc_logic > grp_fu_241_ce;
    sc_signal< sc_lv<6> > ap_NS_fsm;
    sc_signal< sc_lv<64> > sum_of_squares_split_addr_8_reg_3600;
    static const sc_logic ap_const_logic_1;
    static const sc_logic ap_const_logic_0;
    static const sc_lv<6> ap_ST_st0_fsm_0;
    static const sc_lv<6> ap_ST_st1_fsm_1;
    static const sc_lv<6> ap_ST_st2_fsm_2;
    static const sc_lv<6> ap_ST_st3_fsm_3;
    static const sc_lv<6> ap_ST_st4_fsm_4;
    static const sc_lv<6> ap_ST_pp0_stg0_fsm_5;
    static const sc_lv<6> ap_ST_st28_fsm_6;
    static const sc_lv<6> ap_ST_st29_fsm_7;
    static const sc_lv<6> ap_ST_st30_fsm_8;
    static const sc_lv<6> ap_ST_st31_fsm_9;
    static const sc_lv<6> ap_ST_st32_fsm_10;
    static const sc_lv<6> ap_ST_st33_fsm_11;
    static const sc_lv<6> ap_ST_st34_fsm_12;
    static const sc_lv<6> ap_ST_st35_fsm_13;
    static const sc_lv<6> ap_ST_st36_fsm_14;
    static const sc_lv<6> ap_ST_st37_fsm_15;
    static const sc_lv<6> ap_ST_st38_fsm_16;
    static const sc_lv<6> ap_ST_st39_fsm_17;
    static const sc_lv<6> ap_ST_st40_fsm_18;
    static const sc_lv<6> ap_ST_st41_fsm_19;
    static const sc_lv<6> ap_ST_st42_fsm_20;
    static const sc_lv<6> ap_ST_st43_fsm_21;
    static const sc_lv<6> ap_ST_st44_fsm_22;
    static const sc_lv<6> ap_ST_st45_fsm_23;
    static const sc_lv<6> ap_ST_st46_fsm_24;
    static const sc_lv<6> ap_ST_st47_fsm_25;
    static const sc_lv<6> ap_ST_st48_fsm_26;
    static const sc_lv<6> ap_ST_st49_fsm_27;
    static const sc_lv<6> ap_ST_st50_fsm_28;
    static const sc_lv<6> ap_ST_st51_fsm_29;
    static const sc_lv<6> ap_ST_st52_fsm_30;
    static const sc_lv<6> ap_ST_st53_fsm_31;
    static const sc_lv<6> ap_ST_st54_fsm_32;
    static const sc_lv<6> ap_ST_st55_fsm_33;
    static const sc_lv<6> ap_ST_st56_fsm_34;
    static const sc_lv<6> ap_ST_st57_fsm_35;
    static const sc_lv<6> ap_ST_st58_fsm_36;
    static const sc_lv<6> ap_ST_st59_fsm_37;
    static const sc_lv<6> ap_ST_st60_fsm_38;
    static const sc_lv<6> ap_ST_st61_fsm_39;
    static const sc_lv<6> ap_ST_st62_fsm_40;
    static const sc_lv<6> ap_ST_st63_fsm_41;
    static const sc_lv<6> ap_ST_st64_fsm_42;
    static const sc_lv<6> ap_ST_st65_fsm_43;
    static const sc_lv<6> ap_ST_st66_fsm_44;
    static const sc_lv<1> ap_const_lv1_0;
    static const sc_lv<8> ap_const_lv8_0;
    static const sc_lv<64> ap_const_lv64_0;
    static const sc_lv<64> ap_const_lv64_1;
    static const sc_lv<64> ap_const_lv64_2;
    static const sc_lv<64> ap_const_lv64_3;
    static const sc_lv<64> ap_const_lv64_4;
    static const sc_lv<64> ap_const_lv64_5;
    static const sc_lv<64> ap_const_lv64_6;
    static const sc_lv<64> ap_const_lv64_7;
    static const sc_lv<64> ap_const_lv64_8000000000000000;
    static const sc_lv<8> ap_const_lv8_C8;
    static const sc_lv<8> ap_const_lv8_1;
    static const sc_lv<2> ap_const_lv2_0;
    static const sc_lv<2> ap_const_lv2_1;
    static const sc_lv<5> ap_const_lv5_1;
    // Thread declarations
    void thread_ap_clk_no_reset_();
    void thread_ap_NS_fsm();
    void thread_ap_done();
    void thread_ap_idle();
    void thread_ap_sig_bdd_100();
    void thread_exitcond1_fu_258_p2();
    void thread_grp_fu_219_ce();
    void thread_grp_fu_219_opcode();
    void thread_grp_fu_219_p0();
    void thread_grp_fu_219_p1();
    void thread_grp_fu_223_ce();
    void thread_grp_fu_223_p0();
    void thread_grp_fu_223_p1();
    void thread_grp_fu_228_ce();
    void thread_grp_fu_228_p0();
    void thread_grp_fu_228_p1();
    void thread_grp_fu_232_ce();
    void thread_grp_fu_232_opcode();
    void thread_grp_fu_232_p0();
    void thread_grp_fu_232_p1();
    void thread_grp_fu_238_ce();
    void thread_grp_fu_238_p0();
    void thread_grp_fu_241_ce();
    void thread_grp_fu_241_p0();
    void thread_sum();
    void thread_sum_of_squares_split_addr_1_gep_fu_139_p3();
    void thread_sum_of_squares_split_addr_2_gep_fu_152_p3();
    void thread_sum_of_squares_split_addr_3_gep_fu_160_p3();
    void thread_sum_of_squares_split_addr_4_gep_fu_168_p3();
    void thread_sum_of_squares_split_addr_5_gep_fu_176_p3();
    void thread_sum_of_squares_split_addr_6_gep_fu_184_p3();
    void thread_sum_of_squares_split_addr_7_gep_fu_192_p3();
    void thread_sum_of_squares_split_addr_8_reg_3600();
    void thread_sum_of_squares_split_addr_gep_fu_125_p3();
    void thread_sum_of_squares_split_address0();
    void thread_sum_of_squares_split_address1();
    void thread_sum_of_squares_split_ce0();
    void thread_sum_of_squares_split_ce1();
    void thread_sum_of_squares_split_d0();
    void thread_sum_of_squares_split_d1();
    void thread_sum_of_squares_split_we0();
    void thread_sum_of_squares_split_we1();
    void thread_tmp_2_fu_274_p1();
    void thread_vector1_data_V_read();
    void thread_vector1_keep_V_read();
    void thread_vector1_last_V_read();
    void thread_vector2_data_V_read();
    void thread_vector2_keep_V_read();
    void thread_vector2_last_V_read();
    void thread_hdltv_gen();
};

}

using namespace ap_rtl;

#endif
