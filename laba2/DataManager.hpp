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

class Repository
{
public:
    virtual void add(PersonalData &record) = 0;
    virtual void remove(const PersonalData &record) = 0;
    virtual void update(const PersonalData &old_data, const PersonalData &new_data) = 0;
    virtual PersonalData get(const size_t &array_index) const = 0;
    virtual long long find(const PersonalData &record) const = 0;
    virtual long long size() const = 0;
    virtual void print_repository() const = 0;
};

class IndexedStructure
{
public:
    virtual void add(const PersonalData &record) = 0;
    virtual void remove(const PersonalData &record) = 0;
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
        data_bank.add(record);      // добавляем запись в массив (внутри метода add массива уже выставляется правильный array_index)
        indexed_struct.add(record); // добавляем запись с правильным индексом в дерево
    };

    void remove(const PersonalData &record)
    {
        long long array_index = data_bank.find(record);

        if (array_index == -1) // если записи нет в массиве, то удалять нечего
            return;

        PersonalData last_record = data_bank.get(data_bank.size() - 1);
        data_bank.remove(record); // затерли данные в массиве (теперь на месте удалённой записи стоит последняя запись массива)

        if (array_index == data_bank.size()) // если удалённая запись была последней в массиве
        {   // то обновлять индекс в дереве не нужно, так как на её месте уже стоит последняя запись массива с правильным индексом, просто удалим её из дерева
            indexed_struct.remove(record); // удалили из дерева запись
            return;
        }

        indexed_struct.remove(record);                         // удалили из дерева запись
        indexed_struct.add(last_record);                       // передобавили в дерево запись (которая теперь стоит на месте удалённой записи) с новым индексом
        last_record.array_index = data_bank.size() - 1;
        indexed_struct.remove(last_record); // удалили из дерева последнюю запись со старым индексом
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
