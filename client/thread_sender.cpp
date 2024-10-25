#include "thread_sender.h"

ThreadSender::ThreadSender(ClientProtocol& protocol, Queue<uint8_t>& messages_to_server):
        protocol(protocol),
        is_alive(true),
        messages_to_server(messages_to_server) {}


void ThreadSender::run() {
    try {
        while (is_alive) {
            uint8_t message = messages_to_server.pop();
            this->protocol.send_msg(&message);
        }
    } catch (const std::exception& e) {
        return;
    }
}


void ThreadSender::stop_thread() {
    this->is_alive = false;
    messages_to_server.close();
    this->join();
}


bool ThreadSender::is_still_alive() const { return is_alive; }
