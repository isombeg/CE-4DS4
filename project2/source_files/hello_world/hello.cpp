#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/rc_channels.h>
#include <drivers/drv_hrt.h>
#include <uORB/Publication.hpp>
#include <uORB/topics/test_motor.h>
#include <uORB/topics/debug_value.h>

#define DC_MOTOR 0
#define SERVO_MOTOR 1

#define STOP_SPEED 0.5
#define SLOW_SPEED 0.57
#define FAST_SPEED 0.8

#define LEFT_DIRECTION 0.75
#define FWD_DIRECTION 0.50
#define RIGHT_DIRECTION 0.25

#define SPEED_MASK 0b0011
#define DIR_MASK 0b1100

extern "C" __EXPORT int hello_world_main(int argc, char *argv[]);


void setStoppingSpeed(uORB::Publication<test_motor_s> test_motor_pub, test_motor_s test_motor){
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = (float)STOP_SPEED;
	test_motor.action = test_motor_s::ACTION_RUN;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;

	test_motor_pub.publish(test_motor);
}

void project2_0()
{
	//int sensor_combined_handle;
	int rc_handle;
	//sensor_combined_s sensor_data;
	rc_channels_s rc_data;
	rc_handle = orb_subscribe(ORB_ID(rc_channels));
	orb_set_interval(rc_handle, 200);
	while(1)
	{
		orb_copy(ORB_ID(rc_channels), rc_handle, &rc_data);
		PX4_INFO("RC0= %f, RC1 = %f, RC2 = %f", (double)rc_data.channels[0],
											(double)rc_data.channels[1],
											(double)rc_data.channels[2]);
		px4_usleep(200000);
	}
}

void project2_1a(){
	test_motor_s test_motor, test_servo;
	double motor_value = 0, servo_value = 0; // a number between 0 to 1
	int rc_handle;

	rc_channels_s rc_data;
	rc_handle = orb_subscribe(ORB_ID(rc_channels));

	//orb_set_interval(rc_handle, 200);

	uORB::Publication<test_motor_s> test_motor_pub(ORB_ID(test_motor));


	while(1)
	{
		//PX4_INFO("Enter speed value (0 to 1). If you enter a value outside the range, the motor will be stopped and the application will be terminated.");
		//scanf("%lf", &motor_value);

		//if(motor_value > 1.0 || motor_value < 0)
		// break;
		orb_copy(ORB_ID(rc_channels), rc_handle, &rc_data);

		motor_value = ((double)rc_data.channels[2]+1)*.5;
		servo_value = ((double)rc_data.channels[0]+1)*.5;

		PX4_INFO("Motor speed is %f", motor_value);
		test_motor.timestamp = hrt_absolute_time();
		test_motor.motor_number = DC_MOTOR;
		test_motor.value = (float)motor_value;
		test_motor.action = test_motor_s::ACTION_RUN;
		test_motor.driver_instance = 0;
		test_motor.timeout_ms = 0;

		test_motor_pub.publish(test_motor);

//		PX4_INFO("Enter speed value (0 to 1). If you enter a value outside the range, the motor will be stopped and the application will be terminated.");
//		scanf("%lf", &servo_value);
//
//		if(servo_value > 1.0 || servo_value < 0)
//		 break;

		PX4_INFO("Servo speed is %f", servo_value);
		test_servo.timestamp = hrt_absolute_time();
		test_servo.motor_number = SERVO_MOTOR;
		test_servo.value = (float)servo_value;
		test_servo.action = test_motor_s::ACTION_RUN;
		test_servo.driver_instance = 0;
		test_servo.timeout_ms = 0;

		test_motor_pub.publish(test_servo);

	}

	PX4_INFO("The motor and servo will be stopped");
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = 0.5;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;

	test_servo.timestamp = hrt_absolute_time();
	test_servo.motor_number = SERVO_MOTOR;
	test_servo.value = (float)servo_value;
	test_servo.driver_instance = 0;
	test_servo.timeout_ms = 0;


	test_motor_pub.publish(test_motor);
	test_motor_pub.publish(test_servo);
}

void experiment_setup_c(){
	test_motor_s test_motor;
	double motor_value = 0; // a number between 0 to 1
	uORB::Publication<test_motor_s> test_motor_pub(ORB_ID(test_motor));

	while(1)
	{
	PX4_INFO("Enter speed value (0 to 1). If you enter a value outside the range, the motor will be stopped and the application will be terminated.");
	scanf("%lf", &motor_value);

	if(motor_value > 1.0 || motor_value < 0)
	 break;

	PX4_INFO("Motor speed is %f", motor_value);
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = (float)motor_value;
	test_motor.action = test_motor_s::ACTION_RUN;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;
	test_motor_pub.publish(test_motor);
	}
	PX4_INFO("The motor will be stopped");
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = 0.5;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;

	test_motor_pub.publish(test_motor);
}

void project2_2(){
	test_motor_s test_motor, test_servo;
	debug_value_s debug_data;
	double motor_value = 0, servo_value = 0; // a number between 0 to 1

	int debug_handle = orb_subscribe(ORB_ID(debug_value));
	orb_set_interval(debug_handle, 500);


	uORB::Publication<test_motor_s> test_motor_pub(ORB_ID(test_motor));

	setStoppingSpeed(test_motor_pub, test_motor);

	while(1)
	{

		orb_copy(ORB_ID(debug_value), debug_handle, &debug_data);
		printf("%d",debug_data.ind);

		if((debug_data.ind & SPEED_MASK) == 1){

			motor_value = STOP_SPEED;
		}
		else if((debug_data.ind & SPEED_MASK) == 2){

			motor_value = SLOW_SPEED;
		}
		else if((debug_data.ind & SPEED_MASK) == 3){
			motor_value = FAST_SPEED;
		}
		if((debug_data.ind & DIR_MASK) == 4){
			servo_value = LEFT_DIRECTION;
		}
		else if((debug_data.ind & DIR_MASK) == 8){
			servo_value = FWD_DIRECTION;
		}
		else if((debug_data.ind & DIR_MASK) == 12){
			servo_value = RIGHT_DIRECTION;
		}


		PX4_INFO("Motor speed is %f", motor_value);
		test_motor.timestamp = hrt_absolute_time();
		test_motor.motor_number = DC_MOTOR;
		test_motor.value = (float)motor_value;
		test_motor.action = test_motor_s::ACTION_RUN;
		test_motor.driver_instance = 0;
		test_motor.timeout_ms = 0;

		test_motor_pub.publish(test_motor);

//		PX4_INFO("Enter speed value (0 to 1). If you enter a value outside the range, the motor will be stopped and the application will be terminated.");
//		scanf("%lf", &servo_value);
//
//		if(servo_value > 1.0 || servo_value < 0)
//		 break;

		PX4_INFO("Servo speed is %f", servo_value);
		test_servo.timestamp = hrt_absolute_time();
		test_servo.motor_number = SERVO_MOTOR;
		test_servo.value = (float)servo_value;
		test_servo.action = test_motor_s::ACTION_RUN;
		test_servo.driver_instance = 0;
		test_servo.timeout_ms = 0;

		test_motor_pub.publish(test_servo);

	}

	PX4_INFO("The motor and servo will be stopped");
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = 0.5;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;

	test_servo.timestamp = hrt_absolute_time();
	test_servo.motor_number = SERVO_MOTOR;
	test_servo.value = (float)servo_value;
	test_servo.driver_instance = 0;
	test_servo.timeout_ms = 0;


	test_motor_pub.publish(test_motor);
	test_motor_pub.publish(test_servo);
}


int hello_world_main(int argc, char *argv[]){
	project2_2();
	return 0;
}

