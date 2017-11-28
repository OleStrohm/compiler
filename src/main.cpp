#include <iostream>
#include "automata/automata.h"
#include "automata/regex.h"

int main() {
//	Node s("0");
//	Node n2("1");
//	Node n3("2");
//	Automaton a;
//	a.addStartState(&s);
//	a.addEndState(&s);
//	a.link("0", &s, &s);
//	a.link("1", &s, &n2);
//	a.link("0", &n2, &n3);
//	a.link("1", &n2, &s);
//	a.link("0", &n3, &n2);
//	a.link("1", &n3, &n3);
//
//	Node l0("cool");
//	Node l1("middle");
//	Automaton au;
//	au.addStartState(&l0);
//	au.reset();
//	au.addEndState(&l0);
//	au.addEndState(&l1);
//	au.link("A", &l0, &l1);
//	au.link(EPSILON, "A", &l1, &l1, &a);
//
//	au.feed("A");
//	au.feed("1");
//	au.feed("0");
//
//	au.print();
//
//	std::cout << std::endl;
//	au.printLinks();
	
	Regex reg("regexExample.txt");
	
	return 0;
}