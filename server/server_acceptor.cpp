#include "server_acceptor.h"

AcceptorThread::AcceptorThread(const char *servname) :
                acceptor_skt(servname),
                clients(),
                matches(),             
                connection_count(0) {
    // Esta partida iniciada es solo a modo de prueba, sera removida en
    // un futuro commit. (Esto ademas, usa memoria dinamica!)
    this->matches.push_back(new Match(2));
}

void AcceptorThread::stop() {
    _keep_running = false;
    _is_alive = false;
    this->acceptor_skt.shutdown(2);
    this->acceptor_skt.close();
}

void AcceptorThread::accept_connection() {
    // A modo de prueba.
    if (this->matches.empty()) {
        return;
    }
    Match* match = matches.front();
    
    if (match->can_accept_players()) {
        Socket peer = this->acceptor_skt.accept();
        uint8_t id = connection_count;
        connection_count++;

        // Esto tambien esta a modo de prueba, primero deberiamos conectar
        // al usuario al server, y mas tarde este nos indicaria a que partida
        // de las n activas (n = 1 por ahora) se quiere unir.
        Queue<GameloopMessage>* q = match->get_gameloop_queue();
        UserClient *u = new UserClient(*q, std::move(peer), id);
        this->clients.push_back(u);
        match->add_player(u->get_queue(), id);
    }
}

void AcceptorThread::check_unused_resources() {
    for (auto it = clients.begin(); it != clients.end();) {
        UserClient *client = *it;
        if (!client->is_alive()) {
            client->end_communication();

            // ------ Hardcodeado ------

            // Lo correcto seria identificar a cual partida
            // pertenece el cliente y eliminarlo de ahi.
            Match* match = matches.front();
            match->delete_player(client->get_id());
            if (match->is_finished()) {
                matches.pop_front();
                delete match;
            }
            // -------------------------

            delete (*it);
            it = clients.erase(it);
        } else {
            it++;
        }
    }
}

void AcceptorThread::free_client_resources() {
    if (this->clients.empty()) 
        return;
    
    for (UserClient* user : this->clients) {
        user->end_communication();
        // ------ Hardcodeado ------

        // Lo correcto seria identificar a cual partida
        // pertenece el cliente y eliminarlo de ahi.
        Match* match = matches.front();
        match->delete_player(user->get_id());
        delete (user);
    }
    this->clients.clear();
    matches.front()->stop_game();
    delete matches.front();
    matches.clear();
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
        free_client_resources();
        this->_is_alive = false;
    }
}
