#include <iostream>
#include <chrono>
#include <thread>
// Función dummy de ejemplo
void printIterNumber(int iteration) {
    std::cout << "Hello, this is the iteration number: " << iteration << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
}

// Recibe como primer parámetro el puntero a una función a ejecutar al ritmo constante brindado por el segundo parámetro en milisegundos
void constantRateLoop(void (*func)(int), int64_t rate) {

    int iteration = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t t1_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();

    std::cout << "Starting, this should give me a loop of 1 iteration per " << rate << "ms\n";
    while (true) {
        func(iteration);

        auto t2 = std::chrono::high_resolution_clock::now();
        int64_t t2_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2.time_since_epoch()).count();
        int64_t rest = rate - (t2_ms - t1_ms);
        if (rest < 0) {
            auto behind = -rest;
            rest = rate - behind % rate;
            auto lost = behind + rest;
            t1_ms += lost;
            iteration += lost / rate;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1_ms += rate;
        iteration += 1;

    }

}

int main() {
    constantRateLoop(printIterNumber, 500);
    return 0;
}