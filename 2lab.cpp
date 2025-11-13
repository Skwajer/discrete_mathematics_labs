#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Datastruct
{
    std::vector<char> set;
    std::vector<std::pair<char, char>> pairs;
    bool is_equivalence_relation;
    bool is_order_relation;
};

Datastruct extract_data_from_file(std::ifstream &f)
{
    Datastruct data;
    std::string line;
    while(std::getline(f, line))
    {
        if (!line.empty()) break;
    }

    for (char c : line)
    {
        if (c != ' ')
        {
            data.set.push_back(c);
        }
    }
    
    while (getline(f, line))
    {
        if (line.empty()) continue;
        bool read_first = true;

        char first = ' ', second = ' '; 
        for (char c : line)
        {
            if (c != ' ')
            {
                if (read_first)
                {
                    first = c;
                    read_first = false;
                } else{
                    second = c;
                    break;
                }
            }
        }
        if (first != ' ' && second != ' ')
        {
            data.pairs.push_back({first, second});
        }

    }
    std::cout << "Elements of set: ";
    for (char e : data.set) std::cout << e << " ";
    std::cout << "\nrelation pairs: ";
    for (auto& p : data.pairs) std::cout << "(" << p.first << "," << p.second << ") ";
    std::cout << std::endl;
    return data;
}

void check_relation(Datastruct *data)
{
    bool is_reflexive = true, is_antireflexive = true, is_symmetric = true, is_antisymmetric = true, 
        is_asymmetric = true, is_transitive = true, is_antitransitive = true, is_complete = true;

    for (char const set_elem : data->set)
    {
        bool found = false;
        for (auto const pair : data->pairs)
        {
            if (pair.first == set_elem && pair.second == set_elem)
            {
                found = true;
                is_antireflexive = false;
                break;
            }
        }
        if (!found)
        {
            is_reflexive = false;
        }

    }

    for (auto const pair : data->pairs)
    {
        bool found = false;
        for (auto const other_pair : data->pairs)
        {
            if (pair.first == other_pair.second && pair.second == other_pair.first)
            {
                if (pair.first != pair.second) {is_antisymmetric = false;}
                found = true;
                break;
            }
        }
        if (!found)
        {
            is_symmetric = false;
        }
    }

    if (is_antireflexive && is_antisymmetric) {is_asymmetric = true;}

    for (auto const &p1 : data->pairs)
    {
        for(auto const &p2 : data->pairs)
        if (p1.second == p2.first)
        {
            bool found = false;
            for (auto const &p3 : data->pairs)
            {
                if (p3.first == p1.first && p3.second == p2.second)
                {
                    found = true;
                    is_antitransitive = false;
                    break;
                }
            }
            if (!found)
            {
                is_transitive = false;
            }
        }
    }

    for (size_t i = 0; i < data->set.size(); ++i) 
    {
        for (size_t j = i + 1; j < data->set.size(); ++j) 
        {
            char a = data->set[i];
            char b = data->set[j];
            bool found_ab = false, found_ba = false;
            
            for (auto const &pair : data->pairs)
            {
                if (pair.first == a && pair.second == b) found_ab = true;
                if (pair.first == b && pair.second == a) found_ba = true;
            }
            
            if (!found_ab && !found_ba) 
            {
                is_complete = false;
                break;
            }
        }
        if (!is_complete) break;
    }

    if (is_reflexive && is_symmetric && is_transitive) {data->is_equivalence_relation = true;}

    if (is_antisymmetric && is_transitive) {data->is_order_relation= true;}

        std::cout << "Свойства отношения:" << std::endl;
    std::cout << "1. Рефлексивность:          " << (is_reflexive ? "+" : "-") << std::endl;
    std::cout << "2. Антирефлексивность:      " << (is_antireflexive ? "+" : "-") << std::endl;
    std::cout << "3. Симметричность:          " << (is_symmetric ? "+" : "-") << std::endl;
    std::cout << "4. Антисимметричность:      " << (is_antisymmetric ? "+" : "-") << std::endl;
    std::cout << "5. Асимметричность:         " << (is_asymmetric ? "+" : "-") << std::endl;
    std::cout << "6. Транзитивность:          " << (is_transitive ? "+" : "-") << std::endl;
    std::cout << "7. Антитранзитивность:      " << (is_antitransitive ? "+" : "-") << std::endl;
    std::cout << "8. Полнота:                 " << (is_complete ? "+" : "-") << std::endl << std::endl;


    std::cout << "Является ли отношением эквивалентности   " << (data->is_equivalence_relation ? "ДА": "НЕТ") << std::endl;
    std::cout << "Является ли отношением порядка           " << (data->is_order_relation? "ДА": "НЕТ") << std::endl;
}

void print_equivalence_info(Datastruct const *data)
{
    
}

void print_min_max_elements(Datastruct const *data)
{

}

int main()
{
    std::string path;
    std::cin >> path;

    std::ifstream input_file(path);
    if (!input_file.is_open())
    {
        throw std::runtime_error("file isn't open");
    }

    Datastruct data = extract_data_from_file(input_file);
    check_relation(&data);
    if (data.is_equivalence_relation)
    {
        print_equivalence_info(&data);
    }

    if (data.is_order_relation)
    {
        print_min_max_elements(&data);
    }

    return 0;
}