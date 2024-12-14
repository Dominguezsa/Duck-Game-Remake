#include "server_acceptor.h"

AcceptorThread::AcceptorThread(const std::string& servname):
        acceptor_skt(servname.c_str()), clients() {}

void AcceptorThread::stop() {
    _is_alive = false;
    this->acceptor_skt.shutdown(2);
    this->acceptor_skt.close();
    free_all_resources();
}

void AcceptorThread::accept_connection() {
    // Debug message
    std::cout << "Accepting connection\n";

    Socket peer = this->acceptor_skt.accept();
    ClientSession* client = new ClientSession(std::move(peer), this->matches);
    this->clients.push_back(client);
    client->start();
}

void AcceptorThread::check_unused_resources() {
    std::cout << "Got a problem clients\n";
    for (auto it = clients.begin(); it != clients.end();) {
        ClientSession* client = *it;
        if (!client->is_alive()) {
            client->stop();
            client->join();
            delete (*it);
            it = clients.erase(it);
        } else {
            it++;
        }
    }
    std::cout << "Clients ok. Got a problem in matches\n";
    matches.remove_finished_matches();
    std::cout << "no problem in matches \n";
}

void AcceptorThread::free_all_resources() {
    if (this->clients.empty())
        return;

    for (ClientSession* client: this->clients) {
        client->stop();
        client->join();
        delete (client);
    }
    this->clients.clear();
    matches.remove_all_matches();
}

void AcceptorThread::run() {
    try {
        while (this->_is_alive) {
            accept_connection();
            std::cout << "accept here 1\n";
            //check_unused_resources();
            std::cout << "This should appear after creating or joining 1\n";
        }
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Unexpected exception: \n");
            closelog();
        }
        this->_is_alive = false;
    }
}
