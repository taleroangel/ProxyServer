#include <iostream>

#include "Client.hpp"
#include "Exception.hpp"

Client::Client(Port port)
    : port(port)
{
}

Client::~Client()
{
}

void Client::createSocket(IPaddress ipv4, Port port)
{
    this->network_socket =
        socket(AF_INET, SOCK_STREAM, 0);

    if (this->network_socket == -1)
        throw SocketException("Socket open");

    // Añadir información de IP y Puerto al Socket
    struct sockaddr_in net_address;
    net_address.sin_family = AF_INET;
    net_address.sin_port = htons(port);
    net_address.sin_addr.s_addr = inet_addr(ipv4.c_str());

    // Connect the Socket
    if (connect(this->network_socket,
                (struct sockaddr *)&net_address,
                sizeof(net_address)) == -1)
        throw SocketException("Socket connect");
}

void Client::closeSocket()
{
    if (close(this->network_socket) < 0)
        throw SocketException("Socket close");
}

void Client::listener(HTTP *http_class)
{
    //! Llamar desde un hilo
    while (http_class->activeListen)
    {
        // Obtener un request HTTP
        HTTP::Data request = http_class->getRequest();

        //Crear el socket
        try
        {
            // Crear un puerto con el Servidor (Puerto TCP 80 - HTTP)
            createSocket(request.destination, this->port);
        }
        catch (SocketException &e)
        {
            perror(e.what());
            std::cerr << "Ignorando petición" << std::endl;

            request.ignore = true;
            http_class->pushResponse(request);
            http_class->popRequest();

            continue;
        }

        //* 1. Send request to Destination
        // Datos a transferir
        HTTP::Data response;

        std::cout << "\nCliente: Enviando datos al server" << std::endl;
        write(this->network_socket,
              request.buffer.c_str(),
              request.buffer_size);
        std::cout << "Cliente: Fin de la transacción" << std::endl;

        //* Leer la respuesta (Redirigir el paquete)

        uint32_t current_memory = 1;
        char *http_buffer_response =
            (char *)calloc(current_memory, sizeof(char));
        // Pedir memoria para un array inicializado en 1

        std::cout << "Cliente: Recibiendo datos del server" << std::endl;
        size_t bytes_read = 0;
        while (true)
        {
            // Leer 1 byte a la vez
            ssize_t bytes =
                read(this->network_socket,
                     http_buffer_response + bytes_read, 1);

            if (bytes == 0)
                break;

            bytes_read += bytes;
            current_memory++;

            // Cada que pida un byte realojo la memoria con el nuevo tamaño
            http_buffer_response = (char *)realloc(
                http_buffer_response, sizeof(char) * current_memory);
        }

        std::cout << "Cliente: Fin de la transacción\n" << std::endl;

        // Enviar la respuesta al servidor
        response.buffer = http_buffer_response;
        response.buffer_size = bytes_read;

        // Proceso de realojo con alta fragmentación de memoria,
        // de ser posible optimizar estos free y realloc
        free(http_buffer_response);

        response.ignore = false;

        http_class->pushResponse(response);

        // Desempilar la respuesta
        http_class->popRequest();

        //Cerrar el socket
        closeSocket();
    }
}