#ifndef __TRANSMISSAOINFO_H__
#define __TRANSMISSAOINFO_H__

#include "IntegracaoDados.h"
#include <Arduino.h>

/**
 * Rotina para transmissao de dados obtidos do DHT e que estão armazenado em um buffer
 * @bufferDHT Vetor de registros com os dados obtidos a cada minuto
 * */
void enviarDHT(DHT_Data *bufferDHT);

/**
 * Rotina para transmissão de dados obtidos do Pluviometro (pulsos a cada quantidade de volume de chuva)
 * @bufferPluviometro Vetor de registro com os dados de chuva obtidos a cada minuto
 * */
void enviarPluviometro(Pluvi_Data *bufferPluviometro);

/**
 * Rotina para inicio de atividades do escrava
 **/
void avisarInicio();

/**
 * Rotina para iniciar a porta serial de comunicaçao com o mestre
 **/ 
bool iniciarPortaSerialMestre();

#endif