#include "VirtWebPage.hpp"
#include "Exception.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#define URLLENGHT 50

VirtWebPage::VirtWebPage(std::string filename)
{
    // Abrir el archivo
    FILE *archivo = fopen(filename.c_str(), "r");

    if (archivo == NULL)
        throw FileException("Apertura de archivo");

    char buffer[120];
    char *token;
    const char sep[3] = ", ";

    while (fgets(buffer, sizeof(buffer), archivo) != NULL)
    {
        char dir[URLLENGHT], is[URLLENGHT];
        memset(dir, 0, URLLENGHT);
        memset(is, 0, URLLENGHT);

        token = strtok(buffer, sep);
        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                sprintf(dir, "%s", token);
                token = strtok(NULL, sep);
            }
            else
            {
                token[strlen(token) - 1] = 0;
                sprintf(is, "%s", token);
            }
        }

        entry new_entry;
        new_entry.input = dir;
        new_entry.output = is;

        // Empujar la nueva entrada
        this->entries.push_back(new_entry);
    }

    fclose(archivo);
}

VirtWebPage::~VirtWebPage()
{
    this->entries.clear();
}

std::string VirtWebPage::getVirtualPage(std::string url)
{
    for (auto value : this->entries)
    {
        if (value.input.compare(url) == 0)
            return value.output;
    }

    throw AddressException("Páginas web virtuales: La dirección no es virtual");
    return std::string(); // Just so the compiler doesn 't complain
}
