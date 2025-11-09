#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Node
{
    char data;
    Node *next;

    Node(char value) : data(value), next(nullptr) {} 

};

class Set
{
    private:
    Node *head;
    static std::vector<Set*> all_sets;

    public:
    Set() : head(nullptr) {}

    Set(char name)
    {
        if (name < 'A' || name > 'Z')
        {
            throw std::invalid_argument("the name of set must be in the range [A, Z]");
        }

    Set *set = find_set(name);
        if (set != nullptr)
        {
            throw std::logic_error("the set already exist");
        }

        head = new Node(name);
        all_sets.push_back(this);
    }

    public:
    char get_name() const
    {
        if (head == nullptr)
        {
            throw std::logic_error("set is not initialized");
        }
        return head->data;
    }

    public:
    static Set *find_set(char name) 
    {
        for (Set *set : all_sets)
        {
            if (set->head->data == name)
            {
                return set;
            }
        }
        return nullptr;
    }

    public:
    void add(char value)
    {
        if (head == nullptr)
        {
            throw std::logic_error("first, create a set");
        }

        Node *new_node = new Node(value);
        Node *prev_iter = nullptr;
        Node *curr_iter  = head->next;
        while (curr_iter != nullptr && value > curr_iter->data)
        {
            prev_iter = curr_iter;
            curr_iter = curr_iter->next;
        }

        if (curr_iter && value == curr_iter->data)
        {
            delete new_node;
            return;
        }

        if (prev_iter == nullptr)
        {
            new_node->next = head->next;
            head->next = new_node;
            return;
        }
        prev_iter->next = new_node;
        new_node->next = curr_iter;
    }

    // void del(char value)
    // {
    //     if (head == nullptr)
    //     {
    //         std::invalid_argument("first, create a set");
    //     }
    //     if (head->next == nullptr)
    //     {
    //         return;
    //     }

    //     Node *temp_ptr;
    //     Node *curr_iter = head->next;
    //     Node *prev_iter = nullptr;
    //     // if (value == curr_iter->data)
    //     // {

    //     // }
    //     while(curr_iter != nullptr && curr_iter->data != value)
    //     {
    //         prev_iter = curr_iter;
    //         curr_iter = curr_iter->next;
    //     }
    //     if (curr_iter != nullptr)
    //     {
    //         temp_ptr = curr_iter;
    //         prev_iter->next = curr_iter->next;
            
    //     }        
    // }

    static void see()
    {
        if (all_sets.size() == 0)
        {
            throw std::logic_error("not a single set has been created yet");
        }

        for (Set *set : all_sets)
        {
            set->see(set->head->data);
        }
    }

    void see(char set_name) const
    {
        if (find_set(set_name) == nullptr)
        {
            throw std::invalid_argument("set is not exist");
        }

        printf("elements of %c set: ", head->data);
        Node *iter = head->next;
        while (iter)
        {
            printf("%c ", iter->data);
            iter = iter->next;
        }
        std::cout << std::endl;
    }

};

std::vector<Set*> Set::all_sets = {};

int main()
{
    Set A('A');
    Set C('C');
    printf("set %c was created\n", A.get_name());

    try 
    {
    Set B('A');
    } catch (std::logic_error) 
    {
        printf("Set with name A already exist\n");
    }

    A.add('a');
    A.add('c');
    A.add('b');

    C.add('a');
    C.add('g');
    C.add('c');
    C.add('J');
    C.add('Z');
    C.add('g');
    C.add('#');
    Set::see();


    return 0;
}