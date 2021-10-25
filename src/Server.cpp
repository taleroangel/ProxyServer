#include <iostream>

#include "Server.hpp"
#include "Exception.hpp"

Server::Server(Port port, uint16_t max_connections, IPaddress ipv4)
    : max_con(max_connections)
{
    // Intentar crear un socket
    try
    {
        // Crear el socket
        this->createSocket(ipv4, port);
    }
    catch (SocketException &e)
    {
        throw e;
    }
}

Server::~Server()
{
    try
    {
        this->closeSocket();
    }
    catch (SocketException &e)
    {
        perror(e.what());
    }
}

void Server::createSocket(
    IPaddress ipv4, Port port)
{
    // Create a new socket
    this->network_socket =
        socket(AF_INET, SOCK_STREAM, 0);
    /* IPv4 data en Stream a través de TCP */

    // Verificar correcta inicialización
    if (this->network_socket == -1)
        throw SocketException("Socket open");

    // Añadir información de IP y Puerto al Socket
    struct sockaddr_in net_address;
    net_address.sin_family = AF_INET;   // Socket is IPv4
    net_address.sin_port = htons(port); // Port Number

    if (ipv4.compare(LISTEN_ANY_IP) == 0)
        net_address.sin_addr.s_addr = INADDR_ANY;

    else
    {
        net_address.sin_addr.s_addr =
            inet_addr(ipv4.c_str()); // IP específica
    }

    // BIND información al socket
    if (
        bind(this->network_socket,
             (struct sockaddr *)&net_address,
             sizeof(net_address)) == -1)
        throw SocketException("Socket bind");

    //  Listen y Accept se hacen en listener()
}

void Server::closeSocket()
{
    if (close(this->network_socket) < 0)
        throw SocketException("Socket close");
}

void Server::listener(HTTP *http_class)
{
    //! Llamada desde un hilo

    // Iniciar escucha
    try
    {
        // Escuchar por el socket a max_con clientes
        if (listen(this->network_socket, this->max_con) < 0)
            throw SocketException("Socket listen");
    }
    catch (SocketException &e)
    {
        perror(e.what());
    }

    //* Ahora el Server puede recibir conexiones
    while (http_class->activeListen)
    {
        std::cout << "Servidor: en estado de escucha" << std::endl;
        FileDescriptor client_socket; // Socket del cliente

        try
        {
            // Aceptar al cliente que se quiere conectar
            client_socket = accept(this->network_socket, NULL, NULL);

            if (client_socket < 0)
                throw SocketException("Socket accept");
        }
        catch (SocketException &e)
        {
            perror(e.what());
        }

        //! Ya se estableció comunicación con el cliente

        //* 1. Recibir su request HTML
        char request_http[HTTP::RequestSize];
        memset(request_http, 0, sizeof(request_http));

        std::cout << "\nServidor: Recibiendo data del navegador" << std::endl;
        ssize_t bytes_read =
            recv(client_socket, &request_http, HTTP::RequestSize, 0);
        std::cout << "Servidor: Fin de la transacción" << std::endl;

        // Colocar bit de finalización
        request_http[bytes_read] = '\0';

        //* 2. Reenviar request HTML al hilo Cliente

        // 2.1 Tokenizar la URL destino
        char url_host[URL_MAX_SIZE];
        std::string pagina;

        try
        {
            HTTP::get_hostname(request_http, url_host);
        }
        catch (std::out_of_range &e)
        {
            std::cerr << e.what() << std::endl;
            continue;
        }

        // Páginas web virtuales
        if (http_class->isVirtPage())
        {
            try
            {
                pagina = http_class->getVirtWebPage()->getVirtualPage(url_host);

                // Copiar los contenidos de la página en la string de URL
                std::cout << "\nPáginas web virtuales: "
                          << url_host << " -> " << pagina;
                strcpy(url_host, pagina.c_str());
            }
            catch (AddressException &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }

        // 2.2 Obtener la IP mediante DNS
        static std::string previous_url("");
        static std::string dir_host("");

        if (previous_url.compare(url_host) != 0)
        {
            try
            {
                dir_host = HTTP::resolveDNS(url_host);
            }
            catch (AddressException &e)
            {
                std::cerr << "\nDirección no encontrada: " << url_host << std::endl;
                std::cout << "Saltando petición" << std::endl;
                close(client_socket);
                continue;
            }

            previous_url = url_host;
        }

        else
        {
            std::cout << "\nServicio DNS: usando dirección previa\n"
                      << std::endl;
        }

        // 2.3 Enviarsela al Cliente
        HTTP::Data data;
        data.buffer_size = bytes_read;
        data.buffer = request_http;
        data.destination = dir_host;
        data.ignore = false;

        http_class->pushRequest(data);

        //* 3. Recibir respuesta del hilo Cliente
        HTTP::Data response = http_class->getResponse();

        std::cout << "\nServidor: Enviando datos al navegador" << std::endl;
        if (response.ignore != true)
        {
            //* 4. Reenviar respuesta al origen (Buscador)
            send(client_socket, response.buffer.c_str(), response.buffer_size, 0);
        }
        std::cout << "Servidor: Fin de la transacción\n"
                  << std::endl;

        http_class->popResponse();

        // Cerrar socket cliente
        try
        {
            if (close(client_socket) < 0)
                throw SocketException("Client socket close");
        }
        catch (SocketException &e)
        {
            perror(e.what());
        }
    }
}