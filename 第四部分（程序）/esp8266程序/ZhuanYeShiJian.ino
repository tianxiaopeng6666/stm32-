// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       ZhuanYeShiJian.ino
    Created:	2021/5/14 16:19:31
    Author:     DESKTOP-RG0UABM\txp
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
#include <ESP8266WiFi.h>
/* 依赖 PubSubClient 2.4.0 */
#include <PubSubClient.h>
/* 依赖 ArduinoJson 5.13.4 */
#include <ArduinoJson.h>




/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID         "666"
#define WIFI_PASSWD       "12345678"


/* 设备的三元组信息*/
#define PRODUCT_KEY       "a1BubRilOOd"
#define DEVICE_NAME       "esp8266"
#define DEVICE_SECRET     "b2e328e2680f15aa0ac23bac3e417051"
#define REGION_ID         "cn-shanghai"

/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY

#define CLIENT_ID         "20210514|securemode=3,timestamp=1234567890,signmethod=hmacsha1|"
// 算法工具: http://iot-face.oss-cn-shanghai.aliyuncs.com/tools.htm 进行加密生成password
// password教程 https://www.yuque.com/cloud-dev/iot-tech/mebm5g
#define MQTT_PASSWD       "CE409F691513C64D6062B43EEC2AA6CDADB1910F"

#define ALINK_BODY_FORMAT         "{\"id\":\"20210514\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_SUB    "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_TOPIC_PROP_POST    "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"


int a = 0;
bool state;//继电器状态
unsigned long lastMs = 0;
WiFiClient espClient;
PubSubClient  client(espClient);


void setup()
{
    Serial.begin(115200);//串口波特率设置
   // Serial.println("Demo Start");
    wifiInit();
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}


// the loop function runs over and over again forever
void loop()
{

    if (Serial.available() > 0) {   // 串口收到字符数大于零。
        a = Serial.parseInt();      // 在串口数据流中查找一个有效整数。
     //   Serial.println(a);         //打印接收到的数字
    }


    if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect();
    }
    client.loop();
    mqttIntervalPost();//mqtt发布消息
}

void callback(char* topic, byte* payload, unsigned int length)//MQTT消息订阅
{
    //  Serial.println((char*)payload);

    StaticJsonBuffer<1024>JsonBuffer;
    JsonObject& root = JsonBuffer.parseObject(payload);
    String params = root["params"];
    String stm32JDQ1 = root["stm32JDQ"];
    // Serial.println(params);
    JsonBuffer.clear();
    StaticJsonBuffer<1024>JsonBuffer1;
    JsonObject& root1 = JsonBuffer1.parseObject(params);
    String stm32JDQ = root1["stm32JDQ"];

    if (stm32JDQ == "0") {
        Serial.println("0");
        digitalWrite(2, HIGH);
    }
    else if (stm32JDQ == "1") {
        Serial.println("1");
        digitalWrite(2, LOW);
    }
    if (stm32JDQ == "2") {
        Serial.println("2");

    }
    else if (stm32JDQ == "3") {
        Serial.println("3");

    }
    if (stm32JDQ1 == "0") {
        Serial.println("0");
        digitalWrite(2, HIGH);
    }
    else if (stm32JDQ1 == "1") {
        Serial.println("1");
        digitalWrite(2, LOW);
    }
    if (stm32JDQ1 == "2") {
        Serial.println("2");

    }
    else if (stm32JDQ1 == "3") {
        Serial.println("3");

    }
}
void mqttIntervalPost()
{
    char param[32];
    char jsonBuf[128];
    //int buffer1 = (int)buffer;
    //Serial.println(buffer1);
    sprintf(param, "{\"CO2\":%d}", a);//向标识符JDQstate发布当前的开关状态
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    boolean d = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}
void wifiInit()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        //    Serial.println("WiFi not Connect");
    }
    // Serial.println("Connected to AP");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
    // Serial.print("espClient [");
    client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接WiFi之后，连接MQTT服务器 */
    client.setCallback(callback);

}
void mqttCheckConnect()
{
    while (!client.connected())
    {
        //  Serial.println("Connecting to MQTT Server ...");
        if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))

        {
            //     Serial.println("MQTT Connected!");
            client.subscribe(ALINK_TOPIC_PROP_SUB);

        }
        else
        {
            //    Serial.print("MQTT Connect err:");
            //    Serial.println(client.state());
            delay(5000);
        }
    }
}

