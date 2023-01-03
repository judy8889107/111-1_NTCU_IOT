import paho.mqtt.client as mqtt
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    channel_ID = "1760300"
    mqtt_topic = "channels/" + channel_ID + "/subscribe"
    client.subscribe(mqtt_topic)
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

mqtt_host = "mqtt3.thingspeak.com"

write_api_key = "F9RRUQHAY5K3NNY8"
mqtt_client_id = "ER4HCSgfIhAUDA0GMiEmBgU"
mqtt_username = "ER4HCSgfIhAUDA0GMiEmBgU"
mqtt_password = "Op8uqXS4J6ivQlo+l9qlFslC"
mqtt_port = 1883 #port



client = mqtt.Client(mqtt_client_id)
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(mqtt_username, mqtt_password)
client.connect(mqtt_host, mqtt_port, 60) # connect(host, port=1883, keepalive=60, bind_address="")
client.loop_forever()
