#include <Arduino.h>
#include <SD.h>
#include "persistencia.h"



/**
 * Rotina responsável pela gravação de dados nas memórias não voláteis
 */
bool GravarDadosDHT(int controleDHT, DHT_Data *bufferDHT)
{
  int i;
  Serial.println("Gravando dados DHT22");    
  File datFile = SD.open("dadosDHT22.csv", FILE_WRITE);
  if ( datFile ) 
  {
    for ( i = 0; i < controleDHT; i++)
    {
        datFile.print("DHT22-Temp");
        datFile.print(csvSeparator);
        datFile.print(bufferDHT[i].instante);
        datFile.print(csvSeparator);
        datFile.println(bufferDHT[i].temperatura);
        datFile.print("DHT22-Umid");
        datFile.print(csvSeparator);
        datFile.print(bufferDHT[i].instante);
        datFile.print(csvSeparator);
        datFile.println(bufferDHT[i].umidade);
        datFile.flush();
        Serial.print("Gravando dados DHT22");


    }
    Serial.println("Arquivo DHT22 atualizado");      
    datFile.close();
    return true;
    }
    else 
    {
      Serial.println("Erro na abertura do arquivo de dados (DHT22)");
        return false;
    }
    return false;
}


bool GravarDadosPluvi( int controlePluvi, Pluvi_Data *bufferPluvi )
{
    int i;
    Serial.println("Gravando dados do pluviometro");
    File datFile = SD.open("dadosPluvi.csv", FILE_WRITE);
    if (datFile)
    {
      for ( i = 0; i < controlePluvi; i++)
      {
        datFile.print("Pluvi1");
        datFile.print(csvSeparator);
        datFile.print(bufferPluvi[i].instante);
        datFile.print(csvSeparator);
        datFile.println(bufferPluvi[i].pulsos);
        datFile.flush();
      }
    Serial.println("Arquivo pluviometro atualizado");
    datFile.close();
    return true;
    }
    else 
      {
          Serial.println("Erro na abertura do arquivo de dados (Pluvi)");
          return false;
      }

    controlePluvi = 0;
    return false;
}

  // Tratando dados do vento
 
bool GravarDadosVento( int controleVento , Vento_Data * bufferVento )
{ 
    int i ;
    Serial.println("Gravando dados do pluviometro");
    File datFile = SD.open("/data/dadosAnemometro.csv", FILE_WRITE);
    if (datFile)
    {
      for ( i = 0; i < controleVento; i++)
      {
        datFile.print("Anemometro;");
        datFile.print(bufferVento[i].instante);
        datFile.print(csvSeparator);
        datFile.println(bufferVento[i].pulsos);
        datFile.flush();
      }
    Serial.println("Arquivo anemometro atualizado");
    datFile.close();
    return true;
    }
    else 
    {  
        Serial.println("Erro na abertura do arquivo de dados (Anemometro)");
        return false;
    }

return false;
}
