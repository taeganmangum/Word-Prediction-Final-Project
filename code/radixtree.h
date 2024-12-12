#ifndef RADIXTREE_H__
#define RADIXTREE_H__

#include <memory>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//The branching factor is max number of children a node can have
//it is equal to the number of letters in the alphabet
#define BRANCHING_FACTOR 26

//isLeaf is used to mark if the node is a word or not. The data is the word, partial word, or character contained in the string, and each node can have up to 26 children.
struct Node {
    bool isLeaf;
    string data;
    Node* children[BRANCHING_FACTOR];
};

//class contains the constructer and deconstructer.
class radixTree {
    public :
    radixTree();
    ~radixTree();
};
//I took the functions out of the class so that I could use them inside the main function for testing purposes.

//This function initiates a node. The data is set to an empty string, child nodes are set to NULL, and isLeaf is set to false.
Node* initNode();

//This functions finds the first NULL child node of the input node. This child node is iniated, marked as a leaf, and the input string is set as its data.
void buildNewNode(Node* node, string input);
    
//This function checks the input node's children. It checks the starting letter of each node's data to see if there is a match to the first letter of the input string. If no match is found, the input node is returned.
Node* search_children(Node* root, string input);

//This function compares the input string to the node's data character by character. It returns the position of the first mismatched character. If input and node data are the same word, the function returns -1.
int getMismatch(Node* node, string input);

//This function erases the matching prefix between the input string and the node's data.
//If there is no matching prefix, the original input string is returned.
string removePrefix(string input, Node* node);

//This function returns the end node (leaf) location of a word. If the word is not in the tree, it returns the node where it would be inserted.
Node* lookupNode (string input, Node* root);

//This functions returns true or false if a word is inside the tree.
bool searchWord(string word, Node* root);

//This function splits a target node as a helper function for insert. It determines the matching portion of the input string and node data, and keeps this matching piece as the new data for that node. It then adds the remaining piece of the original node data and the remaining input string and inserts each into child nodes.
void split(Node* node, string input);

//This function recursively traverses the tree to insert a word.
void insert(string input, Node* root);

//This function deletes a word from the tree. The node corresponding to the word is found using lookupNode. If the node has children, it is unmarked as a leaf. If it does not have children, it is deleted. The word is recursively called to ensure all attached nodes that do not have children are deleted.
void deleteWord (string input, Node* node);

//This function uses a input string prefix to find matching words within the tree. This is done with recursive pre-order traversal. If no matches are found, functions prints "No suggestions found".
void printSuggestion(Node* node, string prefix, string suffix);

//This function will insert words from a text file into a radix tree.
int insertFile(Node* node, string filename);

//This is another menu type functions that allows the user to pick which word file they wish to use to build the tree.
Node* buildTreeOptions();

//This function contains the main menu options for using the radix word suggestion tree.
int menuOptions();

//User interface menu.
void menu();
#endif