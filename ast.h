#include <string>
#include <fstream>
#include <cmath>
extern void yyerror(const char*);
extern void yyerror(const char*, const char);

class Node {
public:
  //Node() : id(0) {}
  virtual ~Node() {}
  virtual float eval() const = 0;
  //float getId() const { return id; }
private:
  //float id;
  //static float idCount;
};

class NumberNode : public Node {
public:
  NumberNode(float n) : Node(), number(n) {} 
  virtual ~NumberNode() {}
  float eval() const { return number; }
private:
  float number;
};

class BinaryNode : public Node {
public:
  BinaryNode(Node* l, Node* r) : Node(), left(l), right(r) {}
  virtual ~BinaryNode(){
    delete left;
    delete right;
  }
  virtual float eval() const = 0;
protected:
  Node *left;
  Node *right;
};

class UnaryMinusNode : public Node {
public:
  UnaryMinusNode(Node* expr) : Node(), node(expr) { }
  ~UnaryMinusNode() { delete node; }
  float eval() const { return -node->eval(); }
protected:
  Node *node;
};

class AddBinaryNode : public BinaryNode {
public:
  AddBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
  float eval() const  { 
   return left->eval()+right->eval(); 
  };
};

class SubBinaryNode : public BinaryNode {
public:
  SubBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
  float eval() const { 
    return left->eval()-right->eval(); 
  };
};

class MulBinaryNode : public BinaryNode {
public:
  MulBinaryNode(Node* left, Node* right) : BinaryNode(left, right) {}
  float eval() const { 
    return left->eval()*right->eval(); 
  };
};


class DivBinaryNode : public BinaryNode {
public:
  DivBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
  float eval() const { 
    float result = right->eval();
    if ( result == 0 ) throw std::string("Can't divide by zero!");
    return left->eval()/right->eval(); 
  };
};

class ModBinaryNode : public BinaryNode {
public:
  ModBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
  float eval() const { 
    return int(left->eval()) % int(right->eval()); 
  };
};

class ExpBinaryNode : public BinaryNode {
public:
  ExpBinaryNode(Node* left, Node* right) : BinaryNode(left, right) { }
  float eval() const { 
    return pow(left->eval(), right->eval()); 
  };
};
