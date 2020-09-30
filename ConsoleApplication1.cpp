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

void dumpElements(const Any& myAny, const std::string& myName, Value& myValue, Document::AllocatorType& allocator) {
    Value nameValue(kStringType);
    nameValue.SetString(myName.c_str(), allocator);
    if (myAny.type() == typeid(int)) {
        if (myName.empty()) {
            nameValue.SetString("Int", allocator);
        }
        Value intValue(kNumberType);
        intValue.SetInt(any_cast<int>(myAny));
        myValue.AddMember(nameValue, intValue, allocator);
    }
    else if (myAny.type() == typeid(std::string)) {
        if (myName.empty()) {
            nameValue.SetString("String", allocator);
        }
        Value strValue(kStringType);
        strValue.SetString(any_cast<std::string>(myAny).c_str(), allocator);
        myValue.AddMember(nameValue, strValue, allocator);
    }
    else if (myAny.type() == typeid(Person)) {
        if (myName.empty()) {
            nameValue.SetString("Person", allocator);
        }
        Person tmpPerson = any_cast<Person>(myAny);
        Value personValue;
        personValue.SetObject();

        dumpElements(tmpPerson.name, "name", personValue, allocator);
        dumpElements(tmpPerson.age, "age", personValue, allocator);
        dumpElements(tmpPerson.address, "address", personValue, allocator);

        Value friendsArrayValue(kArrayType);
        for (int i = 0; i < tmpPerson._friends.size(); i++) {
            Friend& tmpFriend = tmpPerson._friends[i];
            Value friendValue;
            friendValue.SetObject();
            dumpElements(tmpFriend, "", friendValue, allocator);
            friendsArrayValue.PushBack(friendValue, allocator);
        }
        personValue.AddMember("_friends", friendsArrayValue, allocator);

        Value secretValue;
        secretValue.SetObject();
        Any& tmpAny = tmpPerson.secret;
        dumpElements(tmpAny, "secret", personValue, allocator);
        myValue.AddMember(nameValue, personValue, allocator);
    }
    else if (myAny.type() == typeid(Friend)) {
        if (myName.empty()) {
            nameValue.SetString("Friend", allocator);
        }
        Friend tmpFriend = any_cast<Friend>(myAny);
        Value friendValue;
        friendValue.SetObject();

        dumpElements(tmpFriend.relation, "relation", friendValue, allocator);

        Value secretValue;
        secretValue.SetObject();
        Any& tmpAny = tmpFriend.secret;
        dumpElements(tmpAny, "secret", friendValue, allocator);
        myValue.AddMember(nameValue, friendValue, allocator);
    }
    else if (myAny.type() == typeid(Address)) {
        if (myName.empty()) {
            nameValue.SetString("Address", allocator);
        }
        Address tmpAddress = any_cast<Address>(myAny);
        Value addressValue;
        addressValue.SetObject();

        dumpElements(tmpAddress.country, "country", addressValue, allocator);
        dumpElements(tmpAddress.city, "city", addressValue, allocator);
        dumpElements(tmpAddress.street, "street", addressValue, allocator);

        Value neighborsArrayValue(kArrayType);
        for (int i = 0; i < tmpAddress.neighbors.size(); i++) {
            Person& tmpNeighbor = tmpAddress.neighbors[i];
            Value neighborValue;
            neighborValue.SetObject();
            dumpElements(tmpNeighbor, "", neighborValue, allocator);
            neighborsArrayValue.PushBack(neighborValue, allocator);
        }
        addressValue.AddMember("neighbors", neighborsArrayValue, allocator);
        myValue.AddMember(nameValue, addressValue, allocator);
    }
    else if (myAny.type() == typeid(Singer)) {
        if (myName.empty()) {
            nameValue.SetString("Singer", allocator);
        }
        Singer tmpSinger = any_cast<Singer>(myAny);
        Value singerValue;
        singerValue.SetObject();

        dumpElements(tmpSinger.type, "type", singerValue, allocator);
        dumpElements(tmpSinger.age, "age", singerValue, allocator);
        myValue.AddMember(nameValue, singerValue, allocator);
    }
}

std::string dump(Any myAny) {
    Document doc;
    Value& docValue = doc.SetObject();//实例化一个GenericValue到根DOM
    Document::AllocatorType& allocator = doc.GetAllocator();
    dumpElements(myAny, "", docValue, allocator);

    StringBuffer strBuffer;
    PrettyWriter<StringBuffer> writer(strBuffer);
    doc.Accept(writer);
    return strBuffer.GetString();
}

Any parseElements(const rapidjson::Value& obj, const Any& pAny) {
	  Any rtnAny;
/   if ((obj.HasMember("Person") && obj["Person"].IsObject())) {
 //   if (pAny.type() == typeid(Person)) {
        const rapidjson::Value& objPerson = obj["Person"];
        Person tmpPerson;
        if (objPerson.HasMember("name") && objPerson["name"].IsString()) {
            tmpPerson.name = objPerson["name"].GetString();
        }
        if (objPerson.HasMember("age") && objPerson["age"].IsInt()) {
            tmpPerson.age = objPerson["age"].GetInt();
        }
        if (objPerson.HasMember("address") && objPerson["address"].IsObject()) {
            const rapidjson::Value& objAddress = objPerson["address"];
            Any tmpAny = (Any)tmpPerson.address;
            parseElements(objAddress, tmpAny);
            tmpPerson.address = any_cast<Address>(tmpAny);
        }
        if (objPerson.HasMember("_friends") && objPerson["_friends"].IsArray()) {
            const rapidjson::Value& objFriends = objPerson["_friends"];
            for (int i = 0; i < objFriends.Size(); ++i) {
                Friend tmpFriend;
                Any tmpAny = (Any)tmpFriend;
                parseElements(objFriends[i], tmpAny);
                tmpFriend = any_cast<Friend>(tmpAny);
                tmpPerson._friends.push_back(tmpFriend);
            }
        }
        if (objPerson.HasMember("secret") && objPerson["secret"].IsObject()) {
            const rapidjson::Value& objSecret = objPerson["secret"];
            Any tmpAny = (Any)tmpPerson.secret;
            parseElements(objSecret, tmpAny);
            tmpPerson.secret = any_cast<Address>(tmpAny);
        }
        return (Any)tmpPerson;
    }
    else if ((obj.HasMember("Friend") && obj["Friend"].IsObject())) {
//    else if (pAny.type() == typeid(Friend)) {
        std::cout << "enter Friend" << std::endl;
        Friend tmpFriend;
        return (Any)tmpFriend;
    }
    else if ((obj.HasMember("address") && obj["address"].IsObject())) {
//    else if (pAny.type() == typeid(Address)) {
        std::cout << "enter address" << std::endl;
        Address tmpAddress;
        return (Any)tmpAddress;
    }
    return rtnAny;
}
Any parse(string json) {
    Any myAny;
    rapidjson::Document dom;
    if (!dom.Parse(json.c_str()).HasParseError()) {
        //推测对象具体类型
        if ((dom.HasMember("Person") && dom["Person"].IsObject())) {
            //这是Person
            Person tmpPerson;
            //Person tmpPerson = any_cast<Person>(myAny);
            Any tmpAny = (Any)tmpPerson;
            Any rtnAny = parseElements(dom, tmpAny);
            return rtnAny;
        }
    }
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

    Singer s1{"rocker", 18};
    auto json2 = dump(p1);
    std::cout << "打印序列化结果" << std::endl;
    std::cout << json2 << std::endl;     // 打印序列化结果
    std::cout << "打印Person对象" << std::endl;
    std::cout << (Any)p1 << std::endl;    // 打印 Person 对象 
    auto pp = parse(json2);           // 反序列化 
    std::cout << "反序列化后打印对象" << std::endl;
    std::cout << (Any)pp << std::endl;    // 打印 Person 对象 
    // assert(p1 == pp)                 // 反序列化的结果是对的
}
  
