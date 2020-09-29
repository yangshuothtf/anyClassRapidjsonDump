#pragma once
//参考boost::any
#include <iostream>
#include <list>
#include <cassert>
#include <string>
#include <cstdlib>
#include <typeinfo>

using namespace std;
//保存真正数据的接口类
class Placeholder {
public:
    virtual ~Placeholder() { }
public:
    virtual const std::type_info& type() const = 0;
    virtual Placeholder* clone() const = 0;
};
//真正保存和获取数据的类
template<typename ValueType>
class Holder : public Placeholder {
public:
    Holder(const ValueType& value) : held(value){
    }
public:
    virtual const std::type_info& type() const {
        return typeid(ValueType);
    }

    virtual Placeholder* clone() const {
        return new Holder(held);//原型
    }
public:
    //真正的数据
    ValueType held;
};

//自定义Any类
class Any {
public:
    Any() : content(NULL) { }

    //真正数据存在content中
    template<typename ValueType>
    Any(const ValueType& value) : content(new Holder<ValueType>(value)) { }

    //拷贝构造函数
    Any(const Any& other) : content(other.content ? other.content->clone() : 0) { }

    ~Any() {
        if (NULL != content) {
            delete content;
        }
    }
private:
    //placeholder对象指针，指向其子类holder的一个实现
    Placeholder* content;
    template<typename ValueType>
    friend ValueType any_cast(const Any& operand);

public:
    //重载<<
    friend ostream& operator<<(ostream& os, const Any& myAny);

    //查询真实数据的类型
    const std::type_info& type() const {
        return content ? content->type() : typeid(void);
    }
};
template<typename ValueType>
ValueType any_cast(const Any& operand) {
    assert(operand.type() == typeid(ValueType));
    return static_cast<Holder<ValueType>*>(operand.content)->held;
}
