

module	birdLogic (	
	input	logic	clk,
	input	logic	resetN,
	input	logic	startOfFrame,  // short pulse every start of frame 30Hz 
	input logic collision,  //collision if shot hits bird
	input logic [7:0] random, //random number from random generator
	input logic [3:0] starting_life,
	input logic deploy,
	input logic [1:0] speed, //speed of movement
	input logic [1:0] damage, // damage taken
	
	output deploy_poop, // drop bomb
	output logic alive, // is bird alive
	output logic red, // should bird appear red
	output logic signed [1:0] [10:0]	coordinate// output the top left corner 					
);


// a module used to generate the  ball trajectory.  

parameter RANDOM_OFFSET = 0;
parameter int INITIAL_Y = 185; 

localparam int SCREEN_WIDTH = 640;
localparam int SCREEN_HEIGHT = 480;
localparam int INITIAL_X = 280; 
localparam int IMAGE_WIDTH = 32; //bird is 32x32 pixels
localparam int IMAGE_HEIGHT = 32;
localparam int MAX_RANDOM = 255; // max value of random
localparam int INITIAL_STEP = 64;
localparam int SPEED_MULTIPLIER = 32;
localparam int RED_COUNTER_VALUE = 32;
localparam int POOP_COUNTER_SHORT = 100;
localparam int POOP_COUNTER_LONG = 500;
localparam int POOP_THRESHOLD = 240;
localparam int BIT_2 = 2;
localparam int BIT_32 = 32;

int topLeftY_FixedPoint, topLeftX_FixedPoint; // local parameters 
int step; // movement speed
int life; // bird lives
int counter; // time when bird is red and cannot be hurt
int poop_counter; // cooldown between bombs
int random_num;
int chance_to_change = 8; // how likely for bird to change directions (lower number -> less likely)

enum logic [1:0] {idle_state, right_state, left_state} state, next_state;
enum logic {wings_up, wings_down} wing_state, next_wing_state;


const int	FIXED_POINT_MULTIPLIER	=	64;
// FIXED_POINT_MULTIPLIER is used to work with integers in high resolution 
// we do all calulations with topLeftX_FixedPoint  so we get a resulytion inthe calcuatuions of 1/64 pixel 
// we devide at the end by FIXED_POINT_MULTIPLIER which must be 2^n 
const int x_FRAME_SIZE	=	639 * FIXED_POINT_MULTIPLIER; // note it must be 2^n 

//////////--------------------------------------------------------------------------------------------------------------=
// position calculate 
always_comb
begin
	step = INITIAL_STEP + (SPEED_MULTIPLIER * speed); // movement speed
	random_num = random + RANDOM_OFFSET; // adding offset to random
	if (random_num > MAX_RANDOM) begin
		random_num = random_num - MAX_RANDOM;
	end
	
	next_state = state;
	if ((startOfFrame == 1'b1) && (random_num < chance_to_change)) begin // change direction of movement if random below given chance to change
		if (state == idle_state)
		begin
			if (topLeftX_FixedPoint < BIT_2 * IMAGE_WIDTH * FIXED_POINT_MULTIPLIER) // turn right
				next_state = right_state;
			else if (topLeftX_FixedPoint > x_FRAME_SIZE - (BIT_2 * IMAGE_WIDTH * FIXED_POINT_MULTIPLIER)) // turn left
				next_state = left_state;
			else
				next_state = (random_num < (chance_to_change / BIT_2)) ? right_state : left_state;
		end
		else
			next_state = idle_state; // straight
	end
	
end


always @(posedge clk or negedge resetN)
   begin
	   
   if ( !resetN )  // Asynchronic reset
	begin
		state <= idle_state;
	end
   else 		// Synchronic logic FSM
	begin	
		state <= next_state;
	end
	end // always

	
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin
	// initial values
		topLeftX_FixedPoint	<=  INITIAL_X * FIXED_POINT_MULTIPLIER;
		topLeftY_FixedPoint	<=  INITIAL_Y * FIXED_POINT_MULTIPLIER;
		life <= 0;
		poop_counter <= POOP_COUNTER_SHORT;
		deploy_poop <= 1'b0;
	end
	else begin
		//default values
		topLeftX_FixedPoint <= topLeftX_FixedPoint;
		topLeftY_FixedPoint <= topLeftY_FixedPoint;
		if (startOfFrame == 1'b1) begin // perform  position integral only 30 times per second 
			counter <= (counter > 0) ? counter - 1 : 0;
			if (life > 0) // bird alive
				poop_counter <= (poop_counter > 0) ? poop_counter - 1 : 0;
			else
				poop_counter <= poop_counter;
			deploy_poop <= 1'b0;

			
			if (deploy) begin // bird initialised
				life <= starting_life;
			end
			
			if (collision && !red) begin // bird is hit (and not invincible) 
				life <= life - damage;
				counter <= RED_COUNTER_VALUE; // frames to stay red
			end
			
			case (state)
			right_state: 
				if (topLeftX_FixedPoint < (x_FRAME_SIZE - (BIT_32 * FIXED_POINT_MULTIPLIER))) begin
					topLeftX_FixedPoint <= topLeftX_FixedPoint + step;
				end
			left_state: 
				if (((topLeftX_FixedPoint - step) / FIXED_POINT_MULTIPLIER) > (step / FIXED_POINT_MULTIPLIER)) begin
					topLeftX_FixedPoint <= topLeftX_FixedPoint - step;
				end
			default: begin
				topLeftX_FixedPoint <= topLeftX_FixedPoint;
				topLeftY_FixedPoint <= topLeftY_FixedPoint;
			end	
			endcase
			
			if ((life > 0) && (poop_counter == 0) && (random_num > POOP_THRESHOLD)) begin // drop bomb
				deploy_poop <= 1'b1;
				poop_counter <= POOP_COUNTER_LONG;
			end
		end		
	end
end

//get a better (64 times) resolution using integer   
assign 	coordinate[0] = topLeftX_FixedPoint / FIXED_POINT_MULTIPLIER ;   
assign 	coordinate[1] = topLeftY_FixedPoint / FIXED_POINT_MULTIPLIER ;    
assign 	red = counter > 0; 
assign 	alive = ((life > 0) || red) && resetN; // dont disappear until finished flashing red

endmodule
