#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "linked_stack.cpp"
#include "linked_queue.cpp"

using namespace std;
using namespace structures;

typedef struct image_s {
    string* name;
    int width;
    int height;
    string* data;
} image_t;

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

bool build_images_from_file(const string filename, LinkedQueue<image_t*>* queue) {
    string line, tag, content;
    ifstream file;
    file.open(filename);

    bool inside_tag = false;

    image_t* image = (image_t*) malloc(sizeof(image_t));

    while (getline(file, line)) {
        for (char c : line) {
            cout << c << endl; 

            if (c == '<') {
                tag = content = "";
                inside_tag = true;
                continue;
            }
            if (c == '>') {
                if (tag.compare("/name") == 0)
                    *image->name = content;
                // if (tag.compare("/width") == 0)
                //     image->width = stoi(content.data());
                // if (tag.compare("/height") == 0)
                //     image->height = stoi(content.data());
                if (tag.compare("/data") == 0) {
                    *image->data = content;
                    queue->enqueue(image);
                    image = (image_t*) malloc(sizeof(image_t));
                }
                inside_tag = false;
                continue;
            }
            if (inside_tag) {
                tag += c;
            } else {
                content += c;
            }
        }
    }
    free(image);
}

int main () {
    cout << verify_xml_file("./datasets/dataset01.xml") << endl;

    LinkedQueue<image_t*>* image_queue = new LinkedQueue<image_t*>();
    build_images_from_file("./datasets/dataset02.xml", image_queue);
    while (!image_queue->empty()) {
        cout << *image_queue->dequeue()->name << endl;
    }

    return 0;
}