#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

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

    ~Set()
    {
        Node *curr_iter = head;
        while (curr_iter)
        {
            Node *next = curr_iter->next;
            delete curr_iter;
            curr_iter = next;
        }

    auto it = std::find(all_sets.begin(), all_sets.end(), this);
    if (it != all_sets.end())
    {
        all_sets.erase(it);
    }
    }

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

    public:
    void rem(char value)
    {
        if (head == nullptr)
        {
            std::invalid_argument("first, create a set");
        }
        if (head->next == nullptr)
        {
            return;
        }

        Node *temp_ptr;
        Node *curr_iter = head->next;
        Node *prev_iter = head;
        while(curr_iter != nullptr && curr_iter->data != value)
        {
            prev_iter = curr_iter;
            curr_iter = curr_iter->next;
        }
        if (curr_iter != nullptr)
        {
            prev_iter->next = curr_iter->next;
            delete curr_iter;
            
        }        
    }

    public:
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

    public:
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

    public:
    void pow()
    {
        std::vector<char> elements;
        Node* current = head->next;
        while (current)
        {
            elements.push_back(current->data);
            current = current->next;
        }

        int n = elements.size();
        int total = 1 << n;

        std::cout << "Boolean of set " << head->data << ": {\n";
        
        for (int mask = 0; mask < total; mask++)
        {
            std::cout << "  {";
            bool first = true;
            
            for (int i = 0; i < n; i++)
            {
                if (mask & (1 << i))
                {
                    if (!first) std::cout << ", ";
                    std::cout << elements[i];
                    first = false;
                }
            }
            std::cout << "}";
            if (mask < total - 1) std::cout << ",";
            std::cout << "\n";
        }
        std::cout << "}" << std::endl;
    }

    public:
    Set* union_merge(const Set& other) const
    {
        if (head == nullptr || other.head == nullptr)
        {
            throw std::logic_error("sets must be initialized");
        }

        char new_name = 'A';
        for (int i = 0; i < 26; i++)
        {
            new_name = 'A' + i;
            if (Set::find_set(new_name) == nullptr) break;
        }

        Set* result = new Set(new_name);

        Node* currentA = this->head->next;
        Node* currentB = other.head->next;

        while (currentA && currentB)
        {
            if (currentA->data < currentB->data)
            {
                result->add(currentA->data);
                currentA = currentA->next;
            }
            else if (currentA->data > currentB->data)
            {
                result->add(currentB->data);
                currentB = currentB->next;
            }
            else
            {
                result->add(currentA->data);
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        while (currentA) {
            result->add(currentA->data);
            currentA = currentA->next;
        }
        while (currentB) {
            result->add(currentB->data);
            currentB = currentB->next;
        }

        return result;
    }

    public:
    Set* intersection_merge(const Set& other) const
    {
        if (head == nullptr || other.head == nullptr)
        {
            throw std::logic_error("sets must be initialized");
        }

        char new_name = 'A';
        for (int i = 0; i < 26; i++)
        {
            new_name = 'A' + i;
            if (Set::find_set(new_name) == nullptr) break;
        }



        Set* result = new Set(new_name);

        Node* currentA = this->head->next;
        Node* currentB = other.head->next;

        while (currentA != nullptr && currentB != nullptr)
        {
            if (currentA->data < currentB->data)
            {
                currentA = currentA->next;
            }
            else if (currentA->data > currentB->data)
            {
                currentB = currentB->next;
            }
            else
            {
                result->add(currentA->data);
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }
        return result;
    }

    public:
    Set* difference_merge(const Set& other) const
    {
        if (head == nullptr || other.head == nullptr)
        {
            throw std::logic_error("sets must be initialized");
        }

        char new_name = 'A';
        for (int i = 0; i < 26; i++)
        {
            new_name = 'A' + i;
            if (Set::find_set(new_name) == nullptr) break;
        }

        Set* result = new Set(new_name);

        Node* currentA = this->head->next;
        Node* currentB = other.head->next;

        while (currentA != nullptr && currentB != nullptr)
        {
            if (currentA->data < currentB->data)
            {
                
                result->add(currentA->data);
                currentA = currentA->next;
            }
            else if (currentA->data > currentB->data)
            {
               
                currentB = currentB->next;
            }
            else
            {
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }
        while (currentA != nullptr)
        {
            result->add(currentA->data);
            currentA = currentA->next;
        }

        return result;
    }

    public:
    bool is_subset_of(const Set& other) const
    {
        if (head == nullptr || other.head == nullptr)
        {
            throw std::logic_error("sets must be initialized");
        }

        Node* currentA = this->head->next;
        Node* currentB = other.head->next;

        while (currentA != nullptr && currentB != nullptr)
        {
            if (currentA->data < currentB->data)
            {
                return false;
            }
            else if (currentA->data > currentB->data)
            {
                currentB = currentB->next;
            }
            else
            {
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        return (currentA == nullptr);
    }

    private:
    bool is_equal_to(const Set& other) const
    {
        if (head == nullptr || other.head == nullptr)
        {
            throw std::logic_error("sets must be initialized");
        }

        Node* currentA = this->head->next;
        Node* currentB = other.head->next;

        while (currentA != nullptr && currentB != nullptr)
        {
            if (currentA->data != currentB->data)
            {
                return false;
            }
            currentA = currentA->next;
            currentB = currentB->next;
        }
        return (currentA == nullptr && currentB == nullptr);
    }

    public:
    bool operator<(const Set& other) const 
    {
        return is_subset_of(other) && !is_equal_to(other);
    }

    bool operator<=(const Set& other) const 
    {
        return is_subset_of(other);
    }


    bool operator==(const Set& other) const 
    {
        return is_equal_to(other);
    }

    bool operator!=(const Set& other) const 
    {
        return !is_equal_to(other);
    }
};

std::vector<Set*> Set::all_sets = {};

int main()
{
    printf("=== TEST of class set STARTING... ===\n\n");

    Set A('A');
    Set B('B');
    Set C('C');
    
    A.add('a'); A.add('b'); A.add('c');
    B.add('b'); B.add('c'); B.add('d');
    C.add('a'); C.add('c'); C.add('e');

    printf("1. METHOD object.see(name):\n");
    printf("A: "); A.see(A.get_name());
    printf("B: "); B.see(B.get_name());
    printf("C: "); C.see(C.get_name());
    printf("\n");

    printf("2. UNION:\n");
    Set* U1 = A.union_merge(B);
    printf("A ∪ B: "); U1->see(U1->get_name());
    delete U1;
    
    Set* U2 = A.union_merge(C);
    printf("A ∪ C: "); U2->see(U2->get_name());
    delete U2;
    printf("\n");

    printf("3. INTERSECT:\n");
    Set* I1 = A.intersection_merge(B);
    printf("A ∩ B: "); I1->see(I1->get_name());
    delete I1;
    
    Set* I2 = A.intersection_merge(C);
    printf("A ∩ C: "); I2->see(I2->get_name());
    delete I2;
    printf("\n");

    printf("4. DIFFERENCE:\n");
    Set* D1 = A.difference_merge(B);
    printf("A - B: "); D1->see(D1->get_name());
    delete D1;
    
    Set* D2 = B.difference_merge(A);
    printf("B - A: "); D2->see(D2->get_name());
    delete D2;
    printf("\n");

    printf("5. SUBSETS:\n");
    Set D('D');
    D.add('a'); D.add('c');
    printf("D: "); D.see(D.get_name());
    
    printf("D ⊆ A: %s\n", D.is_subset_of(A) ? "true" : "false");
    printf("D ⊆ B: %s\n", D.is_subset_of(B) ? "true" : "false");
    printf("D ⊂ A: %s\n", (D < A) ? "true" : "false");
    printf("\n");

    printf("6. EQUALITY:\n");
    Set E('E');
    E.add('a'); E.add('b'); E.add('c');
    printf("E: "); E.see(E.get_name());
    
    printf("A == E: %s\n", (A == E) ? "true" : "false");
    printf("A == B: %s\n", (A == B) ? "true" : "false");
    printf("A != C: %s\n", (A != C) ? "true" : "false");
    printf("\n");

    printf("7. POW(BULEAN):\n");
    Set F('F');
    F.add('x'); F.add('y'); F.add('#'), F.add('4'); F.add('Z'); F.add('0');
    printf("F: "); F.see(F.get_name());
    printf("Bulean F:\n"); 
    F.pow();
    printf("\n");

    printf("8. COMBINATED OPERATIONS:\n");
    Set* complex = A.union_merge(B);
    Set* complex_final = complex->intersection_merge(C);
    printf("(A ∪ B) ∩ C: "); complex_final->see(complex_final->get_name());
    delete complex;
    delete complex_final;
    printf("\n");

    printf("9. EMPTY SET:\n");
    Set Empty('G');
    printf("empty G: "); Empty.see(Empty.get_name());
    printf("∅ ⊆ A: %s\n", Empty.is_subset_of(A) ? "true" : "false");
    printf("∅ == ∅: %s\n", (Empty == Empty) ? "true" : "false");
    printf("\n");

    printf("10. METHOD see() for all sets:\n");
    Set::see();

    printf("\n=== TESTING COMPLETE ===\n");

    return 0;
}