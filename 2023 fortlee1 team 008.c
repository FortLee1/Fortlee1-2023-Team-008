#include <kipr/wombat.h>


void claw (int fromPosC, int toPosC);

void armRaise (int fromPosArm0, int fromPosArm1, int finalPos);

void armDrop (int fromPosArm0, int fromPosArm1, int finalPos);

void drive(int distance);

void turnR (int degree);

void turnL (int degree);

void camera();


const int SPEEDR = 200; //Max is 500mm/second 

const int SPEEDL = 180;

const int SPEEDA = 1; //Arm speed

//Distance travels in mm

//Arm0 is horizontal at 1024 

//Arm1 is horizontal at 0

//Length of table is 2450mm

//width of table is 1130mm


int main()

{

//Opening Code

create_connect();

 //wait_for_light(0);

shut_down_in(119);

enable_servos();

camera_open();

//Variable to count how far the bot has traveled

int count = 0;


//Sets servos to initial flat position

set_servo_position(0, 1024); //Flat Horizontal

set_servo_position(1, 0); //Flat Horizontal

set_servo_position(2, 1024); //Open Claw


//Testing Arm function

armRaise(1024, 0, 1600);

armDrop(0, 1024, 0);


//Moving Towards BotGal

//Starts at Watch Floor moves forward toward towers and turns toward tower with BotGal

turnR(47);

while((get_create_lbump() == 0) || (get_create_rbump() == 0))

	drive(1000);

turnL(43);

drive(-15);


//Picking up BotGal

armRaise(1024, 0, 1600); //Extends Arm upward

claw(1024, 0); //Closes Claw

set_servo_position(1, 1800); //Lifts arm by a bit in order to move BotGal without knocking over towers


//Put BotGal in analysis lab

//Starts in front of tower and turns left moves forward and turns left again and drives towards analysis lab

turnL(90);

drive(400);

turnL(90);

drive(600);

//Drops arm and opens claw

armDrop(0, 1600, 0);

claw(0, 1024);


//Grab First Cube

//Starts in front of analysis lab and drives towards farthest to the left cube

turnR(180);

drive(600);

turnL(90);

drive(700);

turnR(90);

//Picking up BotGal

armRaise(1024, 0, 1600); //Extends Arm upward

claw(1024, 0); //Closes Claw

set_servo_position(1, 1800); //Lifts arm by a bit in order to move BotGal without knocking over towers


//move towards analysis lab

//Starts in front of farthest left tower and moves towards analysis lab

turnR(90);

drive(700);

turnR(90);

drive(600);

//Drops arm and opens claw

armDrop(0, 1600, 0);

claw(0, 1024);


//Get Cube 2

//Moves back from analysis lab to tower and back to analysis lab putting cube on top of other cube

turnR(180);

drive(600);

//Picking up BotGal

armRaise(1024, 0, 1600); //Extends Arm upward

claw(1024, 0); //Closes Claw

set_servo_position(1, 1800); //Lifts arm by a bit in order to move BotGal without knocking over towers


turnL(180);

drive(600);

//Drops arm and opens claw

armDrop(0, 1600, 500);

claw(0, 1024);


disable_servos();

camera_close();

create_disconnect();

return(0);

}


void drive(int distance)

{

set_create_distance(0);

if( distance > 0)

{

while (get_create_distance() <= distance)

{

create_drive_direct(SPEEDL, SPEEDR);

}

}

else

{

while (get_create_distance() >= distance)

{

create_drive_direct((-1 * SPEEDL), (-1 * SPEEDR));

}

}

create_stop();

}


void turnR(int degree)

{

    set_create_total_angle(0);

    while (get_create_total_angle() >= -degree)

    {

        create_drive_direct(SPEEDL, -SPEEDR);

    }

    create_stop();

}


void turnL (int degree)

{

    set_create_total_angle(0);

    while (get_create_total_angle() < degree)

    {

        create_drive_direct(-SPEEDL, SPEEDR);

    }

    create_stop();

}


void camera()

{

    

int stop = 0;


while (stop == 0)

{

	camera_update();

	if (get_object_count(0) > 0) //If seeing BotGal

	{

		int x = get_object_center_x(0, 0);

        

		if (x < 75)

		{

			//when the center of the object is to the left of bot turns left

			turnL(25);

		}

		else if (x > 85)

		{

		//when the center of the object is to the right of bot turns right

		turnR(25);

		}


		else

		{

		//stops the loop when object is in center of bot

		stop = 1;

		ao();

		}

	}

}

}


void claw (int fromPosC, int toPosC)

{

enable_servos(); 

   	int position = 0; 


    	if (fromPosC < toPosC)

    	{

        		for (position = fromPosC; position <= toPosC; position++)

        		{

            		set_servo_position(0, position); 

            		msleep(1);

}

}


else 

    	{

        		for (position = fromPosC; position >=  toPosC; position--)

        		{

            		set_servo_position(0, position); 

            		msleep(1);

    		}

}

}


void armRaise (int fromPosArm0, int fromPosArm1, int finalPos)

{

	int arm0, arm1, arm1F, topArm, bothArm;

    //Arm0 is horizontal at 1024 

    //Arm1 is horizontal at 0 

    arm0 = fromPosArm0;

    arm1 = fromPosArm1;    

	arm1F = finalPos - 1024;

    

	//Raises both arms to 1024

	for(bothArm = 0; bothArm <= 1024; bothArm++)

	{

		set_servo_position(0, arm0--);

		set_servo_position(1, arm1++);

		msleep(SPEEDA);

	}

            

	//Raises/Lowers top arm depending on final arm value 

	if (arm1F > 0)

	{

		for(topArm = 0; topArm <= arm1F; topArm++)

		{

			set_servo_position(1, arm1++);

			msleep(1);

		}

   	}

	else if (arm1F < 0)

	{ 

		for(topArm = arm1F; topArm >= 0; topArm--)

		{

			set_servo_position(1, arm1--);

			msleep(1);

        }

	}

}


void armDrop (int fromPosArm0, int fromPosArm1, int finalPos)

{	

	int arm0, arm1, arm1F, topArm, bothArm;

    arm0 = fromPosArm0;

    arm1 = fromPosArm1; 

	arm1F = finalPos - 1024;

    

	if(arm1F > 0)

	{

		for(topArm = arm1F; topArm >= 0; topArm--)

    	{

        	set_servo_position(1, arm1--);

           	msleep(SPEEDA);

		}

	}    

	else if (arm1F < 0)

	{

		arm1F += 1024; //finds value in which arm1 needs to reach 1024

		for(topArm = 0; topArm < arm1F; topArm++)

    	{

        	set_servo_position(1, arm1++);

           	msleep(SPEEDA);

        }

    }

    for(bothArm = 0; bothArm <= 1024; bothArm++)

    {

		set_servo_position(0, arm0++);

        set_servo_position(1, arm1--);

        msleep(SPEEDA);

    }

} 