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
        socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

    while (true)
    {
        // Obtener un request HTTP
        HTTP::Request request = http_class->getRequest();

        //Crear el socket
        createSocket(request.IPv4_Dest, this->port);

        //* 1. Send request to Destination

        //! Primero enviar una petición HEAD y luego una petición GET
        //! para obtener el tamaño del buffer

        // Creando el mensaje HEAD para peticiones GET
        char head_request[HTTP::RequestSize];
        try
        {
            HTTP::create_head_message(request.request, head_request);
            send(this->network_socket, head_request, sizeof(head_request), 0);

            // Recibiendo la respuesta HEAD
            char head_response[HTTP::ResponseSize];
            recv(this->network_socket, &head_response, sizeof(head_response), 0);

            // Mostrar la respuesta
            std::cout << std::endl
                      << "Ha llegado una respuesta HEAD:" << std::endl
                      << head_response << std::endl;

            HTTP::ResponseData buff_size = HTTP::get_head_size(head_response);

            // Crear el buffer de respuesta
            char *http_response = new char[buff_size];

            // Enviar el paquete GET
            send(this->network_socket,
                 request.request,
                 sizeof(request.request), 0);

            // Recoger la respuesta
            recv(this->network_socket, http_response, sizeof(http_response), 0);
            std::string http(http_response, sizeof(http_response));
            delete[] http_response;

            // Desempilar la respuesta
            http_class->popRequest();

            //* 3. Send Reponse to Server

            HTTP::Response response;
            response.response = http;
            response.response_size = buff_size;

            http_class->pushResponse(response);
        }

        // Otro tipo de petición
        catch (AddressException &e)
        {
        }

        //Cerrar el socket
        closeSocket();
    }
}