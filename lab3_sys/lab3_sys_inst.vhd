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

	u0 : component lab3_sys
		port map (
			clk_clk                             => CONNECTED_TO_clk_clk,                             --                      clk.clk
			opencores_i2c_0_export_0_scl_pad_io => CONNECTED_TO_opencores_i2c_0_export_0_scl_pad_io, -- opencores_i2c_0_export_0.scl_pad_io
			opencores_i2c_0_export_0_sda_pad_io => CONNECTED_TO_opencores_i2c_0_export_0_sda_pad_io, --                         .sda_pad_io
			push_external_connection_export     => CONNECTED_TO_push_external_connection_export,     -- push_external_connection.export
			reset_reset_n                       => CONNECTED_TO_reset_reset_n,                       --                    reset.reset_n
			seg0_external_connection_export     => CONNECTED_TO_seg0_external_connection_export      -- seg0_external_connection.export
		);

