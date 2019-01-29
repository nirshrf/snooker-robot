library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


Entity top_linear_snooker is
Port (
		gclk     : in std_logic;
      rst      : in std_logic;	
		strike_confirm : in std_logic
);

End top_linear_snooker;

signal Sstrike_confirm : std_LOGIC := '0';

architecture top_linear_snooker_architecture of top_linear_snooker is

component divider is
Port( 
		Gclk : in std_logic;
      rst : in std_logic;
      OcsCLOC,rec_clk,sendClk	:out std_logic
		);
End component;


component logic_control6 is
 Port(
		clk : in std_logic;
      rst : in std_logic;
		TXD : out std_logic;
		sendControl : in std_logic;
      Idata : in std_logic_vector(7 downto 0);
		Ostate  :out  std_logic_vector(7 downto 0)
		);
End component;


component mif is
PORT(
		address		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		clock		: IN STD_LOGIC  := '1';
		q: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
    	);
End component;

component sm is 
port(
		clk : in std_logic;
		rst : in std_logic;
		sendControl : out  std_logic;  
		add : out std_logic_vector(7 downto 0);
		data : in std_logic_vector(7 downto 0);
		done : in std_logic 
		);
end component; 


BEGIN

process (clk,rst)
begin 
if rst = '1' then 
	Sstrike_confirm <= '0';
elsif clk'event AND clk = '1' then 
		Sstrike_confirm <= strike_confirm;
		if Sstrike_confirm = '1' then
			


End top_linear_snooker_architecture;
