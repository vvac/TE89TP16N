#define TXPIN 10
#define high 250
#define zeroL 1500
#define oneL 250
#define syncL 2750
#define pauseL 10500
/*                Sync                          TxCode                        Group  On/Off     ?       Device Pause  */
char oneOFF[] =   "2    1010101001011010010110100110010101010101011001100110    10     10      0101      0101    3";
char oneON[] =    "2    1010101001011010010110100110010101010101011001100110    10     01      0101      0101    3";
char twoOFF[] =   "2    1010101001011010010110100110010101010101011001100110    10     10      0101      0110    3";
char twoON[] =    "2    1010101001011010010110100110010101010101011001100110    10     01      0101      0110    3";
char threeOFF[] = "2    1010101001011010010110100110010101010101011001100110    10     10      0101      1001    3";
char threeON[] =  "2    1010101001011010010110100110010101010101011001100110    10     01      0101      1001    3";
char groupOFF[] = "2    1010101001011010010110100110010101010101011001100110    01     10      0101      0101    3";
char groupON[] =  "2    1010101001011010010110100110010101010101011001100110    01     01      0101      0101    3";

char txCode[] =   "1010101001011010010110100110010101010101011001100110";

void sendZero()
{
  digitalWrite(TXPIN, HIGH);
  delayMicroseconds(high);
  digitalWrite(TXPIN, LOW);
  delayMicroseconds(zeroL);
}

void sendOne()
{
  digitalWrite(TXPIN, HIGH);
  delayMicroseconds(high);
  digitalWrite(TXPIN, LOW);
  delayMicroseconds(oneL);
}

void sendSync()
{
  digitalWrite(TXPIN, HIGH);
  delayMicroseconds(high);
  digitalWrite(TXPIN, LOW);
  delayMicroseconds(syncL);
}

void sendPause()
{
  digitalWrite(TXPIN, HIGH);
  delayMicroseconds(high);
  digitalWrite(TXPIN, LOW);
  delayMicroseconds(pauseL);
}

void sendbuf(char *txt)
{
  for (char *wsk=txt; *wsk; wsk++)
  {
    if (*wsk == '0')
      sendZero();
    else if (*wsk == '1')
      sendOne();
    else if (*wsk == '2')
      sendSync();
    else if (*wsk == '3')
      sendPause();
    //else
      //Serial.println("ERROR!");
  }
}

void sig(char *txt)
{
  for (int i=0; i<6; i++)
  {
    sendbuf(txt);
  }
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available())
  {
    char in = Serial.read();
    switch (in)
    {
      case '1': sig(oneON); break;
      case '2': sig(oneOFF); break;
      case '3': sig(twoON); break;
      case '4': sig(twoOFF); break;
      case '5': sig(threeON); break;
      case '6': sig(threeOFF); break;
      case '7': sig(groupON); break;
      case '8': sig(groupOFF); break;
    }
  }
  delay(100);
}
