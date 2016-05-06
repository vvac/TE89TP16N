#define high 250
#define zeroL 1500
#define oneL 250
#define syncL 2750
#define pauseL 10500
#define BUFFER 256
#define tolerance 40

volatile unsigned int DATA[132];

class Buffer
{
  static unsigned int iterator;
  unsigned int timings[BUFFER];
  
  public:
  unsigned int getCur()
  {
    return timings[iterator];
  }

  void setCur(unsigned int value)
  {
    timings[iterator] = value;
  }
  
  void goR()
  {
    iterator++;
    if (iterator >= BUFFER)
      iterator = 0;
  }

  void goL()
  {
    iterator--;
    if (iterator < 0)
      iterator = BUFFER - 1;
  }
} BUF;

unsigned int Buffer::iterator = 0;

inline bool betweenPercent(long data, long reference, int percent)
{
  int roznica = abs(data - reference);
  if (roznica <= (reference*percent/100) )
    return true;
  else
    return false;
}

unsigned int duration;
unsigned int lastTime;

void handler()
{
  unsigned long time = micros();
  duration = time - lastTime;

  BUF.goR();
  BUF.setCur(duration);

  if (betweenPercent(duration, pauseL, tolerance))
  {
    BUF.goL();
    int message_counter=0;
    while(!betweenPercent(BUF.getCur(), pauseL, tolerance))
    {
      message_counter++;
      BUF.goL();
    }
    if (message_counter == 132)
    {
      BUF.goR();
      for(int i=0; i<message_counter; i++)
      {
        DATA[i] = BUF.getCur();
        BUF.goR();
      }
      
      if (betweenPercent(DATA[0], high, tolerance) && betweenPercent(DATA[1], syncL, tolerance))
      {
        bool error = false;
        Serial.println("-=-=-=-=-=-=-=-=-=-=-\n 0 - logical false\n 1 - logical true\n 2 - sync bit\n 3 - pause bit\n 9 - error\n");
        Serial.print("Raw Data: ");
        for (int i=1; i<message_counter; i+=2)
        {
          if (betweenPercent(DATA[i-1], high, tolerance))
          {
            if (betweenPercent(DATA[i], zeroL, tolerance))
              DATA[i] = 0;
            else if (betweenPercent(DATA[i], oneL, tolerance))
              DATA[i] = 1;
            else if (betweenPercent(DATA[i], syncL, tolerance))
              DATA[i] = 2;
            else if (betweenPercent(DATA[i], pauseL, tolerance))
              DATA[i] = 3;
            else
            {
              DATA[i] = 9;
              error = true;
            }
          } else
          {
            DATA[i] = 9;
            error = true;
          }
          Serial.print(DATA[i]);
        }

        if (error)
        {
          Serial.println("\n\nTransmission ERRORS!!");
        }
        else
        {
          Serial.print("\nTX Code:   ");
          for (int i=3, y=0; y<52; i+=2, y++)
          {
            Serial.print(DATA[i]);
          }
          Serial.print("\nGroup Code:                                                    ");
          for (int i=107, y=0; y<2; i+=2, y++)
          {
            Serial.print(DATA[i]);
          }
          Serial.print("\nOn/Off Code:                                                     ");
          for (int i=111, y=0; y<2; i+=2, y++)
          {
            Serial.print(DATA[i]);
          }
          Serial.print("\n(?) Code:                                                          ");
          for (int i=115, y=0; y<4; i+=2, y++)
          {
            Serial.print(DATA[i]);
          }
          Serial.print("\nDevice nr. Code:                                                       ");
          for (int i=123, y=0; y<4; i+=2, y++)
          {
            Serial.print(DATA[i]);
          }
        }
        Serial.println("\n-=-=-=-=-=-=-=-=-=-=-\n");
      }
    }
  }
  
  lastTime = time;  
}

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  pinMode(2, INPUT);
  attachInterrupt(0, handler, CHANGE);
}

void loop()
{
  delay(100);
}

