// Made by Ari Ozdogan
// I pledge my honor that I have abided by the Stevens Honor System

#include <iostream>
#include <unordered_map> // hashmap

class LRU_Cache {
    // private data types. can only be accessed inside LRU_Cache
    private:
      struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;

        Node(int key_value, int data_value, Node* prev_value = nullptr, Node* next_value = nullptr) : key(key_value), value(data_value), prev(prev_value), next(next_value){} 
      };

      int capacity; // max capacity for linked list
      std::unordered_map<int, Node*> hashmap;
      Node* head;
      Node* tail;

      void insertAtHead(Node* node);
      void removeNode(Node* node);

    public:
      LRU_Cache(int capacity); // constructor
      ~LRU_Cache(); // destructor, runs at end of program automatically to free memory

      // put(key, value) either adds new key-value pair or updates the value if key already exists in hashmap
      // all key-value pairs are added to beginning of linked list regardless of whether the key already exists in the hashmap
      void put(int key, int value);

      // get(key) returns value stored at key in hashmap and adds node to the front of linked list
      int get(int key);

      bool contains(int key); // only checks hashmap

      void printMap() const; // prints private hashmap
      void printList() const;
};

void LRU_Cache::insertAtHead(Node* node) {
  node->next = head->next; // head must always stay the same so each method knows where the start of the linked list is in memory
  node->prev = head;
  head->next->prev = node; // original node after head now comes after new node
  head->next = node;
}

void LRU_Cache::removeNode(Node* node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

// LRU Cache constructor
LRU_Cache::LRU_Cache(int cap) : capacity(cap) {
  head = new Node(-1, -1); // placeholders for key and value (prev and next default to nullptr)
  tail = new Node(-1, -1);
  head->next = tail;
  tail->prev = head;
}

LRU_Cache::~LRU_Cache() {
  Node* current = head;
  // traverses list and deletes all nodes
  while (current != nullptr) {
    Node* next = current->next;
    delete current;
    current = next;
  }
}

void LRU_Cache::put(int key, int value) {  
  auto it = hashmap.find(key);
  // check if key already exists
  if (it != hashmap.end()) { 
    Node* node = it->second;
    node->value = value;

    removeNode(node);
    insertAtHead(node);

    return;
  }

  // checks if hashmap size is >= to max capacity
  if (hashmap.size() >= static_cast<size_t>(capacity)) { // .size() returns size_t data type, must convert int capacity -> size_t
    Node* lru = tail->prev;
    removeNode(lru);
    hashmap.erase(lru->key); // removes key from hashmap
    delete lru;
  }

  // if key doesn't exist
  Node* node = new Node(key, value);
  insertAtHead(node); // inserted at "head" of list (actually head->next)
  hashmap[key] = node; // key with its corresponding node address are stored in hashmap
}

int LRU_Cache::get(int key) {
  auto it = hashmap.find(key);
  if (it == hashmap.end()) {
    return -1;
  }

  Node* node = it->second;
  int node_value = node->value; // accesses value stores within the node stored in hashmap at key

  removeNode(node);
  insertAtHead(node);

  return node_value;
}

bool LRU_Cache::contains(int key) { // true if hashmap contains key, false if not
  auto it = hashmap.find(key);
  if (it != hashmap.end()) {
    return true;
  }
  else {
    return false;
  }
}

void LRU_Cache::printMap() const { // const only reads values
  for (const auto& pair : hashmap) {
    std::cout << "[key: " << pair.first << ", value: " << pair.second << "], ";
  }
  std::cout << std::endl;
}

void LRU_Cache::printList() const {
  Node* temp = head->next;

  while (temp->next != nullptr) {
    std::cout << "[key: " << temp->key << ", value: " << temp->value << "], ";
    temp = temp->next;
  }
  std::cout << std::endl;
}

int main() {
  int capacity = 5; // capacity can be any number (assuming memory can handle it)
  LRU_Cache cache(capacity);

  // fills hashmap to capacity with key-value pairs
  for (int i=0; i<capacity; i++) {
    cache.put(i, (i+1)*10);
  }

  cache.printMap(); // prints all key-value pairs in hashmap
  cache.printList(); // prints all values in linked list in recency order
  
  cache.put(6, 70);
  cache.printMap(); // adds key-value to hashmap
  cache.printList(); // ejects key: 1 and adds key: 6 to beginning of list
  
  std::cout << cache.get(2) << std::endl; // returns 30
  std::cout << cache.get(0) << std::endl; // returns -1 (linked list ejected key: 0)
  std::cout << cache.contains(0) << std::endl; // returns true (hashmap still contains key: 0)
}