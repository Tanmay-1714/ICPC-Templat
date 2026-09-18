#include <bits/stdc++.h>
using namespace std;

const int ALPHABET_SIZE = 2;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode() : children{}, isEndOfWord(false) {}
};

class Trie {
public:
    TrieNode* root;
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - '0';
            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
    }
    
    void deleteWord(const string& word) {
        deleteWordRecursive(root, word, 0);
    }
    
    bool deleteWordRecursive(TrieNode* node, const string& word, int depth) {
        if (node == nullptr) {
            return false;
        }

        if (depth == word.length()) {
            if (!node->isEndOfWord) {
                return false; // Word not present in the trie
            }

            node->isEndOfWord = false;

            // If the node has no children, it can be safely removed
            return nodeHasNoChildren(node);
        }

        int index = word[depth] - '0';
        if (deleteWordRecursive(node->children[index], word, depth + 1)) {
            // Delete the child node if it can be deleted
            delete node->children[index];
            node->children[index] = nullptr;

            // Check if the current node has no children and is not an end-of-word node
            return nodeHasNoChildren(node);
        }

        return false;
    }

    bool nodeHasNoChildren(TrieNode* node) {
        for (TrieNode* child : node->children) {
            if (child != nullptr) {
                return false;
            }
        }
        return !node->isEndOfWord;
    }
    
    bool search(const string& word) {
        TrieNode* node = searchNode(word);
        return (node != NULL && node->isEndOfWord);
    }
    
    TrieNode* searchNode(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - '0';
            if (current->children[index] == NULL) {
                return NULL; // Character not found in the trie
            }
            current = current->children[index];
        }
        return current;
    }

    string argmaxmaximum_xor(const string& word) {
        TrieNode* current = root;
        // see if there is a node which has the number opposite to what i have currently 
        string res = "";
        for(char ch : word) {
            int index = ch - '0';
            if(current->children[1 - index] != NULL) {
                // go down this path 
                res.push_back('0' + (1 - index));
                current = current->children[1 - index];
            } else {
                res.push_back('0' + index);
                current = current->children[index];
                // forced to go down this path
            }
        }
        return res;
    }
    
    void printAllWords() {
        string currentWord;
        printWordsRecursive(root, currentWord);
    }
    
    void printWordsRecursive(TrieNode* node, string& currentWord) {
        if (node == NULL) {
            return;
        }

        if (node->isEndOfWord) {
            cout << currentWord << endl;
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i] != NULL) {
                currentWord.push_back('0' + i);
                printWordsRecursive(node->children[i], currentWord);
                currentWord.pop_back();
            }
        }
    }
};

int main() {
    
    Trie trie;

    // trie.insert(word);
    // trie.search(word)
    // trie.deleteWord(word);

    int n;
    cin >> n;
    vector<int> a(n - 1);
    for(int i = 0; i < n - 1; i++) {
        cin >> a[i];
    }
    // b1 = b1 xor 0
    // b2 = b1 xor a1
    // b3 = b2 xor a2 = b1 xor a1 xor a2 
    // bi = b1 xor a1 xor a2 . . . . . xor ai 

    int cur = 0;
    for(int i = 0; i < n; i++) {
        string word = "";
        for(int j = 31; j >= 0; j--) {
            if((cur & (1LL << j))) {
                word.push_back('1');
            } else {
                word.push_back('0');
            }
        }
        trie.insert(word);
        if(i < n - 1) cur = (cur ^ a[i]);
    }



    // iterate over the possibilities for b1 

    int idx = -1;
    for(int i = 0; i < n; i++) {
      
        string word = "";
        for(int j = 31; j >= 0; j--) {
            if((i & (1LL << j))) {
                word.push_back('1');
            } else {
                word.push_back('0');
            }
        }

        string val = trie.argmaxmaximum_xor(word);
        int res = 0;
        for(int j = 31; j >= 0; j--) {
            if(val[31 - j] == '1') {
                res += (1LL << j);
            }
        }
        int maximum = (res ^ i);
        if(maximum == (n - 1)) {
            idx = i;
            break;
        }
    }

    vector<int> b(n);
    b[0] = idx;

    cout << idx << " ";
    cur = 0;
    // b[1] = b[0] xor a[0]
    for(int i = 1; i < n; i++) {
        cur = (cur ^ a[i - 1]);
        b[i] = (idx ^ cur);

        cout << b[i] << " ";
    }

    cout << '\n';

        
    return 0;
}
