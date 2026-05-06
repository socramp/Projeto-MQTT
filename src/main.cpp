/*
Autor: Marcos Sousa
Projeto: MQTT
Descrição: Pojeto 
Versão: 1.0.0
Data: 06 - 05- 26
*/
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AdaFruit_NeoPixel.h>
#include <LED.h>

#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

//*=====Constantes======
const int pinLampada = 45;
const int pinoLedRGB = 48;
const int qntsLEDs = 1;
const char TOPICO_COMANDO[] = "senai134-g6/comando";

//*=====Instancias=====
Adafruit_NeoPixel ledRGB(qntsLEDs, pinoLedRGB, NEO_GRB + NEO_KHZ800);

//*=====Prototipos=====
void alterarEstadoLampada(bool estadoLampada);
void alterarCorLedRGB(int vermelho, int verde, int azul);
void configurarLedRGB();
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratarJsonComando(const String &mensagem);

void setup()
{
  pinMode(pinLampada, OUTPUT);
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  configurarLedRGB();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
}

void alterarEstadoLampada(bool estadoLampada)
{
  digitalWrite(pinLampada, estadoLampada);
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  debugInfo("====================");
  debugInfo("Mensagem recebida na aplicação");
  debugInfo("====================");

  if (topico == nullptr)
  {
    debugErro("Topico MQTT invalido");
    return;
  }
  debugInfo("Tópico : " + String(topico));
  debugInfo("Mensagem: " + mensagem);

  if (strcmp(topico, TOPICO_COMANDO) == 0)
  {
    tratarJsonComando(mensagem);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}

void configurarLedRGB()
{
  ledRGB.begin();
  ledRGB.setBrightness(80);
  ledRGB.clear();
  ledRGB.show();

  debugInfo("LED RGB configurado no pino " + String(pinoLedRGB));
}

void alterarCorLedRGB(int vermelho, int verde, int azul)
{

  vermelho = constrain(vermelho, 0, 255);
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);

  ledRGB.setPixelColor(0, ledRGB.Color(vermelho, verde, azul));
  ledRGB.show();

  debugInfo("Cor aplicada.");
  debugInfo("R: " + String(vermelho));
  debugInfo("G: " + String(verde));
  debugInfo("B: " + String(azul));
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;

  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro ao interpretar JSON");
    debugErro(erro.c_str());
    return;
  }

  if (!doc["led"].is<JsonObject>())
  {
    debugInfo("Não encontrado o comando para o LED RGB");
  }
  else
  {
    if (!doc["led"]["r"].is<int>() || !doc["led"]["g"].is<int>() || !doc["led"]["g"].is<int>())
    {
      debugErro("Json invalido. Use led.r, led.g, led.b");
    }
    else
    {
      int vermelho = doc["led"]["r"].as<int>();
      int verde = doc["led"]["g"].as<int>();
      int azul = doc["led"]["b"].as<int>();

      alterarCorLedRGB(vermelho, verde, azul);
    }
  }

    if (!doc["lampada"].is<bool>())
    {
      debugErro("Json invalido.");
    }
    else
    {
      bool estadoLampada = doc["lampada"].as<bool>();

      alterarEstadoLampada(estadoLampada);
    }
}