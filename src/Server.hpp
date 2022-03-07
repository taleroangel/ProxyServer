/**
 * @file Server.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * 
 * @brief Clase encargada de suplir el rol de Servidor, esta clase
 * recoge las peticiones HTML del navegador mediante el protocolo TCP
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
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