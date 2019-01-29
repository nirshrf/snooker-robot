--
--  File: top.vhd
--  created by Design Wizard: 01/30/15 11:33:55
--

--{{ Section below this comment is automatically maintained
--   and may be overwritten
--{entity {top} architecture {arc_top}}		
	library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

entity top is 
 Port(
		gclk     : in std_logic;
      rst      : in std_logic;
		input_DI : in std_logic;
		input_EN : in std_logic;
		input_COM :in std_logic;
		input_send : in std_logic;
		strike_confirm : in std_logic;
		DataLoad : out std_logic;	
		TXD      : out std_logic;
		TXD_computer : out std_logic;
		EOF_data : out  std_logic;
		topOstate   :out  std_logic_vector(7 downto 0)
		);
end;

--}} End of automatically maintained section

architecture arc_top of top is
	   
signal topdata  :std_logic_vector(7 downto 0);
signal topdone   : std_logic;
signal topadd : STD_LOGIC_VECTOR(7 downto 0);
signal topdatarom  :std_logic_vector(7 downto 0); 
signal srealClk  :std_logic;
signal s_sendClk  :std_logic;	
signal Ssendcontrol :std_logic := '0';

component logic_control5
	 Port(
		clk : in std_logic;
		rst : in std_logic;
		DataLoad : out std_logic;
		TXD : out std_logic;
		TXD_computer : out std_logic;
		Oflag	: out std_logic;
		COM_flag :in std_logic;
		sendControl : in std_logic;
		DI_flag :in std_logic;
		send_input : in std_logic;
		EN_flag : in std_logic;
		EOF_flag : out std_logic;
		Oadd : out std_logic_vector(7 downto 0);
		Odata : out std_logic_vector(7 downto 0);
		Idata : in std_logic_vector(7 downto 0);
		Ocounter :out  std_logic_vector(3 downto 0);
		Ostate  :out  std_logic_vector(7 downto 0)
	 );
end component;
		
component divider 
	Port( 
		Gclk : in std_logic;
		rst : in std_logic;
		OcsCLOC,rec_clk,sendClk,realClk	:out std_logic
	);	
end component;

component snookermifforreal IS
	PORT
	(
		address		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		clock		: IN STD_LOGIC  := '1';
		q		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
	);
 end component ;
 
begin

	logicUnit : logic_control5
		port map(
			clk => s_sendClk,
			rst => rst,
			DataLoad => open,
			TXD => TXD,
			TXD_computer => TXD_computer,
			Oflag => open,
			COM_flag => input_COM,
			sendControl => strike_confirm,
			send_input => input_send,
			DI_flag => input_DI,
	   	EN_flag => input_EN,
			EOF_flag => EOF_data,
			Oadd => topadd,
			Odata => open,
			Idata => topdatarom,
			Ocounter => open,
			Ostate => topOstate
		);
					
	dataUnit : snookermifforreal  
	PORT MAP (
		clock => s_sendClk,
		address => topadd,
		q => topdatarom
	);
		
	clkUnit : divider
		port map(
			Gclk => gclk,
			rst => rst,
			OcsCLOC => open,
			rec_clk => open,
			sendClk => s_sendClk,
			realClk => open
		);
end arc_top;

