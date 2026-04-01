#pragma once

#include <string>

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
        return (date==o.date) && (full_name == o.full_name) && (request_number == o.request_number) && (description == o.description);
    }
};

class Repository
{
public:
    virtual void add(PersonalData &record) = 0;
    virtual void remove(const Key &key) = 0;
    virtual void update(const PersonalData &old_data, const PersonalData &new_data) = 0;
};

class IndexedStructure
{
public:
    virtual void add(const PersonalData &record) = 0;
    virtual void remove(const Key &key) = 0;
    virtual void update(const PersonalData &old_data, const PersonalData &new_data) = 0;
};

class DataManager
{
private:
    IndexedStructure &indexed_struct;
    Repository &data_bank;

public:
    void add(PersonalData &record)
    {
        indexed_struct.add(record);
        data_bank.add(record);
    };

    void remove(const Key &key)
    {
        indexed_struct.remove(key);
        data_bank.remove(key);
    };

    void update(const PersonalData &old_data, const PersonalData &new_data)
    {
        indexed_struct.update(old_data, new_data);
        data_bank.update(old_data, new_data);
    };
};
