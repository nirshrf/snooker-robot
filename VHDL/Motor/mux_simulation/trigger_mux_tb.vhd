LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;

--  Entity Declaration

ENTITY trigger_mux_tb IS
-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!

-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END trigger_mux_tb;


--  Architecture Body

ARCHITECTURE trigger_mux_tb_architecture OF trigger_mux_tb IS
 
Signal clk  : std_logic:='0';
signal rst : std_logic;
signal enable0 : std_logic:='0';
signal enable1 : std_logic:='0';

Component trigger_mux IS
PORT (
clk : IN STD_LOGIC;
rst : IN STD_LOGIC;
enable0 : OUT STD_LOGIC;
enable1 : OUT STD_LOGIC
);
END COMPONENT;
BEGIN 
U1: trigger_mux
Port Map(
clk=>clk,
rst => rst,
enable0 => enable0,
enable1 => enable1
);

rst <= '0';
clk <=	not clk  after 10 nS;

END trigger_mux_tb_architecture;