#include "../code/radixtree.h"
#include "../code/radixtree.cpp"

#include <iostream>

using namespace std;

//These are some of the tests that I used throughout making my project. I mainly used the debugger to walk through each function and scenario. These tests are not in order or complete, this is just supplemental material. When used, I ran them by copying them to main.cpp and running the debugger.

int main() {
    //lookup Word tests
    Node* tree = initNode();
    Node* ch0 = initNode();
    Node* ch0ch1 = initNode();
    Node* ch0ch2 = initNode();
    tree->children[0] = ch0;
    ch0->children[0] = ch0ch1;
    ch0->children[1] = ch0ch2;
    ch0->data = "te";
    ch0ch1->data = "am";
    ch0ch1->isLeaf = true;
    ch0ch2->data = "st";
    ch0ch2->isLeaf = true;

    Node* tree = initNode();
    insert("water", tree);
    insert("watch", tree);
    insert("watcher", tree);
    insert("wasteful", tree);
    insert("slower", tree);
    insert("slow", tree);
    insert("wasting", tree);
    printSuggestion(tree, "s", "");

    Node* testTeam = lookupNode("team", tree);
    if (testTeam->data == "m"){
        cout << "Passed Test 1" << endl;
    }
    else {
        cout << "Failed Test 1" << endl;
    }
    Node* testTest = lookupNode("test", tree);
    if (ch0ch2 == testTest){
        cout << "Passed Test 2" << endl;
    }
    else {
        cout << "Failed Test 2" << endl;
    }
    Node* testCase2 = lookupNode("testing", tree);
    if (ch0ch2 == testCase2){
        cout << "Passed Case 2" << endl;
    }
    else {
        cout << "Failed Case 2" << endl;
    }
    Node* testCase3 = lookupNode("t", tree);
    if (ch0 == testCase3){
        cout << "Passed Case 3" << endl;
    }
    else {
        cout << "Failed Case 3" << endl;
    }
    Node* testCase4 = lookupNode("tail", tree);
    if (ch0ch1 == testCase4){
        cout << "Passed Case 4" << endl;
    }
    else {
        cout << "Failed Case 4" << endl;
    }
}


    //insert    
    insert("test", tree);
    if (ch0ch2->isLeaf == true){
        cout << "Pass Case 1" << endl;
    }
    else {
        cout << "Fail Case 1" << endl;
    }
    insert("testing", tree);
    if (ch0ch2->children[0]->data == "ing"){
        cout << "Pass Case 2" << endl;
    }
    else {
        cout << "Fail Case 2" << endl;
    }
    insert("tense", tree);
    if (ch0->children[2]->data == "nse"){
        cout << "Pass Case 3" << endl;
    }
    else {
        cout << "Fail Case 3" << endl;
    }
    insert("tea", tree);
    if (ch0ch1->data == "a"){
        cout << "Pass Case 4" << endl;
    }
    else {
        cout << "Fail Case 4" << endl;
    }
    insert("tent", tree);
    if (ch0->children[2]->data == "n"){
        cout << "Pass Case 5" << endl;
    }
    else {
        cout << "Fail Case 5" << endl;
    }
    insert("testify", tree);
    if (ch0ch2->children[0]->data == "i"){
        cout << "Pass Case 6" << endl;
    }
    else {
        cout << "Fail Case 6" << endl;
    }
}