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

entity Logic_control5 is
    Port(
		clk : in std_logic;
      rst : in std_logic;
		DataLoad : out std_logic;
		TXD : out std_logic;
		TXD_computer : out std_logic;
		Oflag	: out std_logic;
		send_input : in std_logic;
		DI_flag : in std_logic;
		EN_flag : in std_logic;
		COM_flag : in std_logic;
		sendControl : in std_logic;
		EOF_flag : out std_logic;
      Oadd : out std_logic_vector(7 downto 0);
      Odata : out std_logic_vector(7 downto 0);
      Idata : in std_logic_vector(7 downto 0);
		Ocounter :out  std_logic_vector(3 downto 0);
		Ostate  :out  std_logic_vector(7 downto 0)
		);
end Logic_control5;

architecture Behavioral of Logic_control5 is

type state_vector is (start,wait_e,readData,WCheck,SendCounter,AddS,DataLoadS,StartBit,Timer,EOF);
signal state : state_vector;

constant RepeatSendAdd : std_logic_vector(7 downto 0):=x"0d";
signal Sadd : std_logic_vector(7 downto 0);
signal counter : integer;
signal Sdata : std_logic_vector(7 downto 0);
signal RepeatCounter : std_logic_vector(7 downto 0);
----------timer 
signal timeCounter : std_logic_vector(31 downto 0);
signal	endTimer : std_logic;
signal  timerFlag : std_logic;
signal DI_condition,timer_condition,sendControl_condition,data_condition,EN_condition,COM_condition : boolean;
signal sEOF_flag,sTXD : std_logic;

begin

DI_condition <= (DI_flag = '1');
COM_condition <= (COM_flag = '1');
EN_condition <= (EN_flag = '1');
timer_condition <= (timeCounter > 10000);
sendControl_condition <= (sendControl = '1' or send_input = '1');
data_condition <= (Idata = 255);

process(clk,rst,COM_condition)
begin 
	if rst = '1' then 
			TXD <= sTXD; 
	elsif rising_edge(clk) then 
		if COM_condition then 
			TXD_computer <= sTXD; 
		else 
			TXD <= sTXD; 
		end if;
	end if;
end process;

process(clk,rst,timer_condition,DI_condition,sendControl_condition,data_condition,EN_condition,COM_condition)
begin
	if rst='1' then 
			sTXD<='1';
			timerFlag<='0';
			DataLoad<='0'; 	
			sEOF_flag <= '0';
			state<=start;
			Sadd<=(others=>'1');
			Sdata<=(OTHERS=>'0');
			counter<=0;
			RepeatCounter<=(others=>'0');
	elsif clk'event and clk='1' then 
		case state is
			when start =>
				timerFlag<='0';
				sTXD<='1';
				sEOF_flag <= '0';
				DataLoad<='0'; 	
				counter<=0;
				Sadd<=(others=>'0');
				Sdata<=(others=>'0');		 
				state<=wait_e;
				RepeatCounter<=(others=>'0');
					
			when  wait_e=>
				RepeatCounter<=(others=>'0');
				timerFlag <= '0';
				sEOF_flag <= '0';
				sTXD<='1';
				DataLoad<='0'; 
				counter<=0;
				Sdata<=Idata;
				if DI_condition AND sendControl_condition AND EN_condition AND COM_condition then 
					Sadd <= Sadd;
					state <= wait_e;
				elsif DI_condition AND (sendControl_condition = false) AND (EN_condition = false) AND (COM_condition = false) then 
					Sadd <= x"51";
					state <= readData;
				elsif (DI_condition = false) AND sendControl_condition AND (EN_condition = false) AND (COM_condition = false) then 
					Sadd <= x"00";
					state <= readData;
				elsif (DI_condition = false) AND (sendControl_condition = false) AND EN_condition AND (COM_condition = false) then
					Sadd <= x"56";
					state <= readData;
				elsif (DI_condition = false) AND (sendControl_condition = false) AND (EN_condition = false) AND COM_condition then
					Sadd <= x"5B";
					state <= readData;
				else 
					Sadd <= Sadd;
					state <= wait_e;
				end if;

			when readData =>
				timerFlag<= timerFlag;
				sEOF_flag <= '0';
				sTXD<='1';
				DataLoad<='1'; 
				Sadd<=Sadd;
				Sdata<=Idata;
				counter<=0;
				state<=DataLoadS;
			 
			when DataLoadS =>
				timerFlag<= timerFlag;
				sTXD<='1';
				Sadd<=Sadd;
				Sdata<=Idata;
				counter<=0;
				DataLoad<='1' ;
				sEOF_flag <= '0';
				state<=WCheck;
				
			when WCheck=>
				sTXD<='1';
				DataLoad<='0';
				Sadd<=Sadd;
				sEOF_flag <= '0';
				Sdata<=Idata;			
				if data_condition then  --end of file FF HEX
					if DI_condition AND sendControl_condition AND EN_condition AND COM_condition then 
						Sadd <= Sadd;
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					elsif DI_condition AND (sendControl_condition = false) AND (EN_condition = false) AND (COM_condition = false) then 
						Sadd <= x"51";
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					elsif (DI_condition = false) AND sendControl_condition AND (EN_condition = false) AND (COM_condition = false) then 
						Sadd <= x"00";
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					elsif (DI_condition = false) AND (sendControl_condition = false) AND EN_condition AND (COM_condition = false)  then
						Sadd <= x"56";
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					elsif (DI_condition = false) AND (sendControl_condition = false) AND (EN_condition = false) AND COM_condition then
						Sadd <= x"5B";
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					else 
						Sadd <= Sadd;
						timerFlag<='0';
						state<=EOF;
						sEOF_flag <= '1';	
					end if;
				else
					sEOF_flag <= '0';
					state<=StartBit;
					timerFlag<='0';
				end if;

			when StartBit =>
				sEOF_flag <= '0';
				timerFlag<= timerFlag;
				sTXD<='0';
				DataLoad<='0';
				Sdata<=Idata;
				state<=SendCounter;
			
			when SendCounter =>
				timerFlag <= timerFlag;
				sEOF_flag <= '0';
				if counter <= 7 then 
					sTXD <= Sdata(counter);
					counter <= counter + 1;
					state <= Sendcounter;
				else
					Sdata <= Idata;
					sTXD <= '1';
					counter <= 0;
					state <= Timer;
				end if;
				
			when Timer => 
				timerFlag <= '1';
				if timer_condition then
					state <= AddS;
					timerFlag <= '0';
				else 
					state <= Timer;
				end if;
				
			when 	AddS =>
				timerFlag <= '0';
				sEOF_flag <= '0';
				Sdata <= Sdata;	
				sTXD <= '1';
				SAdd <= SAdd+'1';
				counter<=0;
				state <= readData;
								
			when EOF =>
				sTXD<='1';
				sEOF_flag <= '1';
				timerFlag <= '0';
				counter<=0;
				if DI_condition AND sendControl_condition AND EN_condition AND COM_condition then 
					Sadd <= Sadd;
					state<=EOF;	
				elsif DI_condition AND (sendControl_condition = false) AND (EN_condition = false) AND (COM_condition = false) then 
					Sadd <= x"51";
					state<=readData;	
				elsif (DI_condition = false) AND sendControl_condition AND (EN_condition = false) AND (COM_condition = false) then 
					Sadd <= x"00";
					state <= readData;
				elsif (DI_condition = false) AND (sendControl_condition = false) AND EN_condition AND (COM_condition = false) then
					Sadd <= x"56";
					state<=readData;
				elsif (DI_condition = false) AND (sendControl_condition = false) AND (EN_condition = false) AND COM_condition then
					Sadd <= x"5B";
					state<=readData;
				else 
					Sadd <= Sadd;
					state<=EOF;	
				end if;
				
			when others=>
				sTXD <= '1';
				DataLoad <= '0' ;	
				Sadd <=(others=>'0');
				Sdata <= (others=>'0');
					
		end case;
	end if;
end process;
	
----------------------------------	
--I/O connections
----------------------------------	
	EOF_flag <= sEOF_flag;
	Oadd<=Sadd;	
	oflag<=timerFlag;
----------------------------------
--TIMER
----------------------------------

process(clk,rst)
begin
if rst = '1' then 
	timeCounter <= (others => '0');
elsif rising_edge(clk) then
	if timerFlag = '1' then 
		if counter < 100000 then
			timeCounter <= timeCounter + '1';
		else 
			timeCounter <= (others => '0');
		end if;
	else 
		timeCounter <= (others => '0');
	end if;
end if;
end process;


	with state select 
		 Ostate<= x"00" when start,
							x"01" when wait_e,
							x"02" when readData,
							x"03" when  WCheck,
							x"04" when  StartBit,
							x"05" when  SendCounter,					
							x"06" when	AddS,			
							x"07" when	DataLoadS,
							x"08" when  Timer,
							x"09" when  EOF;	
				
 end Behavioral;
