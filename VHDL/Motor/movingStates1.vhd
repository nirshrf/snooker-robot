-- WARNING: Do NOT edit the input and output ports in this file in a text
-- editor if you plan to continue editing the block that represents it in
-- the Block Editor! File corruption is VERY likely to occur.

-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions
-- and other software and tools, and its AMPP partner logic
-- functions, and any output files from any of the foregoing
-- (including device programming or simulation files), and any
-- associated documentation or information are expressly subject
-- to the terms and conditions of the Altera Program License
-- Subscription Agreement, Altera MegaCore Function License
-- Agreement, or other applicable license agreement, including,
-- without limitation, that your use is for the sole purpose of
-- programming logic devices manufactured by Altera and sold by
-- Altera or its authorized distributors.  Please refer to the
-- applicable agreement for further details.


-- Generated by Quartus II 64-Bit Version 13.0 (Build Build 232 06/12/2013)
-- Created on Sun Nov 22 11:59:40 2015

LIBRARY ieee;
use ieee.numeric_std.all;
USE ieee.std_logic_1164.all;
use IEEE.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;


--  Entity Declaration

ENTITY movingStates1 IS
-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
PORT
(
	clk : IN STD_LOGIC;
	rst : IN STD_LOGIC;
	echo_count0 : IN std_logic_vector(25 downto 0);
	echo_count1 : IN std_logic_vector(25 downto 0);
	echo_count2 : IN std_logic_vector(25 downto 0);
	echo_count3 : IN std_logic_vector(25 downto 0);
	pwm0_s : OUT STD_LOGIC_VECTOR(13 downto 0);
	pwm1_s : OUT STD_LOGIC_VECTOR(13 downto 0);
	pwm2_s : OUT STD_LOGIC_VECTOR(13 downto 0);
	pwm3_s : OUT STD_LOGIC_VECTOR(13 downto 0);
	direction0_s : OUT STD_LOGIC_VECTOR(1 downto 0);
	direction1_s : OUT STD_LOGIC_VECTOR(1 downto 0);
	direction2_s : OUT STD_LOGIC_VECTOR(1 downto 0);
	ostate : OUT STD_LOGIC_VECTOR(6 downto 0);
	direction3_s : OUT STD_LOGIC_VECTOR(1 downto 0)
);
-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END movingStates1;


--  Architecture Body

ARCHITECTURE movingStates_architecture OF movingStates1 IS

type state_type is (start,drivingDistance);

signal drivingDistance_far,drivingDistance_close,motorToMove,deltaBiggerThan : boolean;
signal current_state , next_state : state_type;
signal delta : std_logic_vector(25 downto 0);


constant speed_0 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(0, 14));
constant speed_rotation : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(3750, 14));
constant speed_50 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(6250, 14));
constant speed_60 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(7500, 14));
constant speed_70 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(8750, 14));
constant speed_80 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(10000, 14));
constant speed_90 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(11250, 14));
constant speed_95 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(11875, 14));
constant speed_100 : std_logic_vector(13 downto 0) := std_logic_vector(to_unsigned(12500, 14));
constant direction_free : std_logic_vector(1 downto 0):= "00";
constant direction_clockWise : std_logic_vector(1 downto 0):= "01";
constant direction_cClockWise : std_logic_vector(1 downto 0):= "10";
constant direction_break : std_logic_vector(1 downto 0):= "11";


BEGIN

drivingDistance_far <= (echo_count0 >180000 and echo_count1>180000);
drivingDistance_close <= (echo_count0 <170000 and echo_count1<170000);
deltaBiggerThan <= (delta > 5000);


process(echo_count0,echo_count1)
begin 
	if (echo_count0 > echo_count1) then -- false = clockwise
		motorToMove <= false;
		delta <= echo_count0 - echo_count1;
	else											-- true = cClockwise
		motorToMove <= true;
		delta <= echo_count1 - echo_count0;
	end if;
end process;

	

process(rst,clk)
BEGIN
	if rst = '1' then
		current_state <= start;
	elsif clk = '1' and clk'event then
		pwm0_s <= speed_95;
		pwm1_s <= speed_0;
		pwm2_s <= speed_95;
		pwm3_s <= speed_0;
		direction0_s <= direction_cClockWise;
		direction2_s <= direction_clockWise;
		direction1_s <= direction_free;
		direction3_s <= direction_free;
	end if;
		
end process;





END movingStates_architecture;
