#include "any.h"

extern std::string dump(Any myAny);

ostream& operator<<(ostream& os, const Any& myAny) {
    std::string str = dump(myAny);
    os << str;
    return os;
};

