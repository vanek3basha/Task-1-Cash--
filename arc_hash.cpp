#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>

//TODO создать hpp
//TODO сейчас класс принимает обьект только с ключами, нужно добавить функцию для обработки хранилища или дать возможность передать функцию которая это сделает
//TODO класс одновременно должен поддерживать все варианты компиляции 
//TODO почистить код

template <typename cash_elem_T, typename key_T>
class cash_t
{
    
    private:
        struct cash_T_node_t
        {
            key_T key;
            cash_elem_T cash_elem;
        };
    
    
        size_t capacity;
        size_t par;

        std::list<cash_T_node_t> cash_list_T1;
        std::list<cash_T_node_t> cash_list_T2;
        std::list<key_T> cash_list_B1;
        std::list<key_T> cash_list_B2;

        
        enum ListId
        {
            NO_ID,
            T1_ID,
            B1_ID,
            T2_ID,
            B2_ID
        };
        
        enum GetFlag
        {
            NO_HIT,
            T1_HIT,
            B1_HIT,
            T2_HIT,
            B2_HIT,
            FROM_HIT,
            ERROR_IN_GET
        };
        
        std::unordered_map<key_T, std::pair<typename std::list<cash_T_node_t>::iterator, ListId>> cash_map_T;
        std::unordered_map<key_T, std::pair<typename std::list<key_T>::iterator, ListId>> cash_map_B;
        std::unordered_map<key_T, cash_elem_T>& storage;
    
    using map_T_it_t = typename std::unordered_map<key_T, std::pair<typename std::list<cash_T_node_t>::iterator, ListId>>::iterator;
    using map_B_it_t = typename std::unordered_map<key_T, std::pair<typename std::list<key_T>::iterator, ListId>>::iterator;

    using list_T_it_t = typename std::list<cash_T_node_t>::iterator;
    using list_B_it_t = typename std::list<key_T>::iterator;


    public:
        cash_t(size_t input_capacity, size_t input_par, std::unordered_map<key_T, cash_elem_T>& input_storage): capacity(input_capacity), par(input_par), storage(input_storage)
        {

        }

        bool ParIncreased()
        {
            par += 1;
            if(cash_list_T2.size() >= (capacity - par))
            {
                key_T oldest_key_T2 = cash_list_T2.back().key;
                cash_list_T2.pop_back();
                cash_list_B2.push_front(oldest_key_T2);
                cash_map_B.emplace(oldest_key_T2, std::pair{cash_list_B2.begin(), B2_ID});
                cash_map_T.erase(oldest_key_T2); 
            }

            if(cash_list_B1.size() >= (capacity - par))
            {
                key_T oldest_key_B1 = cash_list_B1.back();
                cash_list_B1.pop_back();
                cash_map_B.erase(oldest_key_B1);
            }

            return 1;
        }

        bool ParDecreased()
        {
            par -= 1;
            if(cash_list_T1.size() >= par)
            {
                key_T oldest_key_T1 = cash_list_T1.back().key;
                cash_list_T1.pop_back();
                cash_list_B1.push_front(oldest_key_T1);
                cash_map_B.emplace(oldest_key_T1, std::pair{cash_list_B1.begin(), B1_ID});
                cash_map_T.erase(oldest_key_T1);                
            }

            if(cash_list_B2.size() >= par)
            {
                key_T oldest_key_B2 = cash_list_B2.back();
                cash_list_B2.pop_back();
                cash_map_B.erase(oldest_key_B2);
            }

            return 1;
        }

        GetFlag get_cash(key_T key, cash_elem_T& elem)
        {
            map_T_it_t map_it_T = cash_map_T.find(key);
            
            if(map_it_T == cash_map_T.end())
            {
                map_B_it_t map_it_B = cash_map_B.find(key);

                if(map_it_B == cash_map_B.end())
                {
                    return NO_HIT;
                }

                list_B_it_t list_B_it = map_it_B->second.first;
                ListId list_B_id = map_it_B->second.second;
                
                if(list_B_id == B1_ID)
                {
                    cash_list_B1.erase(list_B_it);
                    cash_map_B.erase(map_it_B);

                    ParIncreased();
                    
                    return B1_HIT;
                }

                if(list_B_id == B2_ID)
                {
                    cash_list_B2.erase(list_B_it);
                    cash_map_B.erase(map_it_B);
                    
                    ParDecreased();
                    
                    return B2_HIT;
                }

                return ERROR_IN_GET;
            }

            list_T_it_t list_T_it = map_it_T->second.first;
            ListId list_T_id = map_it_T->second.second;
            elem = list_T_it->cash_elem;

            if(list_T_id == T1_ID)
            {
                if(cash_list_T2.size() >= (capacity - par))
                {
                    key_T oldest_key_T2 = cash_list_T2.back().key;
                    cash_list_T2.pop_back();
                    cash_list_B2.push_front(oldest_key_T2);
                
                    cash_map_B.emplace(oldest_key_T2, std::pair{cash_list_B2.begin(), B2_ID});
                    cash_map_T.erase(oldest_key_T2);
                }

                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T1, list_T_it);
                map_it_T->second.second = T2_ID;
                return T1_HIT;
            }

            if(list_T_id == T2_ID)
            {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T2, list_T_it);
                return T2_HIT;
            }

            return ERROR_IN_GET;
        }

        GetFlag get(key_T key, cash_elem_T& elem)
        {
            GetFlag result_get = get_cash(key, elem);
            
            if(result_get == ERROR_IN_GET)
            {
                return ERROR_IN_GET;
            }

            if(result_get == T1_HIT || result_get == T2_HIT)
            {
                return result_get;
            }

            if((result_get == B1_HIT) || (result_get == B2_HIT))
            {
                GetFlag result_get_from = get_from(key, elem);
                if(result_get_from ==  FROM_HIT)
                {
                    put_in_T2(key, elem);
                    return result_get;
                }

                return ERROR_IN_GET;
            }

            if(result_get == NO_HIT)
            {
                GetFlag result_get_from = get_from(key, elem);
                if(result_get_from == FROM_HIT)
                {
                    put_in_T1(key, elem);
                    return FROM_HIT;
                }

                return NO_HIT;
            }

            return ERROR_IN_GET;
        }

        void put_in_T2(key_T key, cash_elem_T elem)
        {
            if(cash_list_T2.size() >= (capacity - par))
            {
                key_T oldest_key_T2 = cash_list_T2.back().key;
                cash_list_T2.pop_back();
                cash_list_B2.push_front(oldest_key_T2);
                cash_map_B.emplace(oldest_key_T2, std::pair{cash_list_B2.begin(), B2_ID});
                cash_map_T.erase(oldest_key_T2);
            }

            if(cash_list_B2.size() >= par)
            {
                key_T oldest_key_B2 = cash_list_B2.back();
                cash_list_B2.pop_back();
                cash_map_B.erase(oldest_key_B2);
            }

            cash_list_T2.push_front(cash_T_node_t{key, elem});
            cash_map_T.emplace(key, std::pair{cash_list_T2.begin(), T2_ID});
            return;
        }

        void put_in_T1(key_T key, cash_elem_T elem)
        {
            if(cash_list_T1.size() >= par)
            {
                key_T oldest_key_T1 = cash_list_T1.back().key;
                cash_list_T1.pop_back();
                cash_list_B1.push_front(oldest_key_T1);
                cash_map_B.emplace(oldest_key_T1, std::pair{cash_list_B1.begin(), B1_ID});
                cash_map_T.erase(oldest_key_T1);
            }

            if(cash_list_B1.size() >= (capacity - par))
            {
                key_T oldest_key_B1 = cash_list_B1.back();
                cash_list_B1.pop_back();
                cash_map_B.erase(oldest_key_B1);
            }

            cash_list_T1.push_front(cash_T_node_t{key, elem});
            cash_map_T.emplace(key, std::pair{cash_list_T1.begin(), T1_ID});
            return;
        }

        GetFlag get_from(key_T key, cash_elem_T& elem)
        {
            auto it = storage.find(key);

            if(it == storage.end())
            {
            return NO_HIT;
            }

            elem = it->second;

            return FROM_HIT;
        }

        void print_state()
        {
            std::cout << "\nT1: ";
            for(auto& node : cash_list_T1)
                std::cout << node.key << ' ';
        
            std::cout << "\nT2: ";
            for(auto& node : cash_list_T2)
                std::cout << node.key << ' ';
        
            std::cout << "\nB1: ";
            for(auto& key : cash_list_B1)
                std::cout << key << ' ';
        
            std::cout << "\nB2: ";
            for(auto& key : cash_list_B2)
                std::cout << key << ' ';
        
            std::cout << "\npar = " << par << '\n';
            std::cout << "-----------------\n";
        }

        ~cash_t() = default;

};

int main()
{
    std::unordered_map<int, int> storage =
    {
        {1, 10},
        {2, 20},
        {3, 30},
        {4, 40},
        {5, 50},
        {6, 60},
        {7, 70},
        {8, 80}
    };

    cash_t<int, int> cache(4, 2, storage);

    int value = -1;

    std::cout << "============================\n";
    std::cout << "1. get(1) - cold miss\n";
    std::cout << "============================\n";

    auto result = cache.get(1, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "2. get(2) - cold miss\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(2, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "3. get(1) - T1 - HIT\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(1, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "4. get(2) - T1 hit\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(2, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "5. get(4) - cold miss\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(3, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "6. get(5) - cold miss / eviction\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(3, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "7. get(8)\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(8, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "8. get(6) - cold miss\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(6, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    std::cout << "\n============================\n";
    std::cout << "9. get(999) - missing in storage\n";
    std::cout << "============================\n";

    value = -1;
    result = cache.get(999, value);

    std::cout << "result = " << result
              << ", value = " << value << '\n';

    cache.print_state();


    return 0;
}