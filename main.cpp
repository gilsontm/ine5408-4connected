#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "linked_stack.cpp"

using namespace std;
using namespace structures;


bool verify_xml_file (const string filename) {
    string line, tag;    
    ifstream file;
    file.open(filename);
    bool inside_tag = false;
    
    LinkedStack<string> stack = LinkedStack<string>();

    while (getline(file, line)) {
        for (char& c : line) {
            if (c == '<') {
                inside_tag = true;
                tag = "";
                continue;
            }
            if (c == '>') {
                if (!inside_tag || tag.size() < 1)
                    return false; // sintax error

                inside_tag = false;

                if (tag[0] == '/') {
                    // check top of the stack
                    if (stack.empty())
                        return false;

                    tag = tag.substr(1, tag.size() - 1);

                    if (tag.compare(stack.top()) != 0)
                        return false; // tags don't match
                    stack.pop(); // tags match
                } else
                    stack.push(tag);
                tag = "";
                continue;
            }
            if (inside_tag) tag += c;
        }
    }
    file.close();
    if (!stack.empty())
        return false;
    return true;
}

int main () {
    cout << verify_xml_file("./datasets/dataset01.xml") << endl;
    return 0;
}