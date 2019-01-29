LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;

--  Entity Declaration

ENTITY echo_tb IS
-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!

-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END echo_tb;


--  Architecture Body

ARCHITECTURE echo_tb_architecture OF echo_tb IS
 
Signal clk  : std_logic:='0';
signal rst : std_logic;
signal echo : std_logic := '0';
signal   output : std_logic_vector(19 downto 0):=(others => '0');


Component echo_block IS
PORT (
clk : IN STD_LOGIC;
rst : IN STD_LOGIC;
echo : IN STD_LOGIC;
output : OUT STD_LOGIC_Vector(19 downto 0)
);
END COMPONENT;
BEGIN 
U1: echo_block
Port Map(
clk=>clk,
rst => rst,
echo => echo,
output => output
);

RST <='0' ;
echo <= not echo after 1 us;
Clk <= not clk  after 10 ns;

END echo_tb_architecture;
