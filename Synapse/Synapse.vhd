library ieee;
use ieee.std_logic_1164.all;

entity Synapse is 
	port(
		propFlag 	 : in  std_logic;
		cyclecounter : in  std_logic_vector(7 downto 0); --DSN counter
		destination  : in  std_logic_vector(6 downto 0); --input is current location
		buff			 : in  std_logic_vector(3 downto 0); --overall count
		vInfo			 : in  std_logic_vector(12 downto 0);
		vSpike 		 : in  std_logic;
		clk			 : in  std_logic;
		controlWord	 : out std_logic_vector(31 downto 0)
	);
end synapse;

architecture inst1 of Synapse is 
	signal pFlag		 : std_logic;
	signal dest        : std_logic_vector(6 downto 0);  --next state done by router
	signal cur			 : std_logic_vector(6 downto 0);
	signal propResult	 : std_logic_vector(12 downto 0);
	
begin

	process(clk) 
	begin
		
		if (vspike = '1') then 
			pFlag <= propFlag;
			cur <= destination;
			--first 3 bits is current neuron
			if (cur(2 downto 0) = "000") then
				pFlag <= '1'; --1 indicate forward for initial synapse
			elsif (cur(2 downto 0) = "111") then
				pFlag <= '0'; --0 indicates backwards for final synapse
			end if;
			
			dest <= "1111111"; 
			--1 + 4 + 7 + 7 + 13 = 32
			controlWord <= pFlag & buff & dest & cur & propResult;   
		end if;
	end process;
	
	DUT: entity work.Prop port map(vinfo => vinfo, cyclecounter => cyclecounter, buff => buff, clk => clk, result => propResult);
end inst1;