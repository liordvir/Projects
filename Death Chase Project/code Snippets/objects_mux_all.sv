

module	objects_mux_all	(	
//		--------	Clock Input	 	
	input		logic	clk,
	input		logic	resetN,
		// digits	
	input		logic	digitsDrawingRequest,
	input		logic	[7:0] digitsRGB, 
		// player
	input		logic	playerDrawingRequest,
	input		logic	[7:0] playerRGB, 
		// bird
	input		logic	birdsDrawingRequest,
	input		logic	[7:0] birdsRGB, 
		// shot		
	input		logic	shotsDrawingRequest,
	input		logic	[7:0] shotsRGB, 
		// pickup
	input		logic	pickupDrawingRequest,
	input		logic	[7:0] pickupRGB, 
		// poop
	input		logic	poopsDrawingRequest,
	input		logic	[7:0] poopsRGB, 
		// tree
	input		logic	treesDrawingRequest,
	input		logic	[7:0] treesRGB, 
					
		// dynamic ground
	input		logic	dynamic_ground_Request,
	input		logic	[7:0] dynamic_ground_RGB, 
					
		// background 
	input		logic	[7:0] backGroundRGB, 

	output	logic	[7:0] redOut, // full 24 bits color output
	output	logic	[7:0] greenOut, 
	output	logic	[7:0] blueOut 
					
);

logic [7:0] tmpRGB;

assign redOut	  = {tmpRGB[7:5], {5{tmpRGB[5]}}}; //--  extend LSB to create 10 bits per color  
assign greenOut  = {tmpRGB[4:2], {5{tmpRGB[2]}}};
assign blueOut	  = {tmpRGB[1:0], {6{tmpRGB[0]}}};

//
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
			tmpRGB	<= 8'b0;
	end
	else begin
		if (digitsDrawingRequest == 1'b1 )   
			tmpRGB <= digitsRGB;  //first priority 
			
		else if (playerDrawingRequest == 1'b1 )   
			tmpRGB <= playerRGB;  //second priority 

		else if (birdsDrawingRequest == 1'b1 )   
			tmpRGB <= birdsRGB;  //third priority 
			
		else if (shotsDrawingRequest == 1'b1 )   
			tmpRGB <= shotsRGB;  //forth priority 
			
		else if (pickupDrawingRequest == 1'b1 )   
			tmpRGB <= pickupRGB;  //fifth priority 
			
		else if (poopsDrawingRequest == 1'b1 )   
			tmpRGB <= poopsRGB;  //sixth priority 
			
		else if (treesDrawingRequest == 1'b1 )   
			tmpRGB <= treesRGB;  //seventh priority
			
		else if (dynamic_ground_Request == 1'b1 )   
			tmpRGB <= dynamic_ground_RGB;  //next to last priority 
			
		else
			tmpRGB <= backGroundRGB ; // last priority 
		end ; 
	end

endmodule


