library ieee;
use IEEE.numeric_std.all;
use ieee.std_logic_1164.all;

entity Prop is 
	port(
		vinfo 			: in std_logic_vector(12 downto 0);
		cycleCounter 	: in std_logic_vector(7 downto 0); --current cycles in current dsn
		buff				: in std_logic_vector(3 downto 0); --add to cycle counter
		clk 				: in std_logic;
		result 			: out std_logic_vector(12 downto 0)
	);
end Prop;

architecture propagater of Prop is
	signal del_t	  : unsigned(7 downto 0);
	signal del_t2	  : unsigned(7 downto 0);
	signal del_t_max : unsigned(12 downto 0);
	signal del_t_min : unsigned(12 downto 0);
	signal avg		  : unsigned(12 downto 0);

begin

	del_t <= unsigned(buff) + unsigned(cycleCounter);
		
	process(clk) 
		variable MSB	     : integer;
		variable layerCount : integer := 1;--might need signal input for this to indicate from user
		variable layer 	  : integer := 2;--change this range to adjust iteration layers
	begin
	
		if(del_t(7) = '1') then
			MSB := 7;
		elsif(del_t(6) = '1') then
			MSB := 6;
		elsif(del_t(5) = '1') then
			MSB := 5;
		elsif(del_t(4) = '1') then
			MSB := 4;
		elsif(del_t(3) = '1') then
			MSB := 3;
		elsif(del_t(2) = '1') then
			MSB := 2;
		elsif(del_t(1) = '1') then
			MSB := 1;
		elsif(del_t(0) = '1') then
			MSB := 0;
		end if;
		
		del_t_max <= shift_right(unsigned(vinfo), MSB);
		del_t_min <= shift_right(unsigned(vinfo), MSB+1);
 		avg <= shift_right((del_t_max + del_t_min),1);
		
		layer := layer - 1;
		del_t2 <= del_t - shift_left("1",MSB);
		
		for x in 0 to 7 loop --figure out way to run max layer iterations but exit early based on layer 
			
			if(del_t(MSB-layerCount) = '0') then
				del_t_min <= avg;
			else 
				del_t2 <= del_t2 - shift_left("1", MSB-layerCount);
				del_t_max <= avg;
			end if;
			
			avg <= shift_right((del_t_max + del_t_min),1);

			layerCount := layerCount + 1;
			
			exit when x = layer;
			
		end loop;
		
		result <= std_logic_vector(avg);
		
	end process;

end propagater;