#include <LiquidCrystal_I2C.h>

unsigned long CTduration;
unsigned long HTduration;
unsigned long LTduration;
unsigned long hourduration;
unsigned long minduration;

LiquidCrystal_I2C lcd(0x27,16,4); 
//SCL->A5  
//SDA->A4

void setup() {
  Serial.begin(9600);
  pinMode(11, INPUT); //From D18
  pinMode(10,INPUT);  //From D19
  pinMode(6,INPUT);   //From D21
  pinMode(5,INPUT);   //From D12
  pinMode(3,INPUT);   //From D14
}

void loop() {
  long int CTtotal=0;
  long int HTtotal=0;
  long int LTtotal=0;
  long int hourtotal=0;
  long int mintotal=0;

  for (int i=0;i<1000;i++){
    CTduration = pulseIn(11, HIGH);
    CTtotal+=CTduration;
    HTduration = pulseIn(10, HIGH);
    HTtotal+=HTduration;
    LTduration = pulseIn(6, HIGH);
    LTtotal+=LTduration;
    hourduration = pulseIn(5, HIGH);
    hourtotal+=hourduration;
    minduration = pulseIn(3, HIGH);
    mintotal+=minduration;
  }
  int CTavg = CTtotal/1000;
  int CT = CTavg/2+1;
  int HTavg = HTtotal/1000;
  int HT = HTavg/2+1;
  int LTavg = LTtotal/1000;
  int LT = LTavg/2+1;
  int houravg=hourtotal/1000;
  int hour=map(houravg,0,200,0,13);
  hour++;
  Serial.println(hour);
  int minavg=mintotal/1000;
    Serial.println(minavg);
  int minute=map(minavg,0,200,0,60);
  minute++;
  Serial.println(minute);

  Serial.print("CT: ");
  Serial.println(CT);
  Serial.print("HT: ");
  Serial.println(HT);
  Serial.print("LT: ");
  Serial.println(LT);
  Serial.print("Hour: ");
  Serial.println(hour);
  Serial.print("Min: ");
  Serial.println(minute);


  lcd.init();
  lcd.clear();  
  lcd.backlight(); 

  lcd.setCursor(0,1);   
  lcd.print("Current Temp: ");
  lcd.setCursor(16,1); 
  lcd.print(CT);
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(0,2);  
  lcd.print("High Temp: ");
  lcd.setCursor(16,2);  
  lcd.print(HT);
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(0,3);   
  lcd.print("Low Temp: ");
  lcd.setCursor(16,3);  
  lcd.print(LT);
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(0,0);   
  lcd.print("Time:");
  if(hour>=10){
    lcd.setCursor(15,0);  
  }
  else{
    lcd.setCursor(16,0);  
  }
  lcd.print(hour);
  lcd.print(":");
  if(minute<=9){
    lcd.print("0");
  }
  lcd.print(minute);

  delay(10000);
}
