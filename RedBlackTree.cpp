#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>

using namespace std;

//Constants for colors of the nodes
const int RED = 0;
const int BLACK = 1;

//Node struct
struct Node {
  int data;
  Node* left = NULL;
  Node* right = NULL;
  Node* parent = NULL;
  int color = RED;
};

//Function prototypes
void ADD(Node* & root);
void READ(Node* & root);
void insert(Node* & root, int newData);
void insertFix(Node* & root, Node* & newNode);
Node* getUncle(Node* nephew);
void leftRotate(Node* & root, Node* & parentNode);
void rightRotate(Node* & root, Node* & parentNode);
void PRINT(Node* root, int depth);

int main() {
  Node* root = NULL; //Creating the root
  cout << "Welcome to Red-Black Tree" << endl;
  char input[10];

  //While loop will run as long as user doesn't type QUIT
  while(strcmp(input, "QUIT") != 0) {
    cout << "Type ADD, READ, PRINT, or QUIT" << endl;
    cin >> input;

    if (strcmp(input, "ADD") == 0) { //Console input
      ADD(root);
    } else if (strcmp(input, "READ") == 0) { //File input
      READ(root);
    } else if (strcmp(input, "PRINT") == 0) { //Print tree
      PRINT(root, 0);
    } else if (strcmp(input, "QUIT") != 0) { //Invalid command
      cout << "Invalid command" << endl;
    }
  }

  cout << "Goodbye" << endl;
  return 0;
}

void ADD(Node* & root) { //Inputs numbers through the console
  char data[500];
  cout << "Type the numbers to be added to the tree" << endl;
  cin.get();
  cin.get(data, 500);
  cin.get();

  char* ptr;
  ptr = strtok(data, " ");
  while (ptr != NULL) {
    insert(root, atoi(ptr));
    ptr = strtok(NULL, " ");
  }
}

void READ(Node* & root) { //Inputs numbers through a file
  char data[500];
  char fileName[50];
  cout << "Type the name of the file" << endl;
  cin >> fileName;

  ifstream file;
  srand(time(0));
  file.open(fileName);
  if (file == NULL) {
    cout << fileName << " could not be accessed" << endl;
  } else {
    while (file >> data) {
      insert(root, atoi(data));
    }
  }
}

void insert(Node* & root, int newData) { //Inserts a number into the tree
  Node* current = root;
  Node* parent = NULL;

  while (current != NULL) {
    parent = current;
    if (newData < current->data) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  Node* newNode = new Node;
  newNode->data = newData;
  newNode->parent = parent;

  if (parent == NULL) { //Case 1: Insert at root
    root = newNode;
    //root->color = BLACK;
    //return;
  } else if (newNode->data < parent->data) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }

  insertFix(root, newNode);
  
}

void insertFix(Node* & root, Node* & newNode) { //Fixes the red-black tree after insertion
  while (newNode->parent != NULL && newNode->parent->color != BLACK) { //Checks for Case 2: Parent is black
    if (newNode->parent == newNode->parent->parent->left) {
      if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED) { //Case 3: Parent and uncle are red
	newNode->parent->color = BLACK;
	getUncle(newNode)->color = BLACK;
	newNode->parent->parent->color = RED;
	newNode = newNode->parent->parent;
      } else /*if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED)*/ {
	if (newNode == newNode->parent->right) {
	  newNode = newNode->parent;
	  leftRotate(root, newNode);
	}
        newNode->parent->color = BLACK;
	newNode->parent->parent->color = RED;
	rightRotate(root, newNode->parent->parent);
      }
    } else {
      if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED) {
	newNode->parent->color = BLACK;
	getUncle(newNode)->color = BLACK;
	newNode->parent->parent->color = RED;
	newNode = newNode->parent->parent;
      } else /*if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED)*/ {
	if (newNode == newNode->parent->left) {
	  newNode = newNode->parent;
	  rightRotate(root, newNode);
	}
	newNode->parent->color = BLACK;
	newNode->parent->parent->color = RED;
	leftRotate(root, newNode->parent->parent);
      }
    }
    root->color = BLACK;
  }
  root->color = BLACK;
  /*if (newNode->parent->color == BLACK || newNode == root){ //Case 2: Parent is black
    return;
  } else if (newNode->parent->color == RED
	       && getUncle(newNode) != NULL
	     && getUncle(newNode)->color == RED) { //Case 3: Parent and uncle are red
    newNode->parent->color = BLACK;
    getUncle(newNode)->color = BLACK;
    newNode->parent->parent->color = RED;
    insertFix(root, newNode->parent->parent);
  } else if (getUncle(newNode) != NULL
	     && getUncle(newNode)->color == BLACK
	     && newNode->parent == newNode->parent->parent->left
	     && newNode == newNode->parent->right) { //Case 4a: Uncle is black, parent is a left node, newNode is a right node
    leftRotate(root, newNode->parent);
    insertFix(root, newNode->left);
  } else if (getUncle(newNode) != NULL
	     && getUncle(newNode)->color == BLACK
	     && newNode->parent == newNode->parent->parent->right
	     && newNode == newNode->parent->left) { //Case 4b: Uncle is black, parent is a right node, newNode is a left node
    rightRotate(root, newNode->parent);
    insertFix(root, newNode->right);
  } else if (getUncle(newNode) != NULL
	     && getUncle(newNode)->color == BLACK
	     && newNode->parent == newNode->parent->parent->left
	     && newNode == newNode->parent->left) { //Case 5a: Uncle is black, parent is a left node, newNode is a left node
    int parentColor = newNode->parent->color;
    int grandparentColor = newNode->parent->parent->color;
    rightRotate(root, newNode->parent->parent);
    newNode->parent->color = grandparentColor;
    newNode->parent->right->color = parentColor;
  } else if (getUncle(newNode) != NULL
	     && getUncle(newNode)->color == BLACK
	     && newNode->parent == newNode->parent->parent->right
	     && newNode == newNode->parent->right) { //Case 5b: Uncle is black, parent is a right node, newNode is a right node
    int parentColor = newNode->parent->color;
    int grandparentColor = newNode->parent->parent->color;
    leftRotate(root, newNode->parent->parent);
    newNode->parent->color = grandparentColor;
    newNode->parent->left->color = parentColor;
  }*/
}

Node* getUncle(Node* nephew) {
  if (nephew->parent == nephew->parent->parent->left) {
    return nephew->parent->parent->right;
  } else {
    return nephew->parent->parent->left;
  }
}

void leftRotate(Node* & root, Node* & parentNode) {
  Node* childNode = parentNode->right;
  parentNode->right = childNode->left;
  if (childNode->left != NULL) {
    childNode->left->parent = parentNode;
  }
  childNode->parent = parentNode->parent;
  if (parentNode->parent == NULL) {
    root = childNode;
  } else {
    if (parentNode == parentNode->parent->left) {
      parentNode->parent->left = childNode;
    } else {
      parentNode->parent->right = childNode;
    }
  }
  childNode->left = parentNode;
  parentNode->parent = childNode;
}

void rightRotate(Node* & root, Node* & parentNode) {
  Node* childNode = parentNode->left;
  parentNode->left = childNode->right;
  if (childNode->right != NULL) {
    childNode->right->parent = parentNode;
  }
  childNode->parent = parentNode->parent;
  if (parentNode->parent == NULL) {
    root = childNode;
  } else {
    if (parentNode == parentNode->parent->left) {
      parentNode->parent->left = childNode;
    } else {
      parentNode->parent->right = childNode;
    }
  }
  childNode->right = parentNode;
  parentNode->parent = childNode;
}

void PRINT(Node* root, int depth) { //Displays the tree nicely
  if (root != NULL) {
    if (root->right != NULL) {
      PRINT(root->right, depth + 1);
    }
    
    for (int i = 0; i < depth; i++) {
      cout << "             ";
    }

    if (root->color == BLACK) {
      cout << root->data << "(p: ";
      if (root->parent == NULL) {
	cout << "NULL)" << endl;
      } else {
	cout << root->parent->data << ")" << endl;
      }
    } else {
      cout << "\033[31m" << root->data << "(p: ";
      if (root->parent == NULL) {
	cout << "NULL)" << "\033[0m" << endl;
      } else {
	cout << root->parent->data << ")" << "\033[0m" << endl;
      }
    }

    if (root->left != NULL) {
      PRINT(root->left, depth + 1);
    }
  }
  return;
}
