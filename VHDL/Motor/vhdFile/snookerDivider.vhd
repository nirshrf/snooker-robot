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

entity snookerDivider is
	generic (
		baud                : positive := 115200;
      clock_frequency     : positive := 50000000);
	Port( 
		Gclk : in std_logic;
		rst : in std_logic;
		sampleClk,realClk	: out std_logic
		);
end snookerDivider;

architecture Behavioral of snookerDivider is

constant realClk_div1 : integer := clock_frequency / (baud/2);
constant realClk_div2 : integer := clock_frequency / (baud); 
constant sampleClk_div1 : integer := clock_frequency / (baud);
constant sampleClk_div2 : integer := clock_frequency / (baud*2); 
signal realClk_counter : std_logic_vector(31 downto 0) := (others => '0');
signal realClk_signal : std_LOGIC;
signal sampleClk_counter : std_logic_vector(31 downto 0) := (others => '0');
signal sampleClk_signal : std_LOGIC;

begin

realClk <= realClk_signal;
sampleClk <= sampleClk_signal;

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

process(Gclk,rst)
begin
	if rst = '1' then
		sampleClk_counter <= (others => '0');
	elsif rising_edge(Gclk) then
		if (sampleClk_counter >= 0) AND (sampleClk_counter < sampleClk_div2)then 
			sampleClk_counter <= sampleClk_counter + '1';
			sampleClk_signal <= '1';
		elsif (sampleClk_counter >= sampleClk_div2) AND (sampleClk_counter < sampleClk_div1) then 
			sampleClk_counter <= sampleClk_counter + '1';
			sampleClk_signal <= '0';
		else 
			sampleClk_counter <= (others => '0');
		end if;
	end if;
end process;
	
end Behavioral;