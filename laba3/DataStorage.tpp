#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <vector>
#include <stack>


#include "DataManager.hpp"


class HashTable : public IndexedStructure
{
private:
    std::pair<Key, size_t> *table = nullptr;
    short *status;

    std::size_t t_capacity;
    std::size_t t_size = 0;

    const float shrink_koef = 0.25;
    const float enlarge_koef = 0.75;
    const short MIN_SIZE = 10;
    const short DEFAULT_SIZE = 10;

    void shrink_rehash()
    {
        std::pair<Key, size_t> *old_table = table;
        short *old_status = status;
        std::size_t t_old_cap = t_capacity;

        t_capacity = ((t_capacity / 2 >= MIN_SIZE) ? t_capacity : MIN_SIZE);
        t_size = 0;
        table = new std::pair<Key, size_t>[t_capacity];
        status = new short[t_capacity];
        for (std::size_t i = 0; i < t_capacity; i++)
            status[i] = 0;

        for (int i = 0; i < t_old_cap; i++)
        {
            if (old_status[i] == 1)
            {
                add(old_table[i].first, old_table[i].second);
            }
        }

        delete[] old_status;
        delete[] old_table;
    }

    void enlarge_rehash()
    {
        std::pair<Key, size_t> *old_table = table;
        short *old_status = status;
        std::size_t t_old_cap = t_capacity;

        t_capacity = (t_capacity > 0 ? t_capacity * 2 : 1); // костыльный вариант решения для t_capacity = 0;
        t_size = 0;
        table = new std::pair<Key, size_t>[t_capacity];
        status = new short[t_capacity];
        for (std::size_t i = 0; i < t_capacity; i++)
            status[i] = 0;

        for (int i = 0; i < t_old_cap; i++)
        {
            if (old_status[i] == 1)
            {
                add(old_table[i].first, old_table[i].second);
            }
        }

        delete[] old_status;
        delete[] old_table;
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

public:
    HashTable(std::size_t size_)
    {
        if (size_ < MIN_SIZE)
            size_ = MIN_SIZE;
        table = new std::pair<Key, size_t>[size_];
        status = new short[size_];
        t_capacity = size_;
        for (std::size_t i = 0; i < t_capacity; i++)
        {
            status[i] = 0;
        } // set statuses to "empty"
    }
    HashTable()
    {
        table = new std::pair<Key, size_t>[DEFAULT_SIZE];
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

    size_t get_value(std::size_t index)
    {
        return table[index].second;
    }
    Key get_key(std::size_t index)
    {
        return table[index].first;
    }

    void add(Key key, size_t v)
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

    void add(const PersonalData &record) override
    {
        Key key = record.key();
        size_t v = record.array_index;
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
            if (table[j].first == key && status[j] == 1)
            {
                // std::cout << "Key " << key << " found at " << j << " index\n";
                result.push_back(j);
            }

            i++;
        } while (i != t_capacity && status[j] != 0);

        return result;
    }

    void remove(const PersonalData &record) override
    {
        Key key = record.key();
        size_t v = record.array_index;
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

    void update_index(const PersonalData &record, const size_t &new_index) override
    {
        Key key = record.key();
        size_t v = record.array_index;
        remove(record);
        add(key, new_index);
    }

    void print_structure() const override
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

class Array : public Repository
{
private:
    PersonalData array[1000];
    size_t a_size = 0;
    size_t a_capacity = 1000;

public:
    void add(PersonalData &record) override
    {
        if (a_size == a_capacity)
        {
            std::cout << "Array is full, cannot add more records.\n";
            return;
        }

        record.array_index = a_size;
        array[a_size] = record;
        a_size++;
    }
    void remove(const PersonalData &record) override
    {
        for (size_t index = 0; index < a_size; index++)
        {
            if (array[index] == record)
            {
                array[index] = array[a_size - 1];
                a_size--;
                return;
            }
        }
    }
    void update(const PersonalData &old_data, const PersonalData &new_data) override
    {
        for (size_t index = 0; index < a_size; index++)
        {
            if (array[index] == old_data)
            {
                PersonalData updated_record = new_data;
                updated_record.array_index = array[index].array_index;
                array[index] = updated_record;
                return;
            }
        }
    }
    PersonalData get(const size_t &array_index) const override
    {
        if (array_index >= a_size)
        {
            std::cout << "Index out of bounds, returning default PersonalData.\n";
            return PersonalData();
        }
        return array[array_index];
    }
    long long find(const PersonalData &record) const override
    {
        for (size_t index = 0; index < a_size; index++)
        {
            if (array[index] == record)
            {
                return index;
            }
        }
        return -1; // РµСЃР»Рё Р·Р°РїРёСЃСЊ РЅРµ РЅР°Р№РґРµРЅР°, РІРѕР·РІСЂР°С‰Р°РµРј -1
    }
    long long size() const override { return a_size; }

    void print_repository() const override
    {
        for (size_t index = 0; index < a_size; index++)
        {
            std::cout << array[index] << std::endl;
        }
    }
};