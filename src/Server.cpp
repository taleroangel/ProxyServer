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
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    while (true)
    {
        FileDescriptor client_socket; // Socket del cliente
        try
        {
            //TODO: Obtener la información del IP cliente
            // NULL, NULL indica que no necesito la IP del cliente
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
        recv(client_socket, &request_http, sizeof(request_http), 0);

        //1.1 Mostrar el request por pantalla
        std::cout << std::endl
                  << "Ha llegado una petición" << std::endl
                  << request_http << std::endl;

        //* 2. Reenviar request HTML al hilo Cliente

        // 2.1 Tokenizar la URL destino
        char url_host[IP_MAX_SIZE];
        HTTP::get_hostname(request_http, url_host);

        // 2.2 Obtener la IP mediante DNS
        std::string dir_host;
        try
        {
            dir_host = HTTP::resolveDNS(url_host);
        }
        catch (AddressException &e)
        {
            std::cerr << "Dirección no encontrada" << std::endl;
        }

        // 2.3 Enviarsela al Cliente
        http_class->pushRequest(HTTP::Request(request_http, dir_host, url_host));

        //* 3. Recibir respuesta del hilo Cliente
        HTTP::Response response = http_class->getResponse();

        //* 4. Reenviar respuesta HTML al origen
        std::cout << "Reenviando respuesta al navegador" << std::endl;

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