#ifndef MY_TREE_H
#define MY_TREE_H


#include "..\StringLibrary\StringLibrary.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
/// Уважаемый пользователь, следующие определения нужно изменить для конкретной задачи
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
/// \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \\\
///  *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *   \\\

typedef String treeType;

#define treeTypeFormat "%s"

#include "..\StackLibrary\Stack.h"


///  *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *   \\\
/// /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

enum VisitCallType
{
    PREORDER  = 1,
    INORDER   = 2,
    POSTORDER = 4
};

struct Node
{
    treeType value;

    Node* parent;
    Node* nodeLeft;
    Node* nodeRight;
};

struct Tree
{
    Node* root;
    size_t treeLength;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(Tree* tree);

Node* TreeNodeConstructor(const treeType* value);

bool TreeDestructor(Tree* tree);

bool TreeNodeDestructor(Node* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeAddRightNode(Node* parent, Node* child);

bool TreeAddLeftNode(Node* parent, Node* child);

void TreeMeasure(Tree* tree, Node* node, size_t length);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeVisit(Node* node, void (*nodeFunction)(Node*), VisitCallType type);

Node* TreeFindObject(Node* node, treeType object);

Node* TreeFindObjectStack(Node* node, treeType object, Stack* stk);

Node* GetNodeFromStack(Stack* stk, size_t index);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool CreateTreeGraph(const char* fileName, Tree* tree);


#endif
