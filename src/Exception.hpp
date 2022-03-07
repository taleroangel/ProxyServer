/**
 * @file Exception.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Header que contiene las Excepciones personalizadas
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