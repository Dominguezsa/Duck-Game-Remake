#include "thread_sender.h"

ThreadSender::ThreadSender(ClientProtocol& protocol, Queue<uint8_t>& messages_to_server):
        protocol(protocol), is_alive(true), clientQueue(messages_to_server) {}


void ThreadSender::run() {
    try {
        while (is_alive) {
            uint8_t message = clientQueue.pop();
            this->protocol.send_msg(&message);
        }
    } catch (const std::exception& e) {
        return;
    }
    std::cout << "SENDER: run function ended\n";
}


void ThreadSender::stop_thread() {
    this->is_alive = false;
    clientQueue.close();
    this->join();
    std::cout << "SENDER: stop_thread function ended\n";
}


bool ThreadSender::is_still_alive() const { return is_alive; }
