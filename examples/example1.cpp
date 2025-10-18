#include <iostream>

int main(){
    auto test = engproj::data_structures::boundedLRUmap<std::string,int>(4,[](int a){return a;});
    test.insert("hello",3);
    auto ya =test.insert("hwowowllo",2);

    std::cout << test.get_capacity() << std::endl;
    for(auto wow : *ya){
        std::cout << wow << std::endl;
    }
}
