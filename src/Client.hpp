/**
 * @file Client.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de mandar y recibir peticiones hacia el Servidor
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