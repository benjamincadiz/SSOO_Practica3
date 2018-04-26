//
//  Reponedor.cpp
//  
//
//  Created by Benjamin Cadiz de Gracia on 25/4/18.
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

class Reponedor
{
public:
    void reponer()
    {
        std::cout << FBLU("[REPONEDOR]  REPONEDOR CREADO ") << std::endl;
        while(true)
        {
            s_reponer.lock();
        	std::cout << FBLU("[REPONEDOR]  Reponiendo.... ") << std::endl;
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
            std::cout << FBLU("[REPONEDOR]  la comida ha sido repuesta") << std::endl;
        	s_espera_reponer.unlock();
    
        }
    }
};
