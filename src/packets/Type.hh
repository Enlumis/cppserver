
# ifndef        TYPE_HH_
# define        TYPE_HH_

#include <cstdint>

namespace       Type
{
    typedef struct  UserField
    {
        uint32_t    uid;
        std::string username;
        std::string status;
    }               UserField;
};
# endif         /* !TYPE_HH_ */
