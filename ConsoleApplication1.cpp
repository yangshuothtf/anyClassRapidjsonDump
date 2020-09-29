// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <list>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "any.h"

using namespace std;
using namespace rapidjson;

struct Person;
struct Singer { 
    std::string type;
    int age;
};
struct Address { 
    std::string country;
    std::string city;
    std::string street;
    std::vector<Person> neighbors;
};

struct Friend { 
    std::string relation;
    Any secret;
};
struct Person { 
    std::string name;
    int age;
    Address address;
    std::vector<Friend> _friends;
    Any secret;
};

void dumpElements(Any myAny, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
    if (myAny.type() == typeid(int)) {
        writer.Key("Int");
        writer.Int(any_cast<int>(myAny));
    }
    else if (myAny.type() == typeid(std::string)) {
        writer.Key("String");
        writer.String(any_cast<std::string>(myAny).c_str());
    }
    else if (myAny.type() == typeid(Person)) {
        Person tmpPerson = any_cast<Person>(myAny);
        writer.Key("Person");
        writer.StartObject();
        writer.Key("name");
        writer.String(tmpPerson.name.c_str());
        writer.Key("age");
        writer.Int(tmpPerson.age);
        dumpElements(tmpPerson.address, writer);
        writer.Key("_friends");
        writer.StartArray();
        for (int i = 0; i < tmpPerson._friends.size(); i++) {
            dumpElements(tmpPerson._friends[i], writer);
        }
        writer.EndArray();
        dumpElements(tmpPerson.secret, writer);
        writer.EndObject();
    }
    else if (myAny.type() == typeid(Friend)) {
        Friend tmpFriend = any_cast<Friend>(myAny);
        writer.Key("Friend");
        writer.StartObject();
        writer.Key("relation");
        writer.String(tmpFriend.relation.c_str());
        dumpElements(tmpFriend.secret, writer);
        writer.EndObject();
    }
    else if (myAny.type() == typeid(Address)) {
        Address tmpAddress = any_cast<Address>(myAny);
        writer.Key("Address");
        writer.StartObject();
        writer.Key("country");
        writer.String(tmpAddress.country.c_str());
        writer.Key("city");
        writer.String(tmpAddress.city.c_str());
        writer.Key("street");
        writer.String(tmpAddress.street.c_str());
        writer.Key("neighbors");
        writer.StartArray();
        for (int i = 0; i < tmpAddress.neighbors.size(); i++) {
            dumpElements(tmpAddress.neighbors[i], writer);
        }
        writer.EndArray();
        writer.EndObject();
    }
    else if (myAny.type() == typeid(Singer)) {
        Singer tmpSinger = any_cast<Singer>(myAny);
        writer.Key("Singer");
        writer.StartObject();
        writer.Key("type");
        writer.String(tmpSinger.type.c_str());
        writer.Key("age");
        writer.Int(tmpSinger.age);
        writer.EndObject();
    }
}

std::string dump(Any myAny) {
    rapidjson::StringBuffer buf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf);
    writer.StartObject();
    dumpElements(myAny, writer);
    writer.EndObject();
    //const char* json_content = buf.GetString();
    std::string str(buf.GetString());
    return str;
}

void dumpElements2(Any myAny, Value& tmpValue, Document::AllocatorType& allocator) {
    if (myAny.type() == typeid(int)) {
        writer.Key("Int");
        writer.Int(any_cast<int>(myAny));
    }
    else if (myAny.type() == typeid(Person)) {
        Value personValue;
        personValue.SetObject();
        Person tmpPerson = any_cast<Person>(myAny);

        Value nameValue(kStringType);
        nameValue.SetString(tmpPerson.name.c_str(), allocator);
        personValue.AddMember("name", nameValue, allocator);

        Value ageValue(kNumberType);
        ageValue.SetInt(tmpPerson.age);
        personValue.AddMember("age", ageValue, allocator);

    https://www.cnblogs.com/MenAngel/p/11512882.html
    http://blog.chinaunix.net/uid-20682147-id-5817728.html#_Toc14481
    https://blog.csdn.net/u014449046/article/details/79070301

        tmpValue.AddMember("Person", );
        writer.Key("Person");
        writer.StartObject();
        writer.Key("name");
        writer.String(tmpPerson.name.c_str());
        writer.Key("age");
        writer.Int(tmpPerson.age);
        dumpElements(tmpPerson.address, writer);
        writer.Key("_friends");
        writer.StartArray();
        for (int i = 0; i < tmpPerson._friends.size(); i++) {
            dumpElements(tmpPerson._friends[i], writer);
        }
        writer.EndArray();
        dumpElements(tmpPerson.secret, writer);
        writer.EndObject();
    }
}

std::string dump2(Any myAny) {
    Document doc;
    Document::AllocatorType& allocator = doc.GetAllocator();
    Value& docValue = doc.SetObject();//实例化一个GenericValue到根DOM
    dumpElements2(myAny, docValue, allocator);

    Value tempValue1;
    tempValue1.SetObject();

    tempValue1.SetString(name.c_str(), allocator);
    doc.AddMember("name", tempValue1, allocator);
    Value tempValue2(rapidjson::kObjectType);
    tempValue2.SetString(gender.c_str(), allocator);
    doc.AddMember("gender", tempValue2, allocator);
    doc.AddMember("age", age, allocator);
    Value tempValue3(kArrayType);
}
void parseElements(const rapidjson::Value& obj, Any *pAny) {
    Document d;
    
    if (pAny->type() == typeid(Person)) {
        const rapidjson::Value& objPerson = obj["Person"];
        Person tmpPerson = any_cast<Person>(*pAny);
        if (objPerson.HasMember("name") && objPerson["name"].IsString()) {
            tmpPerson.name = objPerson["name"].GetString();
        }
        if (objPerson.HasMember("age") && objPerson["age"].IsInt()) {
            tmpPerson.age = objPerson["age"].GetInt();
        }
        if (objPerson.HasMember("address") && objPerson["address"].IsObject()) {
            const rapidjson::Value& objAddress = objPerson["address"];
            Address tmpAddress;
            parseElements(objAddress, (Any *)(&tmpAddress));
            tmpPerson.address = tmpAddress;
        }

        if (objPerson.HasMember("_friends") && objPerson["_friends"].IsArray()) {
            const rapidjson::Value& objFriends = objPerson["_friends"];
            assert(objFriends.IsArray());

/*            for (SizeType i = 0; i < objFriends.Size(); i++) {
                if (objFriends[i].IsObject()) {
                    int i = 0;
                }
                Friend tmpFriend;
                if (objFriends[i].HasMember("relation")) {
                    int i = 0;
                }
                parseElements(objFriends[i], (Any*)(&tmpFriend));
                tmpPerson._friends.push_back(tmpFriend);

            }*/
            for (Value::ConstValueIterator itr = objFriends.Begin(); itr != objFriends.End(); ++itr) {
                Friend tmpFriend;
                if (itr->HasMember("Friend")) {
                    int i = 0;
                }
                if (itr->HasMember("relation")) {
                    int i = 0;
                }
                parseElements(*itr, (Any*)(&tmpFriend));
                tmpPerson._friends.push_back(tmpFriend);
            }
        }
    }
    else if (pAny->type() == typeid(Friend)) {
        const rapidjson::Value& objFriend = obj["Friend"];
        Friend tmpFriend = any_cast<Friend>(*pAny);
        if (objFriend.HasMember("relation") && objFriend["relation"].IsString()) {
            tmpFriend.relation = objFriend["relation"].GetString();
        }

    }
}
Any parse(string json) {
    Any myAny;
    rapidjson::Document dom;
    if (!dom.Parse(json.c_str()).HasParseError()) {
        //推测对象具体类型
        if ((dom.HasMember("Person") && dom["Person"].IsObject()) ) {
            //这是Person
            Person tmpPerson;
            const rapidjson::Value& objPerson = dom["Person"];
            if (objPerson.HasMember("_friends") && objPerson["_friends"].IsArray()) {
                const rapidjson::Value& objFriends = objPerson["_friends"];
                Type myType1 = objFriends.GetType();
                assert(objFriends.IsArray());
                int aa = objFriends.Size();
                            for (SizeType i = 0; i < objFriends.Size(); i++) {
                                Type myType = objFriends[i].GetType();

                                if (objFriends[i].IsObject()) {
                                    int i = 0;
                                }
                                Friend tmpFriend;
                                if (objFriends[i].HasMember("Friend")) {
                                    int i = 0;
                                }
                                parseElements(objFriends[i], (Any*)(&tmpFriend));
                                tmpPerson._friends.push_back(tmpFriend);

                            }
                for (Value::ConstValueIterator itr = objFriends.Begin(); itr != objFriends.End(); ++itr) {
                    Friend tmpFriend;
                    if (itr->HasMember("Friend")) {
                        int i = 0;
                    }
                    if (itr->HasMember("relation")) {
                        int i = 0;
                    }
                    parseElements(*itr, (Any*)(&tmpFriend));
                    tmpPerson._friends.push_back(tmpFriend);
                }
            }


            parseElements(dom, &((Any)tmpPerson));
            myAny = (Any)tmpPerson;
        }
    }
    std::cout << myAny << std::endl;
    return myAny;
}
int main()
{
    Friend f1{ "my best friend", Singer{"rocker", 18} };
    Friend f2{ "new friend", std::string("little girl") };
    Friend f3{ "third friend", 3 };
    Person p2{ "p2", 3, Address{"china", "shanghai", "putuo"} };
    Address addr1{ "china", "beijing", "wangjing", {p2} };
    Person p1{ "p1", 4, addr1, {f1, f2, f3}, std::string("the kind!") };
    // TODO. 以下是伪代码，需要笔试者具体实现
    auto json1 = dump(p1);               // 序列化    
    std::cout << "打印序列化结果" << std::endl;
    std::cout << json1 << std::endl;     // 打印序列化结果
    std::cout << "打印Person对象" <<std::endl;
    std::cout << (Any)p1 << std::endl;    // 打印 Person 对象 
    auto pp = parse(json1);           // 反序列化 
    std::cout << "反序列化后打印对象" << std::endl;
    std::cout << (Any)pp << std::endl;    // 打印 Person 对象 
    // assert(p1 == pp)                 // 反序列化的结果是对的
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
