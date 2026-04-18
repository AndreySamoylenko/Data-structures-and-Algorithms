#include "rbtree.tpp"

int main(){
    RBtree<int> tree;
    tree.add(50);
    tree.add(60);
    tree.add(70);
    tree.add(80);
    tree.add(90);
    tree.add(95);
    tree.add(85);
    tree.add(100);
    tree.add(120);
    tree.add(110);
    tree.print_structure();

    std::cout<< "   " << std::endl;
    tree.remove(80);
    tree.remove(85);
    tree.remove(60);
    tree.print_structure();


    return 0;
}