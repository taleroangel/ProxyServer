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
};

#endif //_SERVER_HPP_