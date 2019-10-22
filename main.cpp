#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "./linked_stack.cpp"
#include "./linked_queue.cpp"

//! Namespace que engloba as estruturas utilizadas.
/*! 
    O namespace "structures" contém as classes LinkedQueue e
    LinkedStack, bem como a estrutura (struct) pos_s, e seu
    typedef pos_t.
 */
namespace structures {

//! Struct que representa a posição (x, y) de um píxel na imagem.
typedef struct pos_s {
    unsigned int x;
    unsigned int y;
} pos_t;

}  // namespace structures

using namespace std;
using namespace structures;

//! Namespace que engloba as funções do trabalho.
/*! 
    O namespace "functions" contém as funções verify_xml_file,
    connected_components, build_matrix e process_xml_file.
 */
namespace functions {

//! Função que valida um arquivo XML.
/*!
    \param filename uma string que representa o caminho do arquivo XML.
    \return um boolean.

    A função verify_xml_file itera sobre o arquivo XML, cujo caminho é
    passado como parâmetro, e valida o posicionamento das tags. Ela itera
    por cada caracter do arquivo e constrói as tags concatenando esses
    caracteres de acordo com os sinais de abertura e fechamento de tags
    ("<" e ">"). Sempre que encontra um "<", a flag booleana "inside_tag"
    recebe true. Quando se está dentro de uma tag, todo caracter diferente
    de "<" e ">" é concatenado à string "tag". Ao final da tag, quando se 
    encontra um caracter ">", checa-se se a tag encontrada é de abertura
    ou de fechamento. Se for de abertura, ela é adicionada ao topo da pilha.
    Caso contrário, deve-se remover o topo da fila e compará-lo à tag.
    Se a tag atual e o topo da fila não forem correspondentes, ou se a fila
    estiver vazia, o XML é inválido e retorna-se falso.
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

    A função build_matrix é usada para montar uma matriz de zeros, utilizada
    como matriz de resultado para o algoritmo de rotulação dos componentes
    conexos nas imagens binárias do XML. A matriz criada por essa função é na
    verdade representada por um vetor, por maior simplicidade na manipulação
    dos ponteiros. Essa matriz-vetor é alocada, preenchida com zeros e um
    ponteiro é retornado.
*/
int* build_matrix(int width, int height) {
    int size = width * height;
    int* matrix = (int*) malloc(sizeof(int) * size);
    for (int i = 0; i != size; i++) {
        matrix[i] = 0;
    }
    return matrix;
}

//! Função que encontra os elementos 4-conectados em uma imagem binária.
/*!
    \param name uma string que representa o nome da imagem binária.
    \param data uma string contendo a imagem binária.
    \param width um inteiro que representa a largura da imagem.
    \param height um inteiro que representa a altura da imagem.
    \return um inteiro que corresponde ao número de componentes 4-conectados na imagem.

    A função connected_components recebe uma string "data" que representa uma
    imagem binária do XML. Ela recebe também o nome, a largura e a altura dessa
    imagem, e utiliza isso para encontrar e classificar cada componente conexo
    na imagem. São usadas funções lambda dentro da função para reduzir a repetição
    de código. O algoritmo de rotulação utilizado baseia-se em percorrer a imagem, 
    e para cada valor 1 ainda não visitado na imagem, atribui-se o rótulo atual
    àquela posição, que depois é colocada na fila. Em seguida, enquanto a fila
    não estiver vazia, remove-se uma posição da fila e checa-se cada um de seus
    vizinhos (apenas aqueles que são posições válidas na imagem). Caso algum
    vizinho seja igual a 1 e ainda não tenha sido visitado, então este também
    recebe o rótulo atual e é colocado na fila. Quando a fila fica vazia, o
    rótulo atual é incrementado e o processo se repete até que todas as posições
    tenham sido visitadas. Ao final, retorna-se o valor do último rótulo atribuido
    menos 1.
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
    return label - 1;
}

//! Função que itera pelo arquivo XML e processa todas as imagens binárias encontradas.
/*!
    \param filename uma string que representa o nome do arquivo XML.

    A função process_xml_file simplesmente itera pelo arquivo XML passado por parâmetro,
    caracter por caracter, da mesma forma que a função verify_xml_file, montando as
    tags que encontra. Quando encontra uma tag de fechamento "</data>", presume-se que
    já se tenha passado pelas tags "name", "width", "height" e a própria tag "data".
    Portanto, tem-se uma imagem binária com todos os seus atributos, e então se pode
    processar essa imagem utilizando a função connected_components.
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

}  // namespace functions

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