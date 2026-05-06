//WiFiManager.cpp
#include <arduino.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
#include "secrets.h"
#include <WiFi.h>
#include "DebugManager.h"

bool WiFiEstaConectado()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
{
  debugInfo("==========================");
  debugInfo("Iniciando conexão WiFi...");
  debugInfo("==========================");

  //Configura o ESP32 como station, ou seja, ele vai se conectar a um roteador existente
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  debugInfo("Conectando");

  int tentativas =0;
  const int maxTentativa= 30;

  // Aguarda a conexão por até 30 segundos
  while(WiFi.status() != WL_CONNECTED && tentativas < maxTentativa)
  {
    delay(500);
    debugInfoSemLinha(".");
    tentativas++;
  }

  debugInfoSemLinha("\n\r");

  if(WiFi.status() == WL_CONNECTED)
  {
    debugInfo("WiFI conectado com sucesso");
    debugInfoSemLinha("[INFO] Enderço IP: ");
    debugInfoSemLinha(WiFi.localIP().toString());
    debugInfoSemLinha("\n\r");
  } 
  else
  {
    debugErro("Falha ao conectar o WiFi");
    debugErro("Verifique o SSID, senha e o sinal da rede");
  }
}

void garantirWiFiConectado()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    debugInfo("WiFi desconectado. Tentando reconectar...");
    conectarWiFi();
  }

   if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("Não foi possivel reconectar ao WiFi.");
  }
}
