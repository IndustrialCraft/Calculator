#include <iostream>

#include "KeyboardIO.hpp"
#include "MainMenu.hpp"
#include "MathSolver/EquationSystem.hpp"
#include "MathSolver/Expression.hpp"
#include "MathSolver/ExpressionPair.hpp"
#include "MathSolver/ExpressionReader.hpp"
#include "MathSolver/Fraction.hpp"
#include "MathSolver/Term.hpp"
#include "Menu.hpp"
#include "MenuSelector.hpp"

int main() {
    KeyboardIO io;
    MenuSelector menuSelector;
    menuSelector.setMenu(new MainMenu());
    while (true) {
        io.tick();
        menuSelector.getMenu().tick(menuSelector, io);
        io.flushDisplay();
    }
    // std::cout << (Fraction(15, 10) - Fraction(2, 1)).toString() << std::endl;
    // std::cout << ((Term(Fraction(15, 10)) * Part('a') * Part('b') * Part('c') * Part('b') / Part('b')) / (Term(Fraction(1, 2)) * Part('c'))).simplify().at(0).toString() << std::endl;
    // Expression expr1 = ExpressionReader().parse("(x+2)(y-1)-(x+5)(y-2)");
    // Expression expr2 = ExpressionReader().parse("(x-4)(y+7)-(x-3)(y+4)");
    // expr1 = expr1.simplify();
    // Expression expr3 = Expression();
    // std::cout << expr1.toString() << std::endl;
    // ExpressionPair ep1 = ExpressionPair(expr1, expr3, ExpressionPair::EOperator::EQUALS);
    // ExpressionPair ep2 = ExpressionPair(expr2, expr3, ExpressionPair::EOperator::EQUALS);
    // EquationSystem es;
    // es.addEquation(ep1);
    // es.addEquation(ep2);
    // auto result = es.solve();
    // std::cout << expr2.simplify().toString() << std::endl;
    // std::cout << expr1.toString() << expr2.toString() << std::endl;
    // std::cout << result.at(Variable('x')).toString(false) << ";" << result.at(Variable('y')).toString(false) << std::endl;
    return 0;
}