/**
 * @file Server.hpp
 * @author Angel D. Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de manejar los sitios web virtuales
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

#ifndef _VIRTWEBPAGE_HPP_
#define _VIRTWEBPAGE_HPP_

#include <string>
#include <vector>

class VirtWebPage
{
    /* ---- Typedefs privados ---- */
private:
    /**
     * @struct entry
     * @brief Estructura de entradas
     */
    struct entry
    {
        std::string input;  /*< URL enviada por el navegador*/
        std::string output; /*< URL equivalente*/
    };

    /* ---- Variables privadas ---- */
private:
    std::vector<entry> entries; /*< Todas las entradas*/

    /* ---- Constructores ---- */
public:
    /**
     * @brief Crear entradas de página web virtuales
     * 
     * @param filename Nombre de archivo a leer
     */
    explicit VirtWebPage(std::string filename) noexcept(false);

    /**
     * @brief Destruir las entradas de páginas web virtuales
     * 
     */
    ~VirtWebPage();

    /* ---- Métodos públicos ---- */
public:
    /**
     * @brief Obtener la pagina web virtual
     * 
     * @param url URL que mandó el navegador
     * @throw AddressException La dirección que se solicitó no existe
     * 
     * @return std::string URL real
     */
    std::string getVirtualPage(std::string url) noexcept(false);
};

#endif //_VIRTWEBPAGE_HPP_