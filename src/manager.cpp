//
//  manager.cpp
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
#include "Taquilla.cpp"
#include "SistemaDePago.cpp"
#include "Reponedor.cpp"
#include "Cliente.cpp"
#include "PuntoVenta.cpp"
#include "../resources/Colors.h"

int main(){
    Taquilla * taq = new Taquilla();
    Cliente * client = new Cliente();
    SistemaDePago * sisPago = new SistemaDePago();
    PuntoVenta * pv = new PuntoVenta();
    Reponedor * repo = new Reponedor();
    
    //metodo main -- manager --
    int NUM_CLIENTES = 10;
    int PUESTOS_COMIDA = 3;
    s_manager.lock();
    s_solicitud_taquilla.lock();
    s_espera_tickets.lock();
    s_pago.lock();
    s_espera_pago.lock();
    
    
    //creamos una taquilla
    std::thread h_taquilla(&Taquilla::taquilla,taq);
    
    //creamos hilo del reponedor
    std::thread h_reponedor(&Reponedor::reponer,repo);

    
    //creamos sistema de pago
    std::thread h_spago(&SistemaDePago::sistemaPago, sisPago);
    
    //creamos los puntos de venta de comida
    for(int i = 1; i <= PUESTOS_COMIDA; i++) {
        v_puestos_comida.push_back(std::thread(&PuntoVenta::puestoComida,pv, i));
        std::this_thread::sleep_for (std::chrono::milliseconds(200));
    }
    
    //creación de clientes.
    for (int i=1; i <= NUM_CLIENTES; i++){
        cola_tickets.push(std::thread(&Cliente::cliente,client, i));
        std::this_thread::sleep_for (std::chrono::milliseconds(200));
    }
    
    //generación de turnos:
    std::unique_lock<std::mutex> lk (s_taquilla);
    
    for (int i = 1;i <= NUM_CLIENTES;i++){
        if(i>1){lk.lock();}
        std::cout << FMAG("[MANAGER] El turno es del hilo: ") << i << std::endl;
        turno = i;
        cv_tickets.notify_all();
        lk.unlock();
        s_manager.lock();
    }
    
    //espera a la finalización de hilos
    h_taquilla.join();
    
    return EXIT_SUCCESS;
}
