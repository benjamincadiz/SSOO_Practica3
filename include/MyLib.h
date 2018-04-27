//
//  MyLib.h
//  
//
//  Created by Benjamin Cadiz de Gracia on 24/4/18.
//

#ifndef MyLib_h
#define MyLib_h
#include <condition_variable>
#include <future>
#include <iostream>
#define FILAS 6
#define COLUMNAS 12
struct MsgPeticionTicket{
    int id_cliente;
    int n_tickets;
};

struct MsgPeticionComida{
    int id_cliente;
    int n_palomitas;
    int n_bebida;
    bool b_atendido;
    
    MsgPeticionComida(int id, int np, int nb):id_cliente(id), n_palomitas(np), n_bebida(nb){
        b_atendido = false;
    }
    
};

std::mutex s_taquilla, s_manager, s_solicitud_taquilla, s_espera_tickets, s_pago, s_espera_pago,s_reponer,s_espera_reponer;
std::mutex s_clientes_comida, s_mutex_comida, s_mutex_pago;

std::mutex s_finalizar,s_espera_finalizar;

std::condition_variable cv_tickets;
std::condition_variable cv_clientes_comida;
std::condition_variable cv_comida_atendida;
std::condition_variable cv_finalizar;

std::vector<std::thread> v_puestos_comida;
std::queue<std::thread> cola_tickets;
std::queue<std::thread> cola_comida;
std::queue<std::thread> cola_atendidos;

std::queue<MsgPeticionTicket> cola_peticiones_tickets;
std::queue<MsgPeticionComida*> cola_peticiones_comida;
std::queue<int> cantidad_comida;

int turno = -1;
int MAX_ENTRADAS = 5;
int NUM_CLIENTES = 10;



int sala[FILAS][COLUMNAS];

bool sistePago= true;
bool taqui = true;
bool puntoVent= true;
bool repone = true;



#endif /* MyLib_h */
