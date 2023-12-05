// =========================================================================
// Arduino UNO National Semiconductor DIGITALKER Shield
// =========================================================================

#include <stdio.h>
#include <SPI.h>

#define MAXCHARS    16

// the 74HCT595 shift registers are selected using pin D10 on the UNO
#define DT_SPI_SS 10

// DIGITALKER write pulse is on pin A1 on the UNO
#define DT_WRITE  A1

char rxCharBuff[ MAXCHARS ];
uint8_t currentROM = 0;

void setup() {
  Serial.begin( 9600 );
  Serial.println(F("\n\nNational Semiconductor Digitalker Test Program"));
  Serial.println(F("=============================================="));
  Serial.println(F("Defaulting to ROM set 0."));
  Serial.println(F("Use command R1 for ROM set 1, R2 for ROM set 2 etc."));
  Serial.println(F("To speak a word/phrase, simply type in the word or phrase number."));  

  // configure the SPI SS pin for the shift registers
  pinMode( DT_SPI_SS, OUTPUT );
  digitalWrite( DT_SPI_SS, HIGH );

  // configure the write strobe pin for the DIGITALKER chip
  pinMode( DT_WRITE, OUTPUT );
  digitalWrite( DT_WRITE, HIGH );

  SPI.begin();
  delay( 100 );
  dtSpeak( 0, 0 );
}

void loop() {
  // process any serial commands received
  waitForUserInput();
  
  if ( strlen( rxCharBuff ) > 0 ) {
    // check for a ROM switch command
    if (( rxCharBuff[0] == 'r' ) || ( rxCharBuff[0] == 'R' )) {
      rxCharBuff[0] = ' ';
      uint32_t newROM = atoi( rxCharBuff );
      if ( newROM <= 15 ) {
        currentROM = newROM;
        Serial.print(F("\nROM Bank "));
        Serial.print( currentROM );
        Serial.println(F(" is now selected."));        
      }
    }
    else
    {
      // must just be a word to speak
      uint32_t newWord = atoi( rxCharBuff );
      if ( newWord <= 255 ) {
        dtSpeak( newWord, currentROM );
        Serial.print(F("\nNow speaking word "));
        Serial.print( newWord );
      }
    }
  }
}

void dtSpeak( uint8_t wordNumber, uint8_t romNumber )
{
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite( DT_SPI_SS, LOW );
  SPI.transfer( romNumber );
  SPI.transfer( wordNumber );
  digitalWrite( DT_SPI_SS, HIGH );
  SPI.endTransaction();
  
  digitalWrite( DT_WRITE, LOW );
  delay( 1 );
  digitalWrite( DT_WRITE, HIGH );
}

// ============================================
// Returns TRUE if character is 0..9 or A..Z
// ============================================
bool isPrintableChar( char ch ) {
  if ( (ch>='0') || (ch<='9') ) return true;
  if ( (ch>='A') || (ch<='Z') ) return true;
  if ( (ch>='a') || (ch<='z') ) return true;

  return false;
}

// ============================================
// Wait for the user to type something
// Any text will be stored in rxCharBuff
// ============================================
void waitForUserInput() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  bool newData = false;

  memset(rxCharBuff, 0, MAXCHARS );
  
  while ( newData == false ) {
    if (Serial.available() > 0) {
      rc = Serial.read();

      if (rc != endMarker) {
        if ( isPrintableChar( rc ) == true ) {
          rxCharBuff[ndx++] = rc;
          if (ndx >= MAXCHARS) ndx = MAXCHARS - 1;
        }
      }
      else {
        if ( ndx > 0 ) {
          rxCharBuff[ndx] = '\0';
          ndx = 0;
          newData = true;
        }
      }
    }
  }
}
