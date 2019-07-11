/***********************************************************************************************
 * HuffmanTree.cpp
 * Author: Matthew Sumpter
 * Description: Implementation file for HuffmanTree class that can compress
 *              a text string to Huffman Code, serialize the associated Huffman Binary Tree,
 *              and decompress a Huffman Code if the serialized Huffman Tree is provided.
 * 
 *              See header file for class architecture
 * *********************************************************************************************/

#include "HuffmanTree.hpp"
#include "HeapQueue.hpp"
#include "HuffmanBase.hpp"

#include <string>
#include <map>
#include <stack>

// using recursive preorder traversal, maps each leaf(character) of the Huffman tree to an associated prefix string based on the
// leaf's position in the tree. [tree] is the Huffman tree, [prefix_map] is the map, [inStr] is the string that gets updated with each
// iteration down the branches of the tree. For every left child traversal, a '0' is added to the string, for each right child traversal,
// a '1' is added to the string. Note (for [prefix_map]): key-> char, value-> prefix string
void HuffmanTree::char_to_prefix(const HuffmanNode* tree, std::map<char, std::string>& prefix_map, std::string inStr)
{
    // assign current node character to variable
    char pref_char = tree->getCharacter();

    // base case (should only run for leaves of the tree)
    // if tree node is not storing a null character, assign it the current string in the map
    if (tree->isLeaf())
    {
        prefix_map[pref_char] = inStr;
    }

    if (tree->left != nullptr)
    {   // left traversal appends a '0' to the prefix string
        std::string newStr = inStr + '0';
        char_to_prefix(tree->left, prefix_map, newStr);
    }
    if (tree->right != nullptr)
    {   // right traversal appends a '1' to the prefix string
        std::string newStr = inStr + '1';
        char_to_prefix(tree->right, prefix_map, newStr);
    } 
}

// using recursive preorder traversal, maps each generated prefix string to the characters found in the leaves. Works the same
// as char_to_prefix() function, but in prefix_to_char(), key-> prefix string, value-> char
void HuffmanTree::prefix_to_char(const HuffmanNode *tree, std::map<std::string, char>& prefix_map, std::string inStr)
{
    // assign current node character to variable
    char pref_char = tree->getCharacter();

    // base case (should only run for leaves of the tree)
    // if tree node is not storing a null character, assign it the current string in the map
    if (tree->isLeaf())
    {
        prefix_map[inStr] = pref_char;
    }

    if (tree->left != nullptr)
    { // left traversal appends a '0' to the prefix string
        std::string newStr = inStr + '0';
        prefix_to_char(tree->left, prefix_map, newStr);
    }
    if (tree->right != nullptr)
    { // right traversal appends a '1' to the prefix string
        std::string newStr = inStr + '1';
        prefix_to_char(tree->right, prefix_map, newStr);
    }
}

// counts the number of nodes in [tree] using a recursive preorder traversal, updating [_size] member variable
void HuffmanTree::preorder_count(const HuffmanNode* tree)
{
    ++_size;
    if (tree->left != nullptr)
        preorder_count(tree->left);
    if (tree->right != nullptr)
        preorder_count(tree->right);
}

// deallocated all nodes in [tree] using recursive postorder traversal. Used by destructor.
void HuffmanTree::delete_tree(HuffmanNode* tree)
{
    if (tree->left != nullptr)
        delete_tree(tree->left);
    if (tree->right != nullptr)
        delete_tree(tree->right);

    delete tree;
}

// using recursive postorder traversal, serializes [tree] into a string [serial]
void HuffmanTree::serialize_tree(const HuffmanNode* tree, std::string* serial) const
{
    if (tree->left != nullptr)
        serialize_tree(tree->left, serial);
    if (tree->right != nullptr)
        serialize_tree(tree->right, serial);

    // capture current nodes character in a variable
    char tree_char = tree->getCharacter();

    if (tree->isLeaf())
    {   // if current node is a leaf, append 'L' and the leaf's character
        serial->push_back('L');
        serial->push_back(tree_char);
    }
    else if ( tree->isBranch() )
    {   // if current node is a branch, append 'B'
        serial->push_back('B');
    }
}

// compresses [inputStr] to a Huffman Code, and returns the result
std::string HuffmanTree::compress(const std::string inputStr)
{
    std::map<char, size_t> characterFreq;    // map each character to its frequency in [inputStr]

    // for every character in the string, add it to the map if not already present
    // or update its frequency
    for(char c : inputStr)
        characterFreq[c]++;

    HeapQueue<HuffmanNode*, HuffmanNode::Compare> priority;  // priority queue for generating Huffman Tree

    // for every unique character in [inputStr], create a HuffmanNode and add it to a min priority queue
    for (auto it = characterFreq.begin(); it != characterFreq.end(); ++it)
    {
        HuffmanNode* new_node = new HuffmanNode(it->first, it->second);
        priority.insert(new_node);
    }

    // while there are multiple elements in [priority] heap, generate new Huffman Tree branches
    while (priority.size() > 1)
    {
        // remove two minimum Huffman Nodes
        HuffmanNode* left = priority.min();
        priority.removeMin();
        HuffmanNode* right = priority.min();
        priority.removeMin();

        size_t combined_freq = left->getFrequency() + right->getFrequency();

        // create parent node that represents the two removed nodes combined frequency
        HuffmanNode* parent = new HuffmanNode('\0', combined_freq, nullptr, left, right);
        
        left->parent = parent;
        right->parent = parent;

        priority.insert(parent);        // push parent node back onto priority heap
    }

    // final Huffman Node in list is the root of HuffmanTree
    _root = priority.min();
    priority.removeMin();

    preorder_count(_root);       // count number of nodes

    // fill [prefix] map with prefix values for characters
    std::map<char, std::string> prefix;
    char_to_prefix(_root, prefix, "");

    std::string compressedStr = "";

    // run through input string and generate new string using prefix values for characters
    for (char c : inputStr)
    {
        compressedStr += prefix[c];
    }

    return compressedStr;
}

// serializes a generated Huffman Tree into a postorder string of Leaves, Leaf values, and Branches, and returns the result.
// Used for later decompressing Huffman Codes
std::string HuffmanTree::serializeTree() const
{
    std::string serial = "";

    // serialize_tree() will update [serial] to reflect the current HuffmanTree
    serialize_tree(_root, &serial);

    return serial;
}

// using a Huffman code [inputCode] and a serialized Huffman Tree [serializedTree] ( generated from compress() and serializeTree() )
// decompresses [inputCode] into its original format, and returns the result
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)
{

    std::stack<HuffmanNode *> tree_stack;
    
    // iterate through [serializedTree] and reconstruct Huffman Tree
    for (auto p = serializedTree.begin(); p != serializedTree.end(); ++p)
    {
        if (*p == 'L')
        {   // for 'L', generate a new leaf node with the following character as its element, and push it onto the stack
            char new_char = *(++p);
            HuffmanNode* new_leaf = new HuffmanNode(new_char, 0);
            tree_stack.push(new_leaf);
        }
        else if (*p == 'B')
        {   // for 'B', generate a new branch: pop the top two nodes from the stack, join them with a new parent(branch) node, and push the
            // branch back onto the stack

            HuffmanNode* right = tree_stack.top();
            tree_stack.pop();
            HuffmanNode* left = tree_stack.top();
            tree_stack.pop();

            HuffmanNode* new_branch = new HuffmanNode('\0', 0);
            new_branch->left = left;
            new_branch->right = right;

            left->parent = new_branch;
            right->parent = new_branch;

            tree_stack.push(new_branch);
        }
    }

    // the final element on the stack will be the full Huffman Tree
    _root = tree_stack.top();

    std::string decompressed = "";
  
    // fill [prefix] map with prefix values for characters
    std::map<std::string, char> prefix;
    prefix_to_char(_root, prefix, "");
    
    // iterate through [inputCode]. Because character prefix codes are unique, continue appending more characters from
    // [inputCode] until a match is found, and then append the matching character to the decompressed string
    for (auto p = inputCode.begin(); p != inputCode.end();)
    {
        std::string prefix_string = "";

        // while [prefix_string] does not match a character in [prefix] map, append another character from [inputCode]
        while( prefix.find(prefix_string) == prefix.end() )
        {
            prefix_string.push_back(*(p++));
        }

        // once a match is found, append the associated character to the decompressed string
        decompressed += prefix[prefix_string];
    }
    
    return decompressed;
}