
module	birdDraw	(	
		input	logic	clk,
		input	logic	resetN,
		input logic signed [1:0] [10:0]	coordinate,
		input	logic	InsideRectangle, //input that the pixel is within a leftside bracket  
		input logic flash, //should bird flash red (got hit)
		input logic alive, //is bird alive
		input logic duty50, //wings up or down
		input	logic	[0:OBJECT_HEIGHT_Y-1] [0:OBJECT_WIDTH_X-1] [8-1:0] wings_up_object_colors,
		input	logic	[0:OBJECT_HEIGHT_Y-1] [0:OBJECT_WIDTH_X-1] [8-1:0] wings_down_object_colors,

		output	logic	drawingRequest, //output that the pixel should be dispalyed 
		output	logic	[7:0] RGBout  //rgb value from the bitmap 
 ) ;

// this is the devider used to acess the right pixel 
localparam  int OBJECT_NUMBER_OF_Y_BITS = 5;  // 2^5 = 32 
localparam  int OBJECT_NUMBER_OF_X_BITS = 5;  // 2^5 = 32 
localparam  int OBJECT_HEIGHT_Y = 1 <<  OBJECT_NUMBER_OF_Y_BITS ;
localparam  int OBJECT_WIDTH_X = 1 <<  OBJECT_NUMBER_OF_X_BITS;

localparam logic [7:0] TRANSPARENT_ENCODING = 8'hFF ;// RGB value in the bitmap representing a transparent pixel 
localparam logic [7:0] DEFAULT_COLOR = 8'h33;

parameter logic [7:0] RED = 8'hE0;
parameter logic [7:0] COLOR = DEFAULT_COLOR;


logic [0:OBJECT_HEIGHT_Y-1] [0:OBJECT_WIDTH_X-1] [8-1:0] object_colors;

always_comb
begin
	object_colors = duty50 ? wings_up_object_colors : wings_down_object_colors;
end


// pipeline (ff) to get the pixel color from the array 	 

//////////--------------------------------------------------------------------------------------------------------------=
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
		RGBout <=	8'h00;
	end
	else
	begin
		if (InsideRectangle == 1'b1 ) // inside an external leftside bracket 
		begin
			if (flash && (object_colors[coordinate[1]][coordinate[0]] != TRANSPARENT_ENCODING))
			begin
				//bird is red
				RGBout <= RED;
			end
			else 
			begin
				if (object_colors[coordinate[1]][coordinate[0]] == DEFAULT_COLOR)
					RGBout <= COLOR;
				else
					RGBout <= object_colors[coordinate[1]][coordinate[0]];	
			end
		end 
		else 
			RGBout <= TRANSPARENT_ENCODING ; // force color to transparent so it will not be displayed 
	end 
end

//////////--------------------------------------------------------------------------------------------------------------=
// decide if to draw the pixel or not 
assign drawingRequest = ((RGBout != TRANSPARENT_ENCODING) && alive) ? 1'b1 : 1'b0 ; // get optional transparent command from the bitmpap   

endmodule
