/**
 * @file Exception.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Header que contiene las Excepciones personalizadas
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

#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <stdexcept>
#include <string>

/**
 * @class SocketException
 * @brief Excepción de fallo en creación de un Socket
 */
class SocketException : public std::exception
{
	std::string message; /*< Error message */

public:
	/**
	 * @brief Constructor de la Excepción de Socket
	 *
	 * @param error Mensaje de error (Opcional)
	 */
	explicit SocketException(
		const std::string &error = "Error al crear el socket")
		: message(error)
	{
	}

	/**
	 * @brief Mostrar el mensaje de error
	 *
	 * @return const char* Mensaje de error
	 */
	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};

/**
 * @class AddressException
 * @brief Excepción al intentar Conectar el socket a determinada dirección
 * IP no existente o un Puerto ocupado
 */
class AddressException : public std::exception
{
	std::string message; /*< Error message */

public:
	/**
	 * @brief Constructor de la Excepción de Conexión
	 *
	 * @param error Mensaje de error (Opcional)
	 */
	explicit AddressException(
		const std::string &error = "Error de direcciones IPv4")
		: message(error)
	{
	}

	/**
	 * @brief Mostrar el mensaje de error
	 *
	 * @return const char* Mensaje de error
	 */
	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};

/**
 * @class FileException
 * @brief Excepción al intentar abrir un archivo no existente
 */
class FileException : public std::exception
{
	std::string message; /*< Error message */

public:
	/**
	 * @brief Constructor de la Excepción de Conexión
	 *
	 * @param error Mensaje de error (Opcional)
	 */
	explicit FileException(
		const std::string &error = "Error de archivo")
		: message(error)
	{
	}

	/**
	 * @brief Mostrar el mensaje de error
	 *
	 * @return const char* Mensaje de error
	 */
	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};

#endif //_EXCEPTION_HPP_