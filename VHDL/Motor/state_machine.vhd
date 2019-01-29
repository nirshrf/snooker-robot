LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all; 
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Entity Declaration

ENTITY state_machine IS
	-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
	PORT
	(
		clk : in std_logic;
		rst : in std_logic;
		start_bit : in std_logic;
		sendControl : out std_logic
	);
	-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!
	
END state_machine;


--  Architecture Body

ARCHITECTURE sm_architecture of state_machine is

signal condition_start_bit : boolean;

BEGIN
condition_start_bit <= (start_bit = '1');


process (rst,clk,condition_start_bit)
begin
	if rst = '1' then
		sendControl <= '0'; 
	elsif clk = '1' AND clk'event then
		if condition_start_bit then
				sendControl <= '1'; 
		else 
				sendControl <= '0'; 
		end if;
	end if;
end process; 

	

END sm_architecture;
