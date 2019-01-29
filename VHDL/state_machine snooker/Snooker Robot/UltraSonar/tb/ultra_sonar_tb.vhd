LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;

--  Entity Declaration

ENTITY ultra_sonar_tb IS
-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!

-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END ultra_sonar_tb;


--  Architecture Body

ARCHITECTURE ultra_sonar_tb_architecture OF ultra_sonar_tb IS
 
Signal clk  : std_logic:='0';
signal rst : std_logic;
signal   trigger : std_logic;

Component ultra_sonar IS
PORT (
clk : IN STD_LOGIC;
rst : IN STD_LOGIC;
trigger : OUT STD_LOGIC
);
END COMPONENT;
BEGIN 
U1: ultra_sonar
Port Map(
clk=>clk,
rst => rst,
trigger=>trigger
);

rst <= '0';
clk <=not clk  after 10 nS;

END ultra_sonar_tb_architecture;