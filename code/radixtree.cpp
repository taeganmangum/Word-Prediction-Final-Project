#include "radixtree.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

radixTree::~radixTree() {}


Node* initNode(){
    Node* ret= new Node;
    ret->isLeaf == false;
    ret->data = "";
    for (int i = 0; i < BRANCHING_FACTOR; i++){
        ret->children[i] = NULL;
    }
    return ret;
}

void buildNewNode(Node* node, string input){
    //search for NULL child and create new node
    for (int i = 0; i < BRANCHING_FACTOR; i ++){
        if (node->children[i] == NULL){
        Node* newChild = initNode();
        newChild->data = input;
        newChild->isLeaf = true;
        node->children[i] = newChild;
        return;
        }
    }
}

Node* search_children(Node* root, string input){
    for (int i = 0; i < BRANCHING_FACTOR; i++){
        Node* child = root->children[i];
        if (child != NULL){
            if (child->data[0] == input[0]){
                return child;
            }
        }
    }
    //if no child has matching first character of data to input, return root to insert
    return root;
}

int getMismatch(Node* node, string input){
    int numFound = 0;
    for (int i = 0; i < node->data.length(); i++){
        if (input[i] == node->data[i]){
            numFound++;
        }
        else{
            break;
        }
    }
    //if input and node data are the same word
    int compare = input.compare(node->data);
    if (compare == 0){
        return -1;
    }
    else{
        return numFound;
    }
}

string removePrefix(string input, Node* node){
    int numFound = getMismatch(node, input);
    //getMismatch returns -1 if node data and input are the same
    if (numFound == -1){
        return input;
    }
    else{
        return input.erase(0, numFound);
    }
}

 Node* lookupNode (string input, Node* root){
    //set cursor to traverse through tree
    Node* cursor = root;
    while (cursor != NULL){
        Node* child = search_children(cursor, input);
        cursor = child;
        //remove prefix of input string matching node's data
        string suffix = removePrefix(input, cursor);
        //if nothing is removed then the word is complete, otherwise move down the tree recursively with lookup function
        int compare = suffix.compare(input);
        if (compare == 0){
            //suffix and input are equivalent
            return cursor;
        }
        else {
            cursor = lookupNode(suffix, cursor);
            return cursor;
        }
    }
    return cursor;
}

bool searchWord(string word, Node* root){
    bool contains = false;
    //find node with matching prefix or word
    Node* target = lookupNode(word, root);
    int lengthNode = target->data.length();
    int lengthInput = word.length();
    string prefix = word.substr(0, lengthInput-lengthNode);
    //remove prefix to compare if suffix of input and node->data match
    string suffix = word.substr(prefix.length());
    int comp = suffix.compare(target->data);
    //word inside tree if two suffixes match and node marked as leaf
    if (target->isLeaf == true && comp == 0){
        contains = true;
    }
    return contains;
}

void split(Node* node, string input){
    //find first mismatched character
    int charCounter = getMismatch(node, input);
    string nodeDataOrig = node->data;
    //create substrings with new data for current node and child node
    string matched = nodeDataOrig.substr(0,charCounter);
    string newInput = nodeDataOrig.substr(charCounter);
    //copy current (split) node's invariants and matched data to a new node
    Node* newNode = initNode();
    newNode->data = newInput;
    newNode->isLeaf = true;
    node->data = matched;
    node->isLeaf = false;
    //find null child to add the new node to
    int counter = 0;
    for (int i = 0; i < BRANCHING_FACTOR; i ++){
        if (node->children[i] == NULL){
            node->children[i] = newNode;
            counter = i;
            break;
        }
    }
    //add children of current split node to new node
    for (int i = 0; i < BRANCHING_FACTOR; i ++){
        if (node->children[i] != NULL && node->children[i]->data[0] != newInput[0]){
            newNode->children[i] = node->children[i];
            node->children[i] = NULL;
            delete node->children[i];
            newNode->isLeaf = false;
        }
    }
    //insert the leftover part of the input string (if it exists). Will become a new child node.
    if (input.length() > matched.length()){
        string leftover = input.substr(charCounter);
        insert(leftover, node);
        node->isLeaf = false;
        return;
    }
    else {
        return;
    }
}

void insert(string input, Node* root){
    Node* cursor = root;
    string substring = input;
    //check children to see if word is already in tree
    for (int i = 0; i < BRANCHING_FACTOR; i++){
        if (cursor->children[i] != NULL){
            if (cursor->children[i]->data[0] == substring[0]){
                cursor = cursor->children[i];
                int misMatch = getMismatch(cursor, substring);
                //input matches node->data if getMismatch is 1, word is already in tree
                if (misMatch == -1){
                    //mark node as leaf to signify the word is in the tree
                    cursor->isLeaf = true;
                    return;
                }
                else if (substring.length() > cursor->data.length()){
                    string prefixCheck = substring.substr(0,cursor->data.length());
                    //substring matches cursor node data but has leftover suffix
                    if (prefixCheck == cursor->data){
                        substring = substring.substr(cursor->data.length());
                        insert(substring,cursor);
                        return;
                    }
                    //cursor node data not inside substring, split node needed
                    else {
                        split(cursor, substring);
                        //the split node is not a word, isLeaf marked false
                        cursor->isLeaf = false;
                        return;
                    }
                }
                else {
                    //substring shorter than cursor node data, split needed
                    split(cursor, substring);
                    //if node->data matches input the node should mark a word
                    if (cursor->data == input){
                        cursor->isLeaf = true;
                    }
                    else{
                        cursor->isLeaf = false;
                    }
                    return;
                }
            }
        }
    }
    //no child node with data matching first char of substring, input substring
    buildNewNode(cursor, substring);
    return;
}

void deleteWord (string input, Node* node){
    //find node that should mark word
    Node* target = lookupNode(input, node);
    //remove prefix to check if suffix of node->data and input match
    int dataLen = target->data.length();
    int prefixLen = input.length()-dataLen;
    string suffix = input.substr(prefixLen);
    //if suffixes don't match or node not marked as a leaf, the word is not in tree
    int comp = suffix.compare(target->data);
    if (comp != 0 || target->isLeaf == false){
        return;
    }
    //if the word is in the tree but it's node has children - do not delete it! Mark it as not a leaf.
    for (int i = 0; i < BRANCHING_FACTOR; i++){
        if (target->children[i] != NULL){
            target->isLeaf = false;
            return;
        }
    }
    //if the node does not have children it can be deleted
    target->isLeaf = false;
    target->data = "";
    target = nullptr;
    delete target;
    return;
}

void printSuggestion(Node* node, string prefix, string suffix){
    //checks if input prefix matches the current word's prefix
    string check = suffix.substr(0,prefix.length());
    if (node->isLeaf == true && suffix.length() > 0 && check == prefix){
        cout << suffix << endl;
    }
    //preorder traversal, update suffix as it goes down the tree
    for (int i = 0; i < BRANCHING_FACTOR; i++){
        string temp = suffix;
        if (node->children[i] != NULL){
            temp += node->children[i]->data;
            printSuggestion(node->children[i], prefix, temp);
        }
    }
    return;
  }

int insertFile(Node* node, string filename){
    fstream file;
    string word;
    file.open(filename, ios::in);
    if (file.is_open()){
        while (getline(file, word)) {
            insert(word, node);
        }
        file.close();
        return 0;
    }
    else {
        cout << "Error: Unable to read file" << endl;
        return -1;
    }
}

Node* buildTreeOptions(){
    cout << "Tree building options: \n";
    cout << "----------------------\n";
    cout << "1: 100 most common English words\n" << "2. 1000 most common English words\n" << "3. Custom file\n";
    cout << "Please select an option to create the radix tree: ";
    int userInput;
    cin >> userInput;
    Node* tree = initNode();
    if (userInput == 1){
        int file = insertFile(tree, "common-words.txt");
        if (file == 0){
            cout << "\n100 word tree built.\n";
            cout << "--------------------\n" << endl;
        }
        else{
            cout << "File failed to read. Exiting program...." << endl;
        }
    }
    else if (userInput == 2){
        int file = insertFile(tree, "1-1000.txt");
        if (file == 0){
            cout << "\n1000 word tree built.\n";
            cout << "---------------------\n" << endl;
        }
        else{
            cout << "File failed to read. Exiting program...." << endl;
        }
    }
    else if (userInput == 3){
        cout << "Please enter the file name being used: \n";
        string fileName;
        cin >> fileName;
        int file = insertFile(tree, fileName);
        if (file == 0){
            cout << "Word tree built with " << fileName << "\n";
            cout << "----------------------------" << endl;
        }
        else{
            cout << "File failed to read. Exiting program...." << endl;
        }
    }
    else{
        cout << "Option not recognized. Exiting program." << endl;
    }
    return tree;
}

int menuOptions(){
    cout << "Menu Options\n";
    cout << "-------------\n";
    cout << "1. Retrieve Word Suggestions from Radix Tree\n";
    cout << "2. Add word to Radix Tree\n";
    cout << "3. Search for word in Radix Tree\n";
    cout << "4. Delete word in Radix Tree\n";
    cout << "0. To Exit\n";
    cout << "Please enter a number: ";
    int userInput;
    cin >> userInput;
    return userInput;
}

void menu(){
    cout << "Welcome to the Word Suggestion Radix Tree Interface\n";
    cout << "*****************************************************\n";
    cout << "Press enter to continue\n";
    cin.get();
    Node* tree = buildTreeOptions();
    if (tree->children[0] == NULL){
        return;
    }
    bool done = false;
    while (!done){
        int userInput = menuOptions();
        if (userInput == 1){
            cout << "Option 1: Retrieve Word Suggestions\n";
            cout << "------------------------------------\n";
            cout << "Enter the prefix or letter you want matches for: ";
            string prefix;
            cin >> prefix;
            //makes user input lowercase
            for (auto& c : prefix) { 
                c = tolower(c);
            }
            cout << "Suggestions\n";
            cout << "-----------\n";
            printSuggestion(tree, prefix, "");
            cout << "------------\n" << endl;
        }
        else if (userInput == 2){
            cout << "Option 2: Add Word to Radix Tree\n";
            cout << "--------------------------------\n";
            cout << "Enter word you would like to insert: ";
            string word;
            cin >> word;
            //makes user input lowercase
            for (auto& c : word) { 
                c = tolower(c);
            }
            insert(word, tree);
            cout << "\n" << word << " has been inserted.\n" << endl;
        }
        else if (userInput == 3){
            cout << "Option 3: Search for Word in Radix Tree\n";
            cout << "----------------------------------------\n";
            cout << "Enter the word you wuld like to search for: ";
            string word;
            cin >> word;
            //makes user input lowercase
            for (auto& c : word) { 
                c = tolower(c);
            }
            bool search;
            search = searchWord(word, tree);
            if (search){
                cout << "\n" << word << " is in the radix tree.\n" << endl;
            }
            else {
                cout << "\n" << word << " is not in the radix tree.\n" << endl;;
            }
        }
        else if (userInput == 4){
            cout << "Option 4: Delete word from Radix Tree\n";
            cout << "-------------------------------------\n";
            cout << "Enter the word you wish to delete from the radix tree: ";
            string word;
            cin >> word;
            //makes user input lowercase
            for (auto& c : word) { 
                c = tolower(c);
            }
            bool search;
            search = searchWord(word, tree);
            if (search){
                deleteWord(word, tree);
                cout << "\n" << word << " is now deleted from the radix tree.\n" << endl;
            }
            else {
                cout << "\n" << word << " is not in the radix tree. Delete cannot be completed.\n" << endl;
            }
        }
        else {
            cout << "\nExit selected or input not recognized. Thank you for using the word suggestion tree!\n" << "Quitting...\n" << endl;
            done = true;
        }
    }
    return;
  }

