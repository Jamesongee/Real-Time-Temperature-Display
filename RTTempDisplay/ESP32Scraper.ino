#include "WiFi.h"
#include "ESPAsyncWebServer.h"
 
const char* ssid = "Stallion Part 2.4";
const char* password =  "boognish";
const int CT_PWM = 0;
const int HT_PWM = 1; 
const int LT_PWM = 2; 
const int Hour_PWM = 3; 
const int Minute_PWM = 4;     // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 5000;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

const int CT_Output = 18;     //Pin D18
const int HT_Output = 19;     //Pin D19
const int LT_Output = 21;     //Pin D21
const int Hour_Output = 12;   //Pin D12
const int Minute_Output = 14; //Pin D14
const int DELAY_MS = 100; 
AsyncWebServer server(80);
AsyncWebSocket ws("/test");
static int ct;
static int ht;
static int lt;
static int hour;
static int minute;

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  static char current_temp[3];
  static char high_temp[3];
  static char low_temp[3];
  static char hour_inp[3];
  static char minute_inp[3];

  static int tempcount=0;
  static int check_time=0;

  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
 
  } else if(type == WS_EVT_DATA){
    if (tempcount<3){
      for(int i=0; i < len; i++) {
            if (tempcount==0){
              current_temp[i]=(char)data[i];
            }
            else if (tempcount==1){
              high_temp[i]=(char)data[i];
            }
            else if (tempcount==2){
              low_temp[i]=(char)data[i];
            }
      }
      tempcount++;
      if (tempcount==3){
        current_temp[2]='\0';
        high_temp[2]='\0';
        low_temp[2]='\0';
        sscanf(current_temp, "%d", &ct);
        sscanf(high_temp, "%d", &ht);
        sscanf(low_temp, "%d", &lt);
        Serial.println(ct);
        Serial.println(ht);
        Serial.println(lt);
      }
    }
    else{
      for(int i=0; i < len; i++) {
        if (check_time==0){
          hour_inp[i]=(char)data[i];
        }
        else{
          minute_inp[i]=(char)data[i];
        }
      }
      check_time++;
      if (check_time==2){
        hour_inp[2]='\0';
        minute_inp[2]='\0';
        sscanf(hour_inp, "%d", &hour);
        sscanf(minute_inp, "%u", &minute);
        Serial.println(hour);
        Serial.println(minute);
        check_time=0;
        tempcount=0;
      }
    }
  }
}

void setup() {
  ledcSetup(CT_PWM, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(CT_Output, CT_PWM);
  ledcSetup(HT_PWM, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(HT_Output, HT_PWM);
  ledcSetup(LT_PWM, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LT_Output, LT_PWM);
  ledcSetup(Hour_PWM, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(Hour_Output, Hour_PWM);
  ledcSetup(Minute_PWM, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(Minute_Output, Minute_PWM);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println(WiFi.localIP());
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
 
  server.begin();
}

void loop() {
  int CT_dutycycle = ct;
  CT_dutycycle = map(CT_dutycycle, 0, 100, 0, MAX_DUTY_CYCLE);
  ledcWrite(CT_PWM, CT_dutycycle);

  int HT_dutycycle = ht;
  HT_dutycycle = map(HT_dutycycle, 0, 100, 0, MAX_DUTY_CYCLE);
  ledcWrite(HT_PWM, HT_dutycycle);

  int LT_dutycycle = lt;
  LT_dutycycle = map(LT_dutycycle, 0, 100, 0, MAX_DUTY_CYCLE);
  ledcWrite(LT_PWM, LT_dutycycle);

  int Hour_dutycycle = hour;
  Hour_dutycycle = map(Hour_dutycycle, 0, 13, 0, MAX_DUTY_CYCLE);
  ledcWrite(Hour_PWM, Hour_dutycycle);

  int Minute_dutycycle = minute;
  Minute_dutycycle = map(Minute_dutycycle, 0, 59, 0, MAX_DUTY_CYCLE);
  ledcWrite(Minute_PWM, Minute_dutycycle);

  delay(DELAY_MS);
  // put your main code here, to run repeatedly:

}
