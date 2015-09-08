#include <SoftwareSerial.h>

SoftwareSerial esp8266(10, 11);
#define DEBUG true
#define TIMEOUT 2000

bool ledstate = true;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(100);
  Serial.println("start");
  

  esp8266.begin(115200);                                  
  delay(5000);
  
  sendData("AT+RST\r\n",TIMEOUT,DEBUG); // esp8266をソフトウェアリセット
  delay(5000);

  sendData("AT+CWMODE=1\r\n",TIMEOUT,DEBUG); // アクセスポイントモードに設定
  sendData("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n",TIMEOUT,DEBUG); // ローカルIPアドレスの取得
  sendData("AT+CIFSR\r\n",TIMEOUT,DEBUG); // マルチ接続設定ON 1つ以上のアクセスを受け付ける
  
 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(esp8266.available()) // check if the esp is sending a message 
  {

    
    while(esp8266.available())
    {
      // デバック用
      char c = esp8266.read(); // １文字毎に読んでシリアルに表示
      Serial.write(c);
    } 
  }
}

//データ送信関数
//文字列で受けたデータを１文字ずつesp8266に送信
String sendData(String command, const int timeout, boolean debug)
{
    Serial.println(command);
    String response = "";
    char con[256];
    char i;

    command.toCharArray(con,256);
    
    //esp8266.print(command); // send the read character to the esp8266

    for(i=0; i<command.length(); i++){
      esp8266.write(con[i]);
    }
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
