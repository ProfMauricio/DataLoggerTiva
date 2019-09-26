#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <RtcDS3231.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>

#include "IntegracaoDados.h"
#include "main.h"
#include "TransmissaoInfo.h"
//#include "persistencia.h"

//#######################################################################################################

/**
 * Rotinas de tratamento de leitura da serial
 * */
void tratarEntradaSerial();

//#######################################################################################################

/**
 * Rotina responsável pela gravação de dados nas memórias não voláteis
 */
bool GravarDadosDHT()
{
  int i;
  Serial.println("Gravando dados DHT22");    
  File datFile = SD.open("/data/DHT22.csv", FILE_WRITE);
  if ( datFile ) 
  {
    for ( i = 0; i < contadorDHT; i++)
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
        Serial.println("Gravando dados DHT22");


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


bool GravarDadosPluvi( )
{
    int i;
    Serial.println("Gravando dados do pluviometro");
    File datFile = SD.open("/data/Pluvi.csv", FILE_WRITE);
    if (datFile)
    {
      for ( i = 0; i < contadorPluviometro; i++)
      {
        datFile.print("Pluvi1");
        datFile.print(csvSeparator);
        datFile.print(bufferPluviometro[i].instante);
        datFile.print(csvSeparator);
        datFile.println(bufferPluviometro[i].pulsos);
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

    contadorPluviometro = 0;
    return false;
}

  // Tratando dados do vento
 
bool GravarDadosVento( )
{ 
    int i ;
    Serial.println("Gravando dados do pluviometro");
    File datFile = SD.open("/data/VelVento.csv", FILE_WRITE);
    if (datFile)
    {
      for ( i = 0; i < contadorVento; i++)
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


/**
 * Rotina chamada na interrupção do RTC
 * */
void  RotinaInterrupcaoAlarm()
{
  interruptCountAlarm++;
  interruptFlagAlarm = true;
}

//#######################################################################################################

# define contarStr(x) (sizeof(x)/sizeof(x[0]))
/**
 * Imprime em formato legivel o instante
 * @i instate que será impresso
 **
void DateTime2Char( RtcDateTime i , char *datetimeStr)
{
  sprintf(datetimeStr,"%02u/%02u/%04u %02u:%02u:%02u",            
           i.Day(), i.Month(), i.Year(), i.Hour(), i.Minute(), i.Second());

}
*/
//#######################################################################################################

bool AlarmeAtivo()
{
  bool ativo = false;
  if ( interruptFlagAlarm)
  {
    interruptFlagAlarm = false;
   
    DS3231AlarmFlag flag = rtc.LatchAlarmsTriggeredFlags();
    if ( flag & DS3231AlarmFlag_Alarm2)
    {
      Serial.println("Alarme 2 ativado");
       ativo = true;
    }
  }
  return ativo;
}

//#######################################################################################################

void CapturarOscilacao()
{
  //bufferPluviometro[contadorPluviometro].pulsos++;
  flagOscilacao = true;
  Serial.print("#");
  
}

//#######################################################################################################

void setup() {

  int i  ;
  contadorPluviometro = 0;

  // ajustes no lcd do msp430fr6989
  delay(1000);

  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Wire.begin(1);
  
  
  Serial.print("Compilado em: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  for (i = 0; i < TAM_BUFFER; i++)
  {
      bufferPluviometro[i].pulsos = 0;
      bufferVento[i].pulsos = 0;
  }
  
  do
     Serial.println("Iniciando comunicação com mestre");
  while ( ! iniciarPortaSerialMestre());
  
  avisarInicio();
  
  // ajuste de pinos
  pinMode(pinoRTCSquareWave, INPUT);
  pinMode(PLUVI_PIN, INPUT);

  rtc.Begin();

  RtcDateTime compilador = RtcDateTime(__DATE__,__TIME__);  
  String instante;
  instante = DateTime2String(compilador);

  
  if (!rtc.IsDateTimeValid())
  {
    if ( rtc.LastError() != 0 )
    {
    Serial.println("Erro de comunicação");
    Serial.println(rtc.LastError());
    }
    else
    {
      rtc.SetAgingOffset(compilador);
    }    
  }

  // rtc.SetDateTime(compilador);
  Serial.print("Instante do relogio: ");
  instante = DateTime2String(rtc.GetDateTime());
  Serial.println(instante);
  // ajustes de pinos para interrupcao do rtc
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmTwo);

  // Ajustando o alarme 2 para disparar a cada 1 minuto
  DS3231AlarmTwo alarme2(0,0,0,DS3231AlarmTwoControl_OncePerMinute);
  rtc.SetAlarmTwo(alarme2);
  rtc.LatchAlarmsTriggeredFlags();

  // ativando as interrupções 
  attachInterrupt(digitalPinToInterrupt(pinoRTCSquareWave), RotinaInterrupcaoAlarm, FALLING);
  attachInterrupt(digitalPinToInterrupt(PLUVI_PIN), CapturarOscilacao, FALLING);
  
  Serial.println("Iniciando o sistema de coleta de dados (escravo) ");
  Serial.println("Sistema de coleta de sensores");

  // iniciando SD Card
  Serial.println(F("Verificando SD Card"));
  i = 0;
  while ( i == 0 )
  {
    if ( !SD.begin(CS_SDCARD))
    {
      while ( i < 100)
      { 
        Serial.print(".");
        delay(100);
        i++;
      }
      i = 0;
      Serial.print("Nova tentativa de ativar cartão");

    }  
    else
    {
      Serial.println("Cartão inicializado.");
      File t = SD.open("inicio.txt",FILE_WRITE);
      if ( t )
      {
        Serial.println("Arquivo inicio ok");
        t.println(instante);
        t.close();    
      }
      else
        Serial.println("Falha");
      
      i++;
    }
  }

  // Iniciando sistema de coleta de temperatura/umidade
  Serial.println(F("Iniciando sensor DHT22"));
  sensorHT.begin();
  Serial.println("Umidade :" + String(sensorHT.readHumidity()));
  Serial.println("Temperatura: "+ String(sensorHT.readTemperature()));

  }



/*************************************************************************************
 * Funcao para obter os parametros passados pela serial
 *************************************************************************************/
String obterParametroSerial(int nroParam )
{
  String tmp="";
  tmp.reserve(5);
  switch (nroParam)
  {
  case 1:
    tmp = bufferSerial.substring(6,8);
#if _DEBUG >= DEBUG_SERIAL
    Serial.print("par 1 ->");
    Serial.println(tmp.toInt());
#endif
    break;
  case 2:
    tmp = bufferSerial.substring(9,11);
#if _DEBUG >= DEBUG_SERIAL
    Serial.print("par 2 ->");
    Serial.println(tmp.toInt());
#endif
    break;
  case 3:
    tmp = bufferSerial.substring(12,16);
#if _DEBUG >= DEBUG_SERIAL
    Serial.print("par 3 ->");
    Serial.println(tmp.toInt());
#endif
    break;
  }
  return tmp;
}



/*************************************************************************************
 * Funçao para tratar os dados recebidos pela porta serial
 *************************************************************************************/
void serialEvent() {
  char inChar;
  //Serial.print("Analisando comando\n");
  while (Serial.available()) {
    // get the new byte:
    inChar = (char)Serial.read();
    // add it to the inputString:
    bufferSerial = bufferSerial + String(inChar);
    // Caractere @ inicia modo de prompt de comandos
    if ( (inChar == 10 ) || (inChar == 13) || (inChar == '#')) {
      tratarEntradaSerial();
    }
  }
}

// -----------------------------------------------------------------------------------------------------------------
/*************************************************************************************
 * Mostra na serial os comandos disponveis
 *************************************************************************************/
void help()
{
  Serial.println(F("Help de comandos"));
  Serial.println(F("@time <hh> <mm> <ss>"));
  Serial.println(F("Exemplo-> @time 10 05 30"));
  Serial.println(F("@date <dia> <mes> <ano>"));
  Serial.println(F("Exemplo-> @date 20 01 2013 "));
  Serial.println(F("@eepz Pos_eeprom"));
  Serial.println(F("Exemplo-> @sidf 0 0 "));
  Serial.println(F("@resp -> Reseta ESP"));
  Serial.println(F("@now"));
  Serial.println(F("@exit"));
}


void tratarEntradaSerial()
{
  uint8_t par1, par2, par3;
  uint16_t ano;
  RtcDateTime instante;
  String tmpMsg;
  String  instanteStr;

  // Parando timer para tratar informaçoes que estao sendo recebidas
  // -> não precisa.stop();

  // parando a interrupção
  //  MostrarInstanteAtual();


  // extraindo o tipo de comando enviado
  tmpMsg = bufferSerial.substring(0,5);
#if _DEBUG >= DEBUG_SERIAL
  Serial.println(tmpMsg);
#endif
  //testando se o usuario que ajustar o horario
  if (tmpMsg.equalsIgnoreCase("@time"))
  {
    instante = rtc.GetDateTime();
    tmpMsg = obterParametroSerial(1);
    par1 = (uint8_t) tmpMsg.toInt();
    tmpMsg = obterParametroSerial(2);
    par2 = tmpMsg.toInt();
    tmpMsg =  obterParametroSerial(3);
    par3 = (uint8_t) tmpMsg.toInt();
    RtcDateTime novoInstante;
    novoInstante =   RtcDateTime(instante.Year(),instante.Month(), instante.Day(),par1, par2, par3);
    rtc.SetDateTime(novoInstante);
    instante = rtc.GetDateTime();
    instanteStr = DateTime2String(instante);
    Serial.println(tmpMsg);
  }
  else if (tmpMsg.equalsIgnoreCase("@date"))
  {
    instante = rtc.GetDateTime();
    tmpMsg = obterParametroSerial(1);
    par2 = tmpMsg.toInt();
    tmpMsg = obterParametroSerial(2);
    par3 = tmpMsg.toInt();
    tmpMsg = obterParametroSerial(3);
    ano = tmpMsg.toInt();
    RtcDateTime novoInstante = RtcDateTime(ano, par3, par2,instante.Hour(), instante.Minute(), instante.Second());
    rtc.SetDateTime(novoInstante);
    instante = rtc.GetDateTime();
    instanteStr = DateTime2String(instante);
    Serial.println(tmpMsg);
  }
  else if ( tmpMsg.equalsIgnoreCase("@hisl")) // aviso de ativação do ESP
  {
    Serial.println("Mensagem do mestre: " + tmpMsg.substring(6));    
  }
  else if ( tmpMsg.equalsIgnoreCase("@tirq")) // requisição de instante pelo ESP
  {
    instante = rtc.GetDateTime();
    instanteStr = DateTime2String(instante);
    Serial1.print(instanteStr);
  } 
  else if (tmpMsg.equalsIgnoreCase("@help"))
    help();

  // testando se o comando para sair do modo de comandos
  else if (tmpMsg.equalsIgnoreCase("@exit"))
  {
    Serial.print(F("Saindo do modo de comando"));
  }
  else
  {
    tmpMsg = bufferSerial.substring(0,4);
    if (tmpMsg.equalsIgnoreCase("@now"))
    {
      instante = rtc.GetDateTime();
      instanteStr = DateTime2String(instante );
      Serial.println(instanteStr);
    }
    else 
       Serial.println(F("Comando nao reconhecido"));
  }
  //
  // zerando buffer a cada comando
  bufferSerial = "";
}

String DateTime2String( RtcDateTime i )
{
  char datetimeStr[20];
  sprintf(datetimeStr,"%02u/%02u/%04u %02u:%02u:%02u",            
           i.Day(), i.Month(), i.Year(), i.Hour(), i.Minute(), i.Second());
  return String(datetimeStr);
}


/**
 * Rotina principal de looP
 * */
void loop() {
  // put your main code here, to run repeatedly:
  if ( AlarmeAtivo())
  {
    RtcDateTime t = rtc.GetDateTime();
    Serial.print("Instante: ");
    bufferDHT[contadorDHT].instante = DateTime2String(t);
    Serial.println(bufferDHT[contadorDHT].instante);
    Serial.print("Umidade do ar (%): ");
    Serial.println(sensorHT.readHumidity());
    Serial.print("Temperatura (º): ");
    Serial.println(sensorHT.readTemperature()); 

    // lendo dados do DHT22
    bufferDHT[contadorDHT].umidade = sensorHT.readHumidity();
    bufferDHT[contadorDHT].temperatura = sensorHT.readTemperature();
    contadorDHT++;

    Serial.print("Contador atual : ");
    Serial.println(contadorDHT);
    if ( contadorDHT == TAM_BUFFER)
    {
      enviarDHT(bufferDHT);
      GravarDadosDHT();
    
    }
    Serial.print("Pluviometro - Instante -");
    bufferPluviometro[contadorPluviometro].instante = DateTime2String(t);    
    Serial.println(String(bufferPluviometro[contadorPluviometro].instante));
    Serial.print("Pulsos: ");
    bufferPluviometro[contadorPluviometro].pulsos = contadorPulsosPluviometro;
    contadorPulsosPluviometro = 0;
    Serial.println(bufferPluviometro[contadorPluviometro].pulsos);
    contadorPluviometro++;
    if ( contadorPluviometro == TAM_BUFFER)
    {
      enviarPluviometro(bufferPluviometro);
      GravarDadosPluvi();
    }
      
    
    contadorDHT = contadorDHT % TAM_BUFFER;
    contadorPluviometro = contadorPluviometro % TAM_BUFFER;
    contadorVento = contadorVento % TAM_BUFFER;

  }
  if ( flagOscilacao == true)
  {
 //   Serial.println("P");
 //   Serial.flush();
    contadorPulsosPluviometro++;
    flagOscilacao = false;

  } 
}

