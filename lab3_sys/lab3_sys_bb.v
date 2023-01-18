
module lab3_sys (
	clk_clk,
	opencores_i2c_0_export_0_scl_pad_io,
	opencores_i2c_0_export_0_sda_pad_io,
	push_external_connection_export,
	reset_reset_n,
	seg0_external_connection_export);	

	input		clk_clk;
	inout		opencores_i2c_0_export_0_scl_pad_io;
	inout		opencores_i2c_0_export_0_sda_pad_io;
	input	[1:0]	push_external_connection_export;
	input		reset_reset_n;
	output	[23:0]	seg0_external_connection_export;
endmodule
