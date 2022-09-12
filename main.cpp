#include <iostream>
#include "weak_ptr.h"

struct MyObject {
    int n;
    std::string s;
    cik::WeakPtrFactory<MyObject> weak_factory_{this};
};

int main(int, char **) {
    cik::WeakPtr<MyObject> weak_ptr;
    {
        MyObject object{1, "hello"};
        weak_ptr = object.weak_factory_.GetWeakPtr();
        if (weak_ptr) {
            std::cout << "n : " << weak_ptr->n << " , s : " << weak_ptr->s << std::endl;
        }
    }

    if (!weak_ptr) {
        std::cout << "weak_ptr invalid \n";
    } else {
        std::cout << "weak_ptr valid \n";
    }
}
