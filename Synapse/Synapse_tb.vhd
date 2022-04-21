library ieee;
use ieee.std_logic_1164.all;

entity synapse_tb is 
end synapse_tb;

architecture tb of synapse_tb is

	signal pFlag		 	: std_logic:= '0';
	signal cyclecounter  : std_logic_vector(7 downto 0) := "00000010";
	signal dest        	: std_logic_vector(6 downto 0):= "0000001";  --next state done by router
	signal cur			 	: std_logic_vector(6 downto 0):= "0000000";
	signal buff   			: std_logic_vector(3 downto 0):= "0000";
	signal vinfo 			: std_logic_vector(12 downto 0):= "1000000000000";
	signal vSpike 			: std_logic:= '1';
	signal clk 				: std_logic:= '1';
	signal controlWord	: std_logic_vector(31 downto 0);
	
	
begin

	clk <= not clk after 10 ns;
	
	process
	begin
		wait for 100 ns;
		vspike <= '0';
		wait for 100 ns;
		vspik
	end process;
	
	UUT : entity work.Synapse port map(
	propFlag => pFlag, 
	cyclecounter => cyclecounter,
	destination => dest, 
	buff => buff, 
	vInfo => vinfo, 
	vSpike => vSpike,
	clk => clk, 
	controlWord => controlword);

	
end tb;