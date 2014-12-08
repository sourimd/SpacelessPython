#include <iostream>
#include "Logic.h"
extern int yyparse();
extern int errorcheckflag;
extern FunctionCollections functionCollections;
//extern int errorcheckflag;


int main() {
  if ( yyparse() ) {
    return 1;
  }
  else {
    if(errorcheckflag == 0){
      functionCollections.execute();
      std::cout << "accepted" << std::endl;
    }
      
    else{
      std::cout << "syntax error" << std::endl;
    }

    
  }
  return 0;
}
