-- WARNING: Do NOT edit the input and output ports in this file in a text
-- editor if you plan to continue editing the block that represents it in
-- the Block Editor! File corruption is VERY likely to occur.

-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions
-- and other software and tools, and its AMPP partner logic
-- functions, and any output files from any of the foregoing
-- (including device programming or simulation files), and any
-- associated documentation or information are expressly subject
-- to the terms and conditions of the Altera Program License
-- Subscription Agreement, Altera MegaCore Function License
-- Agreement, or other applicable license agreement, including,
-- without limitation, that your use is for the sole purpose of
-- programming logic devices manufactured by Altera and sold by
-- Altera or its authorized distributors.  Please refer to the
-- applicable agreement for further details.


-- Generated by Quartus II 64-Bit Version 13.0 (Build Build 232 06/12/2013)
-- Created on Sun Nov 22 11:59:40 2015

LIBRARY ieee;
use ieee.numeric_std.all;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;


--  Entity Declaration

ENTITY movingStates IS
-- {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
PORT
(
	clk : IN STD_LOGIC;
	rst : IN STD_LOGIC;
	rs_data : IN STD_LOGIC_VECTOR(7 downto 0);
	echo_count0 : IN std_logic_vector(20 downto 0);
	echo_count1 : IN std_logic_vector(20 downto 0);
	echo_count2 : IN std_logic_vector(20 downto 0);
	echo_count3 : IN std_logic_vector(20 downto 0);
	toggle_await : IN std_logic;
	strike_confirm : out std_logic;
	EOF_flag : in std_logic;
	pwm0_s : OUT STD_LOGIC_VECTOR(15 downto 0);
	pwm1_s : OUT STD_LOGIC_VECTOR(15 downto 0);
	pwm2_s : OUT STD_LOGIC_VECTOR(15 downto 0);
	pwm3_s : OUT STD_LOGIC_VECTOR(15 downto 0);
	direction0_s : OUT STD_LOGIC;
	direction1_s : OUT STD_LOGIC;
	direction2_s : OUT STD_LOGIC;
	direction3_s : OUT STD_LOGIC;
	COM_send : OUT STD_LOGIC;
	ostate : OUT STD_LOGIC_VECTOR(6 downto 0);
	ready_out : OUT STD_LOGIC
);
-- {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!

END movingStates;


--  Architecture Body

ARCHITECTURE movingStates_architecture OF movingStates IS

type state_type is (start,drivingDistance,align,alignedDriving,delayAlign,delayAlignedDriving,moveUntil,afterMoveUntil,
delayMoveUntil,rotate90,delayRotate90,stop,rs_move,rs_rotate,rs_outside,rs_align,rs_backwards,
drivingBackwards,alignAfterBack,stopDrive,zynq_stop,wait_EOF);


signal current_state,next_state : state_type; 

signal drivingDistance_condition_far,drivingDistance_condition_close,drivingDistance_condition,
align_bigright,align_bigleft,align_delta_right,align_delta_left,alignedDriving_condition,moveUntil_condition,
rotate90_condition,rs_right_condition,rs_left_condition,rs_forward_condition,rs_backward_condition,
rs_clockwise_condition,rs_cClockwise_condition,rs_move_condition,rs_rotate_condition,rs_delay_condition,
rs_reset_condition,strike_stop_condition,rs_strike_condition,rs_ready_condition,rs_outside_condition,
rs_start_condition,EOF_flag_condition, zynq_stop_condition,drivingBackwards_condition_far,rs_unready_condition,
drivingBackwards_condition_close,drivingBackwards_condition,align_backwards_delta_right,align_backwards_delta_left : boolean;
signal delay_active : boolean;
signal delay_condition : boolean;
signal delay_time : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(50000000, 32));
signal afterMoveUntil_active : boolean;
signal afterMoveUntil_condition : boolean;
signal rs_delay : boolean;
signal rs_active : boolean;
signal await_nextState : boolean:= false;

signal dynamicSpeed1 : std_logic_vector(15 downto 0);
signal dynamicSpeed3 : std_logic_vector(15 downto 0);

signal rs_data_old : std_logic_vector(7 downto 0);
signal rs_counter : std_logic_vector(16 downto 0);
signal delta : std_logic_vector(20 downto 0);
--signal speed_delta_align : std_logic_vector(13 downto 0);
--signal speed_delta_aligned_driving : std_logic_vector(13 downto 0);
signal delay_counter : std_logic_vector(31 downto 0) := (others => '0');
signal afterMoveUntil_counter : std_logic_vector(31 downto 0) := (others => '0');
signal ready : std_logic;
signal Sstrike_confirm : std_logic := '0';
signal toggle_await_signal: std_logic;
signal counter_flag : std_logic := '0';
signal strike_counter : std_logic_vector (9 downto 0) := (others => '0');

constant speed_0 : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(0, 16));

constant multiplier : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(12, 16));
--constant const_speed_multiplier : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(2, 16));
constant speed0_forward : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(6000, 16));
constant speed2_forward : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(7501, 16));
constant speed0_backward : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(4000, 16));
constant speed2_backward : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(3250, 16));
constant speed1_right : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(4500, 16));
constant speed3_right : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(4500, 16));--
constant speed1_left : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(3250, 16));--
constant speed3_left : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(3250, 16));
constant speed_clockwise : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(3250, 16));
constant speed_cClockwise : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(4000, 16));
constant delay_100m : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(100000000, 32));
constant delay_50m : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(50000000, 32));
constant delay_40m : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(40000000, 32));
constant delay_35m : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(35000000, 32));
constant delay_20m : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(20000000, 32));
--constant direction_free : std_logic_vector(1 downto 0):= "00";
constant direction_clockWise : std_logic := '0';
constant direction_cClockWise : std_logic := '1';
--constant direction_break : std_logic_vector(1 downto 0):= "11";


BEGIN



-- Driving Distance Conditions 
drivingDistance_condition_far <= (echo_count0 < 55000 and echo_count1 < 55000) ; 
drivingDistance_condition_close <=  (echo_count0 > 40000 and echo_count1 > 40000);
drivingDistance_condition <= (drivingDistance_condition_far AND drivingDistance_condition_close);
-- Driving Backwards Conditions
drivingBackwards_condition_far <= (echo_count0 < 90000 or echo_count1 < 90000);
drivingBackwards_condition_close <= (echo_count0 > 70000 or echo_count1 > 70000);
drivingBackwards_condition <= (drivingBackwards_condition_close and drivingBackwards_condition_far);
-- Align Conditions
align_delta_right <= ((align_bigright) AND (delta > 200));
align_delta_left <= ((align_bigleft) AND (delta > 200));
-- Align After Backwards Condition
align_backwards_delta_right <= ((align_bigright) AND (delta > 350));
align_backwards_delta_left <= ((align_bigleft) AND (delta > 350));
-- Aligned Driving Conditions 
alignedDriving_condition <= (echo_count0 > 90000 AND echo_count1 > 75000);
-- Move Until Conditions
moveUntil_condition <= (echo_count2 < 180000 AND echo_count3 < 180000);
-- Rotate90 Condition
rotate90_condition <= (delta < 15000 AND echo_count0 < 100000 AND echo_count1 < 100000);
rs_move_condition <= (rs_data = 2 OR rs_data = 3 OR rs_data = 6 OR rs_data = 7);
rs_rotate_condition <= (rs_data = 4 OR rs_data = 17);
rs_outside_condition <= (rs_data = 9 or rs_data = 8);
rs_right_condition <= (rs_data = 2);
rs_left_condition <= (rs_data = 3);
rs_forward_condition <= (rs_data = 6);
rs_backward_condition <= (rs_data = 7);
rs_clockwise_condition <= (rs_data = 4);
rs_cClockwise_condition <= (rs_data = 17);
rs_reset_condition <= (rs_data = 0);
rs_delay_condition <= (rs_delay = true);
rs_strike_condition <= (rs_data = 8 and ready = '1');
rs_ready_condition <= (rs_data = 9);
rs_unready_condition <= (rs_data = 20);
rs_start_condition <= (rs_data = 11);
zynq_stop_condition <= (rs_data = 15);
ready_out <= ready;
--strike_confirm <= Sstrike_confirm;
toggle_await_signal <= toggle_await;
rs_active <= (rs_data /= 0) ;
EOF_flag_condition <= EOF_flag = '1';

process(echo_count0,echo_count1,delta)
begin 
	if(echo_count1 < echo_count0) then
		align_bigright <= true;
		align_bigleft <= false;
		if(delta < 500) then
			dynamicSpeed1 <= std_LOGIC_VECTOR(speed1_right + delta(15 downto 0)*multiplier)(15 downto 0);
		else
			dynamicSpeed1 <= speed1_right + std_logic_vector(to_unsigned(6000, 16));
		end if;
		dynamicSpeed3 <= speed3_right;
		delta <= echo_count0 - echo_count1;
	else 
		align_bigright <= false;
		align_bigleft <= true;
		if(delta < 500) then
			dynamicSpeed1 <= std_LOGIC_VECTOR(speed3_right + delta(15 downto 0)*multiplier)(15 downto 0);
		else
			dynamicSpeed1 <= speed3_right + std_logic_vector(to_unsigned(6001, 16));
		end if;
		dynamicSpeed1 <= speed1_right;
		delta <= echo_count1 - echo_count0;
	end if;
	--speed_delta_align <= speed_40 - delta(13 downto 0);
	--speed_delta_aligned_driving <= speed_100 - delta(13 downto 0);
end process;

process(rst,clk) 
BEGIN
	if rst = '1' then
		current_state <= drivingBackwards;
	elsif clk = '1' and clk'event then
		current_state <= next_state;
	end if;
end process;

process(clk,rst)
begin
	if rst = '1' then
		ready <= '0';
	elsif clk'event and clk = '1' then
		if rs_ready_condition then
			ready <= '1';
		elsif rs_unready_condition then
			ready <= '0';
		else
			ready <= ready;
		end if;
	end if;
end process;


process(rst,clk) 
BEGIN
	if rst = '1' then
		delay_counter <= (others => '0');
		rs_counter <= (others => '0');
		rs_delay <= false;
	elsif clk = '1' and clk'event then
		if	(delay_active) then
			if (delay_counter < delay_time) then
				delay_counter <= delay_counter + '1';
				delay_condition <= false;
			else
				delay_counter <= (others => '0');
				delay_condition <= true;
			end if;
		else
			delay_counter <= (others => '0');
			delay_condition <= false;
		end if;
		if(rs_counter > 10000000) then
			rs_delay <= not rs_delay;
			rs_counter <= (others => '0');
		else
			rs_counter <= rs_counter + '1';
		end if;
		
	end if;	
end process;

process(rst,clk) 
BEGIN
	if rst = '1' then
		afterMoveUntil_counter <= (others => '0');
	elsif clk = '1' and clk'event then
		if	(afterMoveUntil_active) then
			if (afterMoveUntil_counter < 28000000) then
				afterMoveUntil_counter <= afterMoveUntil_counter + '1';
				afterMoveUntil_condition <= false;
			else
				afterMoveUntil_counter <= (others => '0');
				afterMoveUntil_condition <= true;
			end if;
		else
			afterMoveUntil_counter <= (others => '0');
			afterMoveUntil_condition <= false;
		end if;
	end if;	
end process;

process(current_state,next_state,drivingDistance_condition,drivingDistance_condition_far,drivingDistance_condition_close,
align_delta_right,align_delta_left,alignedDriving_condition,delay_condition,afterMoveUntil_condition,
rs_right_condition,rs_left_condition,rs_forward_condition,rs_backward_condition,
rs_move_condition,rs_rotate_condition,strike_stop_condition,rs_strike_condition,
rs_ready_condition,moveUntil_condition,rotate90_condition,rs_reset_condition,
rs_delay_condition,rs_clockwise_condition,rs_cClockwise_condition,Sstrike_confirm,EOF_flag_condition,
drivingBackwards_condition_far,drivingBackwards_condition_close,drivingBackwards_condition,rs_active,rs_start_condition)
begin
	next_state <= current_state;
	case current_state is
			
		when start =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			pwm0_s <= speed_0;
			pwm1_s <= speed_0;
			pwm2_s <= speed_0;
			pwm3_s <= speed_0;
		--	direction0_s <= direction_free;
		--	direction1_s <= direction_free;
		--	direction2_s <= direction_free;
		--	direction3_s <= direction_free;
			delay_active <= false;
			next_state <= drivingDistance;
		end if;
		
		when drivingBackwards=> 
		strike_confirm <= '0';
		com_send <= '0';
		if rs_active = true then 
			next_state <= stop;
		else
			if (drivingBackwards_condition) then
				next_state <= alignAfterBack; 
			elsif (drivingBackwards_condition_far) then 
				pwm0_s <= speed0_forward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_forward;
				pwm3_s <= speed_0;
				direction0_s <= direction_clockWise;
				direction2_s <= direction_cClockWise;
			elsif (drivingBackwards_condition_close) then 
				pwm0_s <= speed0_backward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_backward;
				pwm3_s <= speed_0;
				direction0_s <= direction_cClockWise;
				direction2_s <= direction_clockWise;
			end if;
		end if;
		
		when alignAfterBack=> 
		com_send <= '0';
		strike_confirm <= '0';
		if rs_active = true then 
			next_state <= stop;
		else
			if(align_backwards_delta_right) then
				pwm0_s <= speed_0;
				pwm1_s <= std_logic_vector(to_unsigned(5000, 16));
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
				direction1_s <= direction_cClockWise;
			elsif(align_backwards_delta_left) then
				pwm0_s <= speed_0;
				pwm1_s <= std_logic_vector(to_unsigned(5000, 16));
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
				direction1_s <= direction_clockWise;
			else
				next_state <= stopDrive;
			end if;
		end if;
		
		when stopDrive=> 
		strike_confirm <= '0';
		com_send <= '0';
		if rs_active = true then 
			next_state <= stop;
		else
			pwm0_s <= speed_0;
			pwm1_s <= speed_0;
			pwm2_s <= speed_0;
			pwm3_s <= speed_0;
			next_state <= stopDrive;
		end if;
		
		when drivingDistance=>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			if(drivingDistance_condition) then
				next_state <= align;
			elsif(drivingDistance_condition_far) then
				pwm0_s <= speed0_forward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_forward;
				pwm3_s <= speed_0;
				direction0_s <= direction_clockWise;
				direction2_s <= direction_cClockWise;
			elsif(drivingDistance_condition_close) then
				pwm0_s <= speed0_backward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_backward;
				pwm3_s <= speed_0;
				direction0_s <= direction_cClockWise;
				direction2_s <= direction_clockWise;
			end if;
		end if;
		
		when align =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			if(align_delta_right) then
				pwm0_s <= speed_0;
				pwm1_s <= std_logic_vector(to_unsigned(5000, 16));
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
				direction1_s <= direction_cClockWise;
			elsif(align_delta_left) then
				pwm0_s <= speed_0;
				pwm1_s <= std_logic_vector(to_unsigned(5000, 16));
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
				direction1_s <= direction_clockWise;
			else
				next_state <= delayAlign;
			end if;
		end if;
			
		when delayAlign =>
		strike_confirm <= '0';	
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			delay_active <= true;
			if delay_condition then
				delay_active <= false;
				next_state <= alignedDriving;
			else
				pwm0_s <= speed_0;
				pwm1_s <= speed_0;
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
			end if;
		end if;
			
		when alignedDriving =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			if alignedDriving_condition then 
				next_state <= delayAlignedDriving;
			else
				pwm0_s <= speed_0;
				pwm1_s <= dynamicSpeed1;
				pwm2_s <= speed_0;
				pwm3_s <= dynamicSpeed3;
				direction1_s <= direction_cClockWise;
				direction3_s <= direction_clockWise;
			end if;
		end if;
			
		when delayAlignedDriving =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			delay_time <= delay_20m;
			delay_active <= true;
			if delay_condition then
				delay_active <= false;
				next_state <= moveUntil;
			end if;
		end if;
		
		when moveUntil =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			if moveUntil_condition then 
				next_state <= afterMoveUntil;
			else
				pwm0_s <= speed0_forward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_forward;
				pwm3_s <= speed_0;
				direction0_s <= direction_cClockWise;
				direction2_s <= direction_clockWise;
			 end if;
		end if;
		
		when afterMoveUntil =>
		strike_confirm <= '0';
		com_send <= '0';
	   if rs_start_condition = false then 
			next_state <= stop;
		else	
			afterMoveUntil_active <= true;
			if afterMoveUntil_condition then
				afterMoveUntil_active <= false;
				next_state <= delayMoveUntil;
			else
				pwm0_s <= speed0_forward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_forward;
				pwm3_s <= speed_0;
				direction0_s <= direction_cClockWise;
				direction2_s <= direction_clockWise;
			end if;
		end if;
				
		when delayMoveUntil =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			delay_time <= delay_20m;
			delay_active <= true;
			if delay_condition then
				delay_active <= false;
				next_state <= rotate90;
			else
				pwm0_s <= speed_0;
				pwm1_s <= speed_0;
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
			end if;
		end if;
		
		when rotate90 =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			 if rotate90_condition then
				next_state <= delayRotate90;
			 else 
				pwm0_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm1_s <= std_logic_vector(to_unsigned(2400, 16));
				pwm2_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm3_s <= std_logic_vector(to_unsigned(2400, 16));
				direction0_s <= direction_cClockWise;
				direction1_s <= direction_cClockWise;
				direction2_s <= direction_cClockWise;
				direction3_s <= direction_cClockWise;
			 end if;
		end if;
			
		when delayRotate90 =>
		strike_confirm <= '0';
		com_send <= '0';
		if rs_start_condition = false then 
			next_state <= stop;
		else
			delay_active <= true;
			if delay_condition then
				delay_active <= false;
				next_state <= start;
			else
				pwm0_s <= speed_0;
				pwm1_s <= speed_0;
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
	--			direction0_s <= direction_break;
	--			direction1_s <= direction_break;
	--			direction2_s <= direction_break;
	--			direction3_s <= direction_break;
			end if;
		end if;
		 
		when stop =>
		strike_confirm <= '0';
		com_send <= '0';
		if (rs_move_condition) then
			next_state <= rs_move; -- rs_move
		elsif(rs_rotate_condition) then
			next_state <= rs_rotate; -- rs_rotate
		elsif(rs_reset_condition or rs_unready_condition) then
			next_state <= drivingBackwards; -- Avoid interference with cpp
		elsif (rs_outside_condition) then
			next_state <= rs_outside; -- outsiders
		elsif (rs_start_condition) then 
			next_state <= start; -- Around the table
		elsif zynq_stop_condition then 
			next_state <= zynq_stop; -- zynq stop
		else
			pwm0_s <= speed_0;
			pwm1_s <= speed_0;
			pwm2_s <= speed_0;
			pwm3_s <= speed_0;
		end if;
			
		when rs_move =>
		strike_confirm <= '0';
		com_send <= '0';
		if(rs_right_condition) then 
				pwm0_s <= speed_0;
				pwm1_s <= speed1_right;
				pwm2_s <= speed_0;
				pwm3_s <= speed3_right;
				direction1_s <= direction_cClockWise;
				direction3_s <= direction_clockWise;
		elsif(rs_left_condition) then 
				pwm0_s <= speed_0;
				pwm1_s <= speed1_left;
				pwm2_s <= speed_0;
				pwm3_s <= speed3_left;
				direction1_s <= direction_clockWise;
				direction3_s <= direction_cClockWise;
		elsif(rs_forward_condition) then 
				pwm0_s <= speed0_forward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_forward;
				pwm3_s <= speed_0;
				direction0_s <= direction_cClockWise;
				direction2_s <= direction_clockWise;
		elsif(rs_backward_condition) then 
				pwm0_s <= speed0_backward;
				pwm1_s <= speed_0;
				pwm2_s <= speed2_backward;
				pwm3_s <= speed_0;
				direction0_s <= direction_clockWise;
				direction2_s <= direction_cClockWise;
		else
			next_state <= stop;
		end if;
			
		when rs_rotate =>
		strike_confirm <= '0';
		com_send <= '0';
		if(rs_clockwise_condition) then
				pwm0_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm1_s <= std_logic_vector(to_unsigned(2400, 16));
				pwm2_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm3_s <= std_logic_vector(to_unsigned(2400, 16));
				direction0_s <= direction_clockWise;
				direction1_s <= direction_clockWise;
				direction2_s <= direction_clockWise;
				direction3_s <= direction_clockWise;
		elsif(rs_cClockwise_condition) then
				pwm0_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm1_s <= std_logic_vector(to_unsigned(2400, 16));
				pwm2_s <= std_logic_vector(to_unsigned(2000, 16));
				pwm3_s <= std_logic_vector(to_unsigned(2400, 16));
				direction0_s <= direction_cClockWise;
				direction1_s <= direction_cClockWise;
				direction2_s <= direction_cClockWise;
				direction3_s <= direction_cClockWise;
		 else
				next_state <= stop;
		 end if;
		
		when zynq_stop => 
		pwm0_s <= speed_0;
		pwm1_s <= speed_0;
		pwm2_s <= speed_0;
		pwm3_s <= speed_0;	
		direction0_s <= direction_cClockWise;
		direction1_s <= direction_cClockWise;
		direction2_s <= direction_cClockWise;
		direction3_s <= direction_cClockWise;
		if rs_strike_condition then 
			next_state <= wait_EOF;
		else 
			next_state <= stop;
		end if;
		
		when rs_outside =>
		strike_confirm <= '0';
		com_send <= '0';
		if(rs_ready_condition) then
			pwm0_s <= speed_0;
			pwm1_s <= speed_0;
			pwm2_s <= speed_0;
			pwm3_s <= speed_0;
			next_state <= rs_outside;
		elsif(rs_strike_condition)then
			strike_confirm <= '1';
			delay_active <= true;
			delay_time <= delay_20m;
			if delay_condition then
				delay_active <= false;
				next_state <= wait_EOF;
			else
				pwm0_s <= speed_0;
				pwm1_s <= speed_0;
				pwm2_s <= speed_0;
				pwm3_s <= speed_0;
			end if;
			else 
			next_state <= stop;
		end if;	
		
		when wait_EOF => 
		strike_confirm <= '1';
--		com_send <= '0';
		if EOF_flag_condition then --OR rs_reset_condition  
			strike_confirm <= '0';
--			delay_time <= delay_20m;
--			delay_active <= true;
--			COM_send <= '1';
--			if delay_condition then
--				COM_send <= '0';
--				delay_active <= false;
				next_state <= stop;
--			end if;
		end if;
			
		when others=>
			strike_confirm <= '0';
	end case;
		
end process;

with current_state select
		ostate <= "1111001" when start,--1
					 "0100100" when drivingDistance,--2
					 "0110000" when align,--3
					 "0010010" when alignedDriving,--4
					 "0001000" when others; --A

		 
END movingStates_architecture;