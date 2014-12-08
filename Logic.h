#include <vector>
#include <iostream>
//#include "functionObject.h"
//#include "ast.h"
#include <map>
#include <cstring>
#include <iostream>
#include <map>

#include <string>
#include <fstream>
#include <cmath>
#include <utility>
#include <typeinfo>


extern void yyerror(const char*);
extern void yyerror(const char*, const char);

class Node {
public:
  Node(std::string actualName) : actualName(actualName) {}
  virtual ~Node() {}
  virtual float eval() const = 0;
  virtual std::string getName() = 0;
  virtual void setRetVal(float v) = 0;
  virtual std::string getParameterType() = 0;
  std::string actualName;
  //float getId() const { return id; }
private:
  //std::string name;
  //float id;
  //static float idCount;
};

class NumberNode : public Node {
public:
  NumberNode(float n, std::string name, std::string parameterType, std::string actualName) : Node(actualName), number(n), name(name), parameterType(parameterType) {} 
  virtual ~NumberNode() {}
  float eval() const { return number; }
  std::string getName(){ return name;}
  void setRetVal(float v){ ;}
  std::string getParameterType(){ return parameterType;}
  //std::string actualName;
private:
  float number;
  std::string name;
  std::string parameterType;
};

class BinaryNode : public Node {
public:
  BinaryNode(Node* l, Node* r, std::string actualName) : Node(actualName), left(l), right(r) {}
  virtual ~BinaryNode(){
    delete left;
    delete right;
  }
  virtual float eval() const = 0;
  virtual std::string getName(){ return "binary";}
  virtual void setRetVal(float v){ ;}
  virtual std::string getParameterType(){ return "np";}
  virtual Node * getLeft(){ return left; }
  virtual Node * getRight(){ return right; } 
protected:
  Node *left;
  Node *right;
};

class UnaryMinusNode : public Node {
public:
  UnaryMinusNode(Node* expr, std::string name, std::string parameterType) : Node("unary"), node(expr), name(name), parameterType(parameterType) { }
  ~UnaryMinusNode() { delete node; }
  float eval() const { return -node->eval(); }
  std::string getName(){ return name;}
  void setRetVal(float v){ ;}
  std::string getParameterType() {return "np";}
protected:
  Node *node;
  std::string name;
  std::string parameterType;
};

//-----------------
class FunctionCallNode : public Node {
public:
  FunctionCallNode(std::string functionNodeName, std::vector<Node *> parmList) : Node("functionname"),functionNodeName(functionNodeName), parmList(parmList), retVal(11.0) { }
  ~FunctionCallNode() { ; }//dont forget to delete stuffs
  float eval() const { return retVal; }
  std::string getName(){ return functionNodeName;}
  void setRetVal(float v){ retVal=v;}
  std::string getParameterType() {return "np";}
  std::vector<Node *> getParmList() { return parmList; }
protected:
  std::string functionNodeName;
  std::vector<Node *> parmList;
  float retVal;
  //Node *node;
};
//-----------------

class AddBinaryNode : public BinaryNode {
public:
  AddBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const  { 
   return left->eval()+right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class SubBinaryNode : public BinaryNode {
public:
  SubBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval()-right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class MulBinaryNode : public BinaryNode {
public:
  MulBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right,parameterType), name(name), parameterType(parameterType) {}
  float eval() const { 
    return left->eval()*right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};


class DivBinaryNode : public BinaryNode {
public:
  DivBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    float result = right->eval();
    if ( result == 0 ) throw std::string("Can't divide by zero!");
    return left->eval()/right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class ModBinaryNode : public BinaryNode {
public:
  ModBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return int(left->eval()) % int(right->eval()); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class ExpBinaryNode : public BinaryNode {
public:
  ExpBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return pow(left->eval(), right->eval()); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class EqualsBinaryNode : public BinaryNode {
public:
  EqualsBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval() == right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class GreaterBinaryNode : public BinaryNode {
public:
  GreaterBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval() > right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class LessBinaryNode : public BinaryNode {
public:
  LessBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval() < right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class GreaterEqBinaryNode : public BinaryNode {
public:
  GreaterEqBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval() >= right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};

class LessEqBinaryNode : public BinaryNode {
public:
  LessEqBinaryNode(Node* left, Node* right, std::string name, std::string parameterType) : BinaryNode(left, right, parameterType), name(name), parameterType(parameterType) { }
  float eval() const { 
    return left->eval() <= right->eval(); 
  };
  std::string getName(){ return name;};
  void setRetVal(float v){ ;};
  std::string getParameterType() {return parameterType; };
  //std::string actualName;
private:
  std::string name;
  std::string parameterType;
};
//--------------------------------------------------------------

class Command{
  private:
    std::string type;
    Node * node;
    std::string varNameIfAny;
    // int ifstartlineno;
    // int ifendlineno;
  public:
    Command(std::string s, Node * a, std::string varName): type(s),node(a), varNameIfAny(varName){}
    std::string getType(){
      return type;
    }
    void setType(std::string str){
      type = str;
    }
    std::string getVarName(){
      return varNameIfAny;
    }
    void setVarName(std::string str){
      varNameIfAny = str;
    }
    Node * getNode(){
      return node;
    }
    void setNode(Node * a){
      node = a;
    }
};

class FunctionObject {
	public:
		 
		//std::vector<StatementObject> statementObjects;
		//std::map<std::string,int> variablesSymbolTable;
		FunctionObject(std::string name, std::map<std::string, Node*> varTab, std::map<int, Command* > statements): functionName(name), varTab(varTab), statements(statements)
		{}
		std::string getFunctionName(){return functionName;}
		std::map<int, Command * > getStatements(){ return statements;}
		
    Node* execute(std::vector<FunctionObject> functionObjects, std::vector<Node *> & parmList){
        //std::cout << "herecalled---" << parmList.size() << std::endl;
        Node * returnNode = NULL;
        std::map<std::string, Node *>::iterator pos = varTab.begin(); 
        for(unsigned int z=0; z<parmList.size(); z++){
          varTab[pos->first] = new NumberNode(parmList[z]->eval(), "NA", "parameter", pos->first);
          //std::cout << pos->first << " " << pos->second->eval() << std::endl;
          ++pos;
        }
        int executeIfBlock=2;
        int nonExecutionMode=0;
        //std::cout << "2" << std::endl;
			  std::map<int, Command* > ::const_iterator ptr1 = statements.begin();
			  while ( ptr1 != statements.end() ) {
          std::string ss = std::string(ptr1->second->getType());
			  	
          if(ss.compare("print") == 0){
            if(nonExecutionMode == 1){
              ++ptr1;
              continue;

            }
            std::string fname = ptr1->second->getNode()->getName();
            if(fname.compare("NA")!=0){
              //execute this function of name fname
              for (unsigned int i = 0; i < functionObjects.size(); ++i) {
                if(functionObjects[i].getFunctionName().compare(fname) == 0){
                  std::vector<Node *> parmList = dynamic_cast<FunctionCallNode *>(ptr1->second->getNode())->getParmList();
                  Node* nodeReturned = functionObjects[i].execute(functionObjects, parmList);
                  std::cout << nodeReturned->eval() << std::endl;
                }
              }
            }
            else{
              std::cout << ptr1->second->getNode()->eval() << std::endl;
            }
          }
			    	
			    else if(ss.compare("return") == 0){
              if(nonExecutionMode == 1){
                ++ptr1;
                continue;

              }
              std::string fname1 = ptr1->second->getNode()->getName();
              if(fname1.compare("NA")!=0){
                for(unsigned int k=0; k<functionObjects.size(); k++){
                  if(functionObjects[k].getFunctionName().compare(fname1)==0){
                    std::vector<Node *> parmList = dynamic_cast<FunctionCallNode *>(ptr1->second->getNode())->getParmList();
                    returnNode = functionObjects[k].execute(functionObjects, parmList);
                  }
                }
              }
              else{
                Node * n = ptr1->second->getNode();
                if(n->actualName.compare("noname")==0)
                  returnNode = ptr1->second->getNode();
                //---------------------------
                else if(n->actualName.compare("Add")==0){
                  //std::cout << "entered" << std::endl;
                  Node * left = dynamic_cast<AddBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<AddBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new AddBinaryNode(left, right, "NA", "Add");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //-------------------------------------------
                //---------------------------
                else if(n->actualName.compare("Sub")==0){
                  Node * left = dynamic_cast<SubBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<SubBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new SubBinaryNode(left, right, "NA", "Sub");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //-------------------------------------------
                //---------------------------
                else if(n->actualName.compare("Mul")==0){
                  Node * left = dynamic_cast<MulBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<MulBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new MulBinaryNode(left, right, "NA", "Mul");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //-------------------------------------------
                //---------------------------
                else if(n->actualName.compare("Div")==0){
                  Node * left = dynamic_cast<DivBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<DivBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new DivBinaryNode(left, right, "NA", "Div");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //-------------------------------------------
                //---------------------------
                else if(n->actualName.compare("Exp")==0){
                  Node * left = dynamic_cast<ExpBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<ExpBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new ExpBinaryNode(left, right, "NA", "Exp");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //-------------------------------------------
                //---------------------------
                else if(n->actualName.compare("Mod")==0){
                  Node * left = dynamic_cast<ModBinaryNode *>(n)->getLeft();
                  Node * right = dynamic_cast<ModBinaryNode *>(n)->getRight();
                  if(left->getParameterType().compare("parameter") == 0){
                    left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  }
                  if(right->getParameterType().compare("parameter") == 0){
                    right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  }
                  n = new ModBinaryNode(left, right, "NA", "Mod");
                  ptr1->second->setNode(n);
                  returnNode = ptr1->second->getNode();
                }
                //------------------------------------------- 
                else
                  returnNode = varTab[n->actualName];
              }
			    }
			    else if(ss.compare("assign") == 0){
            if(nonExecutionMode == 1){
              ++ptr1;
              continue;
            }
            std::string fname1 = ptr1->second->getNode()->getName();
            if(fname1.compare("NA")!=0){
              for (unsigned int j = 0; j < functionObjects.size(); ++j) {
                if(functionObjects[j].getFunctionName().compare(fname1) == 0){
                  std::vector<Node *> parmList = dynamic_cast<FunctionCallNode *>(ptr1->second->getNode())->getParmList();
                  Node* nodeReturned1 = functionObjects[j].execute(functionObjects, parmList);

                  ptr1->second->getNode()->setRetVal(nodeReturned1->eval());

                  varTab.erase(ptr1->second->getVarName());
                  varTab[ptr1->second->getVarName()]=new NumberNode(nodeReturned1->eval(),"NA", "np",ptr1->second->getVarName());
                  
                }
              }
            }
            else{
              Node * n = ptr1->second->getNode();
              //------------------------------------------
              if(n->getParameterType().compare("Add") == 0){
                Node * left = dynamic_cast<AddBinaryNode *>(n)->getLeft();
                //std::cout << varTab["b"]->eval() << std::endl;
                Node * right = dynamic_cast<AddBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  
                  //std::cout << dynamic_cast<NumberNode *>(left)->actualName << std::endl;
                  
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  //left = new NumberNode(left->eval(),"NA","np","noname");
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                n = new AddBinaryNode(left, right, "NA", "Add");
                //n = new NumberNode(n->eval(),"NA","parameter","noname");
              }
              //--------------------------------------------------
              //------------------------------------------
              if(n->getParameterType().compare("Sub") == 0){
                Node * left = dynamic_cast<SubBinaryNode *>(n)->getLeft();
                Node * right = dynamic_cast<SubBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                n = new SubBinaryNode(left, right, "NA", "Sub");
              }
              //--------------------------------------------------
              //------------------------------------------
              if(n->getParameterType().compare("Mul") == 0){
                //std::cout << "in mul" << varTab["n"] << std::endl;
                Node * left = dynamic_cast<MulBinaryNode *>(n)->getLeft();
                Node * right = dynamic_cast<MulBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                  //std::cout << "left "<< left->eval() << std::endl;
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                  //std::cout << "right " << right->eval() << std::endl;
                }
                //std::cout << "checking on b "<< varTab["b"]->eval() << std::endl;
                n = new MulBinaryNode(left, right, "NA", "Mul");
              }
              //--------------------------------------------------
              //------------------------------------------
              if(n->getParameterType().compare("Div") == 0){
                Node * left = dynamic_cast<DivBinaryNode *>(n)->getLeft();
                Node * right = dynamic_cast<DivBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                n = new DivBinaryNode(left, right, "NA", "Div");
              }
              //--------------------------------------------------
              //------------------------------------------
              if(n->getParameterType().compare("Mod") == 0){
                Node * left = dynamic_cast<ModBinaryNode *>(n)->getLeft();
                Node * right = dynamic_cast<ModBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                n = new ModBinaryNode(left, right, "NA", "Mod");
              }
              //--------------------------------------------------
              //------------------------------------------
              if(n->getParameterType().compare("Exp") == 0){
                Node * left = dynamic_cast<ExpBinaryNode *>(n)->getLeft();
                Node * right = dynamic_cast<ExpBinaryNode *>(n)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                n = new ExpBinaryNode(left, right, "NA", "Exp");
              }
              //--------------------------------------------------
              n = new NumberNode(n->eval(), "NA", "np", ptr1->second->getVarName());
              ptr1->second->setNode(n);
              varTab[ptr1->second->getVarName()] = ptr1->second->getNode();
              // if(varTab["c"])
              //   std::cout << "c-- "<<varTab["c"]->eval() << std::endl;
              //varTab[ptr1->second->getVarName()] = new NumberNode(n->eval(), "NA", "parameter", ptr1->second->getVarName());
            }
			    }
          else if(ptr1->second->getType().compare("if") == 0){
            Node * nr1 =  ptr1->second->getNode();
            float resultOfComparison;
            if(nr1->getParameterType().compare("Equals")==0){
                Node * left = dynamic_cast<EqualsBinaryNode *>(nr1)->getLeft();
                Node * right = dynamic_cast<EqualsBinaryNode *>(nr1)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                nr1 = new EqualsBinaryNode(left, right, "NA", "Equals");
                resultOfComparison = nr1->eval();
            }
            else if(nr1->getParameterType().compare("GreatEq")==0){
                Node * left = dynamic_cast<GreaterEqBinaryNode *>(nr1)->getLeft();
                Node * right = dynamic_cast<GreaterEqBinaryNode *>(nr1)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                nr1 = new GreaterEqBinaryNode(left, right, "NA", "GreatEq");
                resultOfComparison = nr1->eval();
            }
            else if(nr1->getParameterType().compare("LessEq")==0){
                Node * left = dynamic_cast<LessEqBinaryNode *>(nr1)->getLeft();
                Node * right = dynamic_cast<LessEqBinaryNode *>(nr1)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                nr1 = new LessEqBinaryNode(left, right, "NA", "Equals");
                resultOfComparison = nr1->eval();
            }
            else if(nr1->getParameterType().compare("Great")==0){
                Node * left = dynamic_cast<GreaterBinaryNode *>(nr1)->getLeft();
                Node * right = dynamic_cast<GreaterBinaryNode *>(nr1)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                nr1 = new GreaterBinaryNode(left, right, "NA", "Great");
                resultOfComparison = nr1->eval();
            }
            else if(nr1->getParameterType().compare("Less")==0){
                Node * left = dynamic_cast<LessBinaryNode *>(nr1)->getLeft();
                Node * right = dynamic_cast<LessBinaryNode *>(nr1)->getRight();
                if(left->getParameterType().compare("parameter") == 0){
                  left = varTab[dynamic_cast<NumberNode *>(left)->actualName];
                }
                if(right->getParameterType().compare("parameter") == 0){
                  right = varTab[dynamic_cast<NumberNode *>(right)->actualName];
                }
                nr1 = new LessBinaryNode(left, right, "NA", "Less");
                resultOfComparison = nr1->eval();
            }
            else{
                resultOfComparison = nr1->eval();
            }
            
            if(resultOfComparison != 0.0){
              executeIfBlock = 1;
            }
            else{
              executeIfBlock = 0;
              nonExecutionMode = 1; 
            }
          }
          else if(ptr1->second->getType().compare("else") == 0){
            if(executeIfBlock == 1){
              nonExecutionMode = 1;
            }
            else{
              nonExecutionMode = 0;
            }
          }
          else if(ptr1->second->getType().compare("endif") == 0){
            if(nonExecutionMode == 1){
              nonExecutionMode = 0;
              executeIfBlock = 2;
            }
          }
			    else{
			    }	
			    ++ptr1;
			  }
        return returnNode;
		}
    std::map<std::string, Node*> & getVarTab(){ return varTab; }
	private:
		std::string functionName;
		std::map<std::string, Node*> varTab;
		std::map<int, Command* > statements;
};

//-----------------------------------------------------------------




class FunctionCollections {
public:
  static FunctionCollections& getInstance(){
    static FunctionCollections instance;
    return instance;
  }
  void populate(std::string name, std::map<std::string, Node*> varTab, std::map<int, Command* > statements){
  	FunctionObject tempFunctionObject(name, varTab, statements);
  	functionObjects.push_back(tempFunctionObject);
  }
  void execute(){
  	
  	for (unsigned int i = 0; i < functionObjects.size(); ++i) {
    	if(functionObjects[i].getFunctionName().compare("main") == 0){
    		//std::cout << "Start Executing" << std::endl;
        int executeIfBlock=2;
        int nonExecutionMode=0;
    		std::map<int, Command* > stmt = functionObjects[i].getStatements();
    		std::map<int, Command* > ::const_iterator ptr1 = stmt.begin();
    		while ( ptr1 != stmt.end() ) {
			  	
          if(ptr1->second->getType().compare("print") == 0){
            if(nonExecutionMode == 1){
              ++ptr1;
              continue;

            }
            //------------------
            std::string fname = ptr1->second->getNode()->getName();
            if(fname.compare("NA")!=0){
              for (unsigned int i = 0; i < functionObjects.size(); ++i) {
                if(functionObjects[i].getFunctionName().compare(fname) == 0){
                  std::vector<Node *> parmList = dynamic_cast<FunctionCallNode *>(ptr1->second->getNode())->getParmList();
                  Node* nodeReturned = functionObjects[i].execute(functionObjects, parmList);
                  ptr1->second->getNode()->setRetVal(nodeReturned->eval());
                  std::cout << nodeReturned->eval() << std::endl;
                }
              }
            }
            else{
              std::cout << ptr1->second->getNode()->eval() << std::endl;
            }
            //-----------------------------------
          }
			    	
			    else if(ptr1->second->getType().compare("assign") == 0){
            if(nonExecutionMode == 1){
              ++ptr1;
              continue;
            }
            std::string fname1 = ptr1->second->getNode()->getName();
            if(fname1.compare("NA")!=0){
              //execute this function
              for (unsigned int j = 0; j < functionObjects.size(); ++j) {
                if(functionObjects[j].getFunctionName().compare(fname1) == 0){
                  std::vector<Node *> parmList = dynamic_cast<FunctionCallNode *>(ptr1->second->getNode())->getParmList();
                  //std::cout << "here---" << parmList.size() << std::endl;
                  Node* nodeReturned1 = functionObjects[j].execute(functionObjects, parmList); // functionObjects[i] points to called function
                                    
                  ptr1->second->getNode()->setRetVal(nodeReturned1->eval());
                  
                  std::map<std::string, Node*> vt1 = functionObjects[i].getVarTab();//this shud be host functions varTab

                  vt1[ptr1->second->getVarName()] = new NumberNode(nodeReturned1->eval(),"NA", "np", ptr1->second->getVarName());
                }
              }
            }
            else{

              std::map<std::string, Node*> vt = functionObjects[i].getVarTab();
              vt[ptr1->second->getVarName()] = new NumberNode(ptr1->second->getNode()->eval(),"NA", "np",ptr1->second->getVarName());
              //vt[ptr1->second->getVarName()] = ptr1->second->getNode();
            }

            
			    }
          else if(ptr1->second->getType().compare("if") == 0){
            float resultOfComparison = ptr1->second->getNode()->eval();
            if(resultOfComparison != 0.0){
              executeIfBlock = 1;
            }
            else{
              executeIfBlock = 0;
              nonExecutionMode = 1; 
            }
          }
          else if(ptr1->second->getType().compare("else") == 0){
            if(executeIfBlock == 1){
              nonExecutionMode = 1;
            }
            else{
              nonExecutionMode = 0;
            }
          }
          else if(ptr1->second->getType().compare("endif") == 0){
            if(nonExecutionMode == 1){
              nonExecutionMode = 0;
              executeIfBlock = 2;
            }
          }
          else if(ptr1->second->getType().compare("return") == 0){
            // std::cout << "here" << std::endl;
            // std::cout << ptr1->second->getNode()->getName() << std::endl;
            if(ptr1->second->getNode()->getName().compare("NA")!=0){
              //execute this function
              // for (unsigned int i = 0; i < functionObjects.size(); ++i) {
              //   if(functionObjects[i].getFunctionName().compare("main") == 0){}
              // }
            }
            else{
              ptr1->second->getNode()->eval();
            }
          }
          // this section is for function call not for when function call is used as expression..just pure function call
			    else{
			    }

			    ++ptr1;
			}
    	}
  	}
  }
  //std::vector<FunctionObject> getFunctionObjects(){return functionObjects; }
private:
  //FunctionCollections(const FunctionCollections&);
  //FunctionCollections& operator=(const FunctionCollections&);
  std::vector<FunctionObject> functionObjects;
  FunctionCollections() {}
  
};




