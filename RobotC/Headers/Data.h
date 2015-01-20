#ifndef DATA_H
#define DATA_H
#pragma systemFile

/*Name of log file
  Note that the "txt" file extension is REQUIRED, else the file
  becomes unrecognizable to the NXT. Redownloading F/W is required
  to remove unrecognizable files. */
#define JOY_LOGFILE "JoyLog.txt"

#define TJOY_SIZE 26 //Size of tJoystick in bytes
	/* The size of tJoystick in bytes is calculated as follows:
		bool  UserMode: 		1 byte
		bool  StopPgm:			1 byte

		short joy1_x1: 			2 bytes
		short joy1_y1: 			2 bytes
		short joy1_x2: 			2 bytes
		short joy1_y2: 			2 bytes
		short joy1_Buttons: 2 bytes
		short joy1_TopHat: 	2 bytes

		short joy2_x1: 			2 bytes
		short joy2_y1: 			2 bytes
		short joy2_x2: 			2 bytes
		short joy2_y2: 			2 bytes
		short joy2_Buttons: 2 bytes
		short joy2_TopHat:  2 bytes

		total								26 bytes
	*/


//Absolute maximum for log file (in a multiple of TJOY_SIZE)
#define JOY_MAX_LOGFILE_SIZE 65520 //In bytes. Limited by max size of word.

/* Stores serialized tJoystick. */
typedef struct SerialTJoystick {
	byte data[TJOY_SIZE];
} SerialTJoystick;


TFileHandle logHandle;
TFileIOResult ioResult;
//Stores filesize of current file
short currentFilesize;
//Stores status of whether or not the file is accessible
bool accessible;
//Stores status of whether or not current log handle is in use
bool logHandleInUse;

//Keeps counter of accessed bytes
int accessedBytes = 0;

/* Opens write stream to joystick log file.
   Takes the number of tJoystick structs as parameter.
	 This deletes the old log file.
	 Returns true if writeable, false otherwise. */
bool dataOpenWriteJoyLog(int numTJoy) {
	short size = numTJoy * TJOY_SIZE;
	Delete(JOY_LOGFILE, ioResult);
	if ((ioResult == 0 || ioResult == 135) && size <= JOY_MAX_LOGFILE_SIZE) { //135 is File Not Found
		if (!logHandleInUse) {
			writeDebugStream("Opening File For Writing: %s\n", JOY_LOGFILE);
			OpenWrite(logHandle, ioResult, JOY_LOGFILE, size);
			currentFilesize = size;
			if(ioResult != 0) {
				writeDebugStream("IO Error Opening: %d\n", ioResult);
				accessible = false;
				logHandleInUse = true;
				return false;
			} else {
				writeDebugStream("File Open For Write Successful: %s\n", JOY_LOGFILE):
				accessible = true;
				logHandleInUse = true;
				return true;
			}
		} else {
			writeDebugStream("IO Error: Log Handle In Use.\n");
			accessible = false;
			logHandleInUse = true;
			return false;
		}
	} else {
			writeDebugStream("IO Error Deleting: %d\n", ioResult);
			accessible = false;
			logHandleInUse = true;
			return false;
	}
}

/* Opens read stream to joystick log file.
	 Returns true if readable, false otherwise. */
bool dataOpenReadJoyLog() {
	if (logHandleInUse) {
		writeDebugStream("IO Error: Log Handle In Use.\n");
		accessible = false;
		logHandleInUse = true;
		return false;
	} else {
		writeDebugStream("Opening File For Reading: %s\n", JOY_LOGFILE);
		OpenRead(logHandle, ioResult, JOY_LOGFILE, currentFilesize);
		if(ioResult != 0) {
			writeDebugStream("IO Error Reading: %d\n", ioResult);
			accessible = false;
			logHandleInUse = true;
			return false;
		} else {
			writeDebugStream("File Open For Reading Successful: %s\n", JOY_LOGFILE);
			accessible = true;
			logHandleInUse = true;
			return true;
		}
	}
}

/* Closes stream to joystick log file.
	 Returns true if succeeded, false otherwise.*/
bool dataCloseJoyLog() {
	writeDebugStream("Closing file: %s\n", JOY_LOGFILE);
	Close(logHandle, ioResult);
	if (ioResult != 0) {
		writeDebugStream("IO Error Closing: %d\n", ioResult);
		accessible = false;
		logHandleInUse = true;
		return false;
	} else {
		writeDebugStream("File Close Successful: %s\n", JOY_LOGFILE);
		accessible = false;
		logHandleInUse = false;
		return true;
	}
}

/* Serializes TJoystick struct, stores into SerialTJoystick struct*/
static SerialTJoystick * serializeTJoy(TJoystick *joyStruct) {
	SerialTJoystick serialized;
	byte buffer[TJOY_SIZE];
	memcpy(buffer, joyStruct, TJOY_SIZE);
	for (int i=0; i<TJOY_SIZE; i++) {
		serialized.data[i] = buffer[i];
	}
	return &serialized;
}

/* Deserializes SerialTJoystick data into TJoystick struct */
static TJoystick * deserializeTJoy(SerialTJoystick *serialized) {
	TJoystick deserialized;
	byte buffer[TJOY_SIZE];
	for (int i=0; i<TJOY_SIZE; i++) {
		buffer[i] = serialized->data[i];
	}
	memcpy(&deserialized, buffer, TJOY_SIZE);
	return &deserialized;
}

/* Writes to joystick log file stream.
	 Returns true if written, false if not. */
bool dataWriteJoyLogStream(TJoystick *joy) {
	if (accessible) {
		if ((accessedBytes + TJOY_SIZE) > currentFilesize) {
			writeDebugStream("IO Error: Attempting to write past max filesize.\n");
			return false;
		} else {
			SerialTJoystick *serialized = serializeTJoy(joy);
			for (int j = 0; j < TJOY_SIZE; j++) {
				WriteByte(logHandle, ioResult, serialized->data[j]);
				if (ioResult != 0) {
					writeDebugStream("IO Error Writing: %d\n", ioResult);
					return false;
				}
			}
			writeDebugStream("Accessed bytes: %f\n", accessedBytes);
			accessedBytes += TJOY_SIZE;
			return true;
		}
	} else {
		return false;
	}
}

/* Reads next tJoystick in log file stream. Returns pointer to tJoystick.
	 Pointer will be null if failed to read.*/
TJoystick * dataReadJoystickLog() {
	if (accessible) {
		if (accessedBytes + TJOY_SIZE > currentFilesize) {
			writeDebugStream("IO Error: Attempted to read past current filesize.\n");
			return NULL;
		} else {
			SerialTJoystick serialized;
			for (int j = 0; j < TJOY_SIZE; j++) {
				ReadByte(logHandle, ioResult, serialized.data[j]);
				if (ioResult != 0) {
					writeDebugStream("IO Error Reading: %d\n", ioResult);
					return NULL;
				}
			}
			TJoystick *curJoy = deserializeTJoy(serialized);
			return curJoy;
		}
	} else {
		return NULL;
	}
}

#endif
