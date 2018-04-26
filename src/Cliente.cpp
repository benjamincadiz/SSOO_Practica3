//
//  Cliente.cpp
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
#include "../resources/MyLib.h"
#include "../resources/Colors.h"

class Cliente
{
public:
    void cliente(int id_cliente){
       Cliente cli;
        std::mutex s_fin_comida;
       
        std::cout << FCYN("[CLIENTE] Cliente ") << id_cliente << FCYN(" creado y esperando en cola para tickets ...\n");
        
        std::unique_lock<std::mutex> lk (s_taquilla);
        cv_tickets.wait(lk, [id_cliente]{return (turno == id_cliente);});
        std::cout << FCYN("[CLIENTE] Cliente ") << id_cliente << FCYN(": es mi turno para la taquilla!\n");
        lk.unlock();
        
        //genera peticion
        MsgPeticionTicket mpt;
        mpt.id_cliente = id_cliente;
        mpt.n_tickets = cli.generaNumeroAleatorio(MAX_ENTRADAS);
        
        cola_peticiones_tickets.push(mpt);
        
        s_solicitud_taquilla.unlock();
        s_espera_tickets.lock();
        
        std::cout << FCYN("[CLIENTE] Cliente ") << id_cliente << FCYN(": atendido en TAQUILLA, a la espera en cola de comida!\n");
        s_manager.unlock();
        
        
        //crea petición de comida:
        MsgPeticionComida mpc(id_cliente, cli.generaNumeroAleatorio(MAX_ENTRADAS), cli.generaNumeroAleatorio(MAX_ENTRADAS));
        cola_peticiones_comida.push(&mpc);
        
        //notificación a un puesto de venta de comida y bebida.
        cv_clientes_comida.notify_one();
        
        //espera a confirmacion de punto de venta
        std::unique_lock<std::mutex> lk_comida (s_fin_comida);
        bool *p_flag_atendido = &(mpc.b_atendido);
        cv_comida_atendida.wait(lk_comida, [p_flag_atendido]{return *p_flag_atendido;});
        
        
        std::cout << FCYN("[CLIENTE] Cliente ") << id_cliente << FCYN(" FINALIZA\n");
        
    }
    int generaNumeroAleatorio(int limite){
        return (rand()%(limite-1))+1;
    }
};
