#include <kipr/wombat.h>


int arm(int fromPosA, int toPosA); //longer arm 

void clawOpen();

void clawClosed();

int base(int fromPosB, int toPosB); //Base rotation

void drive(int dist);

void back (int dist);

void turnR (int deg);

void turnL (int deg);

void camera();


const int SPEEDR = 190; //Max is 500mm/second 

const int SPEEDL = 190;

const int SPEEDA = 2; //Arm speed

const int CLAWO = 1110;

const int CLAWC = 440;


int main()

{

	//Opening Code

	create_connect();

	//wait_for_light(0);

	shut_down_in(119);

	enable_servos();

	camera_open();

    

	//Initialize servo positions    

    int basePos = 1024, armPos = 1024;

	set_servo_position(1, basePos); //base

	set_servo_position(2, armPos); //longer arm

	set_servo_position(3, CLAWO); //claw

    

    //Code:

    //Moving Towards BotGal

	//Starts at Watch Floor moves forward toward towers and turns toward tower with BotGal

	turnR(47);

	drive(1050);

	turnR(43);

	drive(100);

    create_stop();

    

    //Picking Up Botgal

    armPos = arm(armPos, 2047); //Lifts arm up

    basePos = base(basePos, 0); //turns crane towards tower

    armPos = arm(armPos, 1750); //Lowers arm onto botgal

    clawClosed();    

    armPos = arm(armPos, 2047); //Raises arm to avoid hitting tower

    basePos = base(basePos, 2047); //resets crane position

    armPos = arm(armPos, 1024); //Lowers arm for safer transport

   

    //Move towards analysis lab

    turnR(90);

    drive(500);

    create_stop();

    clawOpen();

    msleep(10);

    basePos = base(basePos, 1024); //Resets crane position

    

    //Drive towards tower one 

    drive(-500);

    turnR(90);

    drive(200);

    create_stop();

    

    armPos = arm(armPos, 2047); //Lifts arm up

    basePos = base(basePos, 2047); //turns crane towards tower

    armPos = arm(armPos, 1500); //Lowers arm onto botgal

    clawClosed();    

    armPos = arm(armPos, 2047); //Raises arm to avoid hitting tower

    basePos = base(basePos, 1024); //resets crane position

    armPos = arm(armPos, 1024); //Lowers arm for safer transport

    

    //Moving towards tower two

    //Goal is to stack block one on block two and transport both 

    drive(200);

    create_stop();

    

    //Picking Up Botgal

    armPos = arm(armPos, 2047); //Lifts arm up

    basePos = base(basePos, 2047); //turns crane towards tower

    armPos = arm(armPos, 1700); //Lowers arm onto botgal

    clawOpen();

    armPos = arm(armPos, 1500); //Lowers arm onto botgal

    clawClosed();    

    armPos = arm(armPos, 1700); //Raises arm to avoid hitting tower

    basePos = base(basePos, 1024); //resets crane position

    

    //Returning to Analysis lab

    turnL(90);

    drive(500);

    create_stop();

    basePos = base(basePos, 0); //faces crane towards analysis lab

    armPos = arm(armPos, 1024); //lowers cubes

    

	disable_servos();

	camera_close();

	create_disconnect();

	return(0);

}


void drive(int dist)

{

	set_create_distance(0);

	if( dist > 0)

	{

		while (get_create_distance() < dist)

        {

			create_drive_direct(SPEEDL, SPEEDR);

        }

	}

	else if(dist < 0)

	{

		while (get_create_distance() > dist)

		{

			create_drive_direct(-SPEEDL, -SPEEDR);

		}

	}

    else

	create_stop();

}


void turnR (int deg)

{

    set_create_total_angle(0);

    

    while (get_create_total_angle() > -deg)

    {

        create_drive_direct(SPEEDL, -SPEEDR);

    }

    create_stop();

}


void turnL (int deg)

{

    set_create_total_angle(0);

    while (get_create_total_angle() < deg)

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


int base(int fromPosB, int toPosB)

{

    enable_servos();

    int current;

    

    if (fromPosB < toPosB)

    {

	for(current = fromPosB; current < toPosB; current++)

	{

        set_servo_position(1, current);

		msleep(SPEEDA);

	}

    }

    else

    {

        for(current = fromPosB; current > toPosB; current--)

        {

         set_servo_position(1, current);

         msleep(SPEEDA);

        }

    }

    

    return current;


}


void clawOpen()

{

	enable_servos(); 

   	int position;

    for (position = CLAWC; position < CLAWO; position++)

    {

        set_servo_position(3, position); 

        msleep(SPEEDA);

    }

}


void clawClosed()

{

    enable_servos(); 

   	int position;

    for (position = CLAWO; position > CLAWC; position--)

    {

        set_servo_position(3, position); 

        msleep(1);

    }

}


int arm(int fromPosA, int toPosA)

{

	enable_servos();

    int current;

    if(fromPosA < toPosA)

    {

		for(current = fromPosA; current < toPosA; current++)