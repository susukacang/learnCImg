#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
// #include "doAnything.h"
// #include "CImg.h"
using namespace std;
// using namespace cimg_library;


// void doSomething();
// void doSomething() {
//     cout << "did something" << endl;
// }
int rotateImageMain();
// CImg<unsigned char> rotateImage(const CImg<unsigned char> &img, int type);

int processImageMain();

int main()
{
    vector<string> msg{"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string &word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    // doSomething();
    // doAnything();
    // rotateImageMain();
    processImageMain();

    return 0;
}
