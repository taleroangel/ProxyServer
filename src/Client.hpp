/**
 * @file Client.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de mandar y recibir peticiones hacia el Servidor
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