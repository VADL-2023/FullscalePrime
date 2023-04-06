// C++ program to demonstrate
// multithreading using three
// different callables.
#include <iostream>
#include <thread>
using namespace std;

class A {
    public:
        bool stop;
        A():stop(false) {

        }

};

// A dummy function
void foo(A* the_a)
{
    //for (int i = 0; i < Z; i++)
    int i = 0;
    while(!the_a->stop)
    {
        cout << "Thread using function"
                " pointer as callable " << i << endl;
        i++;
    }
}

// A callable object
class thread_obj
{
public:
    void operator()(int x)
    {
        for (int i = 0; i < x; i++)
            cout << "Thread using function"
                    " object as callable\n";
    }
};



// Driver code
int main()
{
    cout << "Threads 1 and 2 and 3 "
            "operating independently"
         << endl;
    A a;
    // This thread is launched by using
    // function pointer as callable
    thread th1(foo, &a);
    int j = 0;
    while(j < 100000) {
        j++;
    }
    a.stop = true;
    // Wait for the threads to finish
    // Wait for thread t1 to finish
    th1.join();
    std::cout << "DONE" << std::endl;
    return 0;
}
