#include "server_user_client.h"

UserClient::UserClient(Queue<GameloopMessage> &gameloop_queue, Socket &&skt, uint8_t id) :
            id(id),
            skt(std::move(skt)),
            protocol(this->skt),
            receiver(gameloop_queue, this->protocol, this->id),
            sender(this->protocol) {}

bool UserClient::is_alive() {
    return receiver.is_alive() && sender.is_alive();
}

uint8_t UserClient::get_id() const {
    return this->id;
}

Queue<DuckState>* UserClient::get_queue() {
    return this->sender.get_queue();
}

void UserClient::end_communication() {
    this->receiver.stop();
    this->receiver.join();

    this->sender.stop();
    this->sender.join();

    this->protocol.end_communication();
}

