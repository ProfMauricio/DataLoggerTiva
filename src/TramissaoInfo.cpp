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

bool avisarInicio()
{
  unsigned long int inicio, tempoDecorrido ;
  bool envioOk = false;
  String ret = "";
  int tentativas = 0;
  while ( !envioOk  && tentativas < MAX_TENTATIVAS )
  {
    serialMestre->print(startMsg);
    serialMestre->print("INIT");
    serialMestre->print(endMsg);
    inicio = millis();
    tempoDecorrido = millis() - inicio;
    // aguardando a chegada de dados
    while( serialMestre->available() == 0  && tempoDecorrido < TIMEOUT )
    {
       tempoDecorrido = millis() - inicio;
       Serial.print(".");
       delay(100);
    } 
    // saiu por chegada de dados?
    if (tempoDecorrido < TIMEOUT)
    {
      ret = serialMestre->readStringUntil(endMsg);
      Serial.print("Msg: ");
      Serial.println(ret);
      if ( ret == "@OK")
        envioOk = true;
      
    }
    else 
      Serial.println("saida por timeout");
    tentativas++;
    Serial.print("Tentativa de aviso de inicio: ");
    Serial.println(tentativas);    
  }
  return envioOk;
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
