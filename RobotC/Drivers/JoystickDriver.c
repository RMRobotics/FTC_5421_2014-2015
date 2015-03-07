////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                    HiTechnic Servo/Motor Controller Device Driver  - UPDATED 1/08/2009//
//
// With the TETRIX system, the PC Controller Station sends messages over Bluetooth to the NXT containing
// current settings of a PC joystick. The joystick settings arrive using the standard NXT BLuetooth
// "message mailbox" facility.
//
// This is a short ROBOTC program to extract the joystick data from a mailbox message and format it
// into data structure that can be easily acccessed by end user programs.
//
// The driver resides in a separate file that can be simply added to any user program with a
// "#include" preprocessor directive. End users should not have to modify this program and can use
// it as is.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma systemFile

#if defined(_Target_Robot_)

	#if defined(NaturalLanguage)
		// Manually Start for Natural Language - Otherwise, ReadMsgFromPC will never let the NL program end.
	#elif (defined(NXT) || defined(EV3))
		#pragma autoStartTasks        // Automatically start this task when the main user program starts.
	#endif

#elif defined(_Target_VirtWorld_)

	#if defined(NaturalLanguage)
		// Virtual Worlds - No need to use most of this driver, so don't start the ReadMsgFromPC task.
	#elif defined(VexIQ) || defined(VEX2) || defined(NXT) || defined(EV3)
		// Virtual Worlds - No need to use most of this driver, so don't start the ReadMsgFromPC task.
	#endif

#elif defined(_Target_Emulator_)

	#if (defined(NXT) || defined(EV3))
	  #warning "This driver may not work with 'Emulator'."
	#else
	  #error "This driver is not supported on this platform."
	#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    Joystick Information Structure
//
//
// Structure containing info from Joystick.
//
// "short" variables are used to prevent conversion errors. For example, negating a byte variable with
// value -128 results in -128 because -128 does not fit in a byte!
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

task readMsgFromPC();

#if defined(_Target_Robot_)
  typedef enum TJoystickButtons
  {
  	Btn1						= 1,
  	Btn2						= 2,
  	Btn3						= 3,
  	Btn4						= 4,
  	Btn5  					= 5,
  	Btn6  					= 6,
  	Btn7    				= 7,
  	Btn8     				= 8,
  	Btn9         		= 9,
  	Btn10         	= 10,
  	Btn11         	= 11,
  	Btn12         	= 12,

  	Pov0 = 13,
    Pov1 = 14,
    Pov2 = 15,
    Pov3 = 16,
    Pov4 = 17,
    Pov5 = 18,
    Pov6 = 19,
    Pov7 = 20
  } TJoystickButtons;

#elif defined(_Target_Emulator_) || defined(_Target_VirtWorld_)
  //enum for USB Joystick Button and D-pad Values
  typedef enum TJoystickButtons
  {
    Btn1 = 0,
    Btn2 = 1,
    Btn3 = 2,
    Btn4 = 3,
    Btn5 = 4,
    Btn6 = 5,
    Btn7 = 6,
    Btn8 = 7,
    Btn9 = 8,
    Btn10 = 9,

    Pov0 = 10,
    Pov1 = 11,
    Pov2 = 12,
    Pov3 = 13,
    Pov4 = 14,
    Pov5 = 15,
    Pov6 = 16,
    Pov7 = 17
  }
  TJoystickButtons;
#endif

  //enum for USB Joystick Values
  typedef enum TJoystickAxis
  {
    JoyX1 = 0,
    JoyY1 = 1,
    JoyX2 = 2,
    JoyY2 = 3
  }
  TJoystickAxis;


#if defined(_Target_Robot_)

	typedef struct TJoystick
	{
	  bool    UserMode;          // Autonomous or Telep-Operated mode
	  bool    StopPgm;           // Stop program

	  short   joy1_x1;           // -128 to +127
	  short   joy1_y1;           // -128 to +127
	  short   joy1_x2;           // -128 to +127
	  short   joy1_y2;           // -128 to +127
	  short   joy1_Buttons;      // Bit map for 12-buttons
	  short   joy1_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".

	  short   joy2_x1;           // -128 to +127
	  short   joy2_y1;           // -128 to +127
	  short   joy2_x2;           // -128 to +127
	  short   joy2_y2;           // -128 to +127
	  short   joy2_Buttons;      // Bit map for 12-buttons
	  short   joy2_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".
	} TJoystick;

	TJoystick joystick;      // User defined variable access
	TJoystick joystickCopy;  // Internal buffer to hold the last received message from the PC. Do not use
  bool bJoystickDriverStarted = false;

#else

	TPCJoystick joystick;

#endif

#if defined(hasJoystickMessageOpcodes)
	intrinsic void getJoystickSettings(TJoystick &joystick) asm(opcdSystemFunctions, byte(sysFuncGetJoysticks), variableRefRAM(joystick));
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// "Macro" to get a non-volatile copy of the last joystick settings so
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_Target_Robot_)

	#define getJoystickSettings(joystick) if(!bJoystickDriverStarted) startTask(readMsgFromPC);	memcpy(joystick, joystickCopy, sizeof(joystick))

	short joy1Btn(TJoystickButtons btn)
	{   return ((joystick.joy1_Buttons & (1 << (btn - 1))) != 0);  }
	short joy2Btn(TJoystickButtons btn)
	{   return ((joystick.joy2_Buttons & (1 << (btn - 1))) != 0);  }
  bool joy1Pov(TJoystickButtons povDirection)
  {
    switch(povDirection)
    {
      case Pov0:     if(joystick.joy1_TopHat == 0)  return true;   break;
      case Pov1:     if(joystick.joy1_TopHat == 1)  return true;   break;
      case Pov2:     if(joystick.joy1_TopHat == 2)  return true;   break;
      case Pov3:     if(joystick.joy1_TopHat == 3)  return true;   break;
      case Pov4:     if(joystick.joy1_TopHat == 4)  return true;   break;
      case Pov5:     if(joystick.joy1_TopHat == 5)  return true;   break;
      case Pov6:     if(joystick.joy1_TopHat == 6)  return true;   break;
      case Pov7:     if(joystick.joy1_TopHat == 7)  return true;   break;
      default:       break;
    }
    return false;
  }

#else

	#define getJoystickSettings getPCJoystickSettings

	short joy1Btn(TJoystickButtons btn)
	{   return ((joystick.joy1_Buttons & (1 << btn)) != 0);  }
  bool joy1Pov(TJoystickButtons povDirection)
  {
    switch(povDirection)
    {
      case Pov0:     if(joystick.joy1_TopHat == 0)  return true;   break;
      case Pov1:     if(joystick.joy1_TopHat == 1)  return true;   break;
      case Pov2:     if(joystick.joy1_TopHat == 2)  return true;   break;
      case Pov3:     if(joystick.joy1_TopHat == 3)  return true;   break;
      case Pov4:     if(joystick.joy1_TopHat == 4)  return true;   break;
      case Pov5:     if(joystick.joy1_TopHat == 5)  return true;   break;
      case Pov6:     if(joystick.joy1_TopHat == 6)  return true;   break;
      case Pov7:     if(joystick.joy1_TopHat == 7)  return true;   break;
      default:       break;
    }
    return false;
  }

#endif


// Code Below Does Not Apply to Virtual/Emulator Robots
#if (defined(NXT) || defined(EV3)) && defined(_Target_Robot_)
const TMailboxIDs kJoystickQueueID = mailbox1;

long ntotalMessageCount = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Receive Messages Task
//
// Dedicated task that continuously polls for a Bluetooth message from the PC containing the joystick
// values. Operaton of this task is nearly transparent to the end user as the earlier "#pragma autoStartTasks"
// statement above will cause it to start running as soon as the user program is started.
//
// The task is very simple. It's an endless loop that continuously looks for a Bluetooth mailbox
// message from the PC. When one is found, it reformats and copies the contents into the internal
// "joystickCopy" buffer.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_Target_Robot_)
bool bDisconnected = false;
bool bOverrideJoystickDisabling = false;
long nNoMessageCounterLimit = 750;
long nNoMessageCounter = 0;

#if defined(NXT) //|| defined(EV3)
task readMsgFromPC()
{
  bJoystickDriverStarted = true;
  bool bMsgFound;

  TFileIOResult nBTCmdRdErrorStatus;
  const int kMaxSizeOfMessage = 18;
  sbyte tempBuffer[kMaxSizeOfMessage];

  // Initialize setting to default values in case communications with PC is broken.

  //joystickCopy.TeamColor = false;
  joystickCopy.UserMode  = false;
  joystickCopy.StopPgm   = true;

  joystickCopy.joy1_x1 = 0;
  joystickCopy.joy1_y1 = 0;
  joystickCopy.joy1_x2 = 0;
  joystickCopy.joy1_y2 = 0;
  joystickCopy.joy1_Buttons = 0;
  joystickCopy.joy1_TopHat = -1;

  joystickCopy.joy2_x1 = 0;
  joystickCopy.joy2_y1 = 0;
  joystickCopy.joy2_x2 = 0;
  joystickCopy.joy2_y2 = 0;
  joystickCopy.joy2_Buttons = 0;
  joystickCopy.joy2_TopHat = -1;

  bool bTempUserMode,bTempStopPgm;

  while (true)
  {
    // Check to see if a message is available.
    bMsgFound = false;
    bDisconnected = false;
    while (true)
    {
      //
      // There may be more than one message in the queue. We want to get to the last received
      // message and discard the earlier "stale" messages. This loop simply discards all but
      // the last message.
      //
      int nSizeOfMessage;


      nSizeOfMessage = cCmdMessageGetSize(kJoystickQueueID);

      if (nSizeOfMessage <= 0)
      {
        if (!bMsgFound)
        {
          if (nNoMessageCounter > nNoMessageCounterLimit)
          {
            hogCPU();
            if (!bOverrideJoystickDisabling)
            {
              bTempUserMode = joystickCopy.UserMode;
              bTempStopPgm = joystickCopy.StopPgm;

              memset(joystickCopy, 0, sizeof(joystickCopy));

              joystickCopy.UserMode = bTempUserMode;
              joystickCopy.StopPgm = bTempStopPgm;
              joystickCopy.joy1_TopHat = -1;
              joystickCopy.joy2_TopHat = -1;
            }
            bDisconnected = true;
            releaseCPU();
          }
          wait1Msec(4);    // Give other tasks a chance to run
          nNoMessageCounter++;
          continue;        // No message this time. Loop again
        }
        else
        {
          bDisconnected = false;
          nNoMessageCounter = 0;
          break;
        }
        //
        // No more messages available and at least one message found. This is not essential but
        // useful to ensure that we're working with the latest message. We simply discard earlier
        // messages. This is useful because there could be many messages queued and we don't
        // want to work with stale data.
        //
      }

      if (nSizeOfMessage > sizeof(tempBuffer))
        nSizeOfMessage = sizeof(tempBuffer);
      nBTCmdRdErrorStatus = cCmdMessageRead(&tempBuffer[0], nSizeOfMessage, kJoystickQueueID);
      nBTCmdRdErrorStatus = nBTCmdRdErrorStatus; //Get rid of info message
      //
      // Repeat loop until there are no more messages in the queue. We only want to process the
      // last message in the queue.
      //
      bMsgFound = true;
    }

    // Once we've reached here, a valid message is available

    hogCPU();   // grab CPU for duration of critical section

    ++ntotalMessageCount;

    joystickCopy.UserMode           = (bool)tempBuffer[1];
    joystickCopy.StopPgm            = (bool)tempBuffer[2];

    joystickCopy.joy1_x1            = tempBuffer[3];
    joystickCopy.joy1_y1            = tempBuffer[4];
    joystickCopy.joy1_x2            = tempBuffer[5];
    joystickCopy.joy1_y2            = tempBuffer[6];
    joystickCopy.joy1_Buttons       = (tempBuffer[7] & 0x00FF) | (tempBuffer[8] << 8);
    joystickCopy.joy1_TopHat        = tempBuffer[9];

    joystickCopy.joy2_x1            = tempBuffer[10];
    joystickCopy.joy2_y1            = tempBuffer[11];
    joystickCopy.joy2_x2            = tempBuffer[12];
    joystickCopy.joy2_y2            = tempBuffer[13];
    joystickCopy.joy2_Buttons       = (tempBuffer[14] & 0x00FF) | (tempBuffer[15] << 8);
    joystickCopy.joy2_TopHat        = tempBuffer[16];

    joystickCopy.joy1_y1            = -joystickCopy.joy1_y1; // Negate to "natural" position
    joystickCopy.joy1_y2            = -joystickCopy.joy1_y2; // Negate to "natural" position

    joystickCopy.joy2_y1            = -joystickCopy.joy2_y1; // Negate to "natural" position
    joystickCopy.joy2_y2            = -joystickCopy.joy2_y2; // Negate to "natural" position


    releaseCPU(); // end of critical section
  }
}
#elif defined(EV3)
task readMsgFromPC()
{
  bJoystickDriverStarted = true;
  bool bMsgFound;

  //TFileIOResult nBTCmdRdErrorStatus;
  const int kMaxSizeOfMessage = 18;
  sbyte tempBuffer[kMaxSizeOfMessage];

  // Initialize setting to default values in case communications with PC is broken.

  //joystickCopy.TeamColor = false;
  joystickCopy.UserMode  = false;
  joystickCopy.StopPgm   = true;

  joystickCopy.joy1_x1 = 0;
  joystickCopy.joy1_y1 = 0;
  joystickCopy.joy1_x2 = 0;
  joystickCopy.joy1_y2 = 0;
  joystickCopy.joy1_Buttons = 0;
  joystickCopy.joy1_TopHat = -1;

  joystickCopy.joy2_x1 = 0;
  joystickCopy.joy2_y1 = 0;
  joystickCopy.joy2_x2 = 0;
  joystickCopy.joy2_y2 = 0;
  joystickCopy.joy2_Buttons = 0;
  joystickCopy.joy2_TopHat = -1;

  bool bTempUserMode,bTempStopPgm;

  while (true)
  {
    // Check to see if a message is available.
    bMsgFound = false;
    bDisconnected = false;
    while (true)
    {
      //
      // There may be more than one message in the queue. We want to get to the last received
      // message and discard the earlier "stale" messages. This loop simply discards all but
      // the last message.
      //
      int nSizeOfMessage;


      nSizeOfMessage = getMailboxMessageSize(kJoystickQueueID);

      if (nSizeOfMessage <= 0)
      {
        if (!bMsgFound)
        {
          if (nNoMessageCounter > nNoMessageCounterLimit)
          {
            hogCPU();
            if (!bOverrideJoystickDisabling)
            {
              bTempUserMode = joystickCopy.UserMode;
              bTempStopPgm = joystickCopy.StopPgm;

              memset(joystickCopy, 0, sizeof(joystickCopy));

              joystickCopy.UserMode = bTempUserMode;
              joystickCopy.StopPgm = bTempStopPgm;
              joystickCopy.joy1_TopHat = -1;
              joystickCopy.joy2_TopHat = -1;
            }
            bDisconnected = true;
            releaseCPU();
          }
          wait1Msec(4);    // Give other tasks a chance to run
          nNoMessageCounter++;
          continue;        // No message this time. Loop again
        }
        else
        {
          bDisconnected = false;
          nNoMessageCounter = 0;
          break;
        }
        //
        // No more messages available and at least one message found. This is not essential but
        // useful to ensure that we're working with the latest message. We simply discard earlier
        // messages. This is useful because there could be many messages queued and we don't
        // want to work with stale data.
        //
      }

      if (nSizeOfMessage > sizeof(tempBuffer))
        nSizeOfMessage = sizeof(tempBuffer);
      readMailbox(0, &tempBuffer[0], nSizeOfMessage);

      // Repeat loop until there are no more messages in the queue. We only want to process the
      // last message in the queue.
      //
      bMsgFound = true;
    }

    // Once we've reached here, a valid message is available

    hogCPU();   // grab CPU for duration of critical section

    ++ntotalMessageCount;

    joystickCopy.UserMode           = (bool)tempBuffer[1];
    joystickCopy.StopPgm            = (bool)tempBuffer[2];

    joystickCopy.joy1_x1            = tempBuffer[3];
    joystickCopy.joy1_y1            = tempBuffer[4];
    joystickCopy.joy1_x2            = tempBuffer[5];
    joystickCopy.joy1_y2            = tempBuffer[6];
    joystickCopy.joy1_Buttons       = (tempBuffer[7] & 0x00FF) | (tempBuffer[8] << 8);
    joystickCopy.joy1_TopHat        = tempBuffer[9];

    joystickCopy.joy2_x1            = tempBuffer[10];
    joystickCopy.joy2_y1            = tempBuffer[11];
    joystickCopy.joy2_x2            = tempBuffer[12];
    joystickCopy.joy2_y2            = tempBuffer[13];
    joystickCopy.joy2_Buttons       = (tempBuffer[14] & 0x00FF) | (tempBuffer[15] << 8);
    joystickCopy.joy2_TopHat        = tempBuffer[16];

    joystickCopy.joy1_y1            = -joystickCopy.joy1_y1; // Negate to "natural" position
    joystickCopy.joy1_y2            = -joystickCopy.joy1_y2; // Negate to "natural" position

    joystickCopy.joy2_y1            = -joystickCopy.joy2_y1; // Negate to "natural" position
    joystickCopy.joy2_y2            = -joystickCopy.joy2_y2; // Negate to "natural" position


    releaseCPU(); // end of critical section
  }
}

#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////
//
//                                        displayDiagnostics
//
// This task will display diagnostic information about a TETRIX robot on the NXT LCD.
//
// If you want to use the LCD for your own debugging use, call the function
// "disableDiagnosticsDisplay()
//
///////////////////////////////////////////////////////////////////////////////////////////

#if defined(Tetrix) && defined(_Target_Robot_)

bool bDisplayDiagnostics = true;  // Set to false in user program to disable diagnostic display

void getUserControlProgram(string& sFileName);

void disableDiagnosticsDisplay()
{
  bDisplayDiagnostics = false;   // Disable diagnostic display
}

task displayDiagnostics()
{
  string sFileName;
  bNxtLCDStatusDisplay = true;
  getUserControlProgram(sFileName);

  while (true)
  {
    if (bDisplayDiagnostics)
    {
      displayTextLine(6, "Teleop FileName:");
      displayTextLine(7, sFileName);

      getJoystickSettings(joystick);                   //Update variables with current joystick values

      if (joystick.StopPgm)
        displayCenteredTextLine(1, "Wait for Start");
      else if (joystick.UserMode)
        displayCenteredTextLine(1, "TeleOp Running");
      else
        displayCenteredTextLine(1, "Auton Running");

      if ( externalBatteryAvg < 0)
        displayTextLine(3, "Ext Batt: OFF");       //External battery is off or not connected
      else
        displayTextLine(3, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);

      displayTextLine(4, "NXT Batt:%4.1f V", nAvgBatteryLevel / (float) 1000);   // Display NXT Battery Voltage

      displayTextLine(5, "FMS Msgs: %d", ntotalMessageCount);   // Display Count of FMS messages
    }

    wait1Msec(200);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//                                        getUserControlProgram
//
// This function returns the name of the TETRIX User Control program. It reads the file
// "FTCConfig.txt" and builds the name of the file from the contents.
//
// Note that the filename includes the ".rxe" (robot executable file) file extension.
//
///////////////////////////////////////////////////////////////////////////////////////////

string kConfigName = "FTCConfig.txt";

void getUserControlProgram(string& sFileName)
{
  byte   nParmToReadByte[2];
  short    nFileSize;
  TFileIOResult nIoResult;
  TFileHandle hFileHandle;

  sFileName = "";
  nParmToReadByte[1] = 0;
  hFileHandle = 0;
  OpenRead(hFileHandle, nIoResult, kConfigName, nFileSize);
  if (nIoResult == ioRsltSuccess)
  {
    for (int index = 0; index < nFileSize; ++index)
    {
      ReadByte(hFileHandle, nIoResult,  nParmToReadByte[0]);
      strcat(sFileName, &nParmToReadByte[0]);
    }

    //
    // Delete the ".rxe" file extension
    //
    int nFileExtPosition;

    nFileExtPosition = strlen(sFileName) - 4;
    if (nFileExtPosition > 0)
      stringDelete(sFileName, nFileExtPosition, 4);
  }
  Close(hFileHandle, nIoResult);
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    waitForStart
//
// Wait for the start of either the autonomous or tele-op phase. User program is running on the NXT
// but the phase has not yet started. The FMS (Field Management System) is continually (every 50 msec)
// sending information to the NXT. This program loops getting the latest value of joystick settings.
// When it finds that the FMS has started the  phase, it immediately returns.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void waitForStart()
{
  while (true)
  {
    getJoystickSettings(joystick);
    if (!joystick.StopPgm)
      break;
  }
  return;
}

#endif

#endif
