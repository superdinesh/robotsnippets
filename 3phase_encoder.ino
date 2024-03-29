/***************************** Variables *********************************/

#define CW             1			// Assign a value to represent clock wise rotation
#define CCW           -1			// Assign a value to represent counter-clock wise rotation

bool HSU_Val = digitalRead(2);		// Set the U sensor value as boolean and read initial state
bool HSV_Val = digitalRead(3);		// Set the V sensor value as boolean and read initial state 
bool HSW_Val = digitalRead(4);		// Set the W sensor value as boolean and read initial state 

int direct = 1;				// Integer variable to store BLDC rotation direction
int pulseCount;				// Integer variable to store the pulse count

float startTime;				// Float variable to store the start time of the current interrupt 
float prevTime; 				// Float variable to store the start time of the previous interrupt 
float pulseTimeW; 			// Float variable to store the elapsed time between interrupts for hall sensor W 
float pulseTimeU; 			// Float variable to store the elapsed time between interrupts for hall sensor U 
float pulseTimeV; 			// Float variable to store the elapsed time between interrupts for hall sensor V 
float AvPulseTime; 			// Float variable to store the average elapsed time between all interrupts 

float PPM;				// Float variable to store calculated pulses per minute
float RPM; 				// Float variable to store calculated revolutions per minute

/***************************** Setup *********************************/

void setup()
{
// Set digital pins 2, 3 and 4 as inputs
  pinMode(2, INPUT);			
  pinMode(3, INPUT);			
  pinMode(4, INPUT);

// Set digital pins 2, 3 and 4 as interrupts that trigger on rising and falling edge changes. Call a function (i.e. HallSensorU) on change
  attachInterrupt(digitalPinToInterrupt(2), HallSensorU, CHANGE);      
  attachInterrupt(digitalPinToInterrupt(3), HallSensorV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), HallSensorW, CHANGE);

// Initialize the print monitor and set baud rate to 9600 
  Serial.begin(9600);
}

/*************************** Main Loop ******************************/

void loop()
{
  if ((millis() - prevTime) > 600) RPM = 0;                                     			// Zero out RPM variable if wheel is stopped

  Serial.print(HSU_Val); Serial.print(HSV_Val); Serial.println(HSW_Val);      	// Display Hall Sensor Values
  Serial.println(direct);                                                    				// Display direction of rotation
  Serial.println(pulseCount);                                                 			// Display the pulse count
  Serial.println(RPM);                            		      			// Display revolutions per minute
}

/************************ Interrupt Functions ***************************/

void HallSensorW()
{
  startTime = millis();						// Set startTime to current microcontroller elapsed time value
  HSW_Val = digitalRead(4);					// Read the current W hall sensor value
  HSV_Val = digitalRead(3);						// Read the current V (or U) hall sensor value 
  direct = (HSW_Val == HSV_Val) ? CW : CCW;			// Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * direct);				// Add 1 to the pulse count
  pulseTimeW = startTime - prevTime;				// Calculate the current time between pulses
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);	// Calculate the average time time between pulses
  PPM = (1000 / AvPulseTime) * 60;					// Calculate the pulses per min (1000 millis in 1 second)
  RPM = PPM / 90;						// Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;						// Remember the start time for the next interrupt
}

void HallSensorV()
{
  startTime = millis();
  HSV_Val = digitalRead(3);
  HSU_Val = digitalRead(2);					// Read the current U (or W) hall sensor value 
  direct = (HSV_Val == HSU_Val) ? CW : CCW;
  pulseCount = pulseCount + (1 * direct);
  pulseTimeV = startTime - prevTime;				
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);		
  PPM = (1000 / AvPulseTime) * 60;					
  RPM = PPM / 90;
  prevTime = startTime;
}

void HallSensorU()
{
  startTime = millis();
  HSU_Val = digitalRead(2);
  HSW_Val = digitalRead(4);					// Read the current W (or V) hall sensor value		
  direct = (HSU_Val == HSW_Val) ? CW : CCW;
  pulseCount = pulseCount + (1 * direct);
  pulseTimeU = startTime - prevTime;				
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);		
  PPM = (1000 / AvPulseTime) * 60;					
  RPM = PPM / 90;
  prevTime = startTime;
}
