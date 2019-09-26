#include "TransmissaoInfo.h"
#include "IntegracaoDados.h"
#include <Arduino.h>

HardwareSerial *serialMestre = &Serial2;

bool iniciarPortaSerialMestre()
{
    serialMestre->begin(VELOCIDADE_COMM);
    while(!serialMestre);
    return true;
}

void avisarInicio()
{
  serialMestre->print(startMsg);
  serialMestre->print("init");
  serialMestre->print(endMsg);
}

void enviarDHT(DHT_Data *bufferDHT)
{
  int i;
  Serial.println("Envio de dados do DHT para Mestre");
  for (i = 0; i < TAM_BUFFER; i++)
  {
    serialMestre->print(startMsg);
    serialMestre->print("DHT");
    serialMestre->print(separatorMsg);
    serialMestre->print(bufferDHT[i].instante);
    serialMestre->print(separatorMsg);
    serialMestre->print(bufferDHT[i].temperatura);
    serialMestre->print(separatorMsg);
    serialMestre->print(bufferDHT[i].umidade);
    serialMestre->print(endMsg);
    delay(100);    
  }
}

void enviarPluviometro(Pluvi_Data *bufferPluviometro)
{
  int i;
  Serial.println("Envio de dados do Pluviometro para Mestre");
  for (i = 0; i < TAM_BUFFER; i++)
  {
    serialMestre->print(startMsg);
    serialMestre->print("PLU");
    serialMestre->print(separatorMsg);
    serialMestre->print(bufferPluviometro[i].instante);
    serialMestre->print(separatorMsg);
    serialMestre->print(bufferPluviometro[i].pulsos);
    serialMestre->print(endMsg);
    bufferPluviometro[i].pulsos=0;
    delay(100);    
  }
}  
