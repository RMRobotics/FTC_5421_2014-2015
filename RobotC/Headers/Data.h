#ifndef DATA_H
#define DATA_H
#pragma systemFile

/*Name of log file
  Note that the "txt" file extension is REQUIRED, else the file
  becomes unrecognizable to the NXT. Redownloading F/W is required
  to remove unrecognizable files. */
#define JOY_LOGFILE "JoyLog.txt"
#define DATA_LOGFILE "DataLogFile.txt"

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

/* Translates error codes into strings, then
   copies the translated string to the given parameter. */
void dataTranslateError(string *errorName, TFileIOResult ioResult) {
	if (ioResult == ioRsltSuccess) {
		*errorName = "Success!";
	} /*else if (ioResult == ioRsltInProgress) {
		*errorName = "In Progress!";
	} else if (ioResult == ioRsltReqPIN) {
		*errorName = "ReqPIN!";
	} else if (ioResult == ioRsltNoMoreHandles) {
		*errorName = "No more handles!";
	} else if (ioResult == ioRsltNoSpace) {
		*errorName = "No space on NXT!";
	} else if (ioResult == ioRsltNoMoreFiles) {
		*errorName = "No more files on NXT!";
	} else if (ioResult == ioRsltEofExpected) {
		*errorName = "End of file expected but not found!";
	} else if (ioResult == ioRsltEndOfFile) {
		*errorName = "End of file!";
	} else if (ioResult == ioRsltNotLinearFile) {
		*errorName = "Not linear file!";
	} else if (ioResult == ioRsltFileNotFound) {
		*errorName = "File not found!";
	} else if (ioResult == ioRsltHandleAlreadyClosed) {
		*errorName = "Handle already closed!";
	} else if (ioResult == ioRsltNoLinearSpace) {
		*errorName = "No linear space on NXT!";
	} else if (ioResult == ioRsltUndefinedError) {
		*errorName = "Undefined error!";
	} else if (ioResult == ioRsltFileIsBusy) {
		*errorName = "File is busy!";
	} else if (ioResult == ioRsltNoWriteBuffers) {
		*errorName = "No write buffers!";
	} else if (ioResult == ioRsltAppendNotPossible) {
		*errorName = "Append not possible!";
	} else if (ioResult == ioRsltFileIsFull) {
		*errorName = "File is full!";
	} else if (ioResult == ioRsltFileExists) {
		*errorName = "File already exists!";
	} else if (ioResult == ioRsltModuleNotFound) {
		*errorName = "Module not found!";
	} else if (ioResult == ioRsltOutOfBoundery) {
		*errorName = "Result out of boundary!";
	} else if (ioResult == ioRsltIllegalFilename) {
		*errorName = "Illegal file name!";
	} else if (ioResult == ioRsltIllegalHandle) {
		*errorName = "Illegal handle!";
	} else if (ioResult == ioRsltBTBusy) {
		*errorName = "Bluetooth busy!";
	} else if (ioResult == ioRsltBTConnectFail) {
		*errorName = "Bluetooth connection failure!";
	} else if (ioResult == ioRsltBTTimeout) {
		*errorName = "Bluetooth timeout!";
	} else if (ioResult == ioRsltFileTxTimeout) {
		*errorName = "File Tx Timeout!";
	} else if (ioResult == ioRsltFileTxDSTExists) {
		*errorName = "File Tx DST Exists!";
	} else if (ioResult == ioRsltFileTxSRCMissing) {
		*errorName = "File Tx SRC missing!";
	} else if (ioResult == ioRsltFileTxStreamError) {
		*errorName = "File Tx stream error!";
	} else if (ioResult == ioRsltFileTxCloseError) {
		*errorName = "File Tx close error!";
	} else if (ioResult == ioRsltCommChannelBusError) {
		*errorName = "Comm channel bus error!";
	} else if (ioResult == ioRsltCommBufferFull) {
		*errorName = "Comm buffer full!";
	} else if (ioResult == ioRsltCommChannelBad) {
		*errorName = "Comm channel bad!";
	} else if (ioResult == ioRsltCommChannelNotReady) {
		*errorName = "Comm channel not ready!";
	} else if (ioResult == ERR_COMM_CHAN_INVALIDX) {
		*errorName = "Comm channel invalid DX!";
	}*/ else if (ioResult == ioRsltBadArgs) {
		*errorName = "Bad args!";
	}/* else if (ioResult == ERR_INVALID_PORT) {
		*errorName = "Invalid port!";
	} else if (ioResult == ERR_INVALID_FIELD) {
		*errorName = "Invalid field!";
	} else if (ioResult == ERR_INVALID_QUEUE) {
		*errorName = "Invalid queue!";
	} else if (ioResult == ERR_INVALID_SIZE) {
		*errorName = "Invalid size!";
	} */ else {
		*errorName = "Non translated error code!";
	}
}


/* Opens write stream to log file, using the given log handle.
	 This deletes the old log file.
	 Returns true if writeable, false otherwise. */
bool dataOpenWrite(TFileHandle *logHandle, string logFile, short sizeInBytes) {
	TFileIOResult ioResult = 0;
	Delete(logFile, ioResult);
	if (ioResult == 0 || ioResult == ioRsltFileNotFound) {
		if (ioResult == ioRsltFileNotFound) {
			writeDebugStream("Attempted to delete file %s, File Not Found\n", logFile);
		}
		writeDebugStream("Opening File For Writing: %s\n", logFile);
		OpenWrite(*logHandle, ioResult, logFile, sizeInBytes);
		if(ioResult != 0) {
			string ioError = "";
			dataTranslateError(&ioError, ioResult);
			writeDebugStream("IO Error Opening: %s\n");
			return false;
		} else {
			writeDebugStream("File Open For Write Successful: %s, %d\n", logFile, *logHandle);
			return true;
		}
	} else {
		string ioError = "";
		dataTranslateError(&ioError, ioResult);
		writeDebugStream("IO Error Deleting: %s\n", ioError);
		return false;
	}
}

/* Opens read stream to log file, using the given file handle.
	 File size is written to the sent file size parameter.
	 Returns true if readable, false otherwise. */
bool dataOpenRead(TFileHandle *logHandle, string logFile, int *fileSize) {
	TFileIOResult ioResult = 0;
	writeDebugStream("Opening File For Reading: %s\n", logFile);
	OpenRead(*logHandle, ioResult, logFile, *fileSize);
	if(ioResult != 0) {
		string ioError = "";
		dataTranslateError(&ioError, ioResult);
		writeDebugStream("IO Error Reading: %s\n", ioError);
		return false;
	} else {
		writeDebugStream("File Open For Reading Successful: %s, %d\n", logFile, *logHandle);
		return true;
	}
}

/* Closes stream to log file using given handle.
	 Returns true if succeeded, false otherwise.*/
bool dataClose(TFileHandle *logHandle) {
  TFileIOResult ioResult = 0;
	writeDebugStream("Closing file handle: %d\n", *logHandle); //TFileHandle is a short
	Close(*logHandle, ioResult);
	if (ioResult != 0) {
		string ioError = "";
		dataTranslateError(&ioError, ioResult);
		writeDebugStream("IO Error Closing: %s\n", ioError);
		return false;
	} else {
		writeDebugStream("File Handle Close Successful: %d\n", *logHandle);
		return true;
	}
}

/* Writes to log file stream using given handle.
	 Returns true if written, false if not. */
bool dataWriteBytes(TFileHandle *logHandle, byte *bytes, short numBytes) {
	TFileIOResult ioResult = 0;
	for (int i=0; i<numBytes; i++) {
		WriteByte(*logHandle, ioResult, bytes[i]);
		if (ioResult != 0) {
			string ioError = "";
			dataTranslateError(&ioError, ioResult);
			writeDebugStream("IO Error writing a byte: %s, %d\n", ioError, *logHandle);
			return false;
		}
	}
	return true;
}

/* Reads next byte in log file using given handle.
   Copies read byte into given byte parameter.
   Returns true if succeeded, false if not. */
bool dataRead(TFileHandle *logHandle, byte *dataByte) {
	TFileIOResult ioResult = 0;
	ReadByte(*logHandle, ioResult, *dataByte);
	if (ioResult != 0) {
		string ioError = "";
		dataTranslateError(&ioError, ioResult);
		writeDebugStream("IO Error reading a byte: %s, %d\n", ioError, *logHandle);
		return false;
	} else {
		return true;
	}
}

/* Reads next tJoystick in log file stream using given log handle.
   Copies tJoystick to sent tJoystick parameter.
	 Returns true if successful, false if not.*/
bool dataReadTJoystick(TFileHandle *logHandle, TJoystick *tJoystick) {
	byte buffer[TJOY_SIZE];
	for (int j = 0; j < TJOY_SIZE; j++) {
		byte nextByte = 0;
		if(dataRead(logHandle, &nextByte)) {
			buffer[j] = nextByte;
		} else {
			writeDebugStream("Failed to read tJoystick from handle: %d!\n", *logHandle);
			return false;
		}
	}
	memcpy(tJoystick, buffer, TJOY_SIZE);
	return true;
}

#endif
