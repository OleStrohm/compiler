#include <iostream>
#include "automata.h"

int main() {
	Node s("0");
	Node n2("1");
	Node n3("2");
	Automata a;
	a.addStartState(&s);
	a.addEndState(&s);
	a.link('0', &s, &s);
	a.link('1', &s, &n2);
	a.link('0', &n2, &n3);
	a.link('1', &n2, &s);
	a.link('0', &n3, &n2);
	a.link('1', &n3, &n3);

	Node l0("cool");
	Node l1("middle");
	Automata au;
	au.addStartState(&l0);
	au.reset();
	au.addEndState(&l0);
	au.addEndState(&l1);
	au.link('A', &l0, &l1);
	au.link('B', &l1, &l0, &a);

	au.feed('A');
	au.feed('B');
	au.feed('1');
	au.feed('1');
	au.feed('B');
	au.feed('A');

	a.print();
	return 0;
}