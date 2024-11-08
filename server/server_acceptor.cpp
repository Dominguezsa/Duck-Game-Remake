#include "server_acceptor.h"

AcceptorThread::AcceptorThread(const std::string& servname):
        acceptor_skt(servname.c_str()), clients(), matches(), connection_count(0) {}

void AcceptorThread::stop() {
    _keep_running = false;
    _is_alive = false;
    this->acceptor_skt.shutdown(2);
    this->acceptor_skt.close();

    free_all_resources();
}

void AcceptorThread::accept_connection() {
    // Debug message
    std::cout << "Accepting connection\n";

    Socket peer = this->acceptor_skt.accept();
    uint8_t id = connection_count;
    ClientSession* client = new ClientSession(std::move(peer), id, this->matches);
    this->clients.push_back(client);

    client->start();
    client->run();
    connection_count++;
}

void AcceptorThread::check_unused_resources() {
    for (auto it = clients.begin(); it != clients.end();) {
        ClientSession* client = *it;
        if (!client->is_alive()) {
            client->end_communication();
            delete (*it);
            it = clients.erase(it);
        } else {
            it++;
        }
    }
    matches.remove_finished_matches();
}

void AcceptorThread::free_all_resources() {
    if (this->clients.empty())
        return;

    for (ClientSession* client: this->clients) {
        client->end_communication();
        delete (client);
    }
    this->clients.clear();
    matches.remove_all_matches();
}

void AcceptorThread::run() {
    try {
        while (this->_is_alive) {
            accept_connection();
            check_unused_resources();
        }
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Unexpected exception: \n");
        }
        free_all_resources();
        this->_is_alive = false;
    }
}
