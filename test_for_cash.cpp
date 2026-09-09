#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>

using namespace std;



template <typename cash_elem_T, typename key_T>
class cash_t
{
    
    private:
        struct cash_node_t
        {
            cash_elem_T cash_elem;
            key_T key;
        };
    
    
        size_t capacity;
        list<cash_node_t> cash_list;
        unordered_map<key_T, typename list<cash_node_t>::iterator> cash_map;
    
    
    using map_it_t = typename std::unordered_map<key_T, typename std::list<cash_node_t>::iterator>::iterator;

    public:
        cash_t(size_t input_capacity)
        {
            capacity = input_capacity;
        }

        bool get(key_T key, cash_elem_T& elem)
        {
            map_it_t map_it = cash_map.find(key);
            
            if(map_it == cash_map.end())
            {
                cout << "Элемент по вашему ключу не найден" << endl; 
                return 0;
            }

            cash_list.splice(cash_list.begin(), cash_list, map_it->second);

            elem = map_it->second->cash_elem;

            return 1;
        }


        bool get(key_T key)
        {
            map_it_t map_it = cash_map.find(key);
            
            if(map_it == cash_map.end())
            {
                cout << "Элемент по вашему ключу не найден" << endl; 
                return 0;
            }

            cash_list.splice(cash_list.begin(), cash_list, map_it->second);

            return 1;
        }

        void put(key_T key, cash_elem_T elem)
        {
                map_it_t map_it = cash_map.find(key);
                
                if(map_it != cash_map.end())
                {   
                    map_it->second->cash_elem = elem;
                    cash_list.splice(cash_list.begin(), cash_list, map_it->second);
                    return;
                }

                if(cash_list.size() >= capacity)
                {
                    key_T oldest_key = cash_list.back().key;
                    cash_list.pop_back();
                    cash_map.erase(oldest_key);
                }

                cash_list.push_front(cash_node_t{elem, key});
                cash_map[key] = cash_list.begin();

                return;
        }

        bool get_ARC(key_T, cash_elem_T& elem)
        {
            
        }


        ~cash_t()
        {

        }

};

int main()
{
    cash_t<int, int> cash_test(4);
    // vector<int, int> vector_test;
    for(size_t i = 1; i <= 10; i++)
    {
        cash_test.put(i, i * 10);
    }
    int value = 0;
    cash_test.get(10, value);
    cout << value << endl;
    return 0;
}