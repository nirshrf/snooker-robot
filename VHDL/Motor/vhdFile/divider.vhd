--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    15:07:56 03/13/06
-- Design Name:    
-- Module Name:    divider - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity divider is
	generic (
		baud                : positive:= 115200;
      clock_frequency     : positive := 50000000);
	Port( 
		Gclk : in std_logic;
		rst : in std_logic;
		OcsCLOC,rec_clk,sendClk,realClk	:out std_logic
		);
end divider;

architecture Behavioral of divider is

constant realClk_div1 : integer := clock_frequency / (baud);
constant realClk_div2 : integer := clock_frequency / (baud * 2); 
SIGNAL csCLOC, Srec_clk, SsendClk :STD_LOGIC;
SIGNAL count :  std_logic_vector(7 downto 0);
signal realClk_counter : std_logic_vector(31 downto 0) := (others => '0');
signal realClk_signal : std_LOGIC;

begin

realClk <= realClk_signal;

process(Gclk,rst)
begin
	if rst = '1' then
		realClk_counter <= (others => '0');
	elsif rising_edge(Gclk) then
		if (realClk_counter >= 0) AND (realClk_counter < realClk_div2)then 
			realClk_counter <= realClk_counter + '1';
			realClk_signal <= '1';
		elsif (realClk_counter >= realClk_div2) AND (realClk_counter < realClk_div1) then 
			realClk_counter <= realClk_counter + '1';
			realClk_signal <= '0';
		else 
			realClk_counter <= (others => '0');
		end if;
	end if;
end process;

PROCESS(Gclk, rst)
begin
if rst='1' then
	count<=(others=>'0');
	csCLOC<='0';
	
elsif Gclk'event and Gclk='1' then
	if count< 54  then	 --chip scope108 
		count<=count+'1';
		csCLOC<=csCLOC;
	else
		count<=(others=>'0');
		csCLOC<=NOT csCLOC;
	end if;
end if;  
end process;
 OcsCLOC<= csCLOC;

process(csCLOC, rst)	--recieve clock
begin
if rst='1' then
	Srec_clk<='0';
elsif csCLOC'event and csCLOC='1' then
	Srec_clk<= not Srec_clk;
end if;  
end process;

rec_clk<=  Srec_clk;	

process(Srec_clk, rst)	--send clock
begin
if rst='1' then
	SsendClk<='0';
elsif Srec_clk'event and Srec_clk='1' then
	SsendClk<= not SsendClk;
end if;  
end process;

sendClk <= SsendClk;
	
end Behavioral;
