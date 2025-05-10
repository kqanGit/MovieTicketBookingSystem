#ifndef _ISEAT_H_
#define _ISEAT_H_

class ISeat {
public:
    virtual ~ISeat() = default;
    float virtual Price() const = 0;
};

#endif 