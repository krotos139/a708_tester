
#include <UTFT.h>
#include <SD.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

//unsigned char image_update_flag[320][40];
//int iup_pix[7] = {5, 10, 20, 40, 60, 100, 170};
int iup_pix[7] = {16, 32, 64, 128, 192, 320, 520};
//float iup_a[7] = {23.0, 11.4, 5.7, 2.8, 1.9, 1.1, 0.7};
float iup_a[7] = {11.5, 6.0, 2.7, 1.5, 1.1, 0.6, 0.3};
float iup_last[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

bool ispaint(int di, float a) {
  int i;
  int iup_pix_r;
  if (a < iup_last[6]) {
    for (i = 0; i<7; i++) {
      iup_last[i] = -30.0;
    }
  }
  
  for (i = 0; i<7; i++) {
    if (di < iup_pix[i]) {
      iup_pix_r = i;
      break;
    }
  }
  //float iup_a_r = iup_a[iup_pix_r];
  if ( (a - iup_last[iup_pix_r]) > iup_a[iup_pix_r] ) {
    iup_last[iup_pix_r] = a;
    return true;
  }
  if (iup_last[iup_pix_r] == a) {
    return true;
  }
  return false;
}


uint8_t arinc[1600/8];

UTFT myGLCD(HX8357B,38,39,40,41);

void setup()
{
  
  randomSeed(analogRead(0));
  Serial.begin(9600);
  
// Setup the LCD
  Serial.println("INIT: LCD");
  myGLCD.InitLCD();
  myGLCD.InitLCD();

  Serial.println("INIT: SD");
  pinMode(53, OUTPUT);
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("INIT OK");
}

void setColor(int r, int g, int b) {
  myGLCD.setColor(255-r, 255-g, 255-b);
}
void setBackColor(int r, int g, int b) {
  myGLCD.setBackColor(255-r, 255-g, 255-b);
}

int A708_get(int i) {
  int by = i / 8;
  int bi = i % 8;
  return (arinc[by] >> bi) & 0x01;
}

float A708_angle() {
  long angle = 0;
  float angle_f;
  int i;
  for (i=51;i<=62;i++) {
    angle = (angle << 1) | A708_get(i);
  }
  angle_f = angle * 0.08;
  Serial.println(angle_f);
  return angle_f;
}

void A708_pix_cd(int i, float a) {
  int x, y;
  int data;
  data = A708_get(64 + i*3) + (A708_get(64 + i*3 + 1) << 1) + (A708_get(64 + i*3 + 2) << 2);
  if (data == 0) {
      setColor(50,50,50);
  } else if (data == 1) {
      setColor(0,255,0);
  } else if (data == 2) {
      setColor(255,255,0);
  } else if (data == 3) {
      setColor(255,0,0);
  } else if (data == 4) {
      setColor(255,0,255);
  } else if (data == 5) {
      setColor(0,255,255);
  } else if (data == 6) {
      setColor(255,255,255);
  } else if (data == 7) {
      setColor(255,255,255);
  } else {
      setColor(100,100,100);
  }
  x = cos((a/360)*6.28) * i * 160 / 512;
  y = sin((a/360)*6.28) * i * 160 / 512;
  x += 160;
  y += 160;
    myGLCD.drawPixel(x, y);

}
void A708_pix_c(int i, float a_sin, float a_cos) {
  int x, y, d;
  int data;
  data = A708_get(64 + i*3) + (A708_get(64 + i*3 + 1) << 1) + (A708_get(64 + i*3 + 2) << 2);
  if (data == 0) {
      setColor(50,50,50);
  } else if (data == 1) {
      setColor(0,255,0);
  } else if (data == 2) {
      setColor(255,255,0);
  } else if (data == 3) {
      setColor(255,0,0);
  } else if (data == 4) {
      setColor(255,0,255);
  } else if (data == 5) {
      setColor(0,255,255);
  } else if (data == 6) {
      setColor(255,255,255);
  } else if (data == 7) {
      setColor(255,255,255);
  } else {
      setColor(100,100,100);
  }
  x = a_cos * i * 150 / 512;
  y = a_sin * i * 150 / 512;
  d = i * 5 / 512;
  x += 160;
  y += 160;
  myGLCD.fillRect(x-d, y-d, x, y);
  return;
  if (d < 1) {
    myGLCD.drawPixel(x, y);
  } else {
    myGLCD.fillRect(x-d, y-d, x+d, y+d);
  }
}
int oldx, oldy;
void A708_pix_w(int i, float a_sin, float a_cos) {
  int x, y, d;
  int data;
  x = a_cos * i * 150 / 512;
  y = a_sin * i * 150 / 512;
  if ((x == oldx) && (y == oldy)) return;
  oldx = x;
  oldy = y;
  data = A708_get(64 + i*3) + (A708_get(64 + i*3 + 1) << 1) + (A708_get(64 + i*3 + 2) << 2);
  if (data == 0) {
      setColor(10,10,10);
  } else if (data == 1) {
      setColor(0,255,0);
  } else if (data == 2) {
      setColor(255,255,0);
  } else if (data == 3) {
      setColor(255,0,0);
  } else if (data == 4) {
      setColor(255,0,255);
  } else if (data == 5) {
      setColor(0,255,255);
  } else if (data == 6) {
      setColor(255,255,255);
  } else if (data == 7) {
      setColor(255,255,255);
  } else {
      setColor(100,100,100);
  }
  
  x += 160;
  y += 160;
  myGLCD.drawPixel(x, y);
}
void A708_pix_q(int i, float a) {
  long x, y;
  int data;
  data = A708_get(64 + i*3) + (A708_get(64 + i*3 + 1) << 1) + (A708_get(64 + i*3 + 2) << 2);
  if (data == 0) {
      setColor(10,10,10);
  } else if (data == 1) {
      setColor(0,255,0);
  } else if (data == 2) {
      setColor(255,255,0);
  } else if (data == 3) {
      setColor(255,0,0);
  } else if (data == 4) {
      setColor(255,0,255);
  } else if (data == 5) {
      setColor(0,255,255);
  } else if (data == 6) {
      setColor(255,255,255);
  } else if (data == 7) {
      setColor(255,255,255);
  } else {
      setColor(100,100,100);
  }
    
  x = a * 320 / 360;
  y = i;
  y = y * 319 / 512;
  
  myGLCD.drawPixel(x, y);
  myGLCD.drawPixel(x, y+1);
}

void update_arinc_spec() {
  int x,y;
  int a_label = arinc[0];
int a_control = (arinc[1] >> 0 )& 0x02;
int a_slave = (arinc[1] >> 2 )& 0x01;
int a_wa = (arinc[1] >> 3 )& 0x01;
int a_ww = (arinc[1] >> 4 )& 0x01;
int a_ta = (arinc[1] >> 5 )& 0x01;
int a_ts = (arinc[1] >> 6 )& 0x01;
int a_anticlutter = (arinc[1] >> 7 )& 0x01;
int a_sectoscan = (arinc[2] >> 0 )& 0x01;
int a_stablimit = (arinc[2] >> 1 )& 0x01;
int a_coolf = (arinc[2] >> 2 )& 0x01;
int a_dispf = (arinc[2] >> 3 )& 0x01;
int a_calibf = (arinc[2] >> 4 )& 0x01;
int a_attinf = (arinc[2] >> 5 )& 0x01;
int a_contf = (arinc[2] >> 6 )& 0x01;
int a_antf = (arinc[2] >> 7 )& 0x01;
int a_rtf = (arinc[3] >> 0 )& 0x01;
int a_stabilization = (arinc[3] >> 1 )& 0x01;
int a_mode = (arinc[3] >> 2 )& 0x07;
int a_tilt = ((arinc[3] >> 5 )& 0x03) + ((arinc[4] >> 0 )& 0x0F) << 2;
float a_tilt_f = a_stabilization * 0.25 - ((arinc[4] >> 4 )& 0x01)*16;
int a_gain = ((arinc[4] >> 5 )& 0x03) + ((arinc[5] >> 0 )& 0x0F) << 2;
a_gain *= -1;
int a_range = (arinc[5] >> 4 )& 0x07 + ((arinc[6] >> 0 )& 0x07) << 3;
a_range *= 5;
int a_weif = (arinc[6] >> 3 )& 0x01;
int a_da = (arinc[6] >> 4 )& 0x03;

setColor(0, 255, 0);
  myGLCD.setFont(SmallFont);
  x = 323;
  y = 21;
  myGLCD.printNumI(a_label, x+70, y);
  y += 15;
  myGLCD.printNumI(a_control, x+70, y);
  y += 15;
  myGLCD.printNumI(a_slave, x+70, y);
  y += 15;
  myGLCD.printNumI(a_wa, x+40, y);
    myGLCD.printNumI(a_ww, x+80+40, y);
  y += 15;
  myGLCD.printNumI(a_ta, x+70, y);
  y += 15;
  myGLCD.printNumI(a_ts, x+70, y);
  y += 15;
  myGLCD.printNumI(a_anticlutter, x+70, y);
  y += 15;
  myGLCD.printNumI(a_sectoscan, x+70, y);
  y += 15;
  myGLCD.printNumI(a_stablimit, x+70, y);
  y += 15;
  myGLCD.printNumI(a_coolf, x+40, y);
  myGLCD.printNumI(a_dispf, x+80+40, y);
  y += 15;
  myGLCD.printNumI(a_calibf, x+40, y);
  myGLCD.printNumI(a_attinf, x+80+40, y);
  y += 15;
  myGLCD.printNumI(a_contf, x+40, y);
  myGLCD.printNumI(a_antf, x+80+40, y);
  y += 15;
  myGLCD.printNumI(a_rtf, x+70, y);
  y += 15;
  myGLCD.printNumI(a_stabilization, x+70, y);
  y += 15;
  myGLCD.printNumI(a_mode, x+70, y);
  y += 15;
  myGLCD.printNumF(a_tilt_f, 2, x+70, y);
  y += 15;
  myGLCD.printNumI(a_gain, x+70, y);
  y += 15;
  myGLCD.printNumI(a_range, x+70, y);
  y += 15;
  myGLCD.printNumI(a_weif, x+70, y);
  y += 15;
  myGLCD.printNumI(a_da, x+70, y);
}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int i;
  long r;
  float a, a_cos, a_sin;
  File myFile;

// memcopy
  for (i=0;i<1600/8;i++) {
    arinc[i]=random(255);
  }

// Clear the screen and draw the frame
  myGLCD.clrScr();

  setColor(0, 0, 0);
  setBackColor(0, 0, 0);
  myGLCD.fillRect(0, 0, 479, 320);

  setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Copyright Yuri Yakolvev krotos139@gmail.com", 15, 1);
  myGLCD.print("Software: A708_tester", 15, 21);
  myGLCD.print("Version: 1.0", 15, 41);
  setColor(100, 100, 100);
  myGLCD.setFont(BigFont);
  myGLCD.print("ARINC 708 TESTER", CENTER, 150);

  delay(2000);
  myGLCD.clrScr();
  setColor(0, 0, 0);
  setBackColor(0, 0, 0);
  myGLCD.fillRect(0, 0, 319, 319);
  setColor(250, 250, 250);
  myGLCD.drawRect(320, 0, 479, 319);
  setColor(0, 0, 0);
  setBackColor(0, 0, 0);
  myGLCD.fillRect(321, 1, 478, 318);

  myGLCD.setFont(BigFont);
  setColor(0, 0, 255);
  myGLCD.print("ARINC 708", 323, 2);

  myGLCD.setFont(SmallFont);
  setColor(255, 255, 255);
  x = 323;
  y = 21;
  myGLCD.print("LABEL:", x, y);
  y += 15;
  myGLCD.print("CONTROL:", x, y);
  y += 15;
  myGLCD.print("SLAVE:", x, y);
  y += 15;
  myGLCD.print("WA:", x, y);
    myGLCD.print("WW:", x+80, y);
  y += 15;
  myGLCD.print("TA:", x, y);
  y += 15;
  myGLCD.print("TS:", x, y);
  y += 15;
  myGLCD.print("ANTICL:", x, y);
  y += 15;
  myGLCD.print("SEC SCAN:", x, y);
  y += 15;
  myGLCD.print("ST LIMIT:", x, y);
  y += 15;
  myGLCD.print("COOL F:", x, y);
  myGLCD.print("DISP F:", x+80, y);
  y += 15;
  myGLCD.print("CAL F:", x, y);
  myGLCD.print("AI F:", x+80, y);
  y += 15;
  myGLCD.print("CONT F:", x, y);
  myGLCD.print("ANT F:", x+80, y);
  y += 15;
  myGLCD.print("R/T F:", x, y);
  myGLCD.print("RAI F:", x+80, y);
  y += 15;
  myGLCD.print("STABIL:", x, y);
  y += 15;
  myGLCD.print("MODE:", x, y);
  y += 15;
  myGLCD.print("TILT:", x, y);
  y += 15;
  myGLCD.print("GAIN:", x, y);
  y += 15;
  myGLCD.print("RANGE:", x, y);
  y += 15;
  myGLCD.print("WEI F:", x, y);
  y += 15;
  myGLCD.print("DATA AC:", x, y);



  a = 0;
  r = 0;
  myFile = SD.open("packet.bin");
  for (;;) {
    break;
    r = (r + 1) % (4500);
    a = r*0.0879;

    
    myFile.seek(200*r);
    for (i=0;i<1600/8;i++) {
      arinc[i]=myFile.read();
    }
    for (i=0;i<512;) {
      A708_pix_q(i, a);
      i += 3;
      //i += (i * 30 / 512) +1;
    }
  }
  for (;;) {
    r = (r + 1) % (4096);
    a = r*0.0879;
    if ((r % 100) == 0) {
      update_arinc_spec();
    }

    
    myFile.seek(200*r);
    for (i=0;i<1600/8;i++) {
      arinc[i]=myFile.read();
    }
  
    //a = A708_angle();
    a_cos = cos(((90-a)/360)*6.28);
    a_sin = sin(((90-a)/360)*6.28);
    for (i=0;i<512;) {
//      A708_pix_c(i, a_sin, a_cos);
//      i += (i * 15 / 512) +1;
      if (ispaint(i, a)) {
        A708_pix_w(i, a_sin, a_cos);
      }
      i += 1;
    }
  }
  
  for (;;) {}
  
}

