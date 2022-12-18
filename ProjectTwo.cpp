//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Rahael Coloma
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : CS300 Project 2
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNum; // unique identifier
    string courseName;
    vector<string> coursePrereq{};
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printCourse(Node* node);
    void checkPrereq(Node* node);
    Node* removeNode(Node* node, string courseNum);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void PrintCourse();
    void CheckPrereq();
    void Insert(Course course);
    void Remove(string courseNum);
    void searchCourse(string inputtedCourse);
    int courseRemove(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    //calls removeNode fuction to recursively delete all nodes from tree starting at the root
    removeNode(root, root->course.courseNum);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::PrintCourse() {
    //calls printCourse fuction and passes root 
    printCourse(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::CheckPrereq() {
    //calls checkPrereq fuction and passes root 
    checkPrereq(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    //If statement to check for empty tree
    if (root == nullptr) {
        //Sets new course node as root of tree
        root = new Node(course);
    }

    //Tree is not empty
    else {
        //calls addNode function and inserts course node to tree
        this->addNode(root, course);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string courseNum) {
    //calls removeNode function to remove node with passed courseNum string
    this->removeNode(root, courseNum);
}

/**
 * Search for a course
 */
void BinarySearchTree::searchCourse(string courseNum) {
    //Create/set current node equal to root of tree
    Node* curNode = root;

    //While loop to traverse tree - curNode will be null once it reaches the bottom
    while (curNode != nullptr) {
        //If statement to compare course numbers - function will return 0 if they match
        if (curNode->course.courseNum.compare(courseNum) == 0) {
            //display course information with necessary formatting
            cout << curNode->course.courseNum << ", " << curNode->course.courseName << endl;
            cout << "Prerequisites: ";

            //If statement to check if course has prerequisites
            if (!curNode->course.coursePrereq.empty()) {

                //For loop to cycle coursePrereq vector
                for (int i = 0; i < curNode->course.coursePrereq.size(); ++i) {

                    //If statement to check last prerequisite
                    if (i != curNode->course.coursePrereq.size() - 1) {
                        //Displays ',' 
                        cout << " " << curNode->course.coursePrereq.at(i) << ", ";
                    }
                    else {
                        cout << " " << curNode->course.coursePrereq.at(i);
                    }
                }
            }
            //Else - No prerequisites
            else {
                cout << "None";
            }
            cout << endl;
            return ;
        }

        //if statement to compare course numbers - function will return negative if course number is smaller than current Node
        else if (courseNum.compare(curNode->course.courseNum) < 0) {
            //Set current node to left Node of parent for traversal
            curNode = curNode->left;
        }

        //bidId is larger than current Node
        else {
            //Set current node to right Node of parent for traversal
            curNode = curNode->right;
        }
    }
    cout << "Course Number not found. Please verify and try again." << endl;
    return ;
}


/* Remove a course from some node (recursive) */
Node* BinarySearchTree::removeNode(Node* node, string courseNum) {
    //courseNum not found
    if (node == nullptr) {
        return node;
    }

    //Traversal down left child through recursive call
    else if (courseNum.compare(node->course.courseNum) < 0) {
        node->left = removeNode(node->left, courseNum);
    }

    //Traversal down right child through recursive call
    else if (courseNum.compare(node->course.courseNum) > 0) {
        node->right = removeNode(node->right, courseNum);
    }

    //Matching courseNum found
    else {
        //Node is a leaf and can be deleted - Set node to nullptr to exit if statement
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }

        //Node only has a left child - Set left child as parent
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }

        //Node only has a right child - Set right child as parent
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }

        //Node has both a left and right child
        else {
            //Initialize temp node to right node
            Node* temp = node->right;

            //Travers left child until minimum is found - set temp node to left child
            while (temp->left != nullptr) {
                temp = temp->left;
            }

            //Set bid data of node to successor bid
            node->course = temp->course;
            //Recursive call to remove successor
            node->right = removeNode(node->right, temp->course.courseNum);
        }
    }
    return node;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    //If statement to compare course numbers to determine child definition. Left < 0 Right > 0
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        //if statement to check if parent has a left child
        if (node->left == nullptr) {
            //Sets left child to new course Node
            node->left = new Node(course);
        }

        //Parent has a left child
        else {
            //recursive call to addNode to traverse down left child
            this->addNode(node->left, course);
        }
    }

    //Node course number is smaller
    else {
        //if statement to check if parent has a right child
        if (node->right == nullptr) {
            //Sets right child to new course Node
            node->right = new Node(course);
        }

        //Parent has a right child
        else {
            //recursive call to addNode to traverse down right child
            this->addNode(node->right, course);
        }
    }
}
/**
 * Prints for list of courses
 */
void BinarySearchTree::printCourse(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {

        //recursive call to printCourse for the left child
        printCourse(node->left);
        //display course information with necessary formatting
        if (!node->course.courseNum.empty()) {
            cout << node->course.courseNum << ", " << node->course.courseName << endl;
        }
        //recursive call to printCourse for the right child
        printCourse(node->right);
    }
}

/**
 * Verifies if prerequisite course is part of the course list
 */
int BinarySearchTree::courseRemove(string courseNum) {
    //Create/set current node equal to root of tree
    Node* curNode = root;

    //While loop to traverse tree - curNode will be null once it reaches the bottom
    while (curNode != nullptr) {
        //If statement to compare course numbers - function will return 0 if they match
        if (curNode->course.courseNum.compare(courseNum) == 0) {
            //Course Found - do not remove
            return 0;
        }

        //if statement to compare course numbers - function will return negative if course number is smaller than current Node
        else if (courseNum.compare(curNode->course.courseNum) < 0) {
            //Set current node to left Node of parent for traversal
            curNode = curNode->left;
        }

        //bidId is larger than current Node
        else {
            //Set current node to right Node of parent for traversal
            curNode = curNode->right;
        }
    }
    //course not found - remove
    return 1;
}


/**
 * Checks for courses with prerequisites and passes them to the courseRemove function
 */
void BinarySearchTree::checkPrereq(Node* node) {
    if (node != nullptr) {

        //recursive call to printCourse for the left child
        checkPrereq(node->left);
        //display course information with necessary formatting
        //If statement to check if course has prerequisites
        if (!node->course.coursePrereq.empty()) {

            //For loop to cycle coursePrereq vector
            for (int i = 0; i < node->course.coursePrereq.size(); ++i) {
                
                //If statement to check if prerequisite is in course list
                if (courseRemove(node->course.coursePrereq.at(i)) == 1) {
                    
                    //Function call to remove invalid course
                    Remove(node->course.courseNum);
                    return;
                }
            }
        }
        //recursive call to printCourse for the right child
        checkPrereq(node->right);
    }

}

/**
 * loads the course file and parses the information
 */
void loadCourses(string fileName, BinarySearchTree* bst) {
    //Define Variables
    int count = 0;
    ifstream file(fileName);
    string fileLine, tokenString;
    vector<string> stringTokens;

    //If statement to error check if file was able to be opened
    if (file.is_open()) {

        //While loop to end of file
        while (file.good()) {
            //Create course struct
            Course course;
            //Read line from file
            getline(file, fileLine);
            //Pass line to stringstream
            stringstream inStream(fileLine);

            //while loop to add parsed strings to string vector
            while (getline(inStream, tokenString, ',')) { 
                stringTokens.push_back(tokenString);
            }

            //Error check line with less than 2 elements
            if (stringTokens.size() < 2) {
                stringTokens.clear();
            }

            //Error check missing course number
            else if (stringTokens.at(0).empty()) {
                cout << "Error: Course Information not properly entered.\nLoading next course." << endl;
                stringTokens.clear();
            }

            //Error check missing course name
            else if (stringTokens.at(1).empty()) {
                cout << "Error: Course Information not properly entered.\nLoading next course." << endl;
                stringTokens.clear();
            }

            //Complete course information
            else {
                //Store course number and name to course struct
                course.courseNum = stringTokens.at(0);
                course.courseName = stringTokens.at(1);

                //if statement to check for prerequisites
                if (stringTokens.size() > 2) {

                    //for loop to add prerequisites to struct vector member
                    for (int i = 2; i < stringTokens.size(); ++i) {
                        course.coursePrereq.push_back(stringTokens.at(i));
                    }
                }
                //function to clear string vector for next iteration
                stringTokens.clear();
                
            }
            //Call to insert function to add course struct to binary search tree
            bst->Insert(course);

        }
        //Close file
        file.close();
        //Call to CheckPrereq function
        bst->CheckPrereq();
        cout << "File successfully loaded." << endl;
    }
    //File failed to open
    else {
        cout << "File failed to open.\nPlease verify file name and try again." << endl;
    }   
}

int main(int argc, char* argv[]) {

    //Create Binary Search Tree and declare main class variables
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string coursesFile;
    string inputtedCourse;
    int userInput = 0;
    
    // process command line arguments
    switch (argc) {
    case 2:
        coursesFile = argv[1];
        break;
    case 3:
        coursesFile = argv[1];
        break;
    default:
        coursesFile = "sample.txt";
    }

    cout << "Welcome to the course planner." << endl;

    //While loop to keep program running until user aborts
    while (userInput != 9) {

        //Display program menu 1-3. Option 9 exits program
        cout << endl;
        cout << "   1. Load Data Structure." << endl;
        cout << "   2. Print Course List." << endl;
        cout << "   3. Print Course." << endl;
        cout << "   9. Exit.\n" << endl;
        cout << "What would you like to do? ";

        //While loop to validate user input
        while(!(cin >> userInput)) {
            //Clears cin error flags
            cin.clear();
            //Discards input waiting in buffer
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Input invalid. Please try again." << endl;
        }
    
        
        //Switch statement - passing user input
        switch (userInput) {

        case 1:

            //Request for user input
            cout << "Please enter the name of the Course File. " << endl;
            //Stores user into into variable
            cin >> coursesFile;
            //Function call - passing user inputted file and binary search tree
            loadCourses(coursesFile, bst);
            break;

        case 2:

            //Display header
            cout << "Here is a sample schedule:\n" << endl;
            //Function call through bst pointer 
            bst->PrintCourse();
            break;

        case 3:

            //Request for user input
            cout << "What course do you want to know about? ";
            //Stores user input into variable
            cin >> inputtedCourse;
            //Function call that converts the user input string into capital letters
            transform(inputtedCourse.begin(), inputtedCourse.end(),inputtedCourse.begin(), ::toupper);
            //Function call through bst pointer - passing user inputted course
            bst->searchCourse(inputtedCourse);
            break;

        case 9:
            break;

        default:
            cout << userInput << " is not a valid option. Please try again." << endl;
        }
    }
    cout << "Thank you for using the course planner!" << endl;
    return 0;
}
