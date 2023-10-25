import paho.mqtt.client as mqtt
import json

user = 'aluno'
endereco = 'mqtt.eclipseprojects.io'
port = 1883
topico = 'jogoFelipeAdiv'


def on_message(client, userdata, msg):
  dados = json.loads(msg.payload)
  print(f"nome: {dados['nome']} | placar: {dados['placar']}")


subscribe = mqtt.Client()
subscribe.username_pw_set(user, '123')
subscribe.connect(endereco, port)
subscribe.subscribe(topico)
subscribe.on_message = on_message
subscribe.loop_forever()