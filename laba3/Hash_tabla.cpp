// #pragma once
#include <utility>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

struct Date
{
    unsigned short int day;
    unsigned short int month;
    unsigned int year;
    size_t date_to_number() const { return (day + month * 100 + year * 10000); }

    bool operator<(const Date &o) const { return date_to_number() < o.date_to_number(); }
    bool operator>(const Date &o) const { return date_to_number() > o.date_to_number(); }
    bool operator==(const Date &o) const { return date_to_number() == o.date_to_number(); }
    bool operator!=(const Date &o) const { return date_to_number() != o.date_to_number(); }

    friend std::ostream &operator<<(std::ostream &os, const Date &d)
    {
        os << ((d.day < 10) ? "0" : "") << (unsigned short int)d.day << "." << ((d.month < 10) ? "0" : "") << (unsigned short int)d.month << "." << (unsigned int)d.year;
        return os;
    }
};

struct FIO
{
    std::string surname;
    std::string name;
    std::string patronymic;

    bool operator<(const FIO &o) const
    {
        if (surname < o.surname)
            return true;
        if (surname == o.surname && name < o.name)
            return true;
        if (surname == o.surname && name == o.name && patronymic < o.patronymic)
            return true;
        return false;
    }
    bool operator>(const FIO &o) const
    {
        if (surname > o.surname)
            return true;
        if (surname == o.surname && name > o.name)
            return true;
        if (surname == o.surname && name == o.name && patronymic > o.patronymic)
            return true;
        return false;
    }
    bool operator==(const FIO &o) const { return (surname == o.surname && name == o.name && patronymic == o.patronymic); }
    bool operator!=(const FIO &o) const { return !(surname == o.surname && name == o.name && patronymic == o.patronymic); }

    friend std::ostream &operator<<(std::ostream &os, const FIO &f)
    {
        os << f.surname << " " << f.name << " " << f.patronymic;
        return os;
    }
};

struct Key
{
    Date date;
    FIO full_name;
    bool operator<(const Key &o) const
    {
        if (date < o.date)
            return true;
        if (date == o.date && full_name < o.full_name)
            return true;
        return false;
    }
    bool operator>(const Key &o) const
    {
        if (date > o.date)
            return true;
        if (date == o.date && full_name > o.full_name)
            return true;
        return false;
    }
    bool operator==(const Key &o) const
    {
        return (date == o.date && full_name == o.full_name);
    }
    bool operator!=(const Key &o) const
    {
        return !(date == o.date && full_name == o.full_name);
    }
    friend std::ostream &operator<<(std::ostream &os, const Key &k)
    {
        os << k.date << " | " << k.full_name;
        return os;
    }
};

struct PersonalData
{
    Date date;
    FIO full_name;
    size_t request_number;
    std::string description;
    size_t array_index; // индекс в массиве, который хранит все данные

    bool operator<(const PersonalData &o) const { return (key() < o.key()); }
    bool operator>(const PersonalData &o) const { return (key() > o.key()); }
    bool operator==(const PersonalData &o) const
    {
        return (date == o.date) && (full_name == o.full_name) && (request_number == o.request_number) && (description == o.description);
    }

    friend std::ostream &operator<<(std::ostream &os, const PersonalData &pd)
    {
        os << pd.date << " | " << pd.full_name << " | " << (size_t)pd.request_number << " | " << pd.description;
        return os;
    }

    Key key() const
    {
        Key k;
        k.date = date;
        k.full_name = full_name;
        return k;
    }
};

template <typename V>
class HashTable
{
private:
    std::pair<Key, V> *table = nullptr;
    short *status;

    std::size_t t_capacity;
    std::size_t t_size = 0;

    const float shrink_koef = 0.25;
    const float enlarge_koef = 0.75;
    const short MIN_SIZE = 10;
    const short DEFAULT_SIZE = 100;

    void shrink_rehash()
    {
        std::pair<Key, V> *old_table = table;
        short *old_status = status;
        std::size_t t_old_cap = t_capacity;

        t_capacity = ((t_capacity / 2 >= MIN_SIZE) ? t_capacity : MIN_SIZE);
        t_size = 0;
        table = new std::pair<Key, V>[t_capacity];
        status = new short[t_capacity];
        for (std::size_t i = 0; i < t_capacity; i++)
            status[i] = 0;

        for (int i = 0; i < t_old_cap; i++)
        {
            if (old_status[i] == 1)
            {
                insert(old_table[i].first, old_table[i].second);
            }
        }

        delete[] old_status;
        delete[] old_table;
    }

    void enlarge_rehash()
    {
        std::pair<Key, V> *old_table = table;
        short *old_status = status;
        std::size_t t_old_cap = t_capacity;

        t_capacity = (t_capacity > 0 ? t_capacity * 2 : 1); // ���������� ������� ������� ��� t_capacity = 0;
        t_size = 0;
        table = new std::pair<Key, V>[t_capacity];
        status = new short[t_capacity];
        for (std::size_t i = 0; i < t_capacity; i++)
            status[i] = 0;

        for (int i = 0; i < t_old_cap; i++)
        {
            if (old_status[i] == 1)
            {
                insert(old_table[i].first, old_table[i].second);
            }
        }

        delete[] old_status;
        delete[] old_table;
    }

public:
    HashTable(std::size_t size_)
    {
        if (size_ < MIN_SIZE)
            size_ = MIN_SIZE;
        table = new std::pair<Key, V>[size_];
        status = new short[size_];
        t_capacity = size_;
        for (std::size_t i = 0; i < t_capacity; i++)
        {
            status[i] = 0;
        } // set statuses to "empty"
    }
    HashTable()
    {
        table = new std::pair<Key, V>[DEFAULT_SIZE];
        status = new short[DEFAULT_SIZE];
        t_capacity = DEFAULT_SIZE;
        for (std::size_t i = 0; i < t_capacity; i++)
        {
            status[i] = 0;
        } // set statuses to "empty"
    }

    ~HashTable()
    {
        delete[] table;
        delete[] status;
    }

    V get_value(std::size_t index)
    {
        return table[index].second;
    }
    Key get_key(std::size_t index)
    {
        return table[index].first;
    }

    std::size_t hash_(Key key)
    {
        std::size_t index = 0;
        std::string full_key = "";

        full_key += std::to_string(key.date.day);
        full_key += std::to_string(key.date.month);
        full_key += std::to_string(key.date.year);
        full_key += key.full_name.name + key.full_name.surname + key.full_name.patronymic;

        int counter = 0;
        int buffer = 0;
        for (char c : full_key)
        {
            buffer += c;
            counter++;
            if (counter == 4)
            {
                counter = 0;
                index += buffer;
            }
        }

        return index;
    }

    std::size_t hash(Key key, std::size_t i)
    {
        return (std::size_t)(hash_(key) + (i + i * i) / 2.0) % t_capacity;
    }

    void insert(Key key, V v)
    {
        if (t_size == t_capacity)
        {
            std::cout << "Key " << key << " not inserted (table is full)" << std::endl;
            return;
        }

        std::size_t i = 0;

        do
        {
            std::size_t j = hash(key, i);

            if (status[j] != 1)
            {
                table[j].first = key;
                table[j].second = v;
                status[j] = 1;

                t_size++;
                if ((double)(t_size) / (t_capacity) > enlarge_koef)
                    enlarge_rehash();
                return;
            }
            else if (key == table[j].first && v == table[j].second)
            {
                return;
            }
            i++;
        } while (i != t_capacity);

        std::cout << "Key " << key << " not inserted (table is full 2)" << std::endl;
    }

    std::vector<std::size_t> search(Key key)
    {
        std::vector<std::size_t> result;

        if (t_size == 0)
        {
            return result; // not found due to emptiness of the table
        }

        std::size_t i = 0;
        std::size_t j = 0;

        do
        {
            j = hash(key, i);
            if (table[j].first == key and status[j] == 1)
            {
                // std::cout << "Key " << key << " found at " << j << " index\n";
                result.push_back(j);
            }

            i++;
        } while (i != t_capacity && status[j] != 0);

        return result;
    }

    void remove(Key key, V v)
    {
        if (t_size == 0)
        {
            return;
        }

        std::size_t i = 0;
        std::size_t j = 0;

        do
        {
            j = hash(key, i);
            if (table[j].first == key && table[j].second == v && status[j] == 1)
            {
                status[j] = 2;
                t_size--;
                if ((double)t_size / t_capacity < shrink_koef)
                    shrink_rehash();
                return;
            }

            i++;
        } while (i != t_capacity && status[j] != 0);
    }

    void print_table()
    {
        for (int i = 0; i < t_capacity; i++)
        {
            switch (status[i])
            {
            case 0:
                std::cout << i << " | ---EMPTY--- | ---EMPTY--- \n";
                break;
            case 1:
                std::cout << i << " | " << table[i].first << " | " << table[i].second << '\n';
                break;
            case 2:
                std::cout << i << " | --DELETED-- | --DELETED-- \n";
                break;
            default:
                break;
            }
        }
    }
};

template <typename T>
void print_vector(const std::vector<T> vect)
{
    int n = vect.size();
    std::cout << "[";
    for (int i = 0; i < n; i++)
    {
        std::cout << vect[i] << ' ';
    }
    std::cout << "]" << std::endl;
}

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

void read_data_from_file(const std::string &filename, HashTable<PersonalData> &table)
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
        line = line.substr(line.find('|') + 2);
        // Остаток строки - описание
        record.description = line;
        record.array_index = index;
        table.insert(record.key(),record);
        // std::cout << record << std::endl;

        index++;
    }
}

int main()
{
    HashTable<PersonalData> htfile(10);
    read_data_from_file("input.txt", htfile);
    htfile.print_table();
    std::cout << std::endl;
    std::cout << std::endl;



    HashTable<int> ht(10);

    Key k1{{1, 1, 2020}, {"I", "I", "I"}};
    Key k2{{1, 2, 2020}, {"I", "I", "I"}};
    Key k3{{1, 3, 2020}, {"I", "I", "I"}};
    Key k4{{1, 4, 2020}, {"I", "I", "I"}};
    Key k5{{1, 4, 2020}, {"I", "I", "hohoho"}};

    ht.insert(k1, 100);
    ht.insert(k1, 100);
    ht.insert(k1, 100);
    ht.insert(k1, 100);
    ht.insert(k2, 200);
    ht.insert(k2, 300);
    ht.insert(k4, 400);
    ht.insert(k1, 500);
    ht.insert(k2, 700);
    ht.insert(k3, 800);
    ht.insert(k3, 900);

    ht.print_table();
    std::cout << std::endl;

    print_vector(ht.search(k1));
    print_vector(ht.search(k2));
    print_vector(ht.search(k3));
    print_vector(ht.search(k4));
    print_vector(ht.search(k5));
    std::cout << "empty vector means 'not found'\n";
    std::cout << std::endl;


    ht.remove(k1, 100);
    ht.remove(k1, 200);
    ht.remove(k2, 100);
    ht.remove(k2, 300);

    ht.print_table();
    std::cout << std::endl;

    return 0;
}