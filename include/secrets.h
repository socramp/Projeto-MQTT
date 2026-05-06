#ifndef SECRETS_H
#define SECRETS_H

// configuração de WIFI

extern const char *WIFI_SSID;
extern const char *WIFI_SENHA;

//===============================//
// MQTT
//===============================//

extern const bool USAR_AWL_IOT;

extern const char *MQTT_BROKER;
extern const int MQTT_PORTA;

extern const char *MQTT_CLIENT_ID;

extern const char *MQTT_USUARIO;
extern const char *MQTT_SENHA;

extern const bool MQTT_TLS;

extern const char MQTT_CERTIFICADO_CA[];

extern const char *TOPICOS_PUBLICAR[];

extern const int TOTAL_TOPICOS_PUBLICAR;

extern const char *TOPICOS_RECEBER[];

extern const int TOTAL_TOPICOS_RECEBER;

//===============================//
// DEBUG
//===============================//

// 0 = sem mensagens
// 1 = apenas erros
// 2 = todas as mensagens
extern const int DEBUG_NIVEL_INICIAL;

// Pino usado para forçar todas as respostas
extern const int PINO_HABILITA_DEBUG_COMPLETO;

#endif