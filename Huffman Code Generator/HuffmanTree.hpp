/***************************************************************************************************************************************
 * HuffmanTree.hpp (PP4)
 * Author: Matthew Sumpter
 * Description: Header file for HuffmanTree class. The HuffmanTree class
 *              provides functionality for compressing and decompressing strings
 *              of text with Huffman codes (http://compression.ru/download/articles/huff/huffman_1952_minimum-redundancy-codes.pdf).
 * 
 *              Stores the root of a Huffman binary tree and the number of nodes in the tree.
 * 
 *              See implementation file for detailed function descriptions
 * *************************************************************************************************************************************/

#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include "HuffmanBase.hpp"

#include <string>
#include <map>

class HuffmanTree : public HuffmanTreeBase
{
    private:
        HuffmanNode* _root;                  // pointer to the root
        int _size;                           // number of elements in tree

        void char_to_prefix(const HuffmanNode* tree, std::map<char, std::string>& prefix_map, std::string inStr);
        void prefix_to_char(const HuffmanNode* tree, std::map<std::string, char>& prefix_map, std::string inStr);

        void preorder_count(const HuffmanNode* tree);

        void delete_tree(HuffmanNode* tree);
        void serialize_tree(const HuffmanNode* tree, std::string* serial) const;

    public:
        HuffmanTree(): _root(nullptr), _size(0) {};             // constructor
        ~HuffmanTree() { delete_tree(_root); };                 // deconstructor
        int size() const { return _size; };
        bool empty() const { return size() == 0; };             // is tree empty?

        std::string compress(const std::string inputStr);
        std::string serializeTree() const;
        std::string decompress(const std::string inputCode, const std::string serializedTree);
};

#endif // HUFFMANTREE_HPP