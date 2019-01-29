--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    19:15:30 05/29/06
-- Design Name:    
-- Module Name:    Logic_control - Behavioral
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

entity Logic_control6 is
    Port(
		clk : in std_logic;
		rst : in std_logic;
		sendControl : in std_logic;
		Idata : in std_logic_vector(7 downto 0);
		address : out std_logic_vector(7 downto 0);
		TXD : out std_logic;
		EOF_flag : out  std_logic;
		topOstate  :out  std_logic_vector(7 downto 0)
		);
end Logic_control6;



architecture Behavioral of Logic_control6 is

type state_vector is (start,wait_e,readData,WCheck,SendCounter,AddS,StartBit,EOF);
signal state : state_vector;
--constant RepeatSendAdd : std_logic_vector(7 downto 0):=x"0d";
signal Sadd : std_logic_vector(7 downto 0);
signal counter : integer := 7;
signal Sdata : std_logic_vector(7 downto 0);

signal sTXD : std_logic;

begin

address <= Sadd;
TXD <= sTXD;

process(clk,rst)
begin
if rst = '1' then 
		sTXD     <=  '1';
		Sadd    <=  (others=>'1');
		Sdata   <=   Idata;
		counter <=  7;
		EOF_flag <= '0';
		state   <=  start;
elsif clk'event and clk='1' then
								
	case state is
	
		when start =>
			EOF_flag <= '0';
			sTXD  <= '1';
			counter <=  7;
			Sdata   <=  (others=>'0');		 
			state   <=   wait_e;
			
		when  wait_e=>
			EOF_flag <= '0';
			sTXD  <= '1';
			counter <= 7;
			Sadd <= Sadd;
			Sdata <= Sdata;
			if sendControl = '0' then 
			  state <= wait_e;
			else
			  state <= readData;
			end if;

		when readData =>
			EOF_flag <= '0';
			sTXD  <= '1';
			Sadd  <= Sadd;
			Sdata <= Idata;
			counter <= 7;
			state   <= WCheck;
			 
			
		when WCheck=>			
			sTXD <= '1';
			Sadd <= Sadd;
			EOF_flag <= '0';
			counter <= 7;
			Sdata <= Idata;			
			if Idata = 255 then  --end of file FF HEX
				Sadd <= x"00";   --"0E"
				EOF_flag <= '1';
				state <= EOF;
			else
				state <= StartBit;
			end if;
				
		when StartBit =>
			sTXD <= '0';
			Sadd <= Sadd;
			Sdata <= Sdata;
			counter <= 7;
			state <= SendCounter;
		
		when SendCounter =>	
				if counter <= 7 then 
					Sdata <= Sdata(6 downto 0)& Sdata(7) ;--M2L
					TXD<=Sdata(0);
					counter <= counter + 1;
					state<= Sendcounter;
				else
					Sdata<=Sdata;
					TXD<='1';
					counter<=(others=>'0');
					state<=AddS;
				end if;
			
		when 	AddS =>
			Sdata <= Sdata;	
			sTXD <= '1';
			SAdd <= SAdd + '1';
			counter <= 7;
			state <= readData;
			
		when EOF =>
			sTXD <= '1';
			EOF_flag <= '1';
			counter <= 7;
			if sendControl = '1' then
				state <= readData;
				Sadd <= x"00";   --"0E"
			else
				state <= EOF;
			end if;
			
		when others=>
			sTXD <= '1';	
			Sadd <= x"00";  --(others=>'0');
			Sdata <= Idata;
			counter <= 7;
				
		end case;
	end if;
end process;
	


----------------------------------


	with state select 
		 topOstate<= x"00" when start,
							x"01" when wait_e,
							x"02" when readData,
							x"03" when  WCheck,
							x"04" when  StartBit,
							x"05" when  SendCounter,					
							x"06" when	AddS,			
							x"07" when  EOF;	
				
 end Behavioral;
