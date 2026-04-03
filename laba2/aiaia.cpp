#include "DataManager.hpp"
#include "DataStorage.tpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <locale>

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
        // std::cout << record << std::endl;

        index++;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    /*
    RBtree tree;
    Array array;
    DataManager manager(tree, array);
    std::cout << "Manager created\n";

    read_data_from_file("input.txt", manager);
    std::cout << "Date read succsefully\n";

    // manager.get_indexed_structure().print_structure();
    manager.get_data_bank().print_repository();
    */

    RBtree tree;
    PersonalData pd;
    pd.date = {1, 0, 0};
    pd.array_index = 1;
    pd.request_number = 1;
    pd.description = "bebebe";
    pd.full_name.name = "a";
    pd.full_name.surname = "a";
    pd.full_name.patronymic = "a";
    std::cout << pd << std::endl;
    std::cout << pd.key() << std::endl;

    tree.add(pd);
    tree.add(pd);
    tree.add(pd);

    pd.date = {1, 1, 0};
    tree.add(pd);
    tree.print_structure();


    // pd.full_name.name = "aa";
    // tree.add(pd);
    // pd.date = {1, 1, 1};
    // tree.add(pd);
    // pd.date = {1, 1, 1};
    // pd.full_name.name = "aaa";
    // tree.add(pd);
    // pd.date = {1, 1, 1};
    // tree.add(pd);
    // pd.date = {1, 2, 1};
    // tree.add(pd);
    // pd.date = {1, 2, 1};
    // pd.full_name.name = "asa";
    // tree.add(pd);
    // pd.date = {1, 1, 3};
    // tree.add(pd);
    // pd.date = {3, 1, 1};
    // tree.add(pd);

    // tree.print_structure();
    // tree.print_in_order();

    return 0;
}