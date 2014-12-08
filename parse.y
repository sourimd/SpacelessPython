%{
#include <iostream>
#include <map>
//#include "ast.h"
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include "Logic.h"
#include <utility>

int errorcheckflag = 0;
extern int yylex();
extern int lineno;
void yyerror(const char * msg);

std::vector<int> functionCallParametersHolder;

FunctionCollections functionCollections = FunctionCollections::getInstance();

//int scoper=1;
//std::map<int, std::map<std::string, float> > tableForVariables;
std::map<std::string, Node*> varTab; // clear the symbol table every time you have a new function
                                     // each function will have its own symbol table
int statementLineNo = 1;
//std::map<int, std::pair<std::string, Node*> > statements;
std::map<int, Command* > statements;
//int ifstart = statementLineNo;
std::vector<Node*> parmList;

%}

%union {
  Node* node;
  float d;
  int i;
  char *str;
}

%token CR

%type <node> expr
%type <node> start
%type <node> funcdef

%type <node> opt_id_list
%type <node> id_list
%type <node> parm_list


%type <node> stmt
%type <node> line

%token ASSIGN
%token COMMA
%token PRINT
%token <str> IDENT 
%token <i> NUMBER
%token <d> FLOAT
%token GLOBAL
%token CALL

%token EQUALS
%token GREATEQ
%token LESSEQ
%token GREAT
%token LESS

%token EXPR
%token LPAREN
%token RPAREN
%token DEF
%token COLON
%token RETURN
%token IF
%token ELSE

%token END
%token ENDIF

%left  PLUS MINUS
%left  MULT DIV MOD
%right EXPON

%%

start   : start funcdef   {;}
        | {;}
        ;

funcdef : DEF IDENT LPAREN opt_id_list RPAREN COLON stmt END CR 
            {
                functionCollections.populate($2, varTab, statements); //pass the varTab along with $2 then vacate it
                
                std::map<std::string, Node*>::iterator pos = varTab.begin();
                while (pos != varTab.end()) {
                      varTab.erase(pos++);
                }
                statements.clear();
                statementLineNo = 1;
                //ifstart = statementLineNo;
            }
        ;

opt_id_list : id_list { $$ = 0; }
            | { $$ = 0; }

id_list : IDENT COMMA id_list   { varTab[std::string($1)] = new NumberNode(0.0, "NA", "parameter",std::string($1) ); }
        | IDENT                 { varTab[std::string($1)] = new NumberNode(0.0, "NA", "parameter",std::string($1)); }
        ;

stmt    : line CR stmt  { 
                            //ifstart = statementLineNo;
                            //++statementLineNo;
                        }
       // | selection CR stmt { std::cout << "1111" << std::endl; }
        | CR stmt { ++statementLineNo; }
        | error CR stmt { errorcheckflag = 1 ; return 1; yyerrok; }
        | { ; }
        ;

line    : IDENT ASSIGN expr {
                                varTab[std::string($1)] = $3;
                                statements[statementLineNo]= new Command("assign",$3, std::string($1));
                                ++statementLineNo;
                            }
        | PRINT expr {
                        statements[statementLineNo]= new Command("print",$2,"");
                        ++statementLineNo;
                     }
        | RETURN expr {
                        statements[statementLineNo]= new Command("return",$2,"");
                        ++statementLineNo;
                      }
        // | IDENT LPAREN opt_parm_list RPAREN { 
        //                                         statements[statementLineNo]= new Command(std::string($1),new NumberNode(0.0),"");
        //                                         ++statementLineNo; 
        //                                     }
        | IF expr COLON 
                        {
                            statements[statementLineNo]= new Command("if",$2,"");
                            ++statementLineNo;
                        }
        | ELSE COLON 
                        {
                            statements[statementLineNo]= new Command("else",NULL,"");
                            ++statementLineNo;
                        }
        | ENDIF          
                        { 
                            statements[statementLineNo]= new Command("endif",NULL,"");
                            ++statementLineNo; 

                        }
        ;

opt_parm_list : parm_list {;}
              | {;}

parm_list 
        : expr COMMA parm_list { //functionCallParametersHolder.push_back($1->eval());

                                    parmList.insert(parmList.begin(), $1);
                                }
        | expr { //functionCallParametersHolder.push_back($1->eval());

                    parmList.insert(parmList.begin(),$1);



               } 

        ;

expr    : expr PLUS expr { $$ = new AddBinaryNode($1,$3, "NA", "Add"); }
        | expr MINUS expr { $$ = new SubBinaryNode($1,$3, "NA", "Sub"); }
        | expr MULT expr { $$ = new MulBinaryNode($1,$3, "NA", "Mul"); }
        | expr EXPON expr { $$ = new ExpBinaryNode($1,$3, "NA", "Exp"); }
        | expr DIV expr { $$ = new DivBinaryNode($1,$3, "NA", "Div");  }
        | expr MOD expr { $$ = new ModBinaryNode($1,$3, "NA", "Mod"); }
        | LPAREN expr EQUALS expr RPAREN { $$ = new EqualsBinaryNode($2,$4, "NA", "Equals"); }
        | LPAREN expr GREATEQ expr RPAREN { $$ = new GreaterEqBinaryNode($2,$4, "NA", "GreatEq"); }
        | LPAREN expr LESSEQ expr RPAREN { $$ = new LessEqBinaryNode($2,$4, "NA", "LessEq"); }
        | LPAREN expr GREAT expr RPAREN { $$ = new GreaterBinaryNode($2,$4, "NA", "Great"); }
        | LPAREN expr LESS expr RPAREN { $$ = new LessBinaryNode($2,$4, "NA", "Less"); }
        
        | MINUS expr { $$ = new UnaryMinusNode($2, "NA", "np"); }
        | NUMBER { $$ = new NumberNode($1, "NA", "np", "noname"); }
        | FLOAT { $$ = new NumberNode($1, "NA", "np", "noname"); }
        | IDENT {   
                    if(varTab[std::string($1)])
                        $$ = varTab[std::string($1)];
                        //$$ = new NumberNode(varTab[std::string($1)]->eval, "NA");
                    else{
                        varTab[std::string($1)] = new NumberNode(0.0, "NA", "np", std::string($1) );
                        $$ = varTab[std::string($1)];
                    }
                    
                } 
        | LPAREN expr RPAREN { $$ = $2; }
        | IDENT LPAREN opt_parm_list RPAREN {
                                                $$ = new FunctionCallNode(std::string($1), parmList);
                                                parmList.clear();
                                            }// have a separate method in functionCollections which will execute this 
                                                // particular function. also pass in the parameters to this functions
                                                // then assign $$ to new NumberNode(whatever is returned from it)
       // | {;}
        ;

        

%%

void yyerror(const char * msg) { 
  std::cout << msg << " at line "<< lineno << std::endl;
}
