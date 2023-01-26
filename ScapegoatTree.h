#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    int getSizeHelper(Node<T> *node) const;
    int getHeightHelper(Node<T> *node) const;
    bool contains(Node<T> *node, const T &element) const;
    bool HCondition() const;
    void insertHelper(Node<T> *node, const T &element);
    Node<T> *getParent(Node<T> *node, const T &element) const;
    void cleaner(Node<T> *&node);
    const T &getMinHelper(Node<T> *node) const;
    const T &getMaxHelper(Node<T> *node) const;
    Node<T> *getNode(Node<T> *node, const T &element) const;
    bool removeHelper(Node<T> *&node, const T &element);
    void copier(Node<T> *& node, Node<T> *obj);
    Node<T> *balanceHelper(Node<T> *node);
    void arrayMaker(Node<T> *node, T* array, int &i);
    void pushBack(Node<T> *node, T* array, int &i);
    Node<T> *treeMaker(T *array, int start, int end);
    const T &getCeilingHelper(Node<T>* node, const T &element) const;
    const T &getFloorHelper(Node<T>* node, const T &element) const;
    const T &finder(Node<T>* node, const T &element) const;

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree() 
{
    /* TODO */
    this->root = NULL;
    upperBound = 0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj) 
{
    /* TODO */
    if(obj.root == NULL) root = NULL;
    else copier(root, obj.root);
    upperBound = obj.upperBound;
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree() 
{
    /* TODO */
    removeAllNodes();
}

template<class T>
void ScapegoatTree<T>::copier(Node<T> *& node, Node<T> *node2)
{
    if(node2!=NULL)
    {
        node = new Node<T>;
        node->element = node2->element;
        copier(node->left,node2->left);
        copier(node->right,node2->right);
    }
    else node=NULL;
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const 
{
    /* TODO */
    return(root?false:true);
}

template<class T>
int ScapegoatTree<T>::getHeight() const 
{
    /* TODO */
    return getHeightHelper(root);
}

template<class T>
int ScapegoatTree<T>::getHeightHelper(Node<T> *node) const 
{
    /* TODO */
    if(!node) return -1;
    return((getHeightHelper(node->right) > getHeightHelper(node->left) ? getHeightHelper(node->right) : getHeightHelper(node->left)) + 1);
}

template<class T>
int ScapegoatTree<T>::getSize() const 
{
    /* TODO */
    return getSizeHelper(root);
}

template<class T>
int ScapegoatTree<T>::getSizeHelper(Node<T> *node) const
{
    if(!node) return 0;
    return(getSizeHelper(node->right) + getSizeHelper(node->left) + 1);  
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element) 
{
    /* TODO */
    if(contains(root, element)) return false;
    if(isEmpty())
    {
        Node<T> *newnode = new Node<T>;
        newnode->element = element;
        upperBound++;
        root = newnode;
        return true;
    }
    insertHelper(root, element);
    upperBound++; 
    if(HCondition()) return true;
    else
    {
        Node<T>* child = getNode(root, element);
        while(true)
        {
            bool eval = (float) getSizeHelper(child) / getSizeHelper(getParent(root, child->element)) > (float) 2/3;
            if(eval)
            {
                Node<T>* scapegoat = getParent(root, child->element);
                Node<T>* parent = getParent(root, scapegoat->element);
                if(parent->left == scapegoat)
                {
                    parent->left = balanceHelper(scapegoat);
                }
                else if(parent->right == scapegoat)
                {
                    parent->right = balanceHelper(scapegoat);
                }               
                return true;
            }
            else
            {
                child = getParent(root, child->element);
            }
        }
        return true;
    }
}

template<class T>
void ScapegoatTree<T>::insertHelper(Node<T> *node, const T &element)
{
    if(element < node->element)
    {
        if(node->left)
        {
            insertHelper(node->left, element);
            return;
        }
        else
        {
            Node<T>* newnode = new Node<T>;
            newnode->element = element;
            node->left = newnode;
            return;
        }
    }
    else
    {
        if(node->right)
        {
            insertHelper(node->right, element);
            return;
        }
        else
        {
            Node<T>* newnode = new Node<T>;
            newnode->element = element;
            node->right = newnode;
            return;
        }
    }
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element)
{
    /* TODO */
    bool result = removeHelper(root, element);
    if((float)upperBound/2 <= getSize() && getSize() <= upperBound) return result;
    else
    {
        // balance it
        balance();
        upperBound = getSize();
        return result;
    }
}

template<class T>
bool ScapegoatTree<T>::removeHelper(Node<T> *&node, const T &element) 
{
    if (node == NULL) return false;
    if(!contains(node, element)) return false;
    else
    {
        if (node->element > element) return removeHelper(node->left, element);
        else if (node->element < element) return removeHelper(node->right , element);
        else    
        {         
            if (node->left !=NULL && node->right != NULL) 
            {
                node->element = getMaxHelper(node->left);
                return removeHelper(node->left, node->element);
            }
            else
            {
                Node<T> * old = node;
                node = (node->left != NULL)? node->left : node->right;          
                delete old;
                return true;
            }
        }
    }
}

template<class T>
void ScapegoatTree<T>::removeAllNodes() 
{
    /* TODO */
    cleaner(root);
    root = NULL;
    upperBound = 0;
}

template<class T>
void ScapegoatTree<T>::cleaner(Node<T> *&node)
{
    if(node == NULL)return;    
    cleaner(node->left);
    cleaner(node->right); 
    delete node;
    node = NULL;

}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const 
{
    /* TODO */
    if(!contains(root, element)) throw NoSuchItemException();
    else return finder(root, element);
}

template<class T>
const T&ScapegoatTree<T>::finder(Node<T>* node, const T &element) const
{
        if (node->element == element) return node->element;
        else if (node->element > element) return finder(node->left,element);
        else return finder(node->right,element);
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const 
{
    if (tp == preorder) 
    {
        /* TODO */
        if (isEmpty()) 
        {
            // the tree is empty.
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } 
    else if (tp == inorder) 
    {
        // check if the tree is empty?
        if (isEmpty()) 
        {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }
        
        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } 
    else if (tp == postorder) 
    {
        /* TODO */
        if (isEmpty()) 
        {
            // the tree is empty.
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const 
{

    if (tp == preorder) 
    {
        /* TODO */
        if (node == NULL) return;
        std::cout << "\t" << node->element;
        if (node->left) 
        {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);
        if (node->right) 
        {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
    } 
    else if (tp == inorder) 
    {
        // check if the node is NULL?
        if (node == NULL) return;

        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) 
        {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->element;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) 
        {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } 
    else if (tp == postorder) 
    {
        /* TODO */
        if (node == NULL) return;
        print(node->left, postorder);
        if (node->left) 
        {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);
        if (node->right) 
        {
            std::cout << "," << std::endl;
        }
        
        std::cout << "\t" << node->element;  

    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const 
{

    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}

template<class T>
bool ScapegoatTree<T>::contains(Node<T> *node, const T &element) const
{
    if (node == NULL) return false;
    else 
    {
        if (node->element == element) return true;
        else if (node->element > element) return contains(node->left,element);
        else return contains(node->right,element);
    }
}

template<class T>
bool ScapegoatTree<T>::HCondition() const
{
    double onehalf = 1.5;
    double upper_limit = log(upperBound)/log(onehalf);
    return( (getHeight() <= upper_limit) ? true : false);
}

template<class T>
Node<T> *ScapegoatTree<T>::getParent(Node<T> *node, const T &element) const
{
    Node<T>* parent = node;
    if(element < parent->element)
    {
        if(element == parent->left->element) return parent;
        else return getParent(parent->left, element);
    }
    else
    {
        if(element == parent->right->element) return parent;
        else return getParent(parent->right, element);
    }
}

template<class T>
Node<T> *ScapegoatTree<T>::getNode(Node<T> *node, const T &element) const
{
    if(node != NULL)
    {
        if(node->element == element) return node;
        else if(element < node->element) return getNode(node->left, element);
        else if(element > node->element) return getNode(node->right, element);
    }
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) 
{
    /* TODO */
    if(this != &rhs)
    {
        removeAllNodes();
        copier(root, rhs.root);
    }
    upperBound = rhs.upperBound;
    return *this;
}

template<class T>
void ScapegoatTree<T>::balance() 
{
    /* TODO */
    root = balanceHelper(root);
}

template<class T>
Node<T> *ScapegoatTree<T>::balanceHelper(Node<T> *node)
{
    //first create the array with inorder traversal
    int size = getSizeHelper(node);
    T *elements = new T[size];
    int inc = 0;
    arrayMaker(node, elements, inc);
    cleaner(node);
    node = NULL;
    Node<T>* temproot = treeMaker(elements, 0, size - 1);
    delete[] elements;
    return temproot;
}

template<class T>
void ScapegoatTree<T>::arrayMaker(Node<T> *node, T* array, int &i)
{
    if(node == NULL) return;
    arrayMaker(node->left, array, i);
    pushBack(node, array, i);
    arrayMaker(node->right, array, i);
}

template<class T>
void ScapegoatTree<T>::pushBack(Node<T> *node, T* array, int &i)
{
    array[i] = node->element;
    i++;
}

template<class T>
Node<T> *ScapegoatTree<T>::treeMaker(T *array, int start, int end)
{
    if(start > end) return NULL;
    int middle = (start + end)/2;
    Node<T>* localroot = new Node<T>;
    localroot->element = array[middle];
    localroot->left = treeMaker(array, start, middle -1);
    localroot->right = treeMaker(array, middle + 1, end);
    return localroot;
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const 
{
    /* TODO */
    if(contains(root, element))
    {
        return get(element);
    }
    return getCeilingHelper(root, element);
}

template<class T>
const T &ScapegoatTree<T>::getCeilingHelper(Node<T>* node, const T &element) const
{
    if(node->element > element)
    {
        if(node->left == NULL) return node->element;
        if(getMaxHelper(node->left) > element) return getCeilingHelper(node->left, element);
        else return node->element;
    }
    else
    {
        if(node->right == NULL) throw NoSuchItemException();
        return getCeilingHelper(node->right, element);
    }
}
template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const 
{
    /* TODO */
    if(contains(root, element))
    {
        return get(element);
    }
    return getFloorHelper(root, element);
}

template<class T>
const T &ScapegoatTree<T>::getFloorHelper(Node<T>* node, const T &element) const
{
    if(node->element < element)
    {
        if(node->right == NULL) return node->element;
        if(getMinHelper(node->right) < element) return getFloorHelper(node->right, element);
        else return node->element;
    }
    else
    {
        if(node->left == NULL) throw NoSuchItemException();
        return getFloorHelper(node->left, element);
    }
}

template<class T>
const T &ScapegoatTree<T>::getMin() const
{
    /* TODO */
    if(isEmpty()) throw NoSuchItemException();
    return getMinHelper(root);
}

template<class T>
const T &ScapegoatTree<T>::getMinHelper(Node<T> *node) const 
{

    Node<T>* temp = node;
    while(temp != NULL && temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp->element;
}

template<class T>
const T &ScapegoatTree<T>::getMax() const 
{
    /* TODO */
    if(isEmpty()) throw NoSuchItemException();
    return getMaxHelper(root);
}

template<class T>
const T &ScapegoatTree<T>::getMaxHelper(Node<T> *node) const
{
    Node<T>* temp = node;
    while(temp != NULL && temp->right != NULL)
    {
        temp = temp->right;
    }
    return temp->element;
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const 
{
    /* TODO */
    return getCeilingHelper(root, element);
}
