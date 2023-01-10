/* Grey Water pumping system
works for either submerged sump pump or inline utility pum

Use a switch to toggle these two modes
*/

int pinOUTLED =13;    // LED will turn on if there was a timeout on the last pumping cycle
int pinOUTRelayPump = 12;    // output relay to switch AC pump ON
int pinOUTBallValveMotor = 11;    // turns motor to open ball valve
int pinINModeSwitchToggle = 6;    // 2 position toggle switch to determine which mode to run in
int pinINButtonStartStop = 5;    // a momentary pusbutton to start the whole process
int pinINSwitchFloat = 7;  // FLoat switch, if turned down, CLosed means water is present at that level
int pinINBallValveClosed = 8;  /goes low when open
                      //               if turned up, Closed means water is not present at that level
  
boolean bFloatSwitchDownIsClosed = 1;  // designates whether float switch is installed facing up or down
                                  // must change to determine logic when water level drops below desired height
                                  //  if started, the pushbutton switch will stop the process instead
int iPumpOffDelaySump = 10000; //time in sec to wait between tank empty signal and actually turning off pump
//int iPumpOffDelaySump = 1000; //time in sec to wait between tank empty signal and actually turning off pump

int iPumpOffDelayUtility = 2000;
int iPurgeTime = 10000; //time to wait while drain valve is open
//int iPurgeTime = 1000; //time to wait while drain valve is open

int iCountTimeout = 1200;  //20 min timeout if pump is on too long, means something is wrong with float switch
int iPumptTime = 0;        //counter to track pump on time

int mode;
int MODE_IDLE = 0;
int MODE_PUMPING = 1;
int MODE_ABORTING = 3;
int MODE_STOPPING = 2;

//boolean bTankIsEmpty = FALSE;  // this flag will change depending on water level check
                                 
void setup()  { 
  // initialize the LED pin as an output:
  pinMode(pinOUTRelayPump, OUTPUT);  
  pinMode(pinOUTBallValveMotor, OUTPUT);  
  pinMode(pinOUTLED, OUTPUT);  
  
  // initialize the pushbutton pin as an input:
  pinMode(pinINModeSwitchToggle, INPUT); 
  pinMode(pinINButtonStartStop, INPUT); 
  pinMode(pinINSwitchFloat, INPUT);
  pinMode(pinINBallValveClosed, INPUT);
  
  digitalWrite(pinOUTBallValveMotor,1);
  digitalWrite(pinOUTRelayPump,1);
  digitalWrite(pinOUTLED,0);
  mode = MODE_IDLE;
} 

void loop() 
{
  //check for requests to change state
  if((mode==MODE_IDLE)&&(digitalRead(pinINButtonStartStop) == 1))
  {
    pump_on();
    digitalWrite(pinOUTLED,1);
    delay(3000);  //debounce switch
    mode=MODE_PUMPING;
  }
  if((mode==MODE_PUMPING)&&(digitalRead(pinINSwitchFloat)==1)&&(mode!=MODE_ABORTING))
  {
    mode=MODE_STOPPING;
    stopPumping();
    digitalWrite(pinOUTLED,0);
    
  }
  if((mode==MODE_PUMPING)&&(digitalRead(pinINButtonStartStop)==1))
  {
    mode=MODE_ABORTING;
    pump_off();
    digitalWrite(pinOUTLED,0);
    //mode=MODE_ABORTING;
  }

    //check for timeout
    
    /*
    //listen for process stop command
    if(digitalRead(pinINButtonStartStop) == 1)
    {
      digitalWrite(pinOUTSolenoidValve,0); 
      //stop requested
      abortPumping();
    }
    */
  
}


void pump_on()
{
  
  {
    if(digitalRead(pinINModeSwitchToggle) == 1) 
    {
       //sump pump mode start process
       //turn pump on
       digitalWrite(pinOUTRelayPump,0);
    } else {
       //utility pump start process
       //open valve first
       openBallValve()
       //digitalWrite(pinOUTSolenoidValve,0);
       //turn pump on
       digitalWrite(pinOUTRelayPump,0);
    }
    //start timer to check for timeout
    //iPumpTime = 0;
  }
}
void pump_off()
{
  
  {
    if(digitalRead(pinINModeSwitchToggle) == 1) 
    {
       //sump pump mode start process
       //turn pump on
       digitalWrite(pinOUTRelayPump,1);
    } else {
       //utility pump start process
       //open valve first
       BallValve(0);
       //digitalWrite(pinOUTSolenoidValve,1);
       //turn pump on
       digitalWrite(pinOUTRelayPump,1);
    }
    //start timer to check for timeout
    //iPumpTime = 0;
    mode = MODE_IDLE;
  }
}
void stopPumping()
{
  //bProcessStarted = false;
  if(digitalRead(pinINModeSwitchToggle) == 1)
  {
    //sump pump mode complete process
    // wait for level to dro a little lower
    delay(iPumpOffDelaySump);
    delay(iPumpOffDelaySump);
    delay(iPumpOffDelaySump);
    delay(iPumpOffDelaySump);
    
    //turn off pump
    digitalWrite(pinOUTRelayPump,1);
    //open drain vale
    BallValve(0);
    //digitalWrite(pinOUTSolenoidValve,0);
    //wait a long time to let drain
    delay(iPurgeTime);
    delay(iPurgeTime);
    delay(iPurgeTime);
    delay(iPurgeTime);
    delay(iPurgeTime);
    delay(iPurgeTime);
    delay(iPurgeTime);
    //add one more
    delay(iPurgeTime);
   
    
    //close valve
    //digitalWrite(pinOUTSolenoidValve,1);
````BallValve(1);
  } else {
   // utility pump mode abort process
   //sump pump mode complete process
    // wait for level to dro a little lower
    delay(iPumpOffDelayUtility);
    //turn off pump
    digitalWrite(pinOUTRelayPump,1);
    //open drain vale
    BallValve(0);
//    digitalWrite(pinOUTSolenoidValve,1);

    //close valve
    BallValve(1);
    //digitalWrite(pinOUTSolenoidValve,1);
  }
  mode = MODE_IDLE;
}

boolean tankEmpty()
{
  //if float down is closed = empty
  if(bFloatSwitchDownIsClosed){
    //float is down
    if(digitalRead(pinINSwitchFloat)==1) {
      return true;
    }
  } else {
    // float down is open = empty
    if(digitalRead(pinINSwitchFloat)==0) {
      return true;
    }
  }
}

void BallValve(int closed)
{
   //0 means open, 1 means close
  if(closed) {
      //turn motor until switch reafs open
      
  }
  
  //turn motor until switch is detected closed
 // 
}

  /*
  if(digitalRead(pinINButtonStartStop)==1)
  {
   digitalWrite(pinOUTSolenoidValve,0); 
  }
  else
  {
       digitalWrite(pinOUTSolenoidValve,1); 
  }
  */
  /*
    if(digitalRead(pinINSwitchFloat)==1)
  { 
   digitalWrite(pinOUTLED,1); 
   
  }
  else
  {
       digitalWrite(pinOUTLED,0); 
  }
  */

