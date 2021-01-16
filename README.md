# dc_motor_controller
Arduino code to control one or two 12 V DC motor using PWM signals wired to a L298N driver (two channel).
A two-axis joystick controls the movement of the motors. Arduino analog pins A0 and A1, sense the joystick axis value and controls the speed of the motors.

This controller controls only one motor at a time, that is, either A0 or A1 is being read. None both. Then, the active motor must return to its home position to allow movement of the other motor. Home position is defined by a threshold (see variables `joystick_th_fw` and `joystick_th_bw` in the code).

Tested on Arduino Nano, L298N shield and a joystick (after disassembling an old playstation remote controller). This code should also work with L293 motor shield (in that case, wiring should be reviewed).

Compiled with Arduino 1.8.10.
