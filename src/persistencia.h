#ifndef __PERSISTENCIA_H__
#define __PERSISTENCIA_H__

#include "IntegracaoDados.h"

bool GravarDadosDHT(int controleDHT, DHT_Data * bufferDHT);
bool GravarDadosPluvi(int controlePluvi, Pluvi_Data *bufferPluvi );
bool GravarDadosVento(int controleVento, Vento_Data *bufferVento);

#endif

