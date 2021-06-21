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
/* ���� PubSubClient 2.4.0 */
#include <PubSubClient.h>
/* ���� ArduinoJson 5.13.4 */
#include <ArduinoJson.h>
#include <SoftwareSerial.h>



/* ��������WIFI SSID������ */
#define WIFI_SSID         "666"
#define WIFI_PASSWD       "12345678"


/* �豸����Ԫ����Ϣ*/
#define PRODUCT_KEY       "a15UpazgJwA"
#define DEVICE_NAME       "ZhuanYeShiJianStm32Esp8266"
#define DEVICE_SECRET     "eda3c060528a8690f7860c13650ceef6"
#define REGION_ID         "cn-shanghai"

/* ���ϻ��������Ͷ˿ںţ�����Ҫ�� */
#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY

#define CLIENT_ID         "20210514|securemode=3,timestamp=1234567890,signmethod=hmacsha1|"
// �㷨����: http://iot-face.oss-cn-shanghai.aliyuncs.com/tools.htm ���м�������password
// password�̳� https://www.yuque.com/cloud-dev/iot-tech/mebm5g
#define MQTT_PASSWD       "FCDDC0336A30A518A9BC7C0F9EF2207C77F684CE"

#define ALINK_BODY_FORMAT         "{\"id\":\"20210514\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_SUB    "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_TOPIC_PROP_POST    "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"


char trans;//���ڴ������Ϣ
bool state;//�̵���״̬
unsigned long lastMs = 0;
WiFiClient espClient;
PubSubClient  client(espClient);

void setup()
{

    Serial.begin(115200);//���ڲ���������
    Serial.println("Demo Start");
    wifiInit();
}


// the loop function runs over and over again forever
void loop()
{
    if (Serial.available())//����ͨѶ
    {
        trans = char(Serial.read());//��Ϊ�ַ��������ַ���ǿ������ת��
        if (trans == '1') state = 1;
        if (trans == '0') state = 0;
        Serial.println(trans);
    }
    if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect();
    }
    client.loop();
    mqttIntervalPost();//mqtt������Ϣ
  
}

void callback(char* topic, byte* payload, unsigned int length)//MQTT��Ϣ����
{

    StaticJsonBuffer<1024>JsonBuffer1;
    JsonObject& root1 = JsonBuffer1.parseObject(payload);
    String stm32JDQ = root1["stm32JDQ"];//json���ݽ���������Ϊ�Լ��ڰ��������õı�ʶ��
    // Serial.println(JDQswitch);
    if (stm32JDQ == "0") {
        Serial.println("0");
    }
    else if (stm32JDQ == "1") {
        Serial.println("1");
    }
}
void mqttIntervalPost()
{
    char param[32];
    char jsonBuf[128];
    sprintf(param, "{\"CO2\":%d}", state);//���ʶ��JDQstate������ǰ�Ŀ���״̬
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
        Serial.println("WiFi not Connect");
    }

    Serial.println("Connected to AP");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


    Serial.print("espClient [");


    client.setServer(MQTT_SERVER, MQTT_PORT);   /* ����WiFi֮������MQTT������ */
    client.setCallback(callback);

}
void mqttCheckConnect()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT Server ...");
        if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))

        {
            Serial.println("MQTT Connected!");
            client.subscribe(ALINK_TOPIC_PROP_SUB);

        }
        else
        {
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

