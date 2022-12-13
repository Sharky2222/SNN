library ieee;
use IEEE.numeric_std.all;
use ieee.std_logic_1164.all;

entity propagater is 
	port(
		info 			   : in std_logic_vector(13 downto 0);
		divisor        : in std_logic_vector(12 downto 0); -- maybe change dimensions later
		valid          : in std_logic;
		clk 				: in std_logic;
		reset          : in std_logic;
		avg 			   : out std_logic_vector(13 downto 0);
		ready          : out std_logic
	);
end propagater;

architecture prop of propagater is

	type statetype is (idle, init, processinfo, result);
	attribute stateencoding : string;
	attribute stateencoding of statetype : type is "00 01 10 11";
	signal presentstate, nextstate : statetype;
	
	--signal declearation here
	signal min	       : std_logic_vector(12 downto 0);
	signal max	       : std_logic_vector(12 downto 0);
	signal counter     : std_logic_vector(12 downto 0);
	signal msb_index   : integer; --ay carumba
	signal average     : std_logic_vector(13 downto 0);
	signal upper       : std_logic_vector(13 downto 0);
	signal lower       : std_logic_vector(13 downto 0);
	signal rdy         : std_logic;
	
begin

process(clk) 
	begin 
		if(rising_edge(clk)) then
        case presentstate is 
            when idle => 
               if (valid = '1') then 
						nextstate <= init; 
               else 
						nextstate <= idle; 
					end if; 
				when init =>
					nextstate <= processinfo;
				when processinfo =>
					if(msb_index = 0) then 
						nextstate <= result;
               else 
						nextstate <= processinfo; 
					end if;
				when result =>
					if(valid = '1') then 
						nextstate <= init;
               else 
						nextstate <= idle; 
				   end if;
				when others  =>  nextstate <= idle;
        end case;
		 end if; 
	end process;
	
	
	process(clk) -- state flip-flops
    begin 
			if(rising_edge(clk)) then
				if(reset = '0') then 
					presentstate <= nextstate; --breaks here
				elsif(reset = '1') then 
					presentstate <= idle; 
				end if;
			end if;
		
    end process; 
    
	process(clk) -- output assignments
	begin
		if(rising_edge(clk)) then
		
			if(presentstate = idle or presentstate = result) then
				rdy <= '1';
			else 
				rdy <= '0';
			end if;
			
			if(presentstate = init) then
			
				--step 1:
				min <= divisor;
				
				--MSB detector
				if min(12) = '1'    then
					msb_index <= 12;
				elsif min(11) = '1' then
					msb_index <= 11;
				elsif min(10) = '1' then
					msb_index <= 10;
				elsif min(9) = '1'  then
					msb_index <= 9;		
				elsif min(8) = '1'  then
					msb_index <= 8;
				elsif min(7) = '1'  then
					msb_index <= 7; 
				elsif min(6) = '1'  then
					msb_index <= 6;
				elsif min(5) = '1'  then
					msb_index <= 5;
				elsif min(4) = '1'  then
					msb_index <= 4;		
				elsif min(3) = '1'  then
					msb_index <= 3;		
				elsif min(2) = '1'  then
					msb_index <= 2;		
				elsif min(1) = '1'  then
					msb_index <= 1;		
				elsif min(0) = '1'  then
					msb_index <= 0;
				end if;	
			
				--step 2:
				--max <= divisor(11 downto 0) & '0'; -- overflow ?
				
				--step 3:				
				upper <= std_logic_vector(shift_right(unsigned(info), msb_index));
				
				--step 4:
				lower <= std_logic_vector(shift_right(unsigned(info), msb_index + 1));
			end if;
			
			if(presentstate = processinfo) then
				--step 5:
				average <= std_logic_vector(unsigned(upper) + unsigned(lower));
				average <= '0' & average(13 downto 1);
				
				msb_index <= msb_index - 1;
					
				if(divisor(msb_index) = '1') then -- ********************************
					upper <= average;
				else
					lower <= average;
				end if;
		
			end if;
			
			if(presentstate = result) then
				avg <= average;
			end if;
			
		end if;
	end process;
	ready <= rdy;
end prop;