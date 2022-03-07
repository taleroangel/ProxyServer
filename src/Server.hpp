/**
 * @file Server.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de suplir el rol de Servidor, esta clase
 * recoge las peticiones HTML del navegador mediante el protocolo TCP
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License. 
 */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "Connection.hpp"

#define URL_MAX_SIZE 120
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