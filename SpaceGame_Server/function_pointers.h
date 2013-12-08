#include <iostream>
#include <vector>
typedef void (*func_t)();
typedef std::map<std::string,func_t> func_map_t;
func_map_t func_map;

void func_test(){
	  std::cout << "DoIt"   << std::endl;

};
void func_test2(){
	  std::cout << "KUlt"   << std::endl;

};