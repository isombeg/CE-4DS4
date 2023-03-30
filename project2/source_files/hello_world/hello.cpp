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

extern "C" __EXPORT int hello_world_main(int argc, char *argv[]);



void project2_2(){
	test_motor_s test_motor, test_servo;
	debug_value_s debug_data;
	double motor_value = 0, servo_value = 0; // a number between 0 to 1

	int debug_handle = orb_subscribe(ORB_ID(debug_value));
	orb_set_interval(debug_handle, 500);


	uORB::Publication<test_motor_s> test_motor_pub(ORB_ID(test_motor));


	while(1)
	{

		orb_copy(ORB_ID(debug_value), debug_handle, &debug_data);
		printf("%d",debug_data.ind);
		if((debug_data.ind & 3) == 0){

			motor_value = 0;
		}
		if((debug_data.ind & 3) == 1){

			motor_value = 0.2;
		}
		if((debug_data.ind & 3) == 2){
			motor_value = 0.4;
		}
		if((debug_data.ind & 12) == 4){
			servo_value = 0.25;
		}
		if((debug_data.ind & 12) == 8){
			servo_value = 0.5;
		}
		if((debug_data.ind & 12) == 12){
			servo_value = 0.75;
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
