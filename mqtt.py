import paho.mqtt.client as mqtt
import serial
import json

arduino = serial.Serial('COM6', 9600)

user = 'aluno'
password = '123'
endereco = 'mqtt.eclipseprojects.io'
port = 1883
topico = 'jogoFelipeAdiv'

publish = mqtt.Client()
publish.username_pw_set(user, password)
publish.connect(endereco, port)

while(True):
    if arduino.isOpen():
        temp_byte = arduino.readline()
        temp_string = temp_byte.decode('utf-8')
        publish.publish(topico, temp_string)
        print(temp_string)