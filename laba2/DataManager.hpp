#pragma once

#include <string>
#include <iostream>

struct Date
{
    unsigned short int day;
    unsigned short int month;
    unsigned int year;
    size_t date_to_number(const Date &d) const { return (d.day + d.month * 100 + d.year * 10000); }

    bool operator<(const Date &o) const { return date_to_number(*this) < date_to_number(o); }
    bool operator>(const Date &o) const { return date_to_number(*this) > date_to_number(o); }
    bool operator==(const Date &o) const { return date_to_number(*this) == date_to_number(o); }
    bool operator!=(const Date &o) const { return date_to_number(*this) != date_to_number(o); }
    friend std::ostream &operator<<(std::ostream &os, const Date &d)
    {
        os << ((d.day < 10) ? "0" : "") << d.day << "." << ((d.month < 10) ? "0" : "")<< d.month << "." << d.year;
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
            return 1;
        if (surname == o.surname && name < o.name)
            return 1;
        if (surname == o.surname && name == o.name && patronymic < o.patronymic)
            return 1;
        return 0;
    }
    bool operator>(const FIO &o) const
    {
        if (surname > o.surname)
            return 1;
        if (surname == o.surname && name > o.name)
            return 1;
        if (surname == o.surname && name == o.name && patronymic > o.patronymic)
            return 1;
        return 0;
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
            return 1;
        if (date == o.date && full_name < o.full_name)
            return 1;
        return 0;
    }
    bool operator>(const Key &o) const
    {
        if (date > o.date)
            return 1;
        if (date == o.date && full_name > o.full_name)
            return 1;
        return 0;
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
    Key key = Key{date, full_name};

    size_t array_index;

    bool operator<(const PersonalData &o) const { return (key < o.key); }
    bool operator>(const PersonalData &o) const { return (key > o.key); }
    bool operator==(const PersonalData &o) const
    {
        return (date == o.date) && (full_name == o.full_name) && (request_number == o.request_number) && (description == o.description);
    }
    friend std::ostream &operator<<(std::ostream &os, const PersonalData &pd)
    {
        os << pd.date << " | " << pd.full_name << " | " << pd.request_number << " | " << pd.description;
        return os;
    }
};

class Repository
{
public:
    virtual void add(PersonalData &record) = 0;
    virtual void remove(const Key &key, const size_t &array_index) = 0;
    virtual void update(const PersonalData &old_data, const PersonalData &new_data) = 0;
    virtual void print_repository() const = 0;
};

class IndexedStructure
{
public:
    virtual void add(const PersonalData &record) = 0;
    virtual void remove(const Key &key, const size_t &array_index) = 0;
    virtual void update(const PersonalData &old_data, const PersonalData &new_data) = 0;
    virtual void print_structure() const = 0;
};

class DataManager
{
private:
    IndexedStructure &indexed_struct;
    Repository &data_bank;

public:
    DataManager(IndexedStructure &indexed_struct, Repository &data_bank) : indexed_struct(indexed_struct), data_bank(data_bank) {}

    void add(PersonalData &record)
    {
        // indexed_struct.add(record);
        data_bank.add(record);
    };

    void remove(const Key &key, const size_t &array_index)
    {
        indexed_struct.remove(key, array_index);
        data_bank.remove(key, array_index);
    };

    void update(const PersonalData &old_data, const PersonalData &new_data)
    {
        indexed_struct.update(old_data, new_data);
        data_bank.update(old_data, new_data);
    };

    IndexedStructure &get_indexed_structure() const { return indexed_struct; }
    Repository &get_data_bank() const { return data_bank; }

    ~DataManager() = default;
};
