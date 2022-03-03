module vga_controller(
  // System Parameter inputs
  input wire          clk, reset,
  input wire          i_wb_clk, i_rst,

  // Output for VGA Hardware
  output logic [3:0]  VGA_R,
  output logic [3:0]  VGA_G,
  output logic [3:0]  VGA_B,
  output wire         VGA_HS,
  output wire         VGA_VS,

  // Wishbone Control Signals 
  input wire   [5:0]  i_wb_adr,
  input wire  [31:0]  i_wb_dat,
  input wire   [3:0]  i_wb_sel,
  input wire 	        i_wb_we,
  input wire 	        i_wb_cyc,
  input wire 	        i_wb_stb,
  output reg  [31:0]  o_wb_rdt,
  output reg 	        o_wb_ack

);
// Internal wires
wire [18:0]  wire_pix;
wire [4:0]   vga_data;
wire         video_on;
wire [11:0]  pixel_row;
wire [11:0]  pixel_column;
wire         clk_vga;
logic        wea;
wire         ena;
logic [18:0] addra;
logic  [3:0]  dina;
logic        wb_vga_ack_ff;

clk_wiz_0 vga_clk (
  // Clock out ports
  .clk_out1(clk_vga),     // output clk_out1
  // Status and control signals
  .reset(reset), // input reset
  // Clock in ports
  .clk_in1(clk)
);      

blk_mem_gen_0 your_instance_name (
  .clka(clk_vga),    // input wire clka
  .ena(ena),      // input wire ena
  .wea(wea),      // input wire [0 : 0] wea
  .addra(addra),  // input wire [18 : 0] addra
  .dina(dina),    // input wire [3 : 0] dina
  .clkb(clk_vga),    // input wire clkb
  .enb(1'b1),      // input wire enb
  .addrb(wire_pix),  // input wire [18 : 0] addrb
  .doutb(vga_data)
);

dtg timing_generator (
  .clock(clk_vga),
  .rst(1'b0),
  .horiz_sync(VGA_HS),
  .vert_sync(VGA_VS),
  .video_on(video_on),
  .pix_num(wire_pix),
  .pixel_row(pixel_row),
  .pixel_column(pixel_column)
);

assign o_wb_ack = wb_vga_ack_ff;
assign ena = wea; 
always_ff @( posedge i_wb_clk ) begin
  if (i_rst) begin
    o_wb_ack = 0;
    wea = 0;
  end
  else begin
    wea = wb_vga_ack_ff & i_wb_we ? i_wb_we: wea;
    wb_vga_ack_ff = ! wb_vga_ack_ff & i_wb_stb & i_wb_cyc;

    if(wea) begin
      addra = i_wb_dat[31:12];
      dina = i_wb_dat[3:0];
    end
  end
end


always_ff @( posedge clk_vga ) begin
  if( video_on === 1'b1 ) begin
    VGA_R <= vga_data;
    VGA_G <= vga_data;
    VGA_B <= vga_data;
  end 
  else begin
    VGA_R <= 4'b0;
    VGA_R <= 4'b0;
    VGA_R <= 4'b0;
  end  
end

/*
always @(posedge wb_clk, posedge wb_rst) begin
  if (wb_rst) begin
    wb_vga_reg = 32'h00060006;
    wb_vga_ack_ff = 0 ;
  end
  else begin
    wb_vga_reg = wb_vga_ack_ff && wb_m2s_vga_we ? wb_m2s_vga_dat : wb_vga_reg;
    wb_vga_ack_ff = ! wb_vga_ack_ff & wb_m2s_vga_stb & wb_m2s_vga_cyc;
  end
end

*/

endmodule