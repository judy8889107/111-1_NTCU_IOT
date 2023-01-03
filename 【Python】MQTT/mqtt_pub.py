import paho.mqtt.client as mqtt

# https://www.cooooder.com/archives/20210303

mqtt_host = "mqtt3.thingspeak.com"
channel_ID = "1760300"
write_api_key = "F9RRUQHAY5K3NNY8"
mqtt_client_id = "ER4HCSgfIhAUDA0GMiEmBgU"
mqtt_username = "ER4HCSgfIhAUDA0GMiEmBgU"
mqtt_password = "Op8uqXS4J6ivQlo+l9qlFslC"
mqtt_port = 1883 #port
mqtt_topic = "channels/" + channel_ID + "/publish/fields/field1" #TOPIC name
payload = "100"

mqttc = mqtt.Client(mqtt_client_id)  # 构造方法 Client(client_id="", clean_session=True, userdata=None, protocol=MQTTv311, transport="tcp")
mqttc.username_pw_set(mqtt_username, mqtt_password)
mqttc.connect(mqtt_host, mqtt_port) # connect(host, port=1883, keepalive=60, bind_address="")
mqttc.publish(mqtt_topic, payload)