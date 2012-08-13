#ifndef UDOCUMENT_H
#define UDOCUMENT_H

class UDocument
{
public:
    virtual bool save();
    virtual bool modified();
    virtual bool execute();
    virtual bool executable();
};

#endif // UDOCUMENT_H
