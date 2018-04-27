//
//  PuntoVenta.cpp
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
#include <future>
#include "../include/MyLib.h"
#include "../include/Colors.h"

class PuntoVenta
{
public:
     void puestoComida(int id_puesto){
    Taquilla * taq = new Taquilla();
        std::cout << FRED("[PV]  Puesto de comida ") << id_puesto << FRED(" creado, esperando por clientes ...\n");
        
        std::unique_lock<std::mutex> lk (s_clientes_comida);
        
        while(puntoVent){
            cv_clientes_comida.wait(lk, []{return !cola_comida.empty();});
            s_mutex_comida.lock(); //acceso a cola de peticiones de comida en exclusión mutua
            //Se atiende petición de comida
            if(cantidad_comida.size() == 0)
            {
                s_reponer.unlock();
                s_espera_reponer.lock();
                for(int i = 0;i<MAX_ENTRADAS;i++) cantidad_comida.push(i);
            }
            MsgPeticionComida *mpc = cola_peticiones_comida.front();
            cola_peticiones_comida.pop();
            std::thread cliente = move(cola_comida.front());
            cola_comida.pop();
            cantidad_comida.pop();
            s_mutex_comida.unlock();
            
            lk.unlock();
            
            std::cout << FRED("[PV]  Puesto de venta ") << id_puesto << FRED(" atiende a cliente ") << mpc->id_cliente << FRED(" [Palomitas:") << mpc->n_palomitas << FRED(", Bebida:") << mpc->n_bebida << FRED("]\n");
            //simulamos tiempo de atención de servicio
            std::this_thread::sleep_for (std::chrono::seconds(2));
            
            //se realiza el pago
            std::cout << FRED("[PV]  Puesto de venta ") << id_puesto << FRED(" solicita pago...\n");
            s_mutex_pago.lock();
            s_pago.unlock();
            s_espera_pago.lock();
            s_mutex_pago.unlock();
            
            
            //manda notificación a cliente
            std::cout << FRED("[PV]  Puesto de venta ") << id_puesto << FRED(" Termina de atender al cliente  ") << mpc->id_cliente << "\n";
            cola_atendidos.push(move(cliente));
            mpc->b_atendido = true;
            cv_comida_atendida.notify_all();
	    if ( mpc->id_cliente == NUM_CLIENTES)
	   {
		taq->ImprimirSala();
            }else{
            lk.lock(); //se prepara semáforo para nuevo posible bloqueo con wait en siguiente iteracion
		}
        }
    }
};
