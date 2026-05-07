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
#include <LiquidCrystal_I2C.h>

#include "LED.h"
#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"

//*=====Variaveis======
bool estadoLampada;

//*=====Constantes======
const int pinoLedRGB = 48;
const int qntsLEDs = 1;
const char TOPICO_COMANDO[] = "senai134-g6/comando";

//*=====Instancias=====
Adafruit_NeoPixel ledRGB(qntsLEDs, pinoLedRGB, NEO_GRB + NEO_KHZ800);
Led lampada(10);

LiquidCrystal_I2C lcd(0x27, 20, 4);

//*=====Prototipos=====
void alterarEstadoLampada();
void alterarCorLedRGB(int vermelho, int verde, int azul);
void configurarLedRGB();
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void tratarJsonComando(const String &mensagem);

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  configurarLedRGB();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 1);
  lcd.print("ESTADO DA MAQUINA:");
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
  lampada.update();

  // int valor = 10;
  // bool estado = true;

  // JsonDocument doc;

  // String mensagem;
  // doc["valor"] = valor;
  // doc["estado"] = estado;

  // serializeJson(doc, mensagem);

  // publicarMensagem(topico, mensagem);
}

void alterarEstadoLampada()
{
  debugInfo("estado lampada : " + String(estadoLampada ? "ligado" : "desligado"));

  lampada.setEstado(estadoLampada);
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

  if (!doc["EstadoMaquina"].is<int>())
  {
    debugInfo("Não encontrado o comando para o direcionar o estado da máquina. Use EstadoMaquina");
  }
  else
  {
    int estadoMaquina = doc["EstadoMaquina"].as<int>();

    lcd.setCursor(1, 2);

    if (estadoMaquina == 0)
    {
      alterarCorLedRGB(0, 0, 0);
      estadoLampada = true;
      lcd.print("Alerta emergencia  ");
    }
    else if (estadoMaquina == 1)
    {
      alterarCorLedRGB(255, 0, 0);
      estadoLampada = false;
      lcd.print("Desligado          ");
    }
    else if (estadoMaquina == 2)
    {
      alterarCorLedRGB(0, 255, 0);
      estadoLampada = false;
      lcd.print("Ligado            ");
    }
    else if (estadoMaquina == 3)
    {
      alterarCorLedRGB(255, 255, 0);
      estadoLampada = false;
      lcd.print("Manutencao        ");
    }
    else if (estadoMaquina == 4)
    {
      alterarCorLedRGB(0, 0, 255);
      estadoLampada = false;
      lcd.print("Parada total      ");
    }
    else
    {
      debugErro("Estado da máquina invalido");
      alterarCorLedRGB(0, 0, 0);
      estadoLampada = false;
      lcd.print("Comando invalido  ");
    }

    alterarEstadoLampada();
  }
}