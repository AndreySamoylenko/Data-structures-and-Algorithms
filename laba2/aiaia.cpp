#include "DataManager.hpp"
#include "DataStorage.tpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

bool string_to_date(const std::string &date_str, Date &date)
{
    std::istringstream ss(date_str);
    char delimiter1, delimiter2;
    if (ss >> date.day >> delimiter1 >> date.month >> delimiter2 >> date.year)
    {
        if (delimiter1 == '.' && delimiter2 == '.')
            return true;
    }
    return false;
}

bool string_to_fio(const std::string &fio_str, FIO &fio)
{
    std::istringstream ss(fio_str);
    if (ss >> fio.surname >> fio.name >> fio.patronymic)
        return true;

    return false;
}

void read_data_from_file(const std::string &filename, DataManager &manager)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    size_t index = 0;
    while (std::getline(file, line))
    {
        PersonalData record;
        if (line.empty())
            continue;
        // Парсим дату
        if (!string_to_date(line.substr(0, line.find('|')), record.date))
        {
            std::cerr << "Error parsing date in line: " << index + 1 << ": " << line << std::endl;
            continue;
        }
        line = line.substr(line.find('|') + 1);
        // Парсим ФИО
        if (!string_to_fio(line.substr(0, line.find('|')), record.full_name))
        {
            std::cerr << "Error parsing FIO in line: " << index + 1 << ": " << line << std::endl;
            continue;
        }
        line = line.substr(line.find('|') + 1);
        // Парсим номер заявки
        try
        {
            record.request_number = std::stoul(line.substr(0, line.find('|')));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing request number in line: " << index + 1 << ": " << line << std::endl;
        }
        line = line.substr(line.find('|') + 1);
        // Остаток строки - описание
        record.description = line;

        manager.add(record);
        index++;
    }
}

int main()
{   
    RBtree tree;
    Array array;
    DataManager manager(tree, array);
    read_data_from_file("input.txt", manager);
    manager.get_indexed_structure().print_structure();
    manager.get_data_bank().print_repository();

    return 0;
}