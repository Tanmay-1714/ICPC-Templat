#include <bits/stdc++.h>
using namespace std;

const int ALPHABET_SIZE = 26;

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
            int index = ch - 'a';
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
        if (node == nullptr) return false;
        if (depth == (int)word.length()) {
            if (!node->isEndOfWord) return false;
            node->isEndOfWord = false;
            return nodeHasNoChildren(node);
        }
        int index = word[depth] - 'a';
        if (deleteWordRecursive(node->children[index], word, depth + 1)) {
            delete node->children[index];
            node->children[index] = nullptr;
            return nodeHasNoChildren(node);
        }
        return false;
    }

    bool nodeHasNoChildren(TrieNode* node) {
        for (TrieNode* child : node->children)
            if (child != nullptr) return false;
        return !node->isEndOfWord;
    }

    bool search(const string& word) {
        TrieNode* node = searchNode(word);
        return (node != NULL && node->isEndOfWord);
    }

    TrieNode* searchNode(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (current->children[index] == NULL) return NULL;
            current = current->children[index];
        }
        return current;
    }

    void printAllWords() {
        string currentWord;
        printWordsRecursive(root, currentWord);
    }

    void printWordsRecursive(TrieNode* node, string& currentWord) {
        if (node == NULL) return;
        if (node->isEndOfWord) cout << currentWord << "\n";
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i] != NULL) {
                currentWord.push_back('a' + i);
                printWordsRecursive(node->children[i], currentWord);
                currentWord.pop_back();
            }
        }
    }

    // Recursively free every node so this Trie can be safely destroyed
    // between test cases without leaking memory.
    void freeAll(TrieNode* node) {
        if (node == nullptr) return;
        for (int i = 0; i < ALPHABET_SIZE; ++i) freeAll(node->children[i]);
        delete node;
    }

    ~Trie() {
        freeAll(root);
    }
};

void solve() {
    // Fresh trie for THIS test case only -- goes out of scope (and its
    // destructor runs, freeing all nodes) at the end of solve().
    Trie trie;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;
        if (command == "insert") {
            string word;
            cin >> word;
            trie.insert(word);
        } else if (command == "words") {
            trie.printAllWords();
        } else if (command == "search") {
            string word;
            cin >> word;
            cout << (trie.search(word) ? "present" : "not present") << "\n";
        } else if (command == "delete") {
            string word;
            cin >> word;
            trie.deleteWord(word);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
