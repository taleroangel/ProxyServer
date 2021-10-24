/**
 * @file Client.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * 
 * @brief Clase encargada de mandar y recibir peticiones hacia el Servidor
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Departamento de Ingeniería de Sistemas
 * Bogotá D.C - Colombia
 * 
 * Copyright © 2021
 */

#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "Connection.hpp"

class Client : public Connection
{

    /* ---- Private member variable ---- */
private:
    FileDescriptor network_socket; /*< Socket*/
    uint16_t port;

    /* ---- Constructors ---- */
public:
    /**
     * @brief Construir un Objecto Cliente
     * @param port Puerto a usar, por defecto [HTTP::TCPport]
     */
    explicit Client(Port port = HTTP::TCPport) noexcept(false);

    /**
     * @brief Destruir el cliente
     */
    ~Client();

    /* ---- Virtual function override ---- */
public:
    void createSocket(
        IPaddress ipv4, Port port = HTTP::TCPport) noexcept(false) override;

    void closeSocket() noexcept(false) override;

    void listener(HTTP *http_class) override;
};

#endif //_CLIENT_HPP_