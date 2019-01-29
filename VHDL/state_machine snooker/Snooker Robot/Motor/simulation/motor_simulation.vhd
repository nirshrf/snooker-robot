LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;


--  Entity Declaration

ENTITY motor_TB IS
	-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
		
	-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END motor_TB;


--  Architecture Body

ARCHITECTURE motor_TB_architecture OF motor_TB IS

COMPONENT motor IS
PORT(
    gclk : IN STD_LOGIC;
		rst : IN STD_LOGIC;
		echo0 : IN STD_LOGIC;
		echo1 : IN STD_LOGIC;
		switch0 : IN STD_LOGIC;
		switch1 : IN STD_LOGIC;
		switch2 : IN STD_LOGIC;
		switch3 : IN STD_LOGIC;
		pwm0 : OUT STD_LOGIC;
		pwm1 : OUT STD_LOGIC;
		pwm2 : OUT STD_LOGIC;
		pwm3 : OUT STD_LOGIC;
		direction0 : OUT STD_LOGIC_VECTOR(1 downto 0);
		direction1 : OUT STD_LOGIC_VECTOR(1 downto 0);
		direction2 : OUT STD_LOGIC_VECTOR(1 downto 0);
		direction3 : OUT STD_LOGIC_VECTOR(1 downto 0);
		enable0 : OUT STD_LOGIC;
		enable1 : OUT STD_LOGIC;
		enable2 : OUT STD_LOGIC;
		enable3 : OUT STD_LOGIC;
		trigger0 : OUT STD_LOGIC;
		trigger1 : OUT STD_LOGIC
	);
	
	END COMPONENT;
--                     SIGNALS

signal gclk : STD_LOGIC := '1';
signal rst :  STD_LOGIC ;
signal echo0 :  STD_LOGIC := '0';
signal echo1 :  STD_LOGIC := '0';
signal switch0 : STD_LOGIC;
signal switch1 :  STD_LOGIC;
signal switch2 :  STD_LOGIC;
signal switch3 :  STD_LOGIC;
signal pwm0 :  STD_LOGIC;
signal pwm1 :  STD_LOGIC;
signal pwm2 :  STD_LOGIC;
signal pwm3 :  STD_LOGIC;
signal direction0 :  STD_LOGIC_VECTOR(1 downto 0);
signal direction1 :  STD_LOGIC_VECTOR(1 downto 0);
signal direction2 :  STD_LOGIC_VECTOR(1 downto 0);
signal direction3 :  STD_LOGIC_VECTOR(1 downto 0); 
signal enable0 :  STD_LOGIC;
signal enable1 :  STD_LOGIC;
signal enable2 :  STD_LOGIC;
signal enable3 :  STD_LOGIC;
signal trigger0 :  STD_LOGIC;
signal trigger1 :  STD_LOGIC; 

--                     BEGIN OF ARCHITECTURE
BEGIN

U1 : motor
port map (
    gclk => gclk,
		rst => rst,
		echo0 => echo0,
		echo1 => echo1,
		switch0 => switch0,
		switch1 => switch1,
		switch2 => switch2,
		switch3 => switch3,
		pwm0 => pwm0,
		pwm1 => pwm1,
		pwm2 => pwm2,
		pwm3 => pwm3,
		direction0 => direction0, 
		direction1 => direction1,
		direction2 => direction2,
		direction3 => direction3,
		enable0 => enable0,
		enable1 => enable1,
		enable2 => enable2,
		enable3 => enable3,
		trigger0 => trigger0,
		trigger1 => trigger1);
  
gclk <= not gclk after 10 ns;
rst <= '1' , '0' after 100 ns;
echo0 <= not echo0 after 1 us ;
echo1 <= not echo1 after 1 us ;
switch0 <= '1' ;
switch1 <= '1' ;
switch2 <= '1' ;
switch3 <= '1' ;
direction0 <= "10";
direction1 <= "10";




END motor_TB_architecture;

