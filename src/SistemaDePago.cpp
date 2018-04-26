//
//  SistemaDePago.cpp
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

class SistemaDePago
{
public:
     void sistemaPago(){
        std::cout << FYEL("[PAGO] SISTEMA DE PAGO CREADO ...\n");
        
        while(true){
            s_pago.lock();
            std::cout << FYEL("[PAGO] SP: Recibida solicitud de pago ...\n");
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
            std::cout << FYEL("[PAGO] SP: PAGO REALIZADO ...\n");
            s_espera_pago.unlock();
        }
    }
};
