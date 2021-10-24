#include "HTTP.hpp"
#include "Exception.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

// Macros
#define _HTTP_LINE_FEED "\r\n"
#define _HTTP_HOST_FIND "\r\nHost:"
#define _HTTP_CONTENT_LENGTH "\r\nContent-Length:"

HTTP::HTTP()
    : activeListen(true)
{
    // Inicializar recursos
    sem_init(&this->request_n, 0, 0);
    sem_init(&this->response_n, 0, 0);
}

HTTP::~HTTP()
{
    this->activeListen = false;
    sem_destroy(&this->request_n);
    sem_destroy(&this->response_n);
}

std::string HTTP::resolveDNS(std::string url, uint8_t attempt)
{
    struct hostent *hostname;
    struct in_addr **addr_list;

    // Obtener la IP
    hostname = gethostbyname(url.c_str());

    // Verificar que exista
    if (hostname == NULL)
        throw AddressException("Dirección no válida");

    // Guardar lista de ip's
    addr_list =
        (struct in_addr **)hostname->h_addr_list;

    // Direccion IPv4
    std::string ipv4;

    try
    {
        ipv4 = inet_ntoa(*addr_list[attempt]);
    }
    catch (std::out_of_range &e)
    {
        throw e;
    }

    // Retornar direccion ipv4
    std::cout << std::endl
              << "Servicio DNS: " << url << " -> " << ipv4 << std::endl;
    return ipv4;
}

void HTTP::createHeadFromGet(std::string http, char *head)
{
    unsigned int start = http.find(_HTTP_GET);

    if (start == std::string::npos)
    {
        throw AddressException("No content lenght");
    }
    unsigned int finish = start + sizeof(_HTTP_GET) - 1;
    http.replace(start, finish, _HTTP_HEAD);
    strcpy(head, http.c_str());
}

HTTP::DataSize HTTP::getContentLength(
    const char head_msg[HTTP::ResponseSize])
{
    std::string data(head_msg);
    unsigned int start = data.find(_HTTP_CONTENT_LENGTH) + sizeof(_HTTP_CONTENT_LENGTH);

    if (start == std::string::npos)
    {
        throw AddressException("No content lenght");
    }

    data = data.substr(start, data.size());

    unsigned int finish = data.find(_HTTP_LINE_FEED);
    data = data.substr(0, finish);

    return std::stoi(data);
}

void HTTP::get_hostname(
    const char http_data[HTTP::RequestSize],
    char *hostname)
{
    // String con los datos
    std::string data(http_data);
    unsigned int start = data.find(_HTTP_HOST_FIND) + sizeof(_HTTP_HOST_FIND);

    if (start == std::string::npos)
        throw std::out_of_range("Invalid HOST");

    data = data.substr(start, data.size());
    unsigned int finish = data.find(_HTTP_LINE_FEED);
    data = data.substr(0, finish);

    // Copy into hostname
    strcpy(hostname, data.c_str());
}

HTTP::RequestType HTTP::getType(std::string http_data)
{
    unsigned int lineFeed = http_data.find(_HTTP_LINE_FEED);
    http_data = http_data.substr(0, lineFeed);

    if (http_data.find(_HTTP_GET) != std::string::npos)
        return GET;

    else if (http_data.find(_HTTP_HEAD) != std::string::npos)
        return HEAD;

    else if (http_data.find(_HTTP_POST) != std::string::npos)
        return POST;

    else if (http_data.find(_HTTP_PUT) != std::string::npos)
        return PUT;

    else
        return OTHER;
}

void HTTP::pushRequest(Data request)
{
    std::cout << "Se ha empilado una petición (Desbloquea Cliente)" << std::endl;
    this->request_q.push(request);
    sem_post(&this->request_n);
}

void HTTP::popRequest()
{
    this->request_q.pop();
}

HTTP::Data HTTP::getRequest()
{
    std::cout << "Se ha desempilado una petición (Bloquea Cliente)" << std::endl;
    sem_wait(&this->request_n);
    return this->request_q.front();
}

void HTTP::pushResponse(HTTP::Data response)
{
    std::cout << "Se ha empilado una respuesta (Desbloquea Servidor)" << std::endl;
    this->response_q.push(response);
    sem_post(&this->response_n);
}

void HTTP::popResponse()
{
    this->response_q.pop();
}

HTTP::Data HTTP::getResponse()
{
    std::cout << "Se ha desempilado una respuesta (Bloquea Servidor)" << std::endl;
    sem_wait(&this->response_n);
    return this->response_q.front();
}