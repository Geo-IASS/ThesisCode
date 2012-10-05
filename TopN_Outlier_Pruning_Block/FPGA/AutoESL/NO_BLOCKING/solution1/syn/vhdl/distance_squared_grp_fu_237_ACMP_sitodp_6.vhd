-- ==============================================================
-- File generated by AutoESL - High-Level Synthesis System (C, C++, SystemC)
-- Version: 2012.1
-- Copyright (C) 2012 Xilinx Inc. All rights reserved.
-- 
-- ==============================================================

Library IEEE;
use IEEE.std_logic_1164.all;
Library work;
use work.all;
use work.AESL_components.all;

entity distance_squared_grp_fu_237_ACMP_sitodp_6 is
    generic (
        ID : INTEGER;
        NUM_STAGE : INTEGER;
        din0_WIDTH : INTEGER;
        dout_WIDTH : INTEGER);
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        ce : IN STD_LOGIC;
        din0 : IN STD_LOGIC_VECTOR(din0_WIDTH - 1 DOWNTO 0);
        dout : OUT STD_LOGIC_VECTOR(dout_WIDTH - 1 DOWNTO 0));
end entity;

architecture arch of distance_squared_grp_fu_237_ACMP_sitodp_6 is
    component ACMP_sitodp is
        generic (
            ID : INTEGER;
            NUM_STAGE : INTEGER;
            din0_WIDTH : INTEGER;
            dout_WIDTH : INTEGER);
        port (
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            ce : IN STD_LOGIC;
            din0 : IN STD_LOGIC_VECTOR(din0_WIDTH - 1 DOWNTO 0);
            dout : OUT STD_LOGIC_VECTOR(dout_WIDTH - 1 DOWNTO 0));
    end component;



begin
    ACMP_sitodp_U :  component ACMP_sitodp
    generic map (
        ID => ID,
        NUM_STAGE => 5,
        din0_WIDTH => din0_WIDTH,
        dout_WIDTH => dout_WIDTH)
    port map (
        clk => clk,
        reset => reset,
        ce => ce,
        din0 => din0,
        dout => dout);

end architecture;

