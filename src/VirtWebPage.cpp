#include "VirtWebPage.hpp"
#include "Exception.hpp"

#include <cstdio>
#include <cstring>

#define URLLENGHT 50

VirtWebPage::VirtWebPage(std::string filename)
{
    // Abrir el archivo
    FILE *archivo = fopen(filename.c_str(), "r");

    if (archivo == NULL)
        throw FileException("Apertura de archivo");

    while (!feof(archivo))
    {
        char dir[URLLENGHT], is[URLLENGHT];
        memset(dir, 0, URLLENGHT);
        memset(is, 0, URLLENGHT);

        fscanf(archivo, "(%s)->(%s)\n", dir, is);

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

    throw AddressException("La dirección no existe");
    return std::string(); // Just so the compiler doesn 't complain
}
