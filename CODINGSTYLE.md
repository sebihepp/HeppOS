# Coding Style 
 
## General 
Generally CamelCase is used.

	uint32_t InterruptCount;
	void ThisIsATest(void) {
	}
	
Exceptions are made for kmain() and \_kstart() and  
Exceptions are made for standard functions like itoa() or print(), to maintain compatibility
## Variables 
Variables should be named that the data they contain are easiest understandable  
Exceptions are made for variables where the use is self-explanatory, like in a simple counting for-loop
### Local variables 
Local variables are preceeded with a underscore (_)

	void main(void) {
		uint32_t _LocalVariable = 0;
	}
	
### Global variables 
Global variables are preceeded with a lower case g (g)

	uint32_t gGlobalVariable = 0;
	
### Parameters 
Parameters of functions or methods are preceeded with a lower case p (p)

	uint32_t FunctionTest(uint32_t pParameter) {
		return 0;
	}
	
### Members of classes 
Members of classes are preceeded with a lower case m (m)

	class Test {
	private:
		uint32_t mMemberVariable;
	};
	
## Types 
Types are followed by one underscore and a lower case t (_t)

	struct StructName_t {
	};

## Classes 
Each Class should implement a method Init() like the following:

	ReturnValue_t Init(*necessary parameters*);
	
This method should be called once initially and only afterwards the class should be used

### Classnames 
Each class's name is preceeded with a capital c (C)

	class CTest;
