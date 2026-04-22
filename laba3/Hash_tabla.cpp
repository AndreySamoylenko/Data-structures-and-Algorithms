// #pragma once
#include <utility>
#include <cstddef>

#include <string>
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

template <typename K, typename V>
class Associate_array
{
public:
    virtual void insert(K k, V v) = 0;
    virtual void remove(K k, V v) = 0;
    virtual V find(K k) const = 0;
};

template <typename V>
class HashTable : public Associate_array<Key, V>
{
private:
    std::pair<Key, V> *table = nullptr;
    std::size_t size;
    short *status;

public:
    HashTable(std::size_t size_)
    {
        table = new std::pair<Key, V>[size_];
        status = new short[size_];
        size = size_;
        for (std::size_t i = 0; i < size; i++)
        {
            status[i] = 0;
        } // set statuses to "empty"
    }
    HashTable()
    {
        table = new std::pair<Key, V>[1000];
        status = new short[1000];
        size = 1000;
        for (std::size_t i = 0; i < size; i++)
        {
            status[i] = 0;
        } // set statuses to "empty"
    }

    ~HashTable()
    {
        delete[] table;
        delete[] status;
    }

    std::size_t hash(Key key)
    {
        std::size_t index = 0;

        index += key.date.day;
        index += key.date.month;
        index += key.date.year;

        for (char c : key.full_name.name + key.full_name.surname + key.full_name.patronymic)
        {
            index += c;
        }

        return index % size;
    }

    void insert(Key key, V v) override
    {
        std::size_t i = hash(key);

        if (status[i] == 1)
        {
            // collision
        }

        table[i].first = key;
        table[i].second = v;
    }

    void remove()(Key key, V v) override
    {
        std::size_t i = hash(key);

        if (status[i] == 0)
        {
            // nothing to remove
            return;
        }
        if (status[i] == 2)
        {
            // collision
        }
        status[i] = 2
    }

    V find(Key key) override
    {
        std::size_t i = hash(key);

        if (status[i] == 0)
        {
            return nullptr;
        }
        if (status[i] == 2){
            // collision
        }
        return table[i].second;
    }
};

int main()
{
    return 0;
}