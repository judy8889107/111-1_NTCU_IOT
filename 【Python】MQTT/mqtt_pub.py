import paho.mqtt.client as mqtt

MQTTServerIP = "192.168.5.191"
MQTTServerPort = 1883 #port
MQTTTopicName = "IoT" #TOPIC name

mqttc = mqtt.Client("python_pub")  # mqtt.Client("ClientID")
mqttc.connect(MQTTServerIP, MQTTServerPort)
mqttc.publish(MQTTTopicName, "Hello")