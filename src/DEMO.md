\page journey Technical Demo Documentation
# Overview

We had at least 4 technical demonstrations (TD) for this semester, accessing different parts of the buggy. They emphasise on the hardware control especially the motors and the software solutions on interfacing the microcontroller and other components like sensors and bluetooth module.

# TD1 : Motor Control

On software side, the key task was getting the encoders and motors working as intended. After testing, I discovered that the distance scaled up with the speed of the motors, which make the distance reading unreliable. So, I settled down on **20% duty cycle** to keep it steady and avoid any residue momentum after stopping. PWM frequency was set at **1000 Hz** because the wheels were louder than the switches, why not? The heatsink was also not too hot to touch. I chose to use **unipolar mode** as this offers less switching losses.
\dot
digraph NUCLEO{
    
    label="NUCLEO F401RE";
	fontname="Arial";
	fontsize="14pt";
    node [shape=record style=filled fillcolor=gray95 fontname="Arial" fontsize="11pt"];
    
    
       
    motor[label = <{<b>motor</b>|Left PWM<br align="left"/>Right PWM<br align="left"/>enable<br align="left"/>left direction<br align="left"/>right direction<br align="left"/>|forward()<br align="left"/>|reverse()<br align="left"/> |turnright()<br align="left"/>|turnleft()<br align="left"/>}>]
    
    
    
    
    
    qei[label = <{<b>QEI</b>|getPulses()<br align="left"/>}>]
    
    
    
    left_encoder[label = <{<b>Left Encoder</b>|velocity<br align="left"/>distance<br align="left"/>| read_velocity()<br align="left"/>| read_distance()<br align="left"/>}>]
    
    right_encoder[label = <{<b>Right Encoder</b>|velocity<br align="left"/>distance<br align="left"/>| read_velocity()<br align="left"/>| read_distance()<br align="left"/>}>]
    
     
     
    edge [dir=back arrowtail=empty style=""]
    qei  -> right_encoder 
    qei  -> left_encoder 
    
  
}

\enddot





One of the tasks was to make a square of 0.5 m, which I did not know they were so strict about the square. **The square had to be in between the wheels**. 

*main.cpp*

    Motor  motor(PC_9,PB_8,PC_8,PC_6,PB_9);
    Encoder  wheel_left(PC_3,PC_2);
    Encoder  wheel_right(PB_14,PB_13);
	
	while(1){
	
		vector<double> linear {...};
		vector<double> rotation {...};
		
		for(size_t i=0; i < linear.size(); i++){
			Motor::forward(linear,....);
			Motor::turnright(rotation,...);
		}
	}

Above shows a snippet on how I implemented the square sequence. Ideally all values in linear vector are 0.5 m and rotation vector are 90° . However, our vectors looked like this

    vector<double> linear { 0.45, 0.33, 0.33, 0.43};
	vector<double> rotation {83, 83, 83, 175};
Note that the last one was more than 90° because the task was to trace the square back! Which was harder when your buggy did not move in straight line. I used bang-bang approach to compensate the tiny deviations between two wheels, implemented in the Motor::forward.

*motor.cpp*
	
	if ( left_encoder > right_encoder)

		right_motor.write(SLOW_PWM*(correction));
		left_motor.write(SLOW_PWM)

	else if ( right_encoder > left_encoder)

		left_motor.write(SLOW_PWM*(correction));
		right_motor.write(SLOW_PWM)
	

Correction is evaluated by ratio of bigger encoder number to smaller encoder number, giving percentage around 110%.

We have received full marks on TD1. This promises us a good start in our journey in producing a winning buggy.

# TD2 : Sensors

Bla bla test

