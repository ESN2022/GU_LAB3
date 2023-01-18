library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity lab3 is
	port( clk 		: in std_logic;
			seg 		: out std_logic_vector(47 downto 0);
			reset_n 	: in std_logic;
			sda 		: inout std_logic;
			scl 		: inout std_logic;
			cs_n		: out std_logic;
			switch 	: in std_logic_vector(1 downto 0)
			);		
end entity;


architecture rtl of lab3 is


    component lab3_sys is
        port (
            clk_clk                             : in    std_logic                     := 'X';             -- clk
            opencores_i2c_0_export_0_scl_pad_io : inout std_logic                     := 'X';             -- scl_pad_io
            opencores_i2c_0_export_0_sda_pad_io : inout std_logic                     := 'X';             -- sda_pad_io
            push_external_connection_export     : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
            reset_reset_n                       : in    std_logic                     := 'X';             -- reset_n
            seg0_external_connection_export     : out   std_logic_vector(23 downto 0)                     -- export
        );
    end component lab3_sys;

	component bin_to_7seg is
		port (
			B : in std_logic_vector (3 downto 0);
			S : out std_logic_vector (7 downto 0)
		);
	end component bin_to_7seg;

	signal seg_bin : std_logic_vector(23 downto 0);
begin
    u0 : component lab3_sys
        port map (
            clk_clk                             => clk,                             --                      clk.clk
            opencores_i2c_0_export_0_scl_pad_io => scl, -- opencores_i2c_0_export_0.scl_pad_io
            opencores_i2c_0_export_0_sda_pad_io => sda, --                         .sda_pad_io
            push_external_connection_export     => switch,     -- push_external_connection.export
            reset_reset_n                       => reset_n,                       --                    reset.reset_n
            seg0_external_connection_export     => seg_bin      -- seg0_external_connection.export
        );

	cs_n <= '1';
 
	u1 : component bin_to_7seg
	port map(
		B => seg_bin(3 downto 0), -- clk.clk
		S => seg(7 downto 0) -- leds_external_connection.export
	);
 
	u2 : component bin_to_7seg
	port map(
		B => seg_bin(7 DOWNTO 4), -- clk.clk
		S => seg(15 DOWNTO 8) -- leds_external_connection.export
	);

	u3 : component bin_to_7seg
	port map(
		B => seg_bin(11 DOWNTO 8), -- clk.clk
		S => seg(23 DOWNTO 16) -- leds_external_connection.export
	);
	
	u4 : component bin_to_7seg
	port map(
		B => seg_bin(15 DOWNTO 12), -- clk.clk
		S => seg(31 DOWNTO 24) -- leds_external_connection.export
	);
	
	
	u5 : component bin_to_7seg
	port map(
		B => seg_bin(19 DOWNTO 16), -- clk.clk
		S => seg(39 DOWNTO 32) -- leds_external_connection.export
	);
	
	u6 : component bin_to_7seg
	port map(
		B => seg_bin(23 DOWNTO 20), -- clk.clk
		S => seg(47 DOWNTO 40) -- leds_external_connection.export
	);
end architecture;