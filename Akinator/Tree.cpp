#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include "Tree.h"


///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
/// ”важаемый пользователь, следующие определени€ нужно изменить дл€ конкретной задачи
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
/// \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \|/  \\\
///  *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *   \\\


void PrintNode(Node* root)
{
    assert(root);

    printf(treeTypeFormat "\n", root->value);
}

static size_t SizeofTreeType(const char* value)
{
    return strlen(value) + 1;
}

static int CompareObject(treeType obj1, treeType obj2)
{
    return strcmp(obj1.ptr, obj2.ptr);
}

///  *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *   \\\
/// /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  /|\  \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///  |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |   \\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static void CreateNodeGraph(FILE* file, Node* node, size_t parentId, bool IsRight);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(Tree* tree)
{
    assert(tree);

    tree->root = nullptr;

    return true;
}

Node* TreeNodeConstructor(const treeType* value)
{
    //assert(value);

    Node* node = (Node*)calloc(1, sizeof(Node));

    if (!node)
    {
        puts("Ќе хватает пам€ти");
        return nullptr;
    }

    node->nodeLeft = nullptr;
    node->nodeRight = nullptr;

    if (value)
        memcpy(&node->value, value, sizeof(treeType));

    return node;
}

bool TreeDestructor(Tree* tree) 
{
    assert(tree);

    TreeNodeDestructor(tree->root);

    return true;
}

bool TreeNodeDestructor(Node* node)
{
    assert(node);

    if (node->nodeLeft)
        TreeNodeDestructor(node->nodeLeft);
    if (node->nodeRight)
        TreeNodeDestructor(node->nodeRight);

    free(node);

    return true;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeAddLeftNode(Node* parent, Node* child)
{
    assert(parent);
    assert(child);

    parent->nodeLeft = child;
    child->parent = parent;

    return true;
}

bool TreeAddRightNode(Node* parent, Node* child)
{
    assert(parent);
    assert(child);

    parent->nodeRight = child;
    child->parent = parent;

    return true;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeVisit(Node* node, void (*nodeFunction)(Node*), VisitCallType type)
{
    assert(node);
    
    if (type & VisitCallType::PREORDER)
        printf(treeTypeFormat, node->value);

    if (node->nodeLeft)
        TreeVisit(node->nodeLeft, nodeFunction, type);
    
    if (type & VisitCallType::INORDER)
        printf(treeTypeFormat, node->value);

    if (node->nodeRight)
        TreeVisit(node->nodeRight, nodeFunction, type);
    
    if (type & VisitCallType::POSTORDER)
        printf(treeTypeFormat, node->value);
}

Node* TreeFindObject(Node* node, treeType object)
{
    assert(node);
    
    if (CompareObject(object, node->value) == 0)
        return node;

    if (node->nodeLeft)
        return TreeFindObject(node->nodeLeft, object);
    
    if (node->nodeRight)
        return TreeFindObject(node->nodeRight, object);
    
    return nullptr;
}

Node* TreeFindObjectStack(Node* node, treeType object, Stack* stk)
{
    assert(node);

    StackPush(stk, &node);

    if (CompareObject(object, node->value) == 0)
        return node;

    if (node->nodeLeft)
        return TreeFindObject(node->nodeLeft, object);
    
    if (node->nodeRight)
        return TreeFindObject(node->nodeRight, object);

    StackPop(stk, nullptr);

    return nullptr;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool CreateTreeGraph(const char* outFileName, Tree* tree)
{
    assert(outFileName);
    assert(tree);

    FILE* file = fopen("treeGraph.gv", "w");

    if (file)
    {
        fputs("digraph G\n{\n    node [shape = plaintext, fontcolor=\"#292C37\"];\n", file);
        CreateNodeGraph(file, tree->root, 0, true);
        fputs("}", file);

        fclose(file);

        size_t cmdSize = strlen(outFileName) + 1 + 4;
        char* cmd = (char*)calloc(cmdSize + 30, sizeof(char));

        if (!cmd)
        {
            puts("Ќе хватает пам€ти");
            free(cmd);
            return false;
        }

        sprintf(cmd, "dot \"treeGraph.gv\" -Tpng > \"%s\"", outFileName);
        system(cmd);

        free(cmd);

        remove("treeGraph.gv");

        return true;
    }

    return false;
}

static void CreateNodeGraph(FILE* file, Node* node, size_t parentId, bool IsRight)
{
    assert(file);
    assert(node);

    static size_t nodeId = 1;
    size_t _nodeId = nodeId;

    if (!(node->value.ptr))
        return;
    
    fprintf(file, "    node%zd[label = <<TABLE BORDER=\"0\" CELLBORDER=\"1\" COLOR=\"#292C37\" CELLSPACING=\"0\"><TR>"
                  "<TD PORT=\"noPort\" BGCOLOR=\"red2\">no</TD>"
                  "<TD PORT=\"questionPort\" BGCOLOR=\"papayawhip\">%.*s</TD>"
                  "<TD PORT=\"yesPort\" BGCOLOR=\"olivedrab3\">yes</TD></TR></TABLE>>];\n", _nodeId, node->value.length,
        node->value.ptr);
    nodeId++;

    if (parentId != 0)
        fprintf(file, "        \"node%zd\":%s -> \"node%zd\":questionPort [color=\"%s\"];\n",
            parentId, IsRight?"yesPort":"noPort", _nodeId, IsRight?"olivedrab3":"red2");

    if (node->nodeLeft)
        CreateNodeGraph(file, node->nodeLeft, _nodeId, false);
    
    if (node->nodeRight)
        CreateNodeGraph(file, node->nodeRight, _nodeId, true);
}