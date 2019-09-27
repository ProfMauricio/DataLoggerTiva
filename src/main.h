#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <RtcDS3231.h>


/**
 * Pino onde o pino de sinal do DHT22 está conectado
 * */
#define DHT_PIN PA_5

/**
 * Pino de sinal do pluviometro
 * 
 **/
#define PLUVI_PIN PB_0

/**₢
 * pino que está conectado ao SQW do RTC
 * */
#define pinoRTCSquareWave PB_1

#define CS_SDCARD PB_5


/** 
 * Buffer que guarda as medidas de umidade/temperatura lidas
 **/ 
DHT_Data bufferDHT[TAM_BUFFER];
volatile int contadorDHT = 0;

/**
 * Buffer que armazena as medidas de vento 
 **/ 
Vento_Data bufferVento[TAM_BUFFER];
volatile int contadorVento = 0;

/**
 * Buffer que guarda a quantidade de pulsos em determinado tempo
 **/ 
Pluvi_Data bufferPluviometro[TAM_BUFFER];
int contadorPluviometro = 0;

/**
 * String que armazena os instantes
 * */
char datetimeStr[20];

/**
 * Objeto que captura os dados do sensor DHT
 * */
DHT sensorHT(DHT_PIN, DHT22);

/**
 * Objeto do rtc para captura de instantes
 * */
RtcDS3231<TwoWire> rtc(Wire);

/**
 * Contador de interrupção por alarme
 * */
volatile uint64_t interruptCountAlarm = 0;
volatile bool interruptFlagAlarm = false;

/**
 * Flag que sinaliza que houve oscilação no pluviometro
 * */
volatile bool flagOscilacao = false;

/**
 * Contador de pulsos do pluviometro utilizado na interrupção para 
 * gravar dados
 * */
volatile int contadorPulsosPluviometro=0;



String bufferSerial;

/**
 * Função que verifica se existe algum alarme ativo do RTC 
 * */
bool AlarmeAtivo();

/**
 * Função de interrupção para captura do dado do pluviometro
 **/
void   CapturarOscilacao();
/**
 * Rotina que converte o instante i em uma String
 * @i instante que será convertido
 **/
String DateTime2String( RtcDateTime i );

/*************************************************************************************
 * Mostra na serial os comandos disponveis
 *************************************************************************************/
void help();

/**
 * Rotina para remover da lista de parametros enviados pela serial os dados 
 **/
String obterParametroSerial(int nroParam );

/**
 * Rotina para tratar eventos de i2c entre microcontroladores
 **/
void receberEventoI2C(int qtos);


/**
 * Rotina chamada na interrupção do RTC
 * */
void   RotinaInterrupcaoAlarm();

/**
 * Evento de chegada de dados na serial
 * */
// void IRAM_ATTR serialEvent();

/**
 * Rotina para tratar os dados que chegam na serial do microcontrolador
 **/ 
void tratarEntradaSerial();



#endif