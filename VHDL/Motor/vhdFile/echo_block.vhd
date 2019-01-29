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
-- Created on Fri Nov 06 09:28:34 2015

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;

--  Entity Declaration

ENTITY echo_block IS
	-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
	PORT
	(
		rst : IN STD_LOGIC;
		clk : IN STD_LOGIC;
		echo : IN STD_LOGIC;
		output : OUT STD_LOGIC_VECTOR (20 downto 0)
	);
	-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END echo_block;


--  Architecture Body

ARCHITECTURE echo_block_architecture OF echo_block IS
signal count : std_logic_vector (20 downto 0);
signal last_output : std_logic_vector(20 downto 0);
signal reset : boolean;

BEGIN

PROCESS (rst,clk)
BEGIN
if rst = '1' then
	count <= (others => '0');
	output <= (others=>'0');
		reset <= false;
elsif clk = '1' AND clk'event then 
	if echo = '1' then
		count <= count + '1';
		reset <= true;
	else 
		if reset then
		  last_output <= count;
		  reset <= false;
		 end if;
		count <= (others => '0');
	end if;
	output <= last_output;
end if;

end process;
	
END echo_block_architecture;
