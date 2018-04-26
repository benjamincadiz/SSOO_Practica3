//
//  Taquilla.cpp
//  
//
//  Created by Benjamin Cadiz de Gracia on 24/4/18.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <chrono>
#include <condition_variable>
#include "../resources/MyLib.h"
#include <future>
#include "../resources/Colors.h"

class Taquilla
{
public:
     void taquilla(){
        //Damos tiempo a B para su bloqueo.
        std::cout << FGRN("[TAQUILLA] Taquilla ABIERTA\n");
        while(true){
            s_solicitud_taquilla.lock();
            
            MsgPeticionTicket mpt = cola_peticiones_tickets.front();
            cola_peticiones_tickets.pop();
            
            std::cout << FGRN("[TAQUILLA]  TAQUILLA: Venta de ") << mpt.n_tickets << FGRN(" tickets a cliente ")<< mpt.id_cliente << FGRN("...\n");
            std::cout << FGRN("[TAQUILLA]  TAQUILLA: Solicitando sistema de pago\n");
            std::this_thread::sleep_for (std::chrono::seconds(1));
            
            //solicitud de pago en exclusión mutua
            s_mutex_pago.lock();
            s_pago.unlock(); //despertamos servicio de pago
            s_espera_pago.lock();//esperamos a confirmacion de pago
            s_mutex_pago.unlock(); //fin seccion critica.
            
            //cliente atendido y cambio a cola de comida
            cola_comida.push(move(cola_tickets.front()));
            cola_tickets.pop();
            
            //desbloquea a cliente que espera por tickets.
            s_espera_tickets.unlock();
            
        }
    }
};
