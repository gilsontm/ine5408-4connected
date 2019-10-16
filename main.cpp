#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "./linked_stack.cpp"
#include "./linked_queue.cpp"

using namespace std;
using namespace structures;

//! ...
namespace functions {

//! Função que valida um arquivo XML.
/*!
    \param filename uma string que representa o caminho do arquivo XML.
    \return um boolean.
*/
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

//! Função que constroi uma matriz representada em um vetor e preenche-a com zeros.
/*!
    \param width um inteiro que corresponde à largura da matriz.
    \param height um inteiro que corresponde à altura da matriz.
    \return um ponteiro de inteiro que aponta para a matriz criada.
*/
int* build_matrix(int width, int height) {
    int size = width * height;
    int* matrix = (int*) malloc(sizeof(int) * size);
    for (int i = 0; i != size; i++) {
        matrix[i] = 0;
    }
    return matrix;
}

//! ...
typedef struct pos_s {
    unsigned int x;
    unsigned int y;
} pos_t;

//! Função que encontra os elementos 4-conectados em uma imagem binária.
/*!
    \param name uma string que representa o nome da imagem binária.
    \param data uma string contendo a imagem binária.
    \param width um inteiro que representa a largura da imagem.
    \param height um inteiro que representa a altura da imagem.
    \return um inteiro que corresponde ao número de componentes 4-conectados na imagem.
*/
int connected_components(const string name, const string data, int width, int height) {
    LinkedQueue<pos_t*>* queue = new LinkedQueue<pos_t*>();

    int* result = build_matrix(width, height);
    int label = 1;

    pos_t* position;

    unsigned int x, y;

    auto not_checked = [width, &data, &result] (int x, int y) -> bool {
        return data[y * width + x] == '1' && result[y * width + x] == 0;
    };

    auto enqueue_position = [&queue] (int x, int y) {
        pos_t* position = (pos_t*) malloc(sizeof(pos_t));
        position->x = x;
        position->y = y;
        queue->enqueue(position);
    };

    auto set_label = [&result, width, label] (int x, int y) {
        result[y * width + x] = label;
    };

    for (int i = 0; i != width; i++) {
        for (int j = 0; j != height; j++) {
            if (not_checked(i, j)) {
                enqueue_position(i, j);
                set_label(i, j);
                while (!queue->empty()) {
                    position = queue->dequeue();
                    x = position->x;
                    y = position->y;
                    free(position);
                    if (x - 1 < width) {
                        if (not_checked((x-1), y)) {
                            enqueue_position((x-1), y);
                            set_label((x-1), y);
                        }
                    }
                    if (x + 1 < width) {
                        if (not_checked((x+1), y)) {
                            enqueue_position((x+1), y);
                            set_label((x+1), y);
                        }
                    }
                    if (y - 1 < height) {
                        if (not_checked(x, (y-1))) {
                            enqueue_position(x, (y-1));
                            set_label(x, (y-1));
                        }
                    }
                    if (y + 1 < height) {
                        if (not_checked(x, (y+1))) {
                            enqueue_position(x, (y+1));
                            set_label(x, (y+1));
                        }
                    }
                }
                label++;
            }
        }
    }
    cout << name << " " << label - 1 << endl;
}

//! Função que itera pelo arquivo XML e processa todas as imagens binárias encontradas.
/*!
    \param filename uma string que representa o nome do arquivo XML.
*/
void process_xml_file(const string filename) {
    string line, tag, name, content;
    int width, height;
    ifstream file;
    file.open(filename);

    bool inside_tag = false;
    tag = content = "";

    while (getline(file, line)) {
        for (char c : line) {
            if (c == '<') {
                tag = "";
                inside_tag = true;
                continue;
            }
            if (c == '>') {
                if (tag.compare("/name") == 0) {
                    // cout << tag << endl;
                    name = content;
                }
                if (tag.compare("/width") == 0)
                    width = stoi(content);
                if (tag.compare("/height") == 0)
                    height = stoi(content);
                if (tag.compare("/data") == 0) {
                    connected_components(name, content, width, height);
                }
                inside_tag = false;
                content = "";
                continue;
            }
            if (inside_tag) {
                tag += c;
            } else {
                content += c;
            }
        }
    }
}

}

int main () {
    string file_path;
    cin >> file_path;
    if (functions::verify_xml_file(file_path)) {
        functions::process_xml_file(file_path);
    } else {
        cout << " error" << endl;
    }

    return 0;
}