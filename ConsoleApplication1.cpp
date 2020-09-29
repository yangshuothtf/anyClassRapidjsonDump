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

void dumpElements2(const Any& myAny, const std::string& myName, Value& myValue, Document::AllocatorType& allocator) {
    //myValue.SetObject();
    Value nameValue(kStringType);
    nameValue.SetString(myName.c_str(), allocator);
    if (myAny.type() == typeid(int)) {
        Value intValue(kNumberType);
        //intValue.SetObject();
        intValue.SetInt(any_cast<int>(myAny));
        if (myName.empty()) {
            myValue.AddMember("Int", intValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, intValue, allocator);
        }
    }
    else if (myAny.type() == typeid(std::string)) {
        Value strValue(kStringType);
        //strValue.SetObject();
        strValue.SetString(any_cast<std::string>(myAny).c_str(), allocator);
        if (myName.empty()) {
            myValue.AddMember("String", strValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, strValue, allocator);
        }
    }
    else if (myAny.type() == typeid(Person)) {
        Person tmpPerson = any_cast<Person>(myAny);
        Value personValue;
        personValue.SetObject();

        dumpElements2(tmpPerson.name, "name", personValue, allocator);
        dumpElements2(tmpPerson.age, "age", personValue, allocator);
        dumpElements2(tmpPerson.address, "address", personValue, allocator);

        Value friendsArrayValue(kArrayType);
        for (int i = 0; i < tmpPerson._friends.size(); i++) {
            Friend& tmpFriend = tmpPerson._friends[i];
            Value friendValue;
            friendValue.SetObject();
            dumpElements2(tmpFriend, "", friendValue, allocator);
     //     dumpElements2(tmpFriend, "", friendsArrayValue, allocator);
            friendsArrayValue.PushBack(friendValue, allocator);
        }
        personValue.AddMember("_friends", friendsArrayValue, allocator);

        Value secretValue;
        secretValue.SetObject();
        Any& tmpAny = tmpPerson.secret;
        dumpElements2(tmpAny, "secret", personValue, allocator);

        if (myName.empty()) {
            myValue.AddMember("Person", personValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, personValue, allocator);
        }
    }
    else if (myAny.type() == typeid(Friend)) {
        Friend tmpFriend = any_cast<Friend>(myAny);
        Value friendValue;
        friendValue.SetObject();

        dumpElements2(tmpFriend.relation, "relation", friendValue, allocator);

        Value secretValue;
        secretValue.SetObject();
        Any& tmpAny = tmpFriend.secret;
        dumpElements2(tmpAny, "secret", friendValue, allocator);

        if (myName.empty()) {
            myValue.AddMember("Friend", friendValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, friendValue, allocator);
        }
    }
    else if (myAny.type() == typeid(Address)) {
        Address tmpAddress = any_cast<Address>(myAny);
        Value addressValue;
        addressValue.SetObject();

        dumpElements2(tmpAddress.country, "country", addressValue, allocator);
        dumpElements2(tmpAddress.city, "city", addressValue, allocator);
        dumpElements2(tmpAddress.street, "street", addressValue, allocator);

        Value neighborsArrayValue(kArrayType);
        for (int i = 0; i < tmpAddress.neighbors.size(); i++) {
            Person& tmpNeighbor = tmpAddress.neighbors[i];
            Value neighborValue;
            neighborValue.SetObject();
            dumpElements2(tmpNeighbor, "", neighborValue, allocator);
            neighborsArrayValue.PushBack(neighborValue, allocator);
        }
        addressValue.AddMember("neighbors", neighborsArrayValue, allocator);

        if (myName.empty()) {
            myValue.AddMember("Address", addressValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, addressValue, allocator);
        }
    }
    else if (myAny.type() == typeid(Singer)) {
        Singer tmpSinger = any_cast<Singer>(myAny);
        Value singerValue;
        singerValue.SetObject();

        dumpElements2(tmpSinger.type, "type", singerValue, allocator);
        dumpElements2(tmpSinger.age, "age", singerValue, allocator);

        if (myName.empty()) {
            myValue .AddMember("Singer", singerValue, allocator);
        }
        else {
            myValue.AddMember(nameValue, singerValue, allocator);
        }
    }
}
// https://blog.csdn.net/weixin_41534481/article/details/108327885
std::string dump2(Any myAny) {
    Document doc;
    Value& docValue = doc.SetObject();//实例化一个GenericValue到根DOM
    Document::AllocatorType& allocator = doc.GetAllocator();
    dumpElements2(myAny, "", docValue, allocator);

    StringBuffer strBuffer;
    PrettyWriter<StringBuffer> writer(strBuffer);
    doc.Accept(writer);
    return strBuffer.GetString();
}

void parseElements(const rapidjson::Value& obj, Any* pAny) {
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
            parseElements(objAddress, (Any*)(&tmpAddress));
            tmpPerson.address = tmpAddress;
        }
        if (objPerson.HasMember("_friends") && objPerson["_friends"].IsArray()) {
            const rapidjson::Value& objFriends = objPerson["_friends"];
            Friend tmpFriend;
            for (int i = 0; i < objFriends.Size(); ++i) {
                parseElements(objFriends[i], (Any*)(&tmpFriend));
                tmpPerson._friends.push_back(tmpFriend);
            }
        }
    }
    else if (pAny->type() == typeid(Friend)) {
        int i = 0;
    }
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
            Any pAny = (Any)tmpPerson;
            parseElements(dom, &pAny);
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

    auto json1 = dump(p1);               // 序列化   
    auto json2 = dump2(p1);
    std::cout << "打印序列化结果" << std::endl;
    std::cout << json2 << std::endl;     // 打印序列化结果
    std::cout << "打印Person对象" << std::endl;
    std::cout << (Any)p1 << std::endl;    // 打印 Person 对象 
    auto pp = parse(json2);           // 反序列化 
    std::cout << "反序列化后打印对象" << std::endl;
    std::cout << (Any)pp << std::endl;    // 打印 Person 对象 
    // assert(p1 == pp)                 // 反序列化的结果是对的
}
