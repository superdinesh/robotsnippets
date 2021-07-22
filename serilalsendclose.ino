// *******************************************************************
//  Arduino Nano 5V example code
//********************************************************************

// ########################## DEFINES ##########################
#define HOVER_SERIAL_BAUD   115200      // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD         115200      // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME         0xABCD       // [-] Start frme definition for reliable serial communication
#define TIME_SEND           100         // [ms] Sending time interval
#define SPEED_MAX_TEST      300         // [-] Maximum speed for testing
// #define DEBUG_RX                        // [-] Debug received data. Prints all bytes to serial (comment-out to disable)

#include <SoftwareSerial.h>
SoftwareSerial HoverSerial(2,3);        // RX, TX

// Global variables
//uint8_t idx = 0;                        // Index for new data pointer
//uint16_t bufStartFrame;                 // Buffer Start Frame
//byte *p;                                // Pointer declaration for the new received data
//byte incomingByte;
//byte incomingBytePrev;

typedef struct{
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} SerialFeedback;
SerialFeedback Feedback;
//SerialFeedback NewFeedback;

// ########################## SETUP ##########################
void setup() 
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("Hoverboard Serial v1.0");

  HoverSerial.begin(HOVER_SERIAL_BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
}

// ########################## SEND ##########################
//void Send(int16_t uSteer, int16_t uSpeed)
void Send(int16_t ucmd1, int16_t ucmd2, int16_t uspeedR_meas, int16_t uspeedL_meas, int16_t ubatVoltage, int16_t uboardTemp, uint16_t ucmdLed)
{
  // Create command
  //Command.start    = (uint16_t)START_FRAME;
  //Command.steer    = (int16_t)uSteer;
  //Command.speed    = (int16_t)uSpeed;
  //Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Write to Serial
 // HoverSerial.write((uint8_t *) &Command, sizeof(Command)); 


    Feedback.start          = (uint16_t)START_FRAME;
    Feedback.cmd1           = (int16_t)ucmd1;
    Feedback.cmd2           = (int16_t)ucmd2;
    Feedback.speedR_meas    = (int16_t)uspeedR_meas;
    Feedback.speedL_meas    = (int16_t)uspeedL_meas;
    Feedback.batVoltage     = (int16_t)ubatVoltage;
    Feedback.boardTemp      = (int16_t)ubatVoltage;
    Feedback.cmdLed     = (uint16_t)ucmdLed;
    Feedback.checksum   = (uint16_t)(Feedback.start ^ Feedback.cmd1 ^ Feedback.cmd2 ^ Feedback.speedR_meas ^ Feedback.speedL_meas 
                                           ^ Feedback.batVoltage ^ Feedback.boardTemp ^ Feedback.cmdLed);
    //HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&Feedback, sizeof(Feedback));
     HoverSerial.write((uint8_t *)&Feedback, sizeof(Feedback));
}

// ########################## RECEIVE ##########################

// ########################## LOOP ##########################


void loop(void)
{ 

  Send(0, 0, 100, 200, 300,400,500); //send 7 different  values from here..

  delay(10);

}

// ########################## END ########################