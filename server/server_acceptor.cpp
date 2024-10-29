#include "server_acceptor.h"

AcceptorThread::AcceptorThread(const char *servname) :
                acceptor_skt(servname),
                clients(),
                matches(),             
                connection_count(0) {
    // Esta partida iniciada es solo a modo de prueba, sera removida en
    // un futuro commit. (Esto ademas, usa memoria dinamica!)
    this->matches.push_back(std::make_unique<Match>(2));
}

void AcceptorThread::stop() {
    _keep_running = false;
    _is_alive = false;
    this->acceptor_skt.shutdown(2);
    this->acceptor_skt.close();
}

void AcceptorThread::accept_connection() {
    Socket peer = this->acceptor_skt.accept();
    uint8_t id = connection_count;
    connection_count++;

    // Esto tambien esta a modo de prueba, primero deberiamos conectar
    // al usuario al server, y mas tarde este nos indicaria a que partida
    // de las n activas (n = 1 por ahora) se quiere unir.
    std::unique_ptr<Match>& match = matches.front();
    Queue<GameloopMessage>* q = match->get_gameloop_queue();
    UserClient *u = new UserClient(*q, std::move(peer), id);
    this->clients.push_back(u);
    match->add_player(u->get_queue(), id);
}
