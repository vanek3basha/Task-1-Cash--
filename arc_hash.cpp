#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>

using namespace std;



template <typename cash_elem_T, typename key_T, typename cash_node_t>
class arc_cash_t
{
    
    private:
        struct cash_T_node_t
        {
            cash_elem_T cash_elem;
            key_T key;
        };
    
    
        size_t capacity;
        size_t p;

        list<cash_node_t> cash_list_T1;
        list<cash_node_t> cash_list_T2;
        list<key_T> cash_list_B1;
        list<key_T> cash_list_B2;

        unordered_map<key_T, pair<typename list<cash_node_t>::iterator, int>> cash_map;
    
        enum ListId
        {
            T1_ID,
            B1_ID,
            T2_ID,
            B2_ID
        };
    
    using map_it_t = typename std::unordered_map<key_T, pair<typename list<cash_node_t>::iterator, int>>::iterator;
    using list_it_t = typename list<cash_node_t>::iterator;

    public:
        arc_cash_t(size_t input_capacity, size_t input_p)
        {
            capacity = input_capacity;
            p = input_p;
        }

        arc_cash_t(size_t input_capacity)
        {
            capacity = input_capacity;
            p = capacity / 2;
        }


        bool get(key_T key, cash_elem_T& elem)
        {
                        map_it_t map_it = cash_map.find(key);
            
            if(map_it == cash_map.end())
            {
                cout << "Элемент по вашему ключу не найден" << endl; 
                return 0;
            }
            
            list_it_t list_it = map_it->second.first;
            int list_id       = map_it->second.second;
            elem = list_it->cash_elem;

            if(list_id == T1_ID)
            {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T1, list_it);
                p = p - 1;
                map_it->second.second = T2_ID;
                return 1;
            }

            if (list_id == T2_ID)
            {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T2, list_it);
                return 1;
            }
            
            return -1;
        }


        bool get(key_T key)
        {
            map_it_t map_it = cash_map.find(key);
            
            if(map_it == cash_map.end())
            {
                cout << "Элемент по вашему ключу не найден" << endl; 
                return 0;
            }
            
            list_it_t list_it = map_it->second.first;
            int list_id       = map_it->second.second;

            if(list_id == T1_ID)
            {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T1, list_it);

                p = p - 1;
                map_it->second.second = T2_ID;
                return 1;
            }

            if (list_id == T2_ID)
            {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T2, list_it);
                return 1;
            }
            
            return -1;
        }

        void put(key_T key, cash_elem_T elem)
        {
                map_it_t map_it = cash_map.find(key);
                
                if(map_it == cash_map.end())
                {   
                    if(cash_list_T1.size() + cash_list_T2.size() >= capacity)
                    {
                        key_T oldest_key = cash_list_T1.back().key;
                        cash_list_T1.pop_back();
                        cash_map.erase(oldest_key);
                    }

                    cash_list_T1.push_front(cash_node_t{elem, key});
                    cash_map[key] = pair(cash_list_T1.begin(), T1_ID);
                    
                    return;
                }

                list_it_t list_it = map_it->second.first;
                int list_id       = map_it->second.second;

                if(list_id == T1_ID)
                {
                    cash_list_T2.splice(cash_list_T2.begin(), cash_list_T1, list_it);
                    p = p - 1;
                    map_it->second.second = T2_ID;
                    return;
                }

                if (list_id == T2_ID)
                {
                cash_list_T2.splice(cash_list_T2.begin(), cash_list_T2, list_it);
                return ;
                }

                return;
        }

        ~arc_cash_t()
        {

        }

};

int main()
{
    arc_cash_t<int, int> cash_test(4, 2);
    // vector<int, int> vector_test;
    for(size_t i = 1; i <= 10; i++)
    {
        cash_test.put(i, i * 10);
        cash_test.get(1);
    }
    int value = 0;
    cash_test.get(1, value);
    cout << value << endl;
    return 0;
}