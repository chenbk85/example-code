//
// @author visaodumuc
// @brief Covariance polymorphic
// C++ cho phép lớp con override một phương thức của lớp cha
// với tham số trả về khác với đã khai báo trong lớp cha
// với điều kiện tham số trả về ở lớp con có thể chuyển đổi (convertible)
// tới tham số trả về của lớp cha.
//
// g++ -stc=c++11 -o test main.cpp
//

#include <iostream>

class Interface {
  public:
    Interface() {}
    virtual ~Interface() = default;

    // Phương thức này được gọi qua Interface
    void Test() {
        std::cout << "call from interface" << std::endl;
    }
    // Phương thức này sẽ được override lại trong lớp con
    virtual Interface *clone() = 0;
};

class Implemention : public Interface {
  public:
    Implemention() {}
    virtual ~Implemention() = default;
    // Override phương thức từ lớp cha
    virtual Implemention *clone() override {
        return new Implemention();
    }
    // Phương thức này được gọi trực tiếp từ Implemention
    void TestFromImplemention() {
        std::cout << "call from implemention" << std::endl;
    }
};

Interface *makeObject() {
    return new Implemention();
}

int main() {
    {
        // Gọi phương thức từ inteface
        Interface *impl = makeObject();
        Interface *inf = impl->clone();
        inf->Test();
        delete inf;
        delete impl;
    }
    {
        // Gọi phương thức từ Implemention
        Implemention impl;
        Implemention *obj = impl.clone();
        obj->TestFromImplemention();
        delete obj;
    }
    return 0;
}