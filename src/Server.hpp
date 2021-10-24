/**
 * @file Server.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * 
 * @brief Clase encargada de suplir el rol de Servidor, esta clase
 * recoge las peticiones HTML del navegador mediante el protocolo TCP
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

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "Connection.hpp"

class Server : public Connection
{
    /* ---- Constants ---- */
private:
    static constexpr uint16_t
        DEF_CONNECTIONS = 5U; /*< Máximo de conexiones permitidos por defecto*/

    /* ---- Private member variable ---- */
private:
    FileDescriptor network_socket; /*< Socket*/
    uint16_t max_con;              /*< Máximo número de conexiones*/

    /* ---- Constructors ---- */
public:
    /**
     * @brief Construir un Obeto Servidor
     * @param port Puerto, para usar por defecto [DEFAULT_PORT]
     * @param max_connections Número máximo de conexiones, 10 por defecto
     * @param ipv4 Dirección IP, por defecto cualquiera
     */
    explicit Server(
        Port port,
        uint16_t max_connections = DEF_CONNECTIONS,
        IPaddress ipv4 = LISTEN_ANY_IP) noexcept(false);

    ~Server();

public:
    /* ---- Virtual function override ---- */
    void createSocket(
        IPaddress ipv4 = LISTEN_ANY_IP,
        Port port = DEFAULT_PORT) noexcept(false) override;

    void closeSocket() noexcept(false) override;

    void listener(HTTP *http_class) override;
};

#endif //_SERVER_HPP_